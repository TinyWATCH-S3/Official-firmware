/*
  Arduino Library for RV-3028-C7

  Original Author: Copyright (c) 2020 Macro Yau
  https://github.com/MacroYau/RV-3028-C7-Arduino-Library

  Modified by Unexpected Maker: 2023-10-7
  - Added the ability to set the time from a tm struct
  - Added the ability to extra date & components by component enum  
*/


#ifndef RV3028C7_H
#define RV3028C7_H

#include "Arduino.h"
#include "Wire.h"

#define RV3028C7_ADDRESS 0x52

/* Register Map */

// Clock Registers
#define REG_CLOCK_SECONDS 0x00 // Seconds
#define REG_CLOCK_MINUTES 0x01 // Minutes
#define REG_CLOCK_HOURS 0x02   // Hours

// Calendar Registers
#define REG_CALENDAR_WEEKDAY 0x03 // Weekday
#define REG_CALENDAR_DATE 0x04    // Date
#define REG_CALENDAR_MONTH 0x05   // Months
#define REG_CALENDAR_YEAR 0x06    // Years

// Alarm Registers
#define REG_ALARM_MINUTES 0x07     // Minutes Alarm
#define REG_ALARM_HOURS 0x08       // Hours Alarm
#define REG_ALARM_WEEKDAY_DAY 0x09 // Weekday / Date Alarm

// Periodic Countdown Timer Control Registers
#define REG_TIMER_VALUE_0 0x0A  // Timer Value 0
#define REG_TIMER_VALUE_1 0x0B  // Timer Value 1
#define REG_TIMER_STATUS_0 0x0C // Timer Status 0 (R)
#define REG_TIMER_STATUS_1 0x0D // Timer Status 1 shadow (R)

// Status and Control Registers
#define REG_STATUS 0x0E         // Status
#define REG_CONTROL_1 0x0F      // Control 1
#define REG_CONTROL_2 0x10      // Control 2
#define REG_GP_BITS 0x11        // GP Bits
#define REG_CLOCK_INT_MASK 0x12 // Clock Interrupt Mask

// Event Control Register
#define REG_EVENT_CONTROL 0x13 // Event Control

// Time Stamp Registers
#define REG_TS_COUNT 0x14   // Count TS (R)
#define REG_TS_SECONDS 0x15 // Seconds TS (R)
#define REG_TS_MINUTES 0x16 // Minutes TS (R)
#define REG_TS_HOURS 0x17   // Hours TS (R)
#define REG_TS_DATE 0x18    // Date TS (R)
#define REG_TS_MONTH 0x19   // Month TS (R)
#define REG_TS_YEAR 0x1A    // Year TS (R)

// UNIX Time Registers
#define REG_UNIX_TIME_0 0x1B // UNIX Time 0 (LSB)
#define REG_UNIX_TIME_1 0x1C // UNIX Time 1
#define REG_UNIX_TIME_2 0x1D // UNIX Time 2
#define REG_UNIX_TIME_3 0x1E // UNIX Time 3 (MSB)

// RAM Registers
#define REG_USER_RAM_1 0x1F // User RAM 1
#define REG_USER_RAM_2 0x20 // User RAM 2

// Password Registers
#define REG_PASSWORD_0 0x21 // Password 0 (LSB)
#define REG_PASSWORD_1 0x22 // Password 1
#define REG_PASSWORD_2 0x23 // Password 2
#define REG_PASSWORD_3 0x24 // Password 3 (MSB)

// EEPROM Memory Control Registers
#define REG_EE_ADDRESS 0x25 // EE Address
#define REG_EE_DATA 0x26    // EE Data
#define REG_EE_COMMAND 0x27 // EE Command

// ID Register
#define REG_ID 0x28 // ID (R)

// Configuration EEPROM with RAM Mirror Registers
#define REG_EEPROM_PW_WENABLE 0x30 // EEPROM Password Enable
#define REG_EEPROM_PASSWORD_0 0x31 // EEPROM Password 0 (LSB)
#define REG_EEPROM_PASSWORD_1 0x32 // EEPROM Password 1
#define REG_EEPROM_PASSWORD_2 0x33 // EEPROM Password 2
#define REG_EEPROM_PASSWORD_3 0x34 // EEPROM Password 3 (MSB)
#define REG_EEPROM_CLKOUT 0x35     // EEPROM Clkout
#define REG_EEPROM_OFFSET 0x36     // EEPROM Offset
#define REG_EEPROM_BACKUP 0x37     // EEPROM Backup

