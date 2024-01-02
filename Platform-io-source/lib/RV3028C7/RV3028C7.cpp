/*
  Arduino Library for RV-3028-C7

  Original Author: Copyright (c) 2020 Macro Yau
  https://github.com/MacroYau/RV-3028-C7-Arduino-Library

  Modified by Unexpected Maker: 2023-10-7
  - Added the ability to set the time from a tm struct
  - Added the ability to extra date & components by component enum  
*/

#include <time.h>

#include "RV3028C7.h"

RV3028C7::RV3028C7() {}

bool RV3028C7::begin(TwoWire &wirePort)
{
  // Wire.begin() should be called in the application code in advance
  _i2cPort = &wirePort;

  // Reads RESET bit to pseudo-verify that the part is a RV-3028-C7
  uint8_t value = readByteFromRegister(REG_CONTROL_2);
  if ((value & 0x01) == 0x00)
  {
    return true;
  }
  else
  {
    return false;
  }
}

uint32_t RV3028C7::getUnixTimestamp()
{
  uint8_t ts[4];
  readBytesFromRegisters(REG_UNIX_TIME_0, ts, 4);
  uint32_t result1 = (ts[3] << 24) | (ts[2] << 16) | (ts[1] << 8) | ts[0];
  readBytesFromRegisters(REG_UNIX_TIME_0, ts, 4);
  uint32_t result2 = (ts[3] << 24) | (ts[2] << 16) | (ts[1] << 8) | ts[0];
  if (result1 == result2)
  {
    return result1;
  }
  else
  {
    return getUnixTimestamp();
  }
}

bool RV3028C7::setUnixTimestamp(uint32_t secondsSinceEpoch, bool syncCalendar)
{
  bool success = false;

  uint8_t ts[4] = {
      (uint8_t)secondsSinceEpoch, (uint8_t)(secondsSinceEpoch >> 8),
      (uint8_t)(secondsSinceEpoch >> 16), (uint8_t)(secondsSinceEpoch >> 24)};
  success = writeBytesToRegisters(REG_UNIX_TIME_0, ts, 4);

  if (syncCalendar)
  {
    time_t t = secondsSinceEpoch;
    struct tm *dateTime = gmtime(&t);
    success = setDateTime(dateTime->tm_year + 1900, dateTime->tm_mon + 1,
                          dateTime->tm_mday, SUN, dateTime->tm_hour,
                          dateTime->tm_min, dateTime->tm_sec, false);
    success = synchronize();
  }

  return success;
}

char *RV3028C7::getCurrentDateTime()
{
  // Updates RTC date time value to array
  readBytesFromRegisters(REG_CLOCK_SECONDS, _dateTime, DATETIME_COMPONENTS);

  // Returns ISO 8601 date time string
  static char iso8601[23];
  sprintf(iso8601, "20%02d-%02d-%02dT%02d:%02d:%02d",
          convertToDecimal(_dateTime[DATETIME_YEAR]),
          convertToDecimal(_dateTime[DATETIME_MONTH]),
          convertToDecimal(_dateTime[DATETIME_DAY_OF_MONTH]),
          convertToDecimal(_dateTime[DATETIME_HOUR]),
          convertToDecimal(_dateTime[DATETIME_MINUTE]),
          convertToDecimal(_dateTime[DATETIME_SECOND]));
  return iso8601;
}

uint8_t RV3028C7::getCurrentDateTimeComponent(DateTimeComponent_t component)
{
  // Updates RTC date time value to array
  readBytesFromRegisters(REG_CLOCK_SECONDS, _dateTime, DATETIME_COMPONENTS);

  /*
  component:
  DATETIME_SECOND
  DATETIME_MINUTE
  DATETIME_HOUR
  DATETIME_DAY_OF_WEEK
  DATETIME_DAY_OF_MONTH
  DATETIME_MONTH
  DATETIME_YEAR
  */

  return convertToDecimal(_dateTime[component]);
}

void RV3028C7::setDateTimeFromISO8601(String iso8601)
{
  return setDateTimeFromISO8601(iso8601.c_str());
}

