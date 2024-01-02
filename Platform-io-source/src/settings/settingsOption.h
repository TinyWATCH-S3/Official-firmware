#pragma once

#include "Arduino.h"
#include "json.h"
#include "utilities/logging.h"

class SettingsOptionInt
{
	public:
		SettingsOptionInt(int * val, int val_min, int val_max, int val_step, bool wrap) : 
			setting_ref(val), value_min(val_min), value_max(val_max), value_step(val_step), value_wrap(wrap) {}
		int change(int dir);
		int get();
		String get_str();

	private:
		int *setting_ref = nullptr;
		int value_min = 0;
		int value_max = 0;
		int value_step = 1;
		bool value_wrap = false;
};

class SettingsOptionFloat
{
	public:
		SettingsOptionFloat(float * val, float val_min, float val_max, float val_step, bool wrap) : 
			setting_ref(val), value_min(val_min), value_max(val_max), value_step(val_step), value_wrap(wrap) {}
		float change(int dir);
		float get();
		String get_str();

	private:
		float *setting_ref = nullptr;
		float value_min = 0;
		float value_max = 0;
		float value_step = 1;
		bool value_wrap = false;
};

class SettingsOptionBool
{
	public:
		SettingsOptionBool(bool * val) : setting_ref(val) {}
		bool change();
		bool get();
		String get_str();

	private:
		bool *setting_ref = nullptr;
};

class SettingsOptionString
{
	public:
		SettingsOptionString(String *val) : setting_ref(val) {}
		void change(String *val);
		String get();

	private:
		String *setting_ref = nullptr;
};