/* Bit Positions and Bitmasks */

// Alarm Registers
#define BP_ALARM_AE 7

// Status Register
#define BP_REG_STATUS_EEBUSY 7
#define BP_REG_STATUS_CLKF 6
#define BP_REG_STATUS_BSF 5
#define BP_REG_STATUS_UF 4
#define BP_REG_STATUS_TF 3
#define BP_REG_STATUS_AF 2
#define BP_REG_STATUS_EVF 1
#define BP_REG_STATUS_PORF 0
#define BM_REG_STATUS_INTERRUPT_FLAGS 0b01011111

// Control 1 Register
#define BP_REG_CONTROL_1_TRPT 7
#define BP_REG_CONTROL_1_WADA 5
#define BP_REG_CONTROL_1_USEL 4
#define BP_REG_CONTROL_1_EERD 3
#define BP_REG_CONTROL_1_TE 2
#define BP_REG_CONTROL_1_TD_MSB 1
#define BP_REG_CONTROL_1_TD_LSB 0

// Control 2 Register
#define BP_REG_CONTROL_2_TSE 7
#define BP_REG_CONTROL_2_CLKIE 6
#define BP_REG_CONTROL_2_UIE 5
#define BP_REG_CONTROL_2_TIE 4
#define BP_REG_CONTROL_2_AIE 3
#define BP_REG_CONTROL_2_EIE 2
#define BP_REG_CONTROL_2_12_24 1
#define BP_REG_CONTROL_2_RESET 0
#define BM_REG_CONTROL_2_INTERRUPT_ENABLE_ALL 0b00111100

// Clock Interrupt Mask Register
#define BP_REG_CLOCK_INT_MASK_CEIE 3
#define BP_REG_CLOCK_INT_MASK_CAIE 2
#define BP_REG_CLOCK_INT_MASK_CTIE 1
#define BP_REG_CLOCK_INT_MASK_CUIE 0

// Event Control Register
#define BP_REG_EVENT_CONTROL_EHL 6
#define BP_REG_EVENT_CONTROL_ET_MSB 5
#define BP_REG_EVENT_CONTROL_ET_LSB 4
#define BP_REG_EVENT_CONTROL_TSR 2
#define BP_REG_EVENT_CONTROL_TSOW 1
#define BP_REG_EVENT_CONTROL_TSS 0

// Configuration EEPROM with RAM Mirror Registers
#define BP_REG_EEPROM_CLKOUT_CLKOE 7
#define BP_REG_EEPROM_CLKOUT_CLKSY 6
#define BP_REG_EEPROM_CLKOUT_PORIE 3
#define BP_REG_EEPROM_CLKOUT_FD_MSB 2
#define BM_REG_EEPROM_CLKOUT_FD 0b00000111

/* EEPROM Memory Commands */

#define EECMD_FIRST 0x00
#define EECMD_UPDATE 0x11
#define EECMD_REFRESH 0x12
#define EECMD_WRITE_ONE_EEPROM_BYTE 0x21
#define EECMD_READ_ONE_EEPROM_BYTE 0x22

#define DATETIME_COMPONENTS 7

enum DateTimeComponent
{
  DATETIME_SECOND = 0,
  DATETIME_MINUTE = 1,
  DATETIME_HOUR = 2,
  DATETIME_DAY_OF_WEEK = 3,
  DATETIME_DAY_OF_MONTH = 4,
  DATETIME_MONTH = 5,
  DATETIME_YEAR = 6
};
typedef uint8_t DateTimeComponent_t;

enum DayOfWeek
{
  SUN = 0,
  MON = 1,
  TUE = 2,
  WED = 3,
  THU = 4,
  FRI = 5,
  SAT = 6
};
typedef uint8_t DayOfWeek_t;

enum ClockOutputFrequency
{
  CLKOUT_32768HZ = 0,
  CLKOUT_8192HZ = 1,
  CLKOUT_1024HZ = 2,
  CLKOUT_64HZ = 3,
  CLKOUT_32HZ = 4,
  CLKOUT_1HZ = 5,
  CLKOUT_PREDEFINED = 6,
  CLKOUT_LOW = 7
};
typedef uint8_t ClockOutputFrequency_t;