void RV3028C7::setDateTimeFromISO8601(const char *iso8601)
{
  // Assumes the input is in the format of "2018-01-01T08:00:00" (hundredths and
  // time zone, if applicable, will be neglected)
  char components[3] = {'0', '0', '\0'};
  uint8_t buffer[6];

  for (uint8_t i = 2, j = 0; i < 20; i += 3, j++)
  {
    components[0] = iso8601[i];
    components[1] = iso8601[i + 1];
    buffer[j] = atoi(components);
  }

  // Since ISO 8601 date string does not indicate day of week, it is set to 0
  // (Sunday) and is no longer correct
  setDateTime(/*year=*/2000 + buffer[0], /*month=*/buffer[1],
              /*dayOfMonth=*/buffer[2], /*dayOfWeek=*/SUN,
              /*hour=*/buffer[3], /*minute=*/buffer[4], /*second=*/buffer[5]);
}

// Set thw time from 
bool RV3028C7::setDateTimeFromTM(tm &timeinfo)
{
  return setDateTime((uint16_t)timeinfo.tm_year+1900, (uint8_t)timeinfo.tm_mon+1, (uint8_t)timeinfo.tm_mday, (uint8_t)timeinfo.tm_wday, (uint8_t)timeinfo.tm_hour, (uint8_t)timeinfo.tm_min, (uint8_t)timeinfo.tm_sec);
}


void RV3028C7::setDateTimeFromHTTPHeader(String str)
{
  return setDateTimeFromHTTPHeader(str.c_str());
}

void RV3028C7::setDateTimeFromHTTPHeader(const char *str)
{
  char components[3] = {'0', '0', '\0'};

  // Checks whether the string begins with "Date: " prefix
  uint8_t counter = 0;
  if (str[0] == 'D')
  {
    counter = 6;
  }

  // Day of week
  uint8_t dayOfWeek = 0;
  if (str[counter] == 'T')
  {
    // Tue or Thu
    if (str[counter + 1] == 'u')
    {
      // Tue
      dayOfWeek = 2;
    }
    else
    {
      dayOfWeek = 4;
    }
  }
  else if (str[counter] == 'S')
  {
    // Sat or Sun
    if (str[counter + 1] == 'a')
    {
      dayOfWeek = 6;
    }
    else
    {
      dayOfWeek = 0;
    }
  }
  else if (str[counter] == 'M')
  {
    // Mon
    dayOfWeek = 1;
  }
  else if (str[counter] == 'W')
  {
    // Wed
    dayOfWeek = 3;
  }
  else
  {
    // Fri
    dayOfWeek = 5;
  }

  // Day of month
  counter += 5;
  components[0] = str[counter];
  components[1] = str[counter + 1];
  uint8_t dayOfMonth = atoi(components);

  // Month
  counter += 3;
  uint8_t month = 0;
  if (str[counter] == 'J')
  {
    // Jan, Jun, or Jul
    if (str[counter + 1] == 'a')
    {
      // Jan
      month = 1;
    }
    else if (str[counter + 2] == 'n')
    {
      // Jun
      month = 6;
    }
    else
    {
      // Jul
      month = 7;
    }
  }
  else if (str[counter] == 'F')
  {
    // Feb
    month = 2;
  }
  else if (str[counter] == 'M')
  {
    // Mar or May
    if (str[counter + 2] == 'r')
    {
      // Mar
      month = 3;
    }
    else
    {
      // May
      month = 5;
    }
  }
  else if (str[counter] == 'A')
  {
    // Apr or Aug
    if (str[counter + 1] == 'p')
    {
      // Apr
      month = 4;
    }
    else
    {
      // Aug
      month = 8;
    }
  }
  else if (str[counter] == 'S')
  {
    // Sep
    month = 9;
  }
  else if (str[counter] == 'O')
  {
    // Oct
    month = 10;
  }
  else if (str[counter] == 'N')
  {
    // Nov
    month = 11;
  }
  else
  {
    month = 12;
  }

  // Year
  counter += 6;
  components[0] = str[counter];
  components[1] = str[counter + 1];
  uint16_t year = 2000 + atoi(components);

  // Time of day
  counter += 3;
  uint8_t buffer[3];
  for (uint8_t i = counter, j = 0; j < 3; i += 3, j++)
  {
    components[0] = str[i];
    components[1] = str[i + 1];
    buffer[j] = atoi(components);
  }

  setDateTime(year, month, dayOfMonth, static_cast<DayOfWeek_t>(dayOfWeek),
              /*hour=*/buffer[0], /*minute=*/buffer[1], /*second=*/buffer[2]);
}

