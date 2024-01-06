/*
 * SettingsOption are classes used to bind user settings from the settings class into tw_Controls
 * so the control can read and change the setting value.
 *
 * Classes are provided for Int, Bool, Float and String
 */

#include "settings/settingsOption.h"
#include "settings/settings.h"

//
// INT
//
int SettingsOptionInt::change(int dir)
{
	int current = *setting_ref;
	current += (dir * value_step);
	if (value_wrap)
	{
		if (current > value_max)
			current = value_min;
		else if (current < value_min)
			current = value_max;
	}
	else
	{
		current = constrain(current, value_min, value_max);
	}

	*setting_ref = current;
	settings.save(true);
	// info_println("int: "+String(*setting_ref));
	return *setting_ref;
}

int SettingsOptionInt::get() { return *setting_ref; }

String SettingsOptionInt::get_str() { return String(*setting_ref); }

//
// FLOAT
//
float SettingsOptionFloat::change(int dir)
{
	int current = *setting_ref;
	current += ((float)dir * value_step);
	if (value_wrap)
	{
		if (current > value_max)
			current = value_min;
		else if (current < value_min)
			current = value_max;
	}
	else
	{
		current = constrain(current, value_min, value_max);
	}

	// info_println("float: "+String(*setting_ref));
	*setting_ref = current;
	settings.save(true);
	return *setting_ref;
}

float SettingsOptionFloat::get() { return *setting_ref; }

String SettingsOptionFloat::get_str() { return String(*setting_ref); }

//
// BOOL
//
bool SettingsOptionBool::change()
{
	*setting_ref = !*setting_ref;
	settings.save(true);
	// info_println("bool: "+String(*setting_ref));
	return *setting_ref;
}

bool SettingsOptionBool::get() { return *setting_ref; }

String SettingsOptionBool::get_str() { return String(*setting_ref); }

//
// STRING
//
void SettingsOptionString::change(String *val)
{
	setting_ref = val;
	settings.save(true);
}

String SettingsOptionString::get() { return *setting_ref; }