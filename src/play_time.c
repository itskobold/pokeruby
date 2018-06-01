#include "global.h"
#include "menu.h"
#include "play_time.h"
#include "start_menu.h"

const u8 gSunriseTimes[8] = {TIME_HOUR_8AM, TIME_HOUR_7AM, TIME_HOUR_6AM, TIME_HOUR_7AM, TIME_HOUR_8AM, TIME_HOUR_9AM, TIME_HOUR_10AM, TIME_HOUR_9AM};
const u8 gSunsetTimes[8] = {TIME_HOUR_6PM, TIME_HOUR_7PM, TIME_HOUR_8PM, TIME_HOUR_7PM, TIME_HOUR_6PM, TIME_HOUR_5PM, TIME_HOUR_4PM, TIME_HOUR_5PM};

enum
{
    STOPPED,
    RUNNING,
    MAXED_OUT
};

static u8 sPlayTimeCounterState;

static void RunMinuteRoutines(void);
static void RunHourRoutines(void);

void PlayTimeCounter_Reset()
{
    sPlayTimeCounterState = STOPPED;

    gSaveBlock2.playTimeHours = 0;
    gSaveBlock2.playTimeMinutes = 0;
    gSaveBlock2.playTimeSeconds = 0;
    gSaveBlock2.playTimeVBlanks = 0;
}

void PlayTimeCounter_Loop()
{
    gSaveBlock2.playTimeHours = 0;
    gSaveBlock2.playTimeMinutes = 0;
    gSaveBlock2.playTimeSeconds = 0;
    gSaveBlock2.playTimeVBlanks = 0;
}

void PlayTimeCounter_Start()
{
    sPlayTimeCounterState = RUNNING;

    if (gSaveBlock2.playTimeHours > 65535)
		PlayTimeCounter_Loop();
}

void PlayTimeCounter_Stop()
{
    sPlayTimeCounterState = STOPPED;
}

void PlayTimeCounter_Update()
{
    if (sPlayTimeCounterState == RUNNING)
    {		
        gSaveBlock2.playTimeVBlanks++;

        if (gSaveBlock2.playTimeVBlanks > 59)
        {
            gSaveBlock2.playTimeVBlanks = 0;
            gSaveBlock2.playTimeSeconds++;

            if (gSaveBlock2.playTimeSeconds > 59)
            {
                gSaveBlock2.playTimeSeconds = 0;
                gSaveBlock2.playTimeMinutes++;
				
				IncrementClockMinute();

                if (gSaveBlock2.playTimeMinutes > 59)
                {
                    gSaveBlock2.playTimeMinutes = 0;
                    gSaveBlock2.playTimeHours++;

                    if (gSaveBlock2.playTimeHours > 65535)
                        PlayTimeCounter_Loop();
                }
            }
        }
    }
}

void PlayTimeCounter_SetToMax(void)
{
    sPlayTimeCounterState = MAXED_OUT;

    gSaveBlock2.playTimeHours = 65535;
    gSaveBlock2.playTimeMinutes = 59;
    gSaveBlock2.playTimeSeconds = 59;
    gSaveBlock2.playTimeVBlanks = 59;
}

void UpdateStartMenuClock(void)
{
	if (gSaveBlock2.statusStartMenuClock == FALSE)
		return;
	
	Menu_BlankWindowRect(3, 15, 27, 18);
	UpdateTimeAndDate();
	UpdateWeatherOrSafariBalls();
}

static void RunMinuteRoutines(void) //called every minute
{
	UpdateStartMenuClock();
}

static void RunHourRoutines(void) //called every hour
{
	UpdateDayNightStatus();
}

void IncrementClockMinute(void) //adds 1 minute to game time
{
	if (gSaveBlock2.timeMinute + 1 > TIME_MINUTE_2)
	{
		gSaveBlock2.timeMinute = TIME_MINUTE_0;
		RunMinuteRoutines();
		IncrementClockHour();
		return;
	}
	
	gSaveBlock2.timeMinute++;
	RunMinuteRoutines();
}

void IncrementClockHour(void) //adds 1 hour to game time
{
	if (gSaveBlock2.timeHour + 1 > TIME_HOUR_11PM)
	{
		gSaveBlock2.timeHour = TIME_HOUR_MIDNIGHT;
		RunHourRoutines();
		IncrementClockDay();
		return;
	}
	gSaveBlock2.timeHour++;
	RunHourRoutines();
}

void IncrementClockDay(void) //adds 1 day to game time
{
	if (gSaveBlock2.timeDay + 1 > TIME_DAY_SUNDAY)
	{
		gSaveBlock2.timeDay = TIME_DAY_MONDAY;
		IncrementClockWeek();
		return;
	}
	gSaveBlock2.timeDay++;
}

void IncrementClockWeek(void) //adds 1 week to game time
{
	if (gSaveBlock2.timeWeek + 1 > TIME_WEEK_1)
	{
		gSaveBlock2.timeWeek = TIME_WEEK_0;
		IncrementClockSeason();
		return;
	}
	gSaveBlock2.timeWeek++;
}

void IncrementClockSeason(void) //adds 1 season to game time
{
	if (gSaveBlock2.timeSeason + 1 > TIME_SEASON_WINTER)
	{
		gSaveBlock2.timeSeason = TIME_SEASON_SPRING;
		IncrementClockYear();
		return;
	}
	gSaveBlock2.timeSeason++;
}

void IncrementClockYear(void) //adds 1 year to game time
{
	if (gSaveBlock2.timeYear + 1 > TIME_YEAR_7)
	{
		gSaveBlock2.timeYear = TIME_YEAR_0;
		return;
	}
	gSaveBlock2.timeYear++;
}

u8 CalculateSubSeason(void)
{
	int i = 1;
	u8 month = (gSaveBlock2.timeSeason * 3);
	
	if (gSaveBlock2.timeWeek == TIME_WEEK_1 && gSaveBlock2.timeDay > TIME_DAY_THURSDAY)
		i++;
	else if (gSaveBlock2.timeWeek == TIME_WEEK_0 && gSaveBlock2.timeDay < TIME_DAY_THURSDAY)
		i--;
	
	month += i;
	
	return month;
}

void UpdateDayNightStatus(void)
{
	u8 hour = gSaveBlock2.timeHour;
	u8 season = (gSaveBlock2.timeSeason * 2);
	u8 status;
	
	if (gSaveBlock2.timeWeek == TIME_WEEK_1)
		season++;
	
	if (hour == gSunriseTimes[season] || hour == (gSunriseTimes[season] + 1)) //if 1 hour before/after sunrise time, it is dawn
		status = TIME_DAWN;
	else if (hour == gSunsetTimes[season] || hour == (gSunsetTimes[season] - 1)) //if 1 hour before/after sunset time, it is dusk
		status = TIME_DUSK;
	else if (hour < gSunriseTimes[season] || hour > gSunsetTimes[season]) //if hour is lower than sunrise or after sunset time, it is night
		status = TIME_NIGHT;
	else //if not any of these, it is day
		status = TIME_DAY;
	
	gSaveBlock2.dayNightStatus = status;
}