bool RV3028C7::setDateTime(uint16_t year, uint8_t month, uint8_t dayOfMonth,
                           DayOfWeek_t dayOfWeek, uint8_t hour, uint8_t minute,
                           uint8_t second, bool syncUnixTime)
{
  // Year 2000 AD is the earliest allowed year in this implementation
  if (year < 2000)
  {
    return false;
  }
  // Century overflow is not considered yet (i.e., only supports year 2000 to
  // 2099)
  _dateTime[DATETIME_YEAR] = convertToBCD(year - 2000);

  if (month < 1 || month > 12)
  {
    return false;
  }
  _dateTime[DATETIME_MONTH] = convertToBCD(month);

  if (dayOfMonth < 1 || dayOfMonth > 31)
  {
    return false;
  }
  _dateTime[DATETIME_DAY_OF_MONTH] = convertToBCD(dayOfMonth);

  if (dayOfWeek > 6)
  {
    return false;
  }
  _dateTime[DATETIME_DAY_OF_WEEK] = convertToBCD(dayOfWeek);

  // Uses 24-hour notation by default
  if (hour > 23)
  {
    return false;
  }
  _dateTime[DATETIME_HOUR] = convertToBCD(hour);

  if (minute > 59)
  {
    return false;
  }
  _dateTime[DATETIME_MINUTE] = convertToBCD(minute);

  if (second > 59)
  {
    return false;
  }
  _dateTime[DATETIME_SECOND] = convertToBCD(second);

  if (syncUnixTime)
  {
    uint32_t unixTimestamp =
        convertToUnixTimestamp(year, month, dayOfMonth, hour, minute, second);
    setUnixTimestamp(unixTimestamp, false);
  }

  return true;
}

void RV3028C7::setDateTimeComponent(DateTimeComponent_t component,
                                    uint8_t value)
{
  // Updates RTC date time value to array
  readBytesFromRegisters(REG_CLOCK_SECONDS, _dateTime, DATETIME_COMPONENTS);

  _dateTime[component] = convertToBCD(value);
}

bool RV3028C7::synchronize()
{
  return writeBytesToRegisters(REG_CLOCK_SECONDS, _dateTime,
                               DATETIME_COMPONENTS);
}

bool RV3028C7::enableClockOutput(ClockOutputFrequency_t frequency)
{
  uint8_t clkout = readByteFromEEPROM(REG_EEPROM_CLKOUT);
  if ((clkout & (1 << BP_REG_EEPROM_CLKOUT_CLKOE)) > 0 &&
      (clkout & BM_REG_EEPROM_CLKOUT_FD) == frequency)
  {
    // Already enabled at desired frequency
    return true;
  }

  clkout |= (1 << BP_REG_EEPROM_CLKOUT_CLKOE); // Enables clock output
  clkout |= (1 << BP_REG_EEPROM_CLKOUT_CLKSY); // Enables synchronized output

  // Sets to selected frequency
  clkout &= ~BM_REG_EEPROM_CLKOUT_FD;
  clkout |= frequency;

  return writeByteToRegister(REG_EEPROM_CLKOUT, clkout) &&
         writeByteToEEPROM(REG_EEPROM_CLKOUT, clkout);
}

bool RV3028C7::disableClockOutput()
{
  uint8_t clkout = readByteFromEEPROM(REG_EEPROM_CLKOUT);
  if ((clkout & (1 << BP_REG_EEPROM_CLKOUT_CLKOE)) == 0)
  {
    // Already disabled
    return true;
  }

  clkout &= ~(1 << BP_REG_EEPROM_CLKOUT_CLKOE); // Disables clock output
  clkout |= BM_REG_EEPROM_CLKOUT_FD;            // Resets to LOW frequency mode

  return writeByteToRegister(REG_EEPROM_CLKOUT, clkout) &&
         writeByteToEEPROM(REG_EEPROM_CLKOUT, clkout);
}

