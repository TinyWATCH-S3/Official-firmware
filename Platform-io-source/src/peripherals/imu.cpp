#include "imu.h"
#include "activity.h"
#include "rtc.h"
#include "utilities/logging.h"
#include "settings/settings.h"

extern Activity activity;
extern RTC rtc;

void IMU::preinit(bool woke_from_sleep)
{
	imu_preinit_ok = true;
	if (imu.beginI2C(i2cAddress, Wire, woke_from_sleep) != BMI2_OK)
	{
		// Not connected, inform user
		info_println("Error: BMI270 not connected, check wiring and I2C address!");
		imu_preinit_ok = false;
		return;
	}
	info_println(F("Found BMI270"));
}

void IMU::init()
{
	imu_ready = imu_preinit_ok;
	mag_ready = true;

	if (imu_ready)
	{
		info_println(F("Setting up BMI270"));

		imu.setAccelPowerMode(BMI2_POWER_OPT_MODE);
		imu.setGyroPowerMode(BMI2_POWER_OPT_MODE, BMI2_POWER_OPT_MODE);

		// Reorient the IMU because it's worn upside down from BOSCH's intended orientation
		// The sensor assumes the axes have a particular orientation with respect to
		// the watch face, where +Y is at 12 o'clock, and +X is at 3 o'clock. You
		// can remap the axes if needed by uncommenting the code below
		bmi2_remap axes;
		axes.x = BMI2_AXIS_NEG_X;
		axes.y = BMI2_AXIS_POS_Y;
		axes.z = BMI2_AXIS_NEG_Z;
		imu.remapAxes(axes);

		// Initialise the step counter and wrist wake system
		setup_step_counter();
		setup_wake_gesture();
	}

	mag = Adafruit_MMC5603(12345);
	if (!mag.begin(MMC56X3_DEFAULT_ADDRESS))
	{
		// I2C mode
		/* There was a problem detecting the MMC5603 ... check your connections */
		info_println("Ooops, no MMC5603 detected ... Check your wiring!");
		mag_ready = false;
	}
	else
	{
		info_println(F("Found MMC56X3"));
		// mag.magnetSetReset();
		// mag.setContinuousMode(true);
	}

	next_imu_read = millis();
}

static bool interrupt_happened = false;
static void step_interrupt() { interrupt_happened = true; }

static void wake_interrupt() { info_println("WRIST INTERRUPT FIRED"); }

void IMU::setup_step_counter()
{
	// Setting up BMI270 features like step counter
	// imu.enableFeature(BMI2_STEP_DETECTOR);
	imu.enableFeature(BMI2_STEP_COUNTER);
	imu.enableFeature(BMI2_STEP_ACTIVITY);

	// When the step counter feature is enabled, it can trigger an interrupt
	// every number of steps defined by the watermark. This has a factor of 20x,
	// so a value of 1 means 20 step intervals. The step counter interrupt is
	// disabled when the watermark is 0 (default)
	imu.setStepCountWatermark(1);

	// Setup interrupts on INT1 - the IMU has 2x INTs
	imu.mapInterruptToPin(BMI2_STEP_COUNTER_INT, BMI2_INT1);
	imu.mapInterruptToPin(BMI2_STEP_ACTIVITY_INT, BMI2_INT1);

	bmi2_int_pin_config intPinConfig;
	intPinConfig.pin_type = BMI2_INT1;
	intPinConfig.int_latch = BMI2_INT_NON_LATCH;
	intPinConfig.pin_cfg[0].lvl = BMI2_INT_ACTIVE_HIGH;
	intPinConfig.pin_cfg[0].od = BMI2_INT_PUSH_PULL;
	intPinConfig.pin_cfg[0].output_en = BMI2_INT_OUTPUT_ENABLE;
	intPinConfig.pin_cfg[0].input_en = BMI2_INT_INPUT_DISABLE;

	int result = imu.setInterruptPinConfig(intPinConfig);

	// Setup interrupt handler
	attachInterrupt(digitalPinToInterrupt(BMI2_INT_PIN1), step_interrupt, RISING);
}

