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
	return "<div><span>SettingsOptionInt for " + fieldname + "</span></div>";
}

void SettingsOptionInt::register_option(int grp)
{
	if (settings.setting_groups.size() < grp + 1)
		settings.setting_groups.push_back({});

	settings.setting_groups[grp].push_back(this);
}

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
	// info_println("int: "+String(*setting_ref));
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
	// return "<div><span>SettingsOptionIntVector for " + String(fieldname) + "</span></div>";

	// char *form_element_id = "";
	// convert_for_form_id(fieldname, form_element_id);

	// info_printf("fieldname: %s\nelement_id: %s\n\n", fieldname, form_element_id);

	String fn = fieldname;
	fn.replace(" ", "_");
	fn.toLowerCase();
	fn.replace("_(sec)", "");
	fn.replace("_(%%)", "");
	// fn = "_set_" + String(index) + "_" + fn;
	fn = String(group) + "," + String(index) + "__" + fn;

	// info_printf("fieldname: %s\nelement_id: %s\n\n", fieldname, fn);

	String html = "					<div class='input-group input-group-sm mb-1'>\n";
	html += "						<div class='row g-2'>\n";
	html += "							<div class='col-sm-12 ps-3 pt-2 pb-0' style='font-size:14px;'>" + fieldname + "</div>\n";
	for (size_t i = 0; i < (*setting_ref).size(); i++)
	{
		html += "								<div class='col-sm-4 form-floating'>\n";
		html += "									<input type='number' class='form-control form-control-sm' id='" + fn + "_" + String(i) + "' name='" + fn + "_" + String(i) + "'value='" + String(get(i)) + "' min='" + String(value_min) + "' max='" + String(value_max) + "' required onchange='' />\n";
		html += "									<label class='ms-1' for='" + fn + "_" + String(i) + "'>Step " + String(i + 1) + "</label>\n";
		html += "								</div>\n";
	}
	html += "							</div>\n";
	html += "						</div>\n";

	// info_println(html);

	return html;
}

void SettingsOptionIntVector::register_option(int grp)
{
	if (settings.setting_groups.size() < grp + 1)
		settings.setting_groups.push_back({});

	settings.setting_groups[grp].push_back(this);
}

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

void SettingsOptionFloat::register_option(int grp)
{
	if (settings.setting_groups.size() < grp + 1)
		settings.setting_groups.push_back({});

	settings.setting_groups[grp].push_back(this);
}

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
	// fn = "_set_" + String(index) + "_" + fn;
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

	// return "<div><span>SettingsOptionBool for " +
	//    fieldname + "</span></div>";

	return html;
}

/*
	<div class='input-group input-group-sm mb-1'>
		<span class='input-group-text' id='inputGroup-sizing-sm'>Enabled</span>
		<select class='form-select form-select-sm' id='_set_widget_ow_enable' name='_set_widget_ow_enable'  onchange='set_input_states();'>
			<option value='1' %SET_WID_OW_ENABLE_YES%>YES</option>
			<option value='0' %SET_WID_OW_ENABLE_NO%>NO</option>
		</select>
	</div>
*/

void SettingsOptionBool::register_option(int grp)
{
	if (settings.setting_groups.size() < grp + 1)
		settings.setting_groups.push_back({});

	settings.setting_groups[grp].push_back(this);
}

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

	setting_ref = val;
	settings.save(false);

	return true;
}

String SettingsOptionString::get() { return *setting_ref; }

String SettingsOptionString::generate_html(uint16_t index)
{
	return "<div><span>SettingsOptionString for " + fieldname + "</span></div>";
}

void SettingsOptionString::register_option(int grp)
{
	if (settings.setting_groups.size() < grp + 1)
		settings.setting_groups.push_back({});

	settings.setting_groups[grp].push_back(this);
}