bool RV3028C7::setDateAlarm(AlarmMode_t mode, uint8_t dayOfMonth, uint8_t hour,
                            uint8_t minute)
{
  // Clears AIE and AF bits to 0
  uint8_t status = readByteFromRegister(REG_STATUS);
  if (!writeByteToRegister(REG_STATUS, (status & ~(1 << BP_REG_STATUS_AF))))
  {
    return false;
  }
  uint8_t control2 = readByteFromRegister(REG_CONTROL_2);
  if (!writeByteToRegister(REG_CONTROL_2,
                           (control2 & ~(1 << BP_REG_CONTROL_2_AIE))))
  {
    return false;
  }

  // Sets WADA bit to 1
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  if (!writeByteToRegister(REG_CONTROL_1,
                           (control1 | (1 << BP_REG_CONTROL_1_WADA))))
  {
    return false;
  }

  // Sets alarm registers from arguments
  uint8_t minutesAlarm = convertToBCD(minute);
  uint8_t hoursAlarm = convertToBCD(hour);
  uint8_t dateAlarm = convertToBCD(dayOfMonth);

  // Sets AE_x bits according to alarm mode
  if (mode == ALARM_ONCE_PER_DAY_OF_MONTH_OR_WEEK)
  {
    dateAlarm &= ~(1 << BP_ALARM_AE);    // AE_WD = 0
    hoursAlarm &= ~(1 << BP_ALARM_AE);   // AE_H = 0
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else if (mode == ALARM_ONCE_PER_HOUR_PER_DAY_OF_MONTH_OR_WEEK)
  {
    dateAlarm &= ~(1 << BP_ALARM_AE);    // AE_WD = 0
    hoursAlarm |= (1 << BP_ALARM_AE);    // AE_H = 1
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else if (mode == ALARM_ONCE_PER_DAY)
  {
    dateAlarm |= (1 << BP_ALARM_AE);     // AE_WD = 1
    hoursAlarm &= ~(1 << BP_ALARM_AE);   // AE_H = 0
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else if (mode == ALARM_ONCE_PER_HOUR)
  {
    dateAlarm |= (1 << BP_ALARM_AE);     // AE_WD = 1
    hoursAlarm |= (1 << BP_ALARM_AE);    // AE_H = 1
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else
  {
    // All disabled
    dateAlarm |= (1 << BP_ALARM_AE);    // AE_WD = 1
    hoursAlarm |= (1 << BP_ALARM_AE);   // AE_H = 1
    minutesAlarm |= (1 << BP_ALARM_AE); // AE_M = 1
  }

  uint8_t alarmRegisters[3] = {minutesAlarm, hoursAlarm, dateAlarm};
  return writeBytesToRegisters(REG_ALARM_MINUTES, alarmRegisters, 3);
}

bool RV3028C7::setWeekdayAlarm(AlarmMode_t mode, DayOfWeek_t dayOfWeek,
                               uint8_t hour, uint8_t minute)
{
  // Clears AIE and AF bits to 0
  uint8_t status = readByteFromRegister(REG_STATUS);
  if (!writeByteToRegister(REG_STATUS, (status & ~(1 << BP_REG_STATUS_AF))))
  {
    return false;
  }
  uint8_t control2 = readByteFromRegister(REG_CONTROL_2);
  if (!writeByteToRegister(REG_CONTROL_2,
                           (control2 & ~(1 << BP_REG_CONTROL_2_AIE))))
  {
    return false;
  }

  // Sets WADA bit to 0
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  if (!writeByteToRegister(REG_CONTROL_1,
                           (control1 & ~(1 << BP_REG_CONTROL_1_WADA))))
  {
    return false;
  }

  // Sets alarm registers from arguments
  uint8_t minutesAlarm = convertToBCD(minute);
  uint8_t hoursAlarm = convertToBCD(hour);
  uint8_t weekdayAlarm = convertToBCD(dayOfWeek);

  // Sets AE_x bits according to alarm mode
  if (mode == ALARM_ONCE_PER_DAY_OF_MONTH_OR_WEEK)
  {
    weekdayAlarm &= ~(1 << BP_ALARM_AE); // AE_WD = 0
    hoursAlarm &= ~(1 << BP_ALARM_AE);   // AE_H = 0
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else if (mode == ALARM_ONCE_PER_HOUR_PER_DAY_OF_MONTH_OR_WEEK)
  {
    weekdayAlarm &= ~(1 << BP_ALARM_AE); // AE_WD = 0
    hoursAlarm |= (1 << BP_ALARM_AE);    // AE_H = 1
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else if (mode == ALARM_ONCE_PER_DAY)
  {
    weekdayAlarm |= (1 << BP_ALARM_AE);  // AE_WD = 1
    hoursAlarm &= ~(1 << BP_ALARM_AE);   // AE_H = 0
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else if (mode == ALARM_ONCE_PER_HOUR)
  {
    weekdayAlarm |= (1 << BP_ALARM_AE);  // AE_WD = 1
    hoursAlarm |= (1 << BP_ALARM_AE);    // AE_H = 1
    minutesAlarm &= ~(1 << BP_ALARM_AE); // AE_M = 0
  }
  else
  {
    // All disabled
    weekdayAlarm |= (1 << BP_ALARM_AE); // AE_WD = 1
    hoursAlarm |= (1 << BP_ALARM_AE);   // AE_H = 1
    minutesAlarm |= (1 << BP_ALARM_AE); // AE_M = 1
  }

  uint8_t alarmRegisters[3] = {minutesAlarm, hoursAlarm, weekdayAlarm};
  return writeBytesToRegisters(REG_ALARM_MINUTES, alarmRegisters, 3);
}

bool RV3028C7::setDailyAlarm(uint8_t hour, uint8_t minute)
{
  return setDateAlarm(ALARM_ONCE_PER_DAY, 1, hour, minute);
}

bool RV3028C7::setHourlyAlarm(uint8_t minute)
{
  return setDateAlarm(ALARM_ONCE_PER_HOUR, 1, 0, minute);
}

bool RV3028C7::disableAlarm() { return setDateAlarm(ALARM_DISABLED, 1, 0, 0); }

bool RV3028C7::setPeriodicCountdownTimer(uint16_t timerValue,
                                         TimerClockFrequency_t frequency,
                                         bool repeat)
{
  if (timerValue < 1)
  {
    return false;
  }

  // Clears TE, TIE, and TF bits to 0
  uint8_t status = readByteFromRegister(REG_STATUS);
  if (!writeByteToRegister(REG_STATUS, (status & ~(1 << BP_REG_STATUS_TF))))
  {
    return false;
  }
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  if (!writeByteToRegister(REG_CONTROL_1,
                           (control1 & ~(1 << BP_REG_CONTROL_1_TE))))
  {
    return false;
  }
  uint8_t control2 = readByteFromRegister(REG_CONTROL_2);
  if (!writeByteToRegister(REG_CONTROL_2,
                           (control2 & ~(1 << BP_REG_CONTROL_2_TIE))))
  {
    return false;
  }

  // Sets TRPT bit to 1 for a repeating timer
  if (repeat)
  {
    control1 |= (1 << BP_REG_CONTROL_1_TRPT);
  }
  else
  {
    control1 &= ~(1 << BP_REG_CONTROL_1_TRPT);
  }

  // Sets timer value and clock frequency
  if (!writeByteToRegister(REG_TIMER_VALUE_0, (uint8_t)timerValue))
  {
    return false;
  }
  if (!writeByteToRegister(REG_TIMER_VALUE_1, (uint8_t)(timerValue >> 8)))
  {
    return false;
  }
  control1 |= (frequency << BP_REG_CONTROL_1_TD_LSB);
  return writeByteToRegister(REG_CONTROL_1, control1);
}

bool RV3028C7::startPeriodicCountdownTimer()
{
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  control1 |= (1 << BP_REG_CONTROL_1_TE);
  return writeByteToRegister(REG_CONTROL_1, control1);
}

bool RV3028C7::stopPeriodicCountdownTimer()
{
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  control1 &= ~(1 << BP_REG_CONTROL_1_TE);
  return writeByteToRegister(REG_CONTROL_1, control1);
}

bool RV3028C7::enableInterrupt(InterruptType_t type)
{
  uint8_t control2 = readByteFromRegister(REG_CONTROL_2);
  control2 |= (1 << type);
  return writeByteToRegister(REG_CONTROL_2, control2);
}

bool RV3028C7::disableInterrupt(InterruptType_t type)
{
  uint8_t control2 = readByteFromRegister(REG_CONTROL_2);
  control2 &= ~(1 << type);
  return writeByteToRegister(REG_CONTROL_2, control2);
}

bool RV3028C7::disableAllInterrupts()
{
  uint8_t control2 = readByteFromRegister(REG_CONTROL_2);
  control2 &= ~BM_REG_CONTROL_2_INTERRUPT_ENABLE_ALL;
  return writeByteToRegister(REG_CONTROL_2, control2);
}

bool RV3028C7::isInterruptDetected(InterruptType_t type)
{
  uint8_t status = readByteFromRegister(REG_STATUS);
  status &= (1 << (type - 1)); // Interrupt flag bit positions are offset by 1
  return (status > 0);
}

bool RV3028C7::clearInterrupt(InterruptType_t type)
{
  uint8_t status = readByteFromRegister(REG_STATUS);
  status &= ~(1 << (type - 1));
  return writeByteToRegister(REG_STATUS, status);
}

bool RV3028C7::clearAllInterrupts()
{
  uint8_t status = readByteFromRegister(REG_STATUS);
  status &= ~BM_REG_STATUS_INTERRUPT_FLAGS;
  return writeByteToRegister(REG_STATUS, status);
}

uint32_t RV3028C7::convertToUnixTimestamp(uint16_t year, uint8_t month,
                                          uint8_t dayOfMonth, uint8_t hour,
                                          uint8_t minute, uint8_t second)
{
  struct tm tm;
  tm.tm_sec = second;
  tm.tm_min = minute;
  tm.tm_hour = hour;
  tm.tm_mday = dayOfMonth;
  tm.tm_mon = month - 1;
  tm.tm_year = year - 1900;
  tm.tm_isdst = 0;
  return mktime(&tm);
}

uint8_t RV3028C7::convertToDecimal(uint8_t bcd)
{
  return (bcd / 16 * 10) + (bcd % 16);
}

uint8_t RV3028C7::convertToBCD(uint8_t decimal)
{
  return (decimal / 10 * 16) + (decimal % 10);
}

bool RV3028C7::waitForEEPROM(uint8_t timeoutMS)
{
  uint32_t deadline = millis() + timeoutMS;
  bool timeout = false;
  while ((readByteFromRegister(REG_STATUS) & (1 << BP_REG_STATUS_EEBUSY)) > 0)
  {
    if (millis() > deadline)
    {
      return false;
    }
  }
  return true;
}

bool RV3028C7::enableEEPROMAutoRefresh()
{
  // Enables auto refresh by clearing EERD control bit to 0
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  control1 &= ~(1 << BP_REG_CONTROL_1_EERD);
  return writeByteToRegister(REG_CONTROL_1, control1);
}

bool RV3028C7::disableEEPROMAutoRefresh()
{
  // Disables auto refresh by setting EERD control bit to 1
  uint8_t control1 = readByteFromRegister(REG_CONTROL_1);
  control1 |= (1 << BP_REG_CONTROL_1_EERD);
  return writeByteToRegister(REG_CONTROL_1, control1);
}

bool RV3028C7::refreshConfigurationEEPROMToRAM()
{
  // Disables auto refresh
  if (!disableEEPROMAutoRefresh())
  {
    return false;
  }

  // Ensures EEPROM is not busy
  if (!waitForEEPROM())
  {
    return false;
  }

  // Sends first EECMD
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_FIRST))
  {
    return false;
  }

  // Sends refresh EECMD and waits ~3.5 ms
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_REFRESH))
  {
    return false;
  }
  if (waitForEEPROM())
  {
    // Re-enables auto refresh
    enableEEPROMAutoRefresh();
  }

  return true;
}

bool RV3028C7::updateConfigurationEEPROMFromRAM()
{
  // Disables auto refresh
  if (!disableEEPROMAutoRefresh())
  {
    return false;
  }

  // Ensures EEPROM is not busy
  if (!waitForEEPROM())
  {
    return false;
  }

  // Sends first EECMD
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_FIRST))
  {
    return false;
  }

  // Sends update EECMD and waits ~63 ms
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_UPDATE))
  {
    return false;
  }
  if (waitForEEPROM())
  {
    // Re-enables auto refresh
    enableEEPROMAutoRefresh();
  }

  return true;
}

