#ifndef KOTLIN_SHIM_H
#define KOTLIN_SHIM_H

#ifdef __cplusplus
extern "C" {
#endif

int kotlin_falloutMain(int argc, char** argv);
int kotlin_mainMenuWindowInit();
void kotlin_mainMenuWindowFree();
void kotlin_mainMenuWindowHide(int animate);
void kotlin_mainMenuWindowUnhide(int animate);
int kotlin_main_menu_is_enabled();
int kotlin_mainMenuWindowHandleEvents();

#ifdef __cplusplus
}
#endif

#endif // KOTLIN_SHIM_H