void IMU::setup_wake_gesture()
{
	imu.enableFeature(BMI2_WRIST_WEAR_WAKE_UP);
	imu.mapInterruptToPin(BMI2_WRIST_WEAR_WAKE_UP_INT, BMI2_INT2);
	// imu.mapInterruptToPin(BMI2_WRIST_GESTURE_INT, BMI2_INT2);

	/*
		Used to configure wrist wake up - but not using this yet.
		bmi2_sens_config wristWakeUpConfig;
		wristWakeUpConfig.type = BMI2_WRIST_WEAR_WAKE_UP;
		wristWakeUpConfig.cfg.wrist_wear_wake_up.min_angle_focus = 1448;
		wristWakeUpConfig.cfg.wrist_wear_wake_up.min_angle_nonfocus = 1774;
		wristWakeUpConfig.cfg.wrist_wear_wake_up.max_tilt_lr = 1024;
		wristWakeUpConfig.cfg.wrist_wear_wake_up.max_tilt_ll = 700;
		wristWakeUpConfig.cfg.wrist_wear_wake_up.max_tilt_pd = 179;
		wristWakeUpConfig.cfg.wrist_wear_wake_up.max_tilt_pu = 1925;
		imu.setConfig(wristWakeUpConfig);
	*/

	bmi2_int_pin_config intPinConfig;
	intPinConfig.pin_type = BMI2_INT2;
	intPinConfig.int_latch = BMI2_INT_NON_LATCH;
	intPinConfig.pin_cfg[1].lvl = BMI2_INT_ACTIVE_LOW;
	intPinConfig.pin_cfg[1].od = BMI2_INT_PUSH_PULL;
	intPinConfig.pin_cfg[1].output_en = BMI2_INT_OUTPUT_ENABLE;
	intPinConfig.pin_cfg[1].input_en = BMI2_INT_INPUT_DISABLE;

	int result = imu.setInterruptPinConfig(intPinConfig);

	attachInterrupt(digitalPinToInterrupt(BMI2_INT_PIN2), wake_interrupt, FALLING);
}

void IMU::set_hibernate(bool state)
{
	if (!imu_ready)
		return;

	if (state)
	{
		imu.disableFeature(BMI2_ACCEL);
		imu.disableFeature(BMI2_GYRO);
	}
	else
	{
		imu.enableFeature(BMI2_ACCEL);
		imu.enableFeature(BMI2_GYRO);
	}
}

void IMU::process_steps()
{
	process_steps(false);
}

void IMU::process_steps(bool force)
{
	if (!imu_preinit_ok)
	{
		info_println("process_steps: IMU not ready");
		return;
	}

	// Wait for interrupt to occur
	if (!interrupt_happened && !force)
	{
		return;
	}

	uint16_t interrupt_status = 0;

	//    Serial.println("interrupt caught!"); Serial.flush();
	// Reset flag for next interrupt
	if (interrupt_happened)
	{
		interrupt_happened = false;
		// Get the interrupt status to know which condition triggered
		imu.getInterruptStatus(&interrupt_status);
		if (force)
			interrupt_status |= BMI270_STEP_CNT_STATUS_MASK; // force in effect
	}
	else
	{
		// no interrupt but we're forced to process
		interrupt_status = BMI270_STEP_CNT_STATUS_MASK;
	}

	bool processed = false;

	// Check if this is the correct interrupt condition
	if (interrupt_status & BMI270_STEP_CNT_STATUS_MASK)
	{
		// Get the step count
		uint32_t _step_count = 0;
		imu.getStepCount(&_step_count);

		if (_step_count > 0)
		{
			step_count = _step_count;
			imu.resetStepCount();
		}

		processed = true;
		info_println("Step count: " + String(step_count));
	}

	if (interrupt_status & BMI270_STEP_ACT_STATUS_MASK)
	{
		// Get the step activity
		movement_activity = 0;
		imu.getStepActivity(&movement_activity);

		if (movement_activity == BMI2_STEP_ACTIVITY_STILL && !processed)
		{
			process_steps(true);
			return;
		}
	}
	if (!(interrupt_status & (BMI270_STEP_CNT_STATUS_MASK | BMI270_STEP_ACT_STATUS_MASK)))
	{
		// info_println("Unknown Interrupt condition! " + String(interrupt_status));
	}

	if (force)
	{
		persist_step_count();
	}
}

uint32_t IMU::get_steps(uint8_t day, uint8_t month, uint16_t year)
{
	if (!imu_ready)
		return 0;

	if (step_count > 0)
	{
		// info_println("returning current step count of "+String(step_count));
		return step_count;
	}

	// days are 1-31, but arrays is 0-30
	day--;
	uint32_t key = (year * 100) + month;
	uint32_t data = activity.data.steps_day[key][day];
	// info_println("returning saved step count of "+String(data)+" from key "+String(key)+" and day "+String(day));
	return (data);
}

