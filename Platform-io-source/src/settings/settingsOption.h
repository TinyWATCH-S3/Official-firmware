#pragma once

#include "Arduino.h"
#include "json.h"
#include "utilities/logging.h"
#include <vector>

class SettingsOptionBase
{

	public:
		// virtual methods
		virtual ~SettingsOptionBase() = default;
		virtual String generate_html(uint16_t index) { return ""; }

		enum Type
		{
			BASE,
			INT,
			BOOL,
			FLOAT,
			STRING,
			INT_VECTOR,
			INT_RANGE
		};

		virtual Type getType() const
		{
			return BASE;
		}

		void convert_for_form_id(const char *input, char *output)
		{
			int i = 0;
			while (input[i] != '\0')
			{
				if (output[i] == ' ')
					output[i] = '_';
				else
					output[i] = tolower(input[i]);
				i++;
			}
			output[i] = '\0'; // Null-terminate the output string
		}

		void register_option(int grp);

		String fieldname = "";
		int group = -1;
		bool data_is_vector = false;
};

class SettingsOptionInt : public SettingsOptionBase
{
	public:
		SettingsOptionInt(int *val, int val_min, int val_max, bool wrap, int _group, const String &_fn) : setting_ref(val), value_min(val_min), value_max(val_max), value_wrap(wrap)
		{
			group = _group;
			fieldname = _fn;
			register_option(_group);
		}

		Type getType() const override
		{
			return INT;
		}

		int change(int dir);
		bool update(int val);
		int get();
		String get_str();
		String generate_html(uint16_t index);

	private:
		int *setting_ref = nullptr;
		int value_min = 0;
		int value_max = 0;
		bool value_wrap = false;
};

class SettingsOptionIntRange : public SettingsOptionBase
{
	public:
		SettingsOptionIntRange(int *val, int val_min, int val_max, int val_step, bool wrap, int _group, const String &_fn) : setting_ref(val), value_min(val_min), value_max(val_max), value_step(val_step), value_wrap(wrap)
		{
			group = _group;
			fieldname = _fn;
			register_option(_group);
		}

		Type getType() const override
		{
			return INT_RANGE;
		}

		int change(int dir);
		bool update(int val);
		int get();
		String get_str();
		String generate_html(uint16_t index);

	private:
		int *setting_ref = nullptr;
		int value_min = 0;
		int value_max = 0;
		int value_step = 1;
		bool value_wrap = false;
};

class SettingsOptionIntVector : public SettingsOptionBase
{
	public:
		SettingsOptionIntVector(std::vector<int> *val, int val_min, int val_max, int val_step, bool wrap, int _group, const String &_fn) : setting_ref(val), value_min(val_min), value_max(val_max), value_step(val_step), value_wrap(wrap)
		{
			group = _group;
			fieldname = _fn;
			data_is_vector = true;
			register_option(_group);
		}

		Type getType() const override
		{
			return INT_VECTOR;
		}

		int change(int index, int dir);
		bool update(int index, int val);
		int get(int index);
		String get_str(int index);
		String generate_html(uint16_t index);
		uint8_t vector_size();

	private:
		std::vector<int> *setting_ref = nullptr;
		int value_min = 0;
		int value_max = 0;
		int value_step = 1;
		bool value_wrap = false;

		//  String html = "";
};

class SettingsOptionFloat : public SettingsOptionBase
{
	public:
		SettingsOptionFloat(float *val, float val_min, float val_max, float val_step, bool wrap, int _group, const String &_fn) : setting_ref(val), value_min(val_min), value_max(val_max), value_step(val_step), value_wrap(wrap)
		{
			group = _group;
			fieldname = _fn;
			register_option(_group);
		}

		Type getType() const override
		{
			return FLOAT;
		}

		float change(int dir);
		bool update(float val);
		float get();
		String get_str();
		String generate_html(uint16_t index);

	private:
		float *setting_ref = nullptr;
		float value_min = 0;
		float value_max = 0;
		float value_step = 1;
		bool value_wrap = false;
};

class SettingsOptionBool : public SettingsOptionBase
{
	public:
		SettingsOptionBool(bool *val, int _group, const String &_fn, const String &_op1, const String &_op2) : setting_ref(val), option1(_op1), option2(_op2)
		{
			group = _group;
			fieldname = _fn;
			register_option(_group);
		}

		Type getType() const override
		{
			return BOOL;
		}

		bool change();
		bool update(bool val);
		bool get();
		String get_str();
		String get_fn();
		String get_op1();
		String get_op2();
		String generate_html(uint16_t index);

	private:
		bool *setting_ref = nullptr;

		String option1 = "";
		String option2 = "";
};

class SettingsOptionString : public SettingsOptionBase
{
	public:
		SettingsOptionString(String *val, int _group, const String &_fn) : setting_ref(val)
		{
			group = _group;
			fieldname = _fn;
			register_option(_group);
		}

		Type getType() const override
		{
			return STRING;
		}

		void change(String *val);
		bool update(String *val);
		String get();
		String generate_html(uint16_t index);

	private:
		String *setting_ref = nullptr;
};
