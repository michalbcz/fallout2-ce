#include "js_integration.h"
#include "scripts.h"
#include "debug.h"
#include "display_monitor.h"

#include <map>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "quickjs.h"

#include "game_sound.h"

namespace fallout {

static JSRuntime* rt = nullptr;
static std::map<int, JSContext*> scriptContexts;

// Helper to read file
static char* readFile(const char* path, size_t* sizeOut) {
    FILE* f = fopen(path, "rb");
    if (!f) return nullptr;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buf = (char*)malloc(size + 1);
    if (!buf) {
        fclose(f);
        return nullptr;
    }

    if (fread(buf, 1, size, f) != (size_t)size) {
        free(buf);
        fclose(f);
        return nullptr;
    }
    buf[size] = 0;
    fclose(f);

    if (sizeOut) *sizeOut = size;
    return buf;
}

static JSValue js_print(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    std::string output;
    for (int i = 0; i < argc; i++) {
        const char* str = JS_ToCString(ctx, argv[i]);
        if (!str) return JS_EXCEPTION;
        if (i > 0) output += " ";
        output += str;
        JS_FreeCString(ctx, str);
    }
    debugPrint("%s\n", output.c_str());
    return JS_UNDEFINED;
}

static JSValue js_game_time_hour(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    return JS_NewInt32(ctx, gameTimeGetHour());
}

static JSValue js_display_message(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc > 0) {
        const char* str = JS_ToCString(ctx, argv[0]);
        if (str) {
            // displayMonitorAddMessage takes non-const char*, so we might need to cast or duplicate if it modifies it.
            // Assuming it doesn't modify or copies it.
            // Let's strdup to be safe if it takes ownership or modifies, but standard practice for C strings is safe enough if just reading.
            // displayMonitorAddMessage likely copies.
            // Checking displayMonitorAddMessage signature in header: void displayMonitorAddMessage(char* string);
            // It takes char*.
            char* buf = strdup(str);
            if (buf) {
                displayMonitorAddMessage(buf);
                free(buf);
            }
            JS_FreeCString(ctx, str);
        }
    }
    return JS_UNDEFINED;
}

static void setupContext(JSContext* ctx) {
    JSValue global_obj = JS_GetGlobalObject(ctx);

    // console.log
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_print, "log", 1));
    JS_SetPropertyStr(ctx, global_obj, "console", console);

    // fallout object
    JSValue fallout = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, fallout, "gameTimeHour", JS_NewCFunction(ctx, js_game_time_hour, "gameTimeHour", 0));
    JS_SetPropertyStr(ctx, fallout, "print", JS_NewCFunction(ctx, js_display_message, "print", 1));

    auto js_play_sound = [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
        if (argc >= 1) {
            const char* path = JS_ToCString(ctx, argv[0]);
            int channel = -1;
            if (path) {
                channel = soundPlayFile(path);
                JS_FreeCString(ctx, path);
            }
            return JS_NewInt32(ctx, channel);
        }
        return JS_UNDEFINED;
    };

    auto js_stop_sound = [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
        if (argc >= 1) {
            int channel;
            if (JS_ToInt32(ctx, &channel, argv[0]) == 0) {
                soundStopSound(channel);
            }
        }
        return JS_UNDEFINED;
    };

    auto js_set_sound_volume = [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
        if (argc >= 2) {
            int channel, vol;
            if (JS_ToInt32(ctx, &channel, argv[0]) == 0 && JS_ToInt32(ctx, &vol, argv[1]) == 0) {
                soundSetSoundVolume(channel, vol);
            }
        }
        return JS_UNDEFINED;
    };

    auto js_play_music = [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
        if (argc >= 1) {
            const char* path = JS_ToCString(ctx, argv[0]);
            if (path) {
                soundPlayMusic(path);
                JS_FreeCString(ctx, path);
            }
        }
        return JS_UNDEFINED;
    };

    auto js_stop_music = [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
        soundStopMusic();
        return JS_UNDEFINED;
    };

    auto js_set_music_volume = [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue {
        if (argc >= 1) {
            int vol;
            if (JS_ToInt32(ctx, &vol, argv[0]) == 0) {
                soundSetMusicVolume2(vol);
            }
        }
        return JS_UNDEFINED;
    };

    JS_SetPropertyStr(ctx, fallout, "playSound", JS_NewCFunction(ctx, js_play_sound, "playSound", 1));
    JS_SetPropertyStr(ctx, fallout, "stopSound", JS_NewCFunction(ctx, js_stop_sound, "stopSound", 1));
    JS_SetPropertyStr(ctx, fallout, "setSoundVolume", JS_NewCFunction(ctx, js_set_sound_volume, "setSoundVolume", 2));
    JS_SetPropertyStr(ctx, fallout, "playMusic", JS_NewCFunction(ctx, js_play_music, "playMusic", 1));
    JS_SetPropertyStr(ctx, fallout, "stopMusic", JS_NewCFunction(ctx, js_stop_music, "stopMusic", 0));
    JS_SetPropertyStr(ctx, fallout, "setMusicVolume", JS_NewCFunction(ctx, js_set_music_volume, "setMusicVolume", 1));
    JS_SetPropertyStr(ctx, global_obj, "fallout", fallout);

    JS_FreeValue(ctx, global_obj);
}