enum AlarmMode
{
  ALARM_DISABLED = 0,
  ALARM_ONCE_PER_DAY_OF_MONTH_OR_WEEK = 1,
  ALARM_ONCE_PER_HOUR_PER_DAY_OF_MONTH_OR_WEEK = 2,
  ALARM_ONCE_PER_DAY = 3,
  ALARM_ONCE_PER_HOUR = 4
};
typedef uint8_t AlarmMode_t;

enum InterruptType
{
  INTERRPUT_PERIODIC_TIME_UPDATE = BP_REG_CONTROL_2_UIE,
  INTERRUPT_PERIODIC_COUNTDOWN_TIMER = BP_REG_CONTROL_2_TIE,
  INTERRUPT_ALARM = BP_REG_CONTROL_2_AIE,
  INTERRUPT_EXTERNAL_EVENT = BP_REG_CONTROL_2_EIE
};
typedef uint8_t InterruptType_t;

enum TimerClockFrequency
{
  TIMER_4096HZ = 0,
  TIMER_64HZ = 1,
  TIMER_1HZ = 2,
  TIMER_MINUTE = 3
};
typedef uint8_t TimerClockFrequency_t;

class RV3028C7
{
public:
  RV3028C7();
  bool begin(TwoWire &wirePort = Wire);

  uint32_t getUnixTimestamp();
  bool setUnixTimestamp(uint32_t secondsSinceEpoch, bool syncCalendar = true);

  char *getCurrentDateTime();
  uint8_t getCurrentDateTimeComponent(DateTimeComponent_t component);
  void setDateTimeFromISO8601(String iso8601);
  void setDateTimeFromISO8601(const char *iso8601);
  bool setDateTimeFromTM(tm &timeinfo);
  void setDateTimeFromHTTPHeader(String str);
  void setDateTimeFromHTTPHeader(const char *str);
  bool setDateTime(uint16_t year, uint8_t month, uint8_t dayOfMonth,
                   DayOfWeek_t dayOfWeek, uint8_t hour, uint8_t minute,
                   uint8_t second = 0, bool syncUnixTime = true);
  void setDateTimeComponent(DateTimeComponent_t component, uint8_t value);
  bool synchronize();

  bool enableClockOutput(ClockOutputFrequency_t frequency);
  bool disableClockOutput();

  bool setDateAlarm(AlarmMode_t mode, uint8_t dayOfMonth, uint8_t hour = 0,
                    uint8_t minute = 0);
  bool setWeekdayAlarm(AlarmMode_t mode, DayOfWeek_t dayOfWeek,
                       uint8_t hour = 0, uint8_t minute = 0);
  bool setDailyAlarm(uint8_t hour = 0, uint8_t minute = 0);
  bool setHourlyAlarm(uint8_t minute = 0);
  bool disableAlarm();

  bool setPeriodicCountdownTimer(uint16_t timerValue,
                                 TimerClockFrequency_t frequency,
                                 bool repeat = true);
  bool startPeriodicCountdownTimer();
  bool stopPeriodicCountdownTimer();

  bool enableInterrupt(InterruptType_t type);
  bool disableInterrupt(InterruptType_t type);
  bool disableAllInterrupts();
  bool isInterruptDetected(InterruptType_t type);
  bool clearInterrupt(InterruptType_t type);
  bool clearAllInterrupts();

  uint32_t convertToUnixTimestamp(uint16_t year, uint8_t month,
                                  uint8_t dayOfMonth, uint8_t hour,
                                  uint8_t minute, uint8_t second);

  uint8_t convertToDecimal(uint8_t bcd);
  uint8_t convertToBCD(uint8_t decimal);

  bool waitForEEPROM(uint8_t timeoutMS = 100);
  bool enableEEPROMAutoRefresh();
  bool disableEEPROMAutoRefresh();
  bool refreshConfigurationEEPROMToRAM();
  bool updateConfigurationEEPROMFromRAM();
  uint8_t readByteFromEEPROM(uint8_t address);
  bool writeByteToEEPROM(uint8_t address, uint8_t value);

  bool readBytesFromRegisters(uint8_t startAddress, uint8_t *destination,
                              uint8_t length);
  bool writeBytesToRegisters(uint8_t startAddress, uint8_t *values,
                             uint8_t length);
  uint8_t readByteFromRegister(uint8_t address);
  bool writeByteToRegister(uint8_t address, uint8_t value);

private:
  uint8_t _dateTime[DATETIME_COMPONENTS];
  TwoWire *_i2cPort;
};

#endif