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

	String html = "					<div class='input-group input-group-sm flex-nowrap'>\n";
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
		html += "									<input type='number' class='form-control form-control-sm' id='" + fn + "_" + String(i) + "' name='" + fn + "_" + String(i) + "' value='" + String(get(i)) + "' min='" + String(value_min) + "' max='" + String(value_max) + "' required />\n";
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
	html += "						<select class='form-select form-select-sm' id='" + fn + "' name='" + fn + "'>\n";

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
	html += "						<input type='text' class='form-control form-control-sm' id='" + fn + "' name='" + fn + "' value='" + get() + "' placeholder='" + placeholder + "'" + (required_field ? "required" : "") + ">\n";
	html += "					</div>\n";

	return html;
}

// void SettingsOptionString::register_option(int grp)
// {
// 	if (settings.settings_groups[grp].groups.size() < grp + 1)
// 		settings.settings_groups[grp].groups.push_back({});

// 	settings.settings_groups[grp].groups.push_back(this);
// }

//
// WIFI STATIONS
//

bool SettingsOptionWiFiStations::update(int index, String _ssid, String _pass)
{
	(*setting_ref)[index].ssid = _ssid;
	(*setting_ref)[index].pass = _pass;
	settings.save(false);

	return true;
}

void SettingsOptionWiFiStations::remove_if_empty()
{
	(*setting_ref).erase(std::remove_if((*setting_ref).begin(), (*setting_ref).end(), [](const wifi_station &station) {
							 return station.ssid.isEmpty() && station.pass.isEmpty();
						 }),
						 (*setting_ref).end());
}

uint8_t SettingsOptionWiFiStations::vector_size() { return (*setting_ref).size(); }
String SettingsOptionWiFiStations::get_ssid(int index) { return (*setting_ref)[index].ssid; }
String SettingsOptionWiFiStations::get_pass(int index) { return (*setting_ref)[index].pass; }

void SettingsOptionWiFiStations::add_station(String ssid, String pass)
{

	wifi_station o = wifi_station();
	o.ssid = ssid;
	o.pass = pass;
	(*setting_ref).push_back(o);
}

String SettingsOptionWiFiStations::generate_html(uint16_t index)
{
	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn = String(group) + "," + String(index) + "__" + fn;

	String html = "					<div class='input-group input-group-sm mb-1'>\n";
	html += "						<div class='row g-2'>\n";
	html += "							<div class='col-sm-12 ps-3 pt-2 pb-0' style='font-size:14px;'>" + fieldname + "</div>\n";
	String highlight = "";
	String readonly = "required";

	for (size_t i = 0; i < (*setting_ref).size(); i++)
	{
		highlight = (settings.config.current_wifi_station == i) ? " style='background-color:#333388;'" : "";
		readonly = (settings.config.current_wifi_station == i) ? " readonly " : "";
		html += "								<div class='input-group input-group-sm'>\n";
		html += "									<span class='input-group-text' id='inputGroup-sizing-sm' " + highlight + ">" + String(i + 1) + "</span>\n";
		html += "									<span class='input-group-text' id='inputGroup-sizing-sm'>SSID</span>\n";
		html += "									<input type='text' class='form-control form-control-sm' id='" + fn + "_ssid_" + String(i) + "' name='" + fn + "_ssid_" + String(i) + "' value='" + String(get_ssid(i)) + "'" + readonly + "/>\n";
		html += "									<span class='input-group-text' id='inputGroup-sizing-sm'>Password</span>\n";
		html += "									<input type='password' class='form-control form-control-sm' id='" + fn + "_pass_" + String(i) + "' name='" + fn + "_pass_" + String(i) + "' value='" + String(get_pass(i)) + "'" + readonly + "/>\n";

		html += "								</div>\n";
	}
	// Add the empty
	int i = (*setting_ref).size();
	html += "								<div class='input-group input-group-sm'>\n";
	html += "									<span class='input-group-text' id='inputGroup-sizing-sm'>+</span>\n";
	html += "									<span class='input-group-text' id='inputGroup-sizing-sm'>SSID</span>\n";
	html += "									<input type='text' class='form-control form-control-sm' id='" + fn + "_ssid_" + String(i) + "' name='" + fn + "_ssid_" + String(i) + "' placeholder='New SSID' />\n";
	html += "									<span class='input-group-text' id='inputGroup-sizing-sm'>Password</span>\n";
	html += "									<input type='password' class='form-control form-control-sm' id='" + fn + "_pass_" + String(i) + "' name='" + fn + "_pass_" + String(i) + "' placeholder='New PASSWORD' />\n";
	html += "								</div>\n";

	html += "							</div>\n";
	html += "						</div>\n";

	return html;
}