uint8_t RV3028C7::readByteFromEEPROM(uint8_t address)
{
  // Disables auto refresh
  if (!disableEEPROMAutoRefresh())
  {
    return false;
  }

  // Ensures EEPROM is not busy
  if (!waitForEEPROM())
  {
    return false;
  }

  // Writes address to EEADDR register
  if (!writeByteToRegister(REG_EE_ADDRESS, address))
  {
    return false;
  }

  // Sends first EECMD
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_FIRST))
  {
    return false;
  }

  // Sends update EECMD and waits ~1.4 ms
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_READ_ONE_EEPROM_BYTE))
  {
    return false;
  }
  if (waitForEEPROM())
  {
    // Re-enables auto refresh
    enableEEPROMAutoRefresh();
  }

  return readByteFromRegister(REG_EE_DATA);
}

bool RV3028C7::writeByteToEEPROM(uint8_t address, uint8_t value)
{
  // Disables auto refresh
  if (!disableEEPROMAutoRefresh())
  {
    return false;
  }

  // Ensures EEPROM is not busy
  if (!waitForEEPROM())
  {
    return false;
  }

  // Writes address to EEADDR register
  if (!writeByteToRegister(REG_EE_ADDRESS, address))
  {
    return false;
  }

  // Writes data to EEDATA register
  if (!writeByteToRegister(REG_EE_DATA, value))
  {
    return false;
  }

  // Sends first EECMD
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_FIRST))
  {
    return false;
  }

  // Sends update EECMD and waits ~16 ms
  if (!writeByteToRegister(REG_EE_COMMAND, EECMD_WRITE_ONE_EEPROM_BYTE))
  {
    return false;
  }
  if (waitForEEPROM())
  {
    // Re-enables auto refresh
    enableEEPROMAutoRefresh();
  }

  return true;
}