int jsInit() {
    if (rt) return 0;
    rt = JS_NewRuntime();
    if (!rt) return -1;
    return 0;
}

int jsLoadScript(int sid, const char* name) {
    if (!rt) return -1;

    // Create new context for this script
    JSContext* ctx = JS_NewContext(rt);
    if (!ctx) return -1;

    setupContext(ctx);

    // Construct path. Assuming name is relative to scripts/ or contains it.
    // scripts.cc passes name from scripts.lst.
    // We should assume base path "scripts/" needs to be prepended if not present.
    // But scripts.cc seems to construct paths.
    // Let's rely on the caller passing a name we can find, or prepend "scripts/".
    // scriptsCreateProgramByName uses `_cd_path_base` + `gScriptsBasePath` + name + ".int".

    // Here we will assume we are passed "scripts/name.js" or just "name.js".
    // I'll try with "scripts/" prepended if file not found?
    // Let's try to just open the name first.

    char path[256];
    snprintf(path, sizeof(path), "scripts/%s", name);

    size_t len;
    char* buf = readFile(path, &len);
    if (!buf) {
        // Try without prefix
        buf = readFile(name, &len);
        if (!buf) {
            debugPrint("JS: Could not load script %s\n", name);
            JS_FreeContext(ctx);
            return -1;
        }
        strncpy(path, name, sizeof(path));
    }

    JSValue val = JS_Eval(ctx, buf, len, path, JS_EVAL_TYPE_GLOBAL);
    free(buf);

    if (JS_IsException(val)) {
        JSValue ex = JS_GetException(ctx);
        const char* str = JS_ToCString(ctx, ex);
        debugPrint("JS Error in %s: %s\n", name, str);
        JS_FreeCString(ctx, str);
        JS_FreeValue(ctx, ex);
        JS_FreeContext(ctx);
        return -1;
    }

    JS_FreeValue(ctx, val);

    scriptContexts[sid] = ctx;
    return 0;
}

int jsUnloadScript(int sid) {
    auto it = scriptContexts.find(sid);
    if (it == scriptContexts.end()) return -1;
    JS_FreeContext(it->second);
    scriptContexts.erase(it);
    return 0;
}

int jsExecProc(int sid, const char* procName) {
    auto it = scriptContexts.find(sid);
    if (it == scriptContexts.end()) return -1;

    JSContext* ctx = it->second;

    if (!procName) return -1;

    JSValue global_obj = JS_GetGlobalObject(ctx);
    JSValue func = JS_GetPropertyStr(ctx, global_obj, procName);
    JS_FreeValue(ctx, global_obj);

    if (JS_IsFunction(ctx, func)) {
        JSValue ret = JS_Call(ctx, func, JS_UNDEFINED, 0, nullptr);
        if (JS_IsException(ret)) {
            JSValue ex = JS_GetException(ctx);
            const char* str = JS_ToCString(ctx, ex);
            debugPrint("JS Error exec %s: %s\n", procName, str);
            JS_FreeCString(ctx, str);
            JS_FreeValue(ctx, ex);
        }
        JS_FreeValue(ctx, ret);
        JS_FreeValue(ctx, func);
        return 0;
    }

    JS_FreeValue(ctx, func);
    return -1; // Proc not found or not a function
}

void jsFree() {
    for (auto& pair : scriptContexts) {
        JS_FreeContext(pair.second);
    }
    scriptContexts.clear();
    if (rt) {
        JS_FreeRuntime(rt);
        rt = nullptr;
    }
}

} // namespace fallout
