#include "rtc.h"
#include "esp32s3/rom/rtc.h"
#include "utilities/logging.h"

bool interruptTriggered = true;
const char *ntpServer = "pool.ntp.org";

String days[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
String months[12] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

bool RTC::init()
{
	if (rtc.begin() == false)
	{
		info_println("Failed to detect RV-3028-C7!");
		return false;
	}

	info_println("RV-3028-C7 connected!");

	enabled = true;

	interruptTriggered = false;

	// pinMode(RTC_INT, INPUT_PULLUP);
	// attachInterrupt(digitalPinToInterrupt(RTC_INT), rtcInterrupt, FALLING);

	requiresNTP = rtc.getCurrentDateTimeComponent(DATETIME_YEAR) < 23;

	next_rtc_read = millis();

	return true;
}

bool RTC::set_time_from_NTP(int16_t utc_offset)
{
	if (!enabled)
		return false;

	bool time_error = false;

	if (utc_offset != 999)
	{
		configTime(utc_offset * 3600, 0, ntpServer);
		delay(100);

		struct tm timeinfo;
		if (!getLocalTime(&timeinfo))
		{
			info_println("Failed to obtain time");
			time_error = true;
		}
		else
		{
			rtc.setDateTimeFromTM(timeinfo);
		}
	}
	else
		time_error = true;

	if (time_error)
	{
		// Hard-coded for time error
		rtc.setDateTimeFromISO8601("1900-01-01T00:00:00");
	}

	// Writes the new date time to RTC, good or bad
	rtc.synchronize();

	return !time_error;
}

void RTC::set_hourly_alarm(uint minuets)
{
	setup_interrupt();

	rtc.setHourlyAlarm(/*minute=*/minuets);
	rtc.enableInterrupt(INTERRUPT_ALARM);
}

String RTC::get_hours_string(bool padded, bool is24hour)
{
	int hour = rtc.getCurrentDateTimeComponent(DATETIME_HOUR);
	if (!is24hour && hour > 12)
		hour -= 12;
	String txt = String(rtc.getCurrentDateTimeComponent(DATETIME_HOUR));
	if (padded && hour < 10)
		return "0" + String(hour);
	return String(hour);
	;
}

String RTC::get_mins_string(bool padded)
{
	String txt = String(rtc.getCurrentDateTimeComponent(DATETIME_MINUTE));
	if (padded && txt.length() < 2)
		return "0" + txt;
	return txt;
}

String RTC::get_secs_string(bool padded)
{
	String txt = String(rtc.getCurrentDateTimeComponent(DATETIME_SECOND));
	if (padded && txt.length() < 2)
		return "0" + txt;
	return txt;
}

String RTC::get_time_string(bool padded, bool is24hour)
{
	String txt = get_hours_string(false, is24hour);
	txt += ":";
	txt += get_mins_string(padded);

	return txt;
}

int RTC::get_hours() { return (rtc.getCurrentDateTimeComponent(DATETIME_HOUR)); }

int RTC::get_mins() { return (rtc.getCurrentDateTimeComponent(DATETIME_MINUTE)); }

int RTC::get_seconds() { return (rtc.getCurrentDateTimeComponent(DATETIME_SECOND)); }

uint16_t RTC::get_day() { return (rtc.getCurrentDateTimeComponent(DATETIME_DAY_OF_MONTH)); }

uint16_t RTC::get_month() { return (rtc.getCurrentDateTimeComponent(DATETIME_MONTH)); }

uint16_t RTC::get_year() { return ((uint16_t)rtc.getCurrentDateTimeComponent(DATETIME_YEAR) + 2000); }

void RTC::get_step_date(uint16_t &day, uint16_t &month, uint16_t &year)
{
	if (millis() - next_rtc_read > 5000)
	{
		next_rtc_read = millis();
		cached_day = get_day();
		cached_month = get_month();
		cached_year = get_year();
	}

	day = cached_day;
	month = cached_month;
	year = cached_year;
}

void rtcInterrupt() { interruptTriggered = true; }

void RTC::setup_interrupt()
{
	interruptTriggered = false;
	attachInterrupt(digitalPinToInterrupt(interruptPin), rtcInterrupt, FALLING);
}

String RTC::get_day_date()
{
	String raw = days[rtc.getCurrentDateTimeComponent(DATETIME_DAY_OF_WEEK)];
	raw += " ";
	raw += months[rtc.getCurrentDateTimeComponent(DATETIME_MONTH) - 1];
	raw += " ";
	raw += String(rtc.getCurrentDateTimeComponent(DATETIME_DAY_OF_MONTH));
	raw += " ";
	raw += String(2000 + rtc.getCurrentDateTimeComponent(DATETIME_YEAR));

	// Mon Oct 30 2023
	return raw;
}

String RTC::get_day_of_week() { return days[rtc.getCurrentDateTimeComponent(DATETIME_DAY_OF_WEEK)]; }

String RTC::get_month_date()
{
	String raw = months[rtc.getCurrentDateTimeComponent(DATETIME_MONTH) - 1].substring(0, 3);
	raw += " ";
	raw += String(rtc.getCurrentDateTimeComponent(DATETIME_DAY_OF_MONTH));
	return raw;
}

bool RTC::check_interrupt()
{
	if (!interruptTriggered)
	{
		return false;
	}

	interruptTriggered = false;
	if (rtc.isInterruptDetected(INTERRUPT_ALARM))
	{
		info_print("Alarm: ");
		// info_println(getTime());
		rtc.clearInterrupt(INTERRUPT_ALARM);
	}

	return true;
}

RTC rtc;
