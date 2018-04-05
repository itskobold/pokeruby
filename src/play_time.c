#include "global.h"
#include "play_time.h"

enum
{
    STOPPED,
    RUNNING,
    MAXED_OUT
};

static u8 sPlayTimeCounterState;

static const u8 gSeasonAndWeekToDayNightStatus[8][24] =
{
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //SPRING WEEK 0
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //SPRING WEEK 1
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //SUMMER WEEK 0
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //SUMMER WEEK 1
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //FALL WEEK 0
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //FALL WEEK 1
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT}, //WINTER WEEK 0
	{TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_DAWN, TIME_DAWN, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DAY, TIME_DUSK, TIME_DUSK, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT, TIME_NIGHT} //WINTER WEEK 1
};

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
				UpdateDayNightStatus();

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

void IncrementClockMinute(void) //adds 1 minute to game time
{
	if (gSaveBlock2.timeMinute + 1 > TIME_MINUTE_2)
	{
		gSaveBlock2.timeMinute = TIME_MINUTE_0;
		IncrementClockHour();
	}
	gSaveBlock2.timeMinute++;
}

void IncrementClockHour(void) //adds 1 hour to game time
{
	if (gSaveBlock2.timeHour + 1 > TIME_HOUR_11PM)
	{
		gSaveBlock2.timeHour = TIME_HOUR_MIDNIGHT;
		IncrementClockDay();
	}
	gSaveBlock2.timeHour++;
}

void IncrementClockDay(void) //adds 1 day to game time
{
	if (gSaveBlock2.timeDay + 1 > TIME_DAY_SUNDAY)
	{
		gSaveBlock2.timeDay = TIME_DAY_MONDAY;
		IncrementClockWeek();
	}
	gSaveBlock2.timeDay++;
}

void IncrementClockWeek(void) //adds 1 week to game time
{
	if (gSaveBlock2.timeWeek + 1 > TIME_WEEK_1)
	{
		gSaveBlock2.timeWeek = TIME_WEEK_0;
		IncrementClockSeason();
	}
	gSaveBlock2.timeWeek++;
}

void IncrementClockSeason(void) //adds 1 season to game time
{
	if (gSaveBlock2.timeSeason + 1 > TIME_SEASON_WINTER)
	{
		gSaveBlock2.timeSeason = TIME_SEASON_SPRING;
		IncrementClockYear();
	}
	gSaveBlock2.timeSeason++;
}

void IncrementClockYear(void) //adds 1 year to game time
{
	if (gSaveBlock2.timeYear + 1 > TIME_YEAR_7)
	{
		gSaveBlock2.timeYear = TIME_YEAR_0;
	}
	gSaveBlock2.timeYear++;
}

void UpdateDayNightStatus(void)
{
	u8 hour = gSaveBlock2.timeHour;
	u8 season = (gSaveBlock2.timeWeek == TIME_WEEK_0) ? gSaveBlock2.timeSeason : gSaveBlock2.timeSeason * 2;
	u8 status;
	int i, j;
	
	for (i = 0; i > season; i++)
	{
		for (j = 0; j > hour; j++)
		{
			status = gSeasonAndWeekToDayNightStatus[i][j];
		}
	}
	
	gSaveBlock2.dayNightStatus = status;
}
