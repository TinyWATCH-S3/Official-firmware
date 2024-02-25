#pragma once

#include "SparkFun_BMI270_Arduino_Library.h"
#include <Adafruit_MMC56x3.h>

#define BMI2_INT_PIN1 6
#define BMI2_INT_PIN2 7

class IMU
{
	public:
		void preinit(bool woke_from_sleep);
		void init();
		void update();
		float get_accel_x();
		float get_accel_y();
		float get_accel_z();
		float get_gyro_x();
		float get_gyro_y();
		float get_gyro_z();
		float get_pitch();
		float get_roll();
		float get_yaw(bool tilt_compensated = true);
		void get_magnetic(float *x, float *y, float *z, bool iron_compensated = true);
		
		bool is_looking_at_face();

		void set_hibernate(bool state);

		// BMI270 Step Counter
		void setup_step_counter();
		void process_steps();
		void process_steps(bool force);
		uint32_t get_steps(uint8_t day, uint8_t month, uint16_t year);
		uint8_t get_movement_activity_id();
		String get_movement_activity();

		// Wake from wrist
		void setup_wake_gesture();

		// BMI270 Wrist Gestures
		void process_wrist_gestures();

		// Ready states
		bool imu_preinit_ok = false;
		bool imu_ready = false;
		bool mag_ready = true;

	private:
		BMI270 imu;
		Adafruit_MMC5603 mag;
		uint8_t i2cAddress = BMI2_I2C_PRIM_ADDR;

		unsigned long next_imu_read = 0;
		float prev_yaw_read = 0;
		float curr_yaw_read = 0;
		float hacky_yaw = 0;
		
		// this should be specific to the unit and not location, so no need to save per user preferences
		float soft_iron[3][3] = {{1.003, 0.008, -0.001}, {0.008, 1.004, 0.000}, {-0.001, -0.000, 0.994}}; // {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}; // 

		uint32_t step_count = 0;
		uint8_t movement_activity = 0;

		bool persist_step_count();
};

extern IMU imu;