uint8_t IMU::get_movement_activity_id()
{
	if (!imu_ready)
		return 0;

	if (movement_activity == BMI2_STEP_ACTIVITY_WALKING)
		return 1;
	else if (movement_activity == BMI2_STEP_ACTIVITY_RUNNING)
		return 2;

	return 0;
}

String IMU::get_movement_activity()
{
	if (!imu_ready)
		return "N/A";

	switch (movement_activity)
	{
	case BMI2_STEP_ACTIVITY_STILL:
	{
		return "STILL";
		break;
	}
	case BMI2_STEP_ACTIVITY_WALKING:
	{
		return "WALKING";
		break;
	}
	case BMI2_STEP_ACTIVITY_RUNNING:
	{
		return "RUNNING";
		break;
	}
	default:
	{
		break;
	}
	}
	return "UNKNOWN";
}

void IMU::process_wrist_gestures()
{
	//
}

void IMU::update()
{
	if (imu_ready)
		imu.getSensorData();
}

float IMU::get_accel_x()
{
	if (!imu_ready)
	  return 0;

	return (imu.data.accelX);
}

float IMU::get_accel_y()
{
	if (!imu_ready)
		return 0;

	return (imu.data.accelY);
}

float IMU::get_accel_z()
{
	if (!imu_ready)
		return 0;

	return (imu.data.accelZ);
}

float IMU::get_gyro_x()
{
  if (!imu_ready)
		return 0;

	return (imu.data.gyroX);
}

float IMU::get_gyro_y()
{
	if (!imu_ready)
		return 0;

	return (imu.data.gyroY);
}

float IMU::get_gyro_z()
{
	if (!imu_ready)
		return 0;

	return (imu.data.gyroZ);
}

float IMU::get_pitch()
{
	if (!mag_ready)
		return 0;

	float p = (180 - (atan2(imu.data.accelY, imu.data.accelZ) * 57.3)) + 0.85;
	return (p - 180) * -1;
}

float IMU::get_roll()
{
	if (!mag_ready)
		return 0;

	return (atan2((imu.data.accelX), sqrt(imu.data.accelY * imu.data.accelY + imu.data.accelZ * imu.data.accelZ)) * 57.3);
}

void IMU::get_magnetic(float *x, float *y, float *z, bool compensated)
{
	if (mag_ready)
	{
		sensors_event_t event;
		mag.getEvent(&event);
		*x = event.magnetic.x;
		*y = event.magnetic.y;
		*z = event.magnetic.z;
	}
	else
	{		
		*x = 0;
		*y = 0;
		*z = 0;
	}

	if (compensated)
	{
		// Apply hard-iron calibration
		*x -= settings.config.compass.hard_iron_x;
		*y -= settings.config.compass.hard_iron_y;
		*z -= settings.config.compass.hard_iron_z;
		
		// Apply soft-iron 
		*x *= settings.config.compass.soft_iron_x;
		*y *= settings.config.compass.soft_iron_y;
		*z *= settings.config.compass.soft_iron_z;
	}
}

float IMU::get_yaw()
{
	float heading = 0;

	if (!mag_ready)
		return heading;

	float mag_x;
	float mag_y;
	float mag_z;
	get_magnetic(&mag_x, &mag_y, &mag_z);
	mag_x = -mag_x;	// invert x because the sensor is upside down

	// Non tilt compensated compass heading
	heading = atan2f(mag_x, mag_y) * RAD_TO_DEG;

	// Apply magnetic declination to convert magnetic heading to geographic heading
	heading += settings.config.compass.magnetic_declination;

	// Normalize to 0-360
	if (heading < 0.0)
		heading = 360.0 + heading;

	if (heading >= 360.0)
		heading = heading - 360.0;

	return heading;
}

bool IMU::is_looking_at_face()
{
	if (!imu_ready)
		return false;

	float p = get_pitch();
	float r = get_roll();
	// Watch on left hand!
	// Silly calculation to detect if the wrist is casing the wearers face.
	return (p >= 30 && p <= 50 && r > -10 && r < 10);
}

bool IMU::persist_step_count()
{
	if (step_count == 0)
		return false;

	// Track steps for day, month, year in settings with date rollover
	uint16_t day, month, year;
	rtc.get_step_date(day, month, year);
	activity.track_steps(step_count, day, month, year);
	auto saved = activity.save(false);
	step_count = 0;
	return saved;
}

IMU imu;
