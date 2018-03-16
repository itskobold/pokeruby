#include "global.h"
#include "play_time.h"

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

//HOENNISLES START
void PlayTimeCounter_Loop()
{
    gSaveBlock2.playTimeHours = 0;
    gSaveBlock2.playTimeMinutes = 0;
    gSaveBlock2.playTimeSeconds = 0;
    gSaveBlock2.playTimeVBlanks = 0;
}
//HOENNISLES END

void PlayTimeCounter_Start()
{
    sPlayTimeCounterState = RUNNING;

//HOENNISLES
    if (gSaveBlock2.playTimeHours > 65535) //VANILLA 999
	//HOENNISLES START
		PlayTimeCounter_Loop();
	//HOENNISLES END
     //   PlayTimeCounter_SetToMax(); VANILLA
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

                if (gSaveBlock2.playTimeMinutes > 59)
                {
                    gSaveBlock2.playTimeMinutes = 0;
                    gSaveBlock2.playTimeHours++;

					//HOENNISLES
                    if (gSaveBlock2.playTimeHours > 65535) //VANILLA 999
                        PlayTimeCounter_Loop(); //VANILLA PlayTimeCounter_Stop
                }
            }
        }
    }
}

void PlayTimeCounter_SetToMax()
{
    sPlayTimeCounterState = MAXED_OUT;

//HOENNISLES
    gSaveBlock2.playTimeHours = 65535; //VANILLA 999
    gSaveBlock2.playTimeMinutes = 59;
    gSaveBlock2.playTimeSeconds = 59;
    gSaveBlock2.playTimeVBlanks = 59;
}