//
// THEMES
//

bool SettingsOptionTheme::update(int index, String _name, int32_t _col_background_dull, uint32_t _col_background_bright, uint32_t _col_control_back, uint32_t _col_primary, uint32_t _col_secondary, uint32_t _col_low_intensity, uint32_t _col_warning, uint32_t _col_error, uint8_t _widget_style)
{
	(*setting_ref)[index].name = _name;
	(*setting_ref)[index].col_background_dull = _col_background_dull;
	(*setting_ref)[index].col_background_bright = _col_background_bright;
	(*setting_ref)[index].col_control_back = _col_control_back;
	(*setting_ref)[index].col_primary = _col_primary;
	(*setting_ref)[index].col_secondary = _col_secondary;
	(*setting_ref)[index].col_low_intensity = _col_low_intensity;
	(*setting_ref)[index].col_warning = _col_warning;
	(*setting_ref)[index].col_error = _col_error;
	(*setting_ref)[index].widget_style = _widget_style;
	settings.save(false);

	return true;
}

void SettingsOptionTheme::remove_if_empty()
{
	(*setting_ref).erase(std::remove_if((*setting_ref).begin(), (*setting_ref).end(), [](const theme &theme) {
							 return theme.name.isEmpty();
						 }),
						 (*setting_ref).end());
}

uint8_t SettingsOptionTheme::vector_size() { return (*setting_ref).size(); }
theme &SettingsOptionTheme::get_theme(int index) { return (*setting_ref)[index]; }

void SettingsOptionTheme::add_theme(String _name, int32_t _col_background_dull, uint32_t _col_background_bright, uint32_t _col_control_back, uint32_t _col_primary, uint32_t _col_secondary, uint32_t _col_low_intensity, uint32_t _col_warning, uint32_t _col_error, uint8_t _widget_style)
{
	theme t = theme();
	t.name = _name;
	t.col_background_dull = _col_background_dull;
	t.col_background_bright = _col_background_bright;
	t.col_control_back = _col_control_back;
	t.col_primary = _col_primary;
	t.col_secondary = _col_secondary;
	t.col_low_intensity = _col_low_intensity;
	t.col_warning = _col_warning;
	t.col_error = _col_error;
	t.widget_style = _widget_style;
	(*setting_ref).push_back(t);
}

String SettingsOptionTheme::rgbToHex(uint32_t rgbColor)
{
	uint8_t red = (rgbColor >> 16) & 0xFF;
	uint8_t green = (rgbColor >> 8) & 0xFF;
	uint8_t blue = rgbColor & 0xFF;

	// Convert each component to a two-character hexadecimal string
	String hexColor = "#";
	hexColor += (red < 0x10 ? "0" : "") + String(red, HEX);
	hexColor += (green < 0x10 ? "0" : "") + String(green, HEX);
	hexColor += (blue < 0x10 ? "0" : "") + String(blue, HEX);

	hexColor.toUpperCase(); // Convert to uppercase (optional)

	return hexColor;
}

String SettingsOptionTheme::rgb565ToHex(uint16_t rgb565)
{
	// Extract red, green, and blue components from the RGB565 value
	uint8_t red = (rgb565 >> 11) & 0x1F;  // 5 bits for red
	uint8_t green = (rgb565 >> 5) & 0x3F; // 6 bits for green
	uint8_t blue = rgb565 & 0x1F;		  // 5 bits for blue

	// Convert to 8-bit per channel RGB values
	red = (red * 255) / 31;		// Scale 5-bit red to 8-bit
	green = (green * 255) / 63; // Scale 6-bit green to 8-bit
	blue = (blue * 255) / 31;	// Scale 5-bit blue to 8-bit

	// Convert each component to a two-character hexadecimal string
	String hexColor = "#";
	hexColor += (red < 0x10 ? "0" : "") + String(red, HEX);
	hexColor += (green < 0x10 ? "0" : "") + String(green, HEX);
	hexColor += (blue < 0x10 ? "0" : "") + String(blue, HEX);

	hexColor.toUpperCase(); // Convert to uppercase (optional)

	return hexColor;
}

