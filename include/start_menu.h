#ifndef GUARD_START_MENU_H
#define GUARD_START_MENU_H

#define NUM_START_MENU_ACTIONS 14

extern u8 (*gMenuCallback)(void);

//Public functions
void CreateStartMenuTask(void (*func)(u8));
void sub_80712B4(u8 taskId);
void RegisteredOptionTask(u8 taskId);
void sub_8071310(void);
void RunRegisteredStartOption(void);
void ScrSpecial_DoSaveDialog(void);
void sub_8071B28(void);
void debug_sub_8075DB4(struct BattleTowerEReaderTrainer *ereaderTrainer, const u8 *b, u32 trainerId);
bool8 debug_sub_8075C30(void);
bool32 UseRegisteredStartOption(void);

void UpdateTimeAndDate(void);
void UpdateWeatherOrSafariBalls(void);

#if DEBUG
extern u32 gUnknown_Debug_03004BD0;
#endif // DEBUG

#endif // GUARD_START_MENU_H