bool RV3028C7::readBytesFromRegisters(uint8_t startAddress,
                                      uint8_t *destination, uint8_t length)
{
  _i2cPort->beginTransmission(RV3028C7_ADDRESS);
  _i2cPort->write(startAddress);
  _i2cPort->endTransmission(false);

  _i2cPort->requestFrom((uint8_t)RV3028C7_ADDRESS, length);
  for (uint8_t i = 0; i < length; i++)
  {
    destination[i] = _i2cPort->read();
  }
  return (_i2cPort->endTransmission() == 0);
}

bool RV3028C7::writeBytesToRegisters(uint8_t startAddress, uint8_t *values,
                                     uint8_t length)
{
  _i2cPort->beginTransmission(RV3028C7_ADDRESS);
  _i2cPort->write(startAddress);
  for (uint8_t i = 0; i < length; i++)
  {
    _i2cPort->write(values[i]);
  }
  return (_i2cPort->endTransmission() == 0);
}

uint8_t RV3028C7::readByteFromRegister(uint8_t address)
{
  uint8_t value = 0;

  _i2cPort->beginTransmission(RV3028C7_ADDRESS);
  _i2cPort->write(address);
  _i2cPort->endTransmission(false);

  _i2cPort->requestFrom(RV3028C7_ADDRESS, 1);
  value = _i2cPort->read();
  _i2cPort->endTransmission();

  return value;
}

bool RV3028C7::writeByteToRegister(uint8_t address, uint8_t value)
{
  _i2cPort->beginTransmission(RV3028C7_ADDRESS);
  _i2cPort->write(address);
  _i2cPort->write(value);
  return (_i2cPort->endTransmission() == 0);
}