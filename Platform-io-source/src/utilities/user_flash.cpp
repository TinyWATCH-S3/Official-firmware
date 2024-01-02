#include "utilities/user_flash.h"

#include <Adafruit_SPIFlash.h>
#include "ff.h"
#include "diskio.h"
#include "diskio_impl.h"
#include "esp_vfs_fat.h"
#include "vfs_api.h"
#include "utilities/logging.h"

#define USER_FLASH_CS 33

#define MOUNTPOINT "/flash"

static Adafruit_FlashTransport_SPI flashTransport(USER_FLASH_CS, SPI);
Adafruit_SPIFlash ada_flash(&flashTransport);

static FSImplPtr vfsImpl(new VFSImpl());
FS flashFS(vfsImpl);

uint8_t pdrv = 0xFF;

// diskio methods to connect FatFS and Adafruit_SPIFlash
// Copied from https://github.com/adafruit/Adafruit_SPIFlash/blob/master/examples/SdFat_format/SdFat_format.ino
static DSTATUS diskio_status(BYTE pdrv)
{
	(void)pdrv;
	return 0;
}

static DSTATUS diskio_initialize(BYTE pdrv)
{
	(void)pdrv;
	return 0;
}

static DRESULT diskio_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
	(void)pdrv;
	return ada_flash.readBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
}

static DRESULT diskio_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
	(void)pdrv;
	return ada_flash.writeBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
}

static DRESULT diskio_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
	(void)pdrv;

	switch (cmd)
	{
	case CTRL_SYNC:
		ada_flash.syncBlocks();
		return RES_OK;

	case GET_SECTOR_COUNT:
		*((DWORD *)buff) = ada_flash.size() / 512;
		return RES_OK;

	case GET_SECTOR_SIZE:
		*((WORD *)buff) = 512;
		return RES_OK;

	case GET_BLOCK_SIZE:
		*((DWORD *)buff) = 8; // erase block size in units of sector size
		return RES_OK;

	default:
		return RES_PARERR;
	}
}

bool UserFlash::init()
{
	static const SPIFlash_Device_t w25q80dv = W25Q80DV;
	ada_flash.begin(&w25q80dv);
	if (ff_diskio_get_drive(&pdrv) != ESP_OK || pdrv == 0xFF)
	{
		debug_println("No available FatFS diskio slot for user flash");
		return false;
	}
	static const ff_diskio_impl_t flash_diskio = {
		.init = &diskio_initialize,
		.status = &diskio_status,
		.read = &diskio_read,
		.write = &diskio_write,
		.ioctl = &diskio_ioctl
	};
	ff_diskio_register(pdrv, &flash_diskio);

	FATFS* fatfs;
	char drv[3] = {(char)('0' + pdrv), ':', 0};
	esp_err_t err = esp_vfs_fat_register(MOUNTPOINT, drv, 5, &fatfs);
	if (err != ESP_OK)
	{
		debug_printf("esp_vfs_fat_register user flash failed (%d)\n", err);
		return false;
	}
	FRESULT fresult = f_mount(fatfs, drv, 1);
	if (fresult != FR_OK)
	{
		debug_printf("Failed to mount user flash, formatting... (%d)\n", fresult);
		size_t work_size = FF_MAX_SS;
		BYTE* work = (BYTE*) malloc(work_size);
		if (work == NULL)
		{
			error_println("Failed to allocate work buffer for formatting user flash");
			return false;
		}
		fresult = f_mkfs(drv, FM_ANY | FM_SFD, 0, work, sizeof(BYTE) * FF_MAX_SS);
		free(work);
		if (fresult != FR_OK)
		{
			error_printf("Failed to format user flash (%d)\n", fresult);
			return false;
		}
		fresult = f_mount(fatfs, drv, 0);
		if (fresult != FR_OK)
		{
			error_printf("Failed to mount user flash (%d)\n", fresult);
			return false;
		}
	}

	vfsImpl->mountpoint(MOUNTPOINT);
	return true;
}

bool UserFlash::deinit()
{
	vfsImpl->mountpoint(NULL);
	char drv[3] = {(char)('0' + pdrv), ':', 0};
	f_unmount(drv);
	esp_vfs_fat_unregister_path(MOUNTPOINT);
	ff_diskio_unregister(pdrv);
	pdrv = 0xFF;
	ada_flash.end();
	return true;
}

bool UserFlash::is_busy()
{
	return ada_flash.isBusy();
}

UserFlash user_flash;
