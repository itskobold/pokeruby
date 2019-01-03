#include "global.h"
#include "menu.h"
#include "main.h"
#include "play_time.h"
#include "start_menu.h"

const u8 gSunriseTimes[8] = {TIME_HOUR_6AM, TIME_HOUR_5AM, TIME_HOUR_4AM, TIME_HOUR_5AM, TIME_HOUR_6AM, TIME_HOUR_7AM, TIME_HOUR_8AM, TIME_HOUR_7AM};
const u8 gSunsetTimes[8] = {TIME_HOUR_7PM, TIME_HOUR_8PM, TIME_HOUR_9PM, TIME_HOUR_8PM, TIME_HOUR_7PM, TIME_HOUR_6PM, TIME_HOUR_5PM, TIME_HOUR_6PM};

enum
{
    STOPPED,
    RUNNING,
    MAXED_OUT
};

static u8 sPlayTimeCounterState;

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
    if (sPlayTimeCounterState == !STOPPED)
    {		
        gSaveBlock2.playTimeVBlanks++;

        if (gSaveBlock2.playTimeVBlanks > 59)
        {
            gSaveBlock2.playTimeVBlanks = 0;
            gSaveBlock2.playTimeSeconds++;
			
			gSaveBlock2.gameMode = 3;
			
			if (gMain.stopClockUpdating == FALSE) //stops clock updating in battle & in start menu/submenus such as bag
				IncrementClockSecond(TRUE);

            if (gSaveBlock2.playTimeSeconds > 59)
            {
                gSaveBlock2.playTimeSeconds = 0;
                gSaveBlock2.playTimeMinutes++;

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

static void RunSecondRoutines(void) //called every second outside of battles/menus
{
}

static void RunMinuteRoutines(void) //called every minute outside of battles/menus
{
}

static void RunHourRoutines(void) //called every hour outside of battles/menus
{
	TryIncrementWaitTime();
}

static void RunDayRoutines(void) //called every day outside of battles/menus
{
}

static void RunWeekRoutines(void) //called every week outside of battles/menus
{
}

static void RunSeasonRoutines(void) //called every season outside of battles/menus
{
}

static void RunYearRoutines(void) //called every year outside of battles/menus
{
}

void IncrementClockSecond(bool8 runRoutines) //adds 1 second to game time
{
	if (gSaveBlock2.timeSeconds + 1 > 59)
	{
		gSaveBlock2.timeSeconds = 0;
		IncrementClockMinute(runRoutines);
		return;
	}
	
	gSaveBlock2.timeSeconds++;
	
	if (runRoutines == TRUE)
		RunSecondRoutines();
}

void IncrementClockMinute(bool8 runRoutines) //adds 1 minute to game time
{
	if (gSaveBlock2.timeMinute + 1 > TIME_MINUTE_2)
	{
		gSaveBlock2.timeMinute = TIME_MINUTE_0;
		IncrementClockHour(runRoutines);
		return;
	}
	
	gSaveBlock2.timeMinute++;
	
	if (runRoutines == TRUE)
		RunMinuteRoutines();
}

void IncrementClockHour(bool8 runRoutines) //adds 1 hour to game time
{
	if (gSaveBlock2.timeHour + 1 > TIME_HOUR_11PM)
	{
		gSaveBlock2.timeHour = TIME_HOUR_MIDNIGHT;
		IncrementClockDay(runRoutines);
		return;
	}
	gSaveBlock2.timeHour++;
	
	UpdateDayNightStatus(); //called seperately from hour routines as it's always needed
	
	if (runRoutines == TRUE)
		RunHourRoutines();
}

void IncrementClockDay(bool8 runRoutines) //adds 1 day to game time
{
	if (gSaveBlock2.timeDay + 1 > TIME_DAY_SUNDAY)
	{
		gSaveBlock2.timeDay = TIME_DAY_MONDAY;
		IncrementClockWeek(runRoutines);
		return;
	}
	gSaveBlock2.timeDay++;
	
	if (runRoutines == TRUE)
		RunDayRoutines();
}

void IncrementClockWeek(bool8 runRoutines) //adds 1 week to game time
{
	if (gSaveBlock2.timeWeek + 1 > TIME_WEEK_1)
	{
		gSaveBlock2.timeWeek = TIME_WEEK_0;
		IncrementClockSeason(runRoutines);
		return;
	}
	gSaveBlock2.timeWeek++;
	
	if (runRoutines == TRUE)
		RunWeekRoutines();
}

void IncrementClockSeason(bool8 runRoutines) //adds 1 season to game time
{
	if (gSaveBlock2.timeSeason + 1 > TIME_SEASON_WINTER)
	{
		gSaveBlock2.timeSeason = TIME_SEASON_SPRING;
		IncrementClockYear(TRUE);
		return;
	}
	gSaveBlock2.timeSeason++;
	
	if (runRoutines == TRUE)
		RunSeasonRoutines();
}

void IncrementClockYear(bool8 runRoutines) //adds 1 year to game time
{
	if (gSaveBlock2.timeYear + 1 > TIME_YEAR_7)
	{
		gSaveBlock2.timeYear = TIME_YEAR_0;
		return;
	}
	gSaveBlock2.timeYear++;
	
	if (runRoutines == TRUE)
		RunYearRoutines();
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
	u8 season = gSaveBlock2.timeSeason * 2;
	
	if (gSaveBlock2.timeWeek == TIME_WEEK_1)
		season++;
	
	if (gSaveBlock2.timeHour == gSunriseTimes[season] || gSaveBlock2.timeHour == (gSunriseTimes[season] + 1)) //if 1 hour before/after sunrise time, it is dawn
		gSaveBlock2.dayNightStatus = TIME_DAWN;
	else if (gSaveBlock2.timeHour == gSunsetTimes[season] || gSaveBlock2.timeHour == (gSunsetTimes[season] - 1)) //if 1 hour before/after sunset time, it is dusk
		gSaveBlock2.dayNightStatus = TIME_DUSK;
	else if (gSaveBlock2.timeHour < gSunriseTimes[season] || gSaveBlock2.timeHour > gSunsetTimes[season]) //if hour is lower than sunrise or after sunset time, it is night
		gSaveBlock2.dayNightStatus = TIME_NIGHT;
	else //if not any of these, it is day
		gSaveBlock2.dayNightStatus = TIME_DAY;
}

void TryIncrementWaitTime(void)
{
	if (gSaveBlock2.waitTime < 60)
		gSaveBlock2.waitTime++;
}