uint16_t SettingsOptionTheme::hexToRgb565(String hexColor)
{
	// Remove the '#' character if it's there
	if (hexColor.startsWith("#"))
	{
		hexColor.remove(0, 1);
	}

	// Extract red, green, and blue components from the hex string
	uint8_t red = strtol(hexColor.substring(0, 2).c_str(), NULL, 16);
	uint8_t green = strtol(hexColor.substring(2, 4).c_str(), NULL, 16);
	uint8_t blue = strtol(hexColor.substring(4, 6).c_str(), NULL, 16);

	// Scale the 8-bit values to 5-bit (red and blue) and 6-bit (green)
	red = (red * 31) / 255;		// Scale 8-bit to 5-bit
	green = (green * 63) / 255; // Scale 8-bit to 6-bit
	blue = (blue * 31) / 255;	// Scale 8-bit to 5-bit

	// Combine into a single RGB565 value
	uint16_t rgb565 = (red << 11) | (green << 5) | blue;

	return rgb565;
}

String SettingsOptionTheme::generate_html(uint16_t index)
{
	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn = String(group) + "," + String(index) + "__" + fn;

	String highlight = "";
	String readonly = "required";
	String html = "\n";

	for (size_t i = 0; i < (*setting_ref).size(); i++)
	{
		// 	highlight = (settings.config.current_wifi_station == i) ? " style='background-color:#333388;'" : "";
		// 	readonly = (settings.config.current_wifi_station == i) ? " readonly " : "";
		readonly = " ";
		theme t = get_theme(i);

		html = "						<div class='settings_frame_inner m-1'>\n";
		html += "							<form hx-post='/update_theme' hx-target='#theme_" + String(i) + "' >\n";
		html += "							<input type='hidden' name='theme_id' id='theme_id' value='" + String(i) + "'>\n";
		html += "							<div class='row g-2'>\n";

		html += "								<div class='row'>\n";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm style='width:150px;'>Theme Name</span>\n";
		html += "											<input type='text' class='form-control form-control-sm' id='" + fn + "_name_" + String(i) + "' name='" + fn + "_name_" + String(i) + "' value='" + t.name + "'" + readonly + "/>\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm style='width:150px;'>Widget Style</span>\n";
		html += "											<input type='number' class='form-control form-control-sm' id='" + fn + "_widget_style_" + String(i) + "' name='" + fn + "_widget_style_" + String(i) + "' value=" + t.widget_style + "" + readonly + "/>\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Background Dull</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_background_dull_" + String(i) + "' name='" + fn + "_col_background_dull_" + String(i) + "' value='" + rgb565ToHex(t.col_background_dull) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Background Bright</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_background_bright_" + String(i) + "' name='" + fn + "_col_background_bright_" + String(i) + "' value='" + rgb565ToHex(t.col_background_bright) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Control Back</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_control_back_" + String(i) + "' name='" + fn + "_col_control_back_" + String(i) + "' value='" + rgb565ToHex(t.col_control_back) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Low Intensity</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_low_intensity_" + String(i) + "' name='" + fn + "_col_low_intensity_" + String(i) + "' value='" + rgb565ToHex(t.col_low_intensity) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Primary</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_primary_" + String(i) + "' name='" + fn + "_col_primary_" + String(i) + "' value='" + rgb565ToHex(t.col_primary) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Secondary</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_secondary_" + String(i) + "' name='" + fn + "_col_secondary_" + String(i) + "' value='" + rgb565ToHex(t.col_secondary) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Warning</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_warning_" + String(i) + "' name='" + fn + "_col_warning_" + String(i) + "' value='" + rgb565ToHex(t.col_warning) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "									<div class='col-6 p-2'>";
		html += "										<div class='input-group input-group-sm'>\n";
		html += "											<span class='input-group-text' id='inputGroup-sizing-sm' style='width:150px;'>Error</span>\n";
		html += "											<input type='color' class='form-control form-control-color' id='" + fn + "_col_error_" + String(i) + "' name='" + fn + "_col_error_" + String(i) + "' value='" + rgb565ToHex(t.col_error) + "' />\n";
		html += "										</div>\n";
		html += "									</div>";
		html += "								</div>";

		html += "							</div>\n";
		html += "							<div class='right align-middle' style='height:36px;'>\n";
		html += "								<span class='flash-span me-2' style='display:none; color:green;'>Theme Updated!</span>\n";
		html += "								<button type='submit' class='btn btn-sm btn-success m-1' style='width:100px;'>Update</button>\n";
		html += "							</div>\n";
		html += "							</form>\n";
		html += "						</div>\n\n";
	}

	return html;
}
