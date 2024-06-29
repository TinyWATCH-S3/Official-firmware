/*
 * SettingsOption are classes used to bind user settings from the settings class into tw_Controls
 * so the control can read and change the setting value.
 *
 * Classes are provided for Int, Bool, Float and String
 */

#include "settings/settings.h"
#include "settings/settingsOption.h"

void SettingsOptionBase::register_option(int grp)
{
	// The settings_groups are nor created yet as this is called before the Settings() constructor is called
	// So we add them as needed here, and then we'll initialise the missing data in the Settings() constructor later
	if (settings.settings_groups.size() < grp + 1)
		settings.settings_groups.push_back({});

	// Now we know the group is there we can add this control to it
	settings.settings_groups[grp].groups.push_back(this);
}

//
// INT
//
int SettingsOptionInt::change(int dir)
{
	int current = *setting_ref;
	current += dir;
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

bool SettingsOptionInt::update(int val)
{
	if (*setting_ref == val)
		return false;

	*setting_ref = val;
	settings.save(false);

	return true;
}

int SettingsOptionInt::get() { return *setting_ref; }

String SettingsOptionInt::get_str() { return String(*setting_ref); }

String SettingsOptionInt::generate_html(uint16_t index)
{
	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn.replace("_(sec)", "");
	fn.replace("_(min)", "");
	fn.replace("_(%%)", "");
	fn = String(group) + "," + String(index) + "__" + fn;

	String html = "					<div class='input-group input-group-sm'>\n";
	html += "						<span class='input-group-text' id='inputGroup-sizing-sm'>" + fieldname + "</span>\n";
	html += "						<input type='number' min='" + String(value_min) + "' max='" + String(value_max) + "' class='form-control form-control-sm' id='" + fn + "' name='" + fn + "' value='" + get() + "' required>\n";
	html += "					</div>";

	return html;
}

// void SettingsOptionInt::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }

//
// INT RANGE
//
int SettingsOptionIntRange::change(int dir)
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

bool SettingsOptionIntRange::update(int val)
{
	if (*setting_ref == val)
		return false;

	*setting_ref = val;
	settings.save(false);

	return true;
}

int SettingsOptionIntRange::get() { return constrain(*setting_ref, value_min, value_max); }

String SettingsOptionIntRange::get_str() { return String(constrain(*setting_ref, value_min, value_max)); }

String SettingsOptionIntRange::generate_html(uint16_t index)
{
	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn.replace("_(sec)", "");
	fn.replace("_(min)", "");
	fn.replace("_(%%)", "");
	fn = String(group) + "," + String(index) + "__" + fn;

	String html = "					<div class='input-group input-group-sm flex-nowrap mt-1'>\n";
	html += "						<span class='input-group-text' id='inputGroup-sizing-sm'>" + fieldname + "</span>\n";
	html += "						<input type='range' min='" + String(value_min) + "' max='" + String(value_max) + "' step='" + String(value_step) + "' class='form-range form-range-sm ms-2 me-2 mt-2' id='" + fn + "' name='" + fn + "' value='" + get_str() + "' required oninput='document.getElementById(\"" + fn + "range\").innerHTML = this.value;'>\n";
	html += "						<span class='input-group-text' id='" + fn + "range'>" + get_str() + "</span>\n";
	html += "					</div>\n";

	return html;
}

// void SettingsOptionIntRange::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }

//
// INT VECTOR
//
int SettingsOptionIntVector::change(int index, int dir)
{
	int current = (*setting_ref)[index];
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

	(*setting_ref)[index] = current;
	settings.save(true);

	return (*setting_ref)[index];
}

bool SettingsOptionIntVector::update(int index, int val)
{
	int current = (*setting_ref)[index];
	if (current == val)
		return false;

	(*setting_ref)[index] = val;
	settings.save(false);

	return true;
}

uint8_t SettingsOptionIntVector::vector_size() { return (*setting_ref).size(); }
int SettingsOptionIntVector::get(int index) { return (*setting_ref)[index]; }

String SettingsOptionIntVector::get_str(int index) { return String((*setting_ref)[index]); }

String SettingsOptionIntVector::generate_html(uint16_t index)
{
	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn.replace("_(sec)", "");
	fn.replace("_(min)", "");
	fn.replace("_(%%)", "");
	fn = String(group) + "," + String(index) + "__" + fn;

	String html = "					<div class='input-group input-group-sm mb-1'>\n";
	html += "						<div class='row g-2'>\n";
	html += "							<div class='col-sm-12 ps-3 pt-2 pb-0' style='font-size:14px;'>" + fieldname + "</div>\n";
	for (size_t i = 0; i < (*setting_ref).size(); i++)
	{
		html += "								<div class='col-sm-4 form-floating'>\n";
		html += "									<input type='number' class='form-control form-control-sm' id='" + fn + "_" + String(i) + "' name='" + fn + "_" + String(i) + "' value='" + String(get(i)) + "' min='" + String(value_min) + "' max='" + String(value_max) + "' required onchange='' />\n";
		html += "									<label class='ms-1' for='" + fn + "_" + String(i) + "'>Step " + String(i + 1) + "</label>\n";
		html += "								</div>\n";
	}
	html += "							</div>\n";
	html += "						</div>\n";

	return html;
}

// void SettingsOptionIntVector::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }

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

bool SettingsOptionFloat::update(float val)
{
	if (*setting_ref == val)
		return false;

	*setting_ref = val;
	settings.save(false);

	return true;
}

float SettingsOptionFloat::get() { return *setting_ref; }

String SettingsOptionFloat::get_str() { return String(*setting_ref); }

String SettingsOptionFloat::generate_html(uint16_t index)
{
	return "<div><span>SettingsOptionFloat for " + fieldname + "</span></div>";
}

// void SettingsOptionFloat::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }

//
// BOOL
//
bool SettingsOptionBool::change()
{
	*setting_ref = !*setting_ref;
	// settings.ui_forced_save = true;
	settings.save(true);
	// info_println("bool: " + String(*setting_ref));
	return *setting_ref;
}

bool SettingsOptionBool::update(bool val)
{
	if (*setting_ref == val)
		return false;

	*setting_ref = val;
	settings.save(false);

	return true;
}

bool SettingsOptionBool::get() { return *setting_ref; }

String SettingsOptionBool::get_str() { return String(*setting_ref); }

String SettingsOptionBool::get_fn() { return fieldname; }
String SettingsOptionBool::get_op1() { return option1; }
String SettingsOptionBool::get_op2() { return option2; }

String SettingsOptionBool::generate_html(uint16_t index)
{
	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn.replace("_(sec)", "");
	fn.replace("_(min)", "");
	fn.replace("_(%%)", "");
	fn = String(group) + "," + String(index) + "__" + fn;

	String html = "					<div class='input-group input-group-sm'>\n";
	html += "						<span class='input-group-text' id='inputGroup-sizing-sm'>" + fieldname + "</span>\n";
	html += "						<select class='form-select form-select-sm' id='" + fn + "' name='" + fn + "' onchange='set_input_states();'>\n";

	html += "							<option value='0' ";
	html += get() ? "" : "selected";
	html += ">" + option1 + " </option>\n";

	html += "							<option value='1' ";
	html += get() ? "selected" : "";
	html += ">" + option2 + " </option>\n";

	html += "						</select>\n";
	html += "					</div>\n";

	return html;
}

// void SettingsOptionBool::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }

//
// STRING
//
void SettingsOptionString::change(String *val)
{
	setting_ref = val;
	settings.save(true);
}

bool SettingsOptionString::update(String *val)
{
	if (*setting_ref == *val)
		return false;

	*setting_ref = *val;
	settings.save(false);

	return true;
}

String SettingsOptionString::get() { return *setting_ref; }

String SettingsOptionString::generate_html(uint16_t index)
{
	// return "<div><span>SettingsOptionString for " + fieldname + "</span></div>";

	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn.replace("_(sec)", "");
	fn.replace("_(min)", "");
	fn.replace("_(%%)", "");
	fn = String(group) + "," + String(index) + "__" + fn;

	String html = "					<div class='input-group input-group-sm'>\n";
	html += "						<span class='input-group-text' id='inputGroup-sizing-sm'>" + fieldname + "</span>\n";
	html += "						<input type='text' class='form-control form-control-sm' id='" + fn + "' name='" + fn + "' value='" + get() + "' required>\n";
	html += "					</div>\n";

	return html;
}

// void SettingsOptionString::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }
