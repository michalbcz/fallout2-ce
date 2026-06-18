#include "kotlin_shim.h"
#include "main.h"
#include "mainmenu.h"

extern "C" {

int kotlin_falloutMain(int argc, char** argv) {
    return fallout::falloutMain(argc, argv);
}

int kotlin_mainMenuWindowInit() {
    return fallout::mainMenuWindowInit();
}

void kotlin_mainMenuWindowFree() {
    fallout::mainMenuWindowFree();
}

void kotlin_mainMenuWindowHide(int animate) {
    fallout::mainMenuWindowHide(animate != 0);
}

void kotlin_mainMenuWindowUnhide(int animate) {
    fallout::mainMenuWindowUnhide(animate != 0);
}

int kotlin_main_menu_is_enabled() {
    return fallout::_main_menu_is_enabled();
}

int kotlin_mainMenuWindowHandleEvents() {
    return fallout::mainMenuWindowHandleEvents();
}

}
