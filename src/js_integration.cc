#include "js_integration.h"
#include "scripts.h"
#include "debug.h"
#include "display_monitor.h"
#include "object.h"
#include "game.h"

#include <map>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>

#include "quickjs.h"

// Dedicated file logger for JS debugging - always writes to js_debug.log
static FILE* jsLogFile = nullptr;

void jsDebugLog(const char* format, ...) {
    if (!jsLogFile) {
        jsLogFile = fopen("js_debug.log", "wt");
        if (!jsLogFile) return;
        fprintf(jsLogFile, "=== Fallout 2 CE - JS Debug Log ===\n\n");
        fflush(jsLogFile);
    }
    va_list args;
    va_start(args, format);
    vfprintf(jsLogFile, format, args);
    va_end(args);
    fflush(jsLogFile);
}

static void jsShowMessage(const char* format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    char* msg = strdup(buf);
    if (msg) {
        fallout::displayMonitorAddMessage(msg);
        free(msg);
    }
}

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

static int getCurrentSid(JSContext* ctx) {
    return (int)(uintptr_t)JS_GetContextOpaque(ctx);
}

static JSValue js_get_self(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    int sid = getCurrentSid(ctx);
    Script* scr;
    if (scriptGetScript(sid, &scr) == -1) return JS_NULL;
    return JS_NewInt64(ctx, (int64_t)scr->owner);
}

static JSValue js_get_source(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    int sid = getCurrentSid(ctx);
    Script* scr;
    if (scriptGetScript(sid, &scr) == -1) return JS_NULL;
    return JS_NewInt64(ctx, (int64_t)scr->source);
}

static JSValue js_get_target(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    int sid = getCurrentSid(ctx);
    Script* scr;
    if (scriptGetScript(sid, &scr) == -1) return JS_NULL;
    return JS_NewInt64(ctx, (int64_t)scr->target);
}

static JSValue js_get_dude(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    return JS_NewInt64(ctx, (int64_t)gDude);
}

static JSValue js_get_fixed_param(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    int sid = getCurrentSid(ctx);
    Script* scr;
    if (scriptGetScript(sid, &scr) == -1) return JS_INT32_MIN;
    return JS_NewInt32(ctx, scr->fixedParam);
}

static JSValue js_mstr(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1) return JS_EXCEPTION;
    int32_t id;
    if (JS_ToInt32(ctx, &id, argv[0])) return JS_EXCEPTION;
    int sid = getCurrentSid(ctx);
    Script* scr;
    if (scriptGetScript(sid, &scr) == -1) return JS_NULL;
    char* msg = _scr_get_msg_str(scr->index + 1, id);
    if (!msg) return JS_NULL;
    return JS_NewString(ctx, msg);
}

static JSValue js_add_timer_event(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 2) return JS_EXCEPTION;
    int32_t delay, param;
    if (JS_ToInt32(ctx, &delay, argv[0])) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &param, argv[1])) return JS_EXCEPTION;
    int sid = getCurrentSid(ctx);
    scriptAddTimerEvent(sid, delay, param);
    return JS_UNDEFINED;
}

static JSValue js_get_global_var(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1) return JS_EXCEPTION;
    int32_t varId;
    if (JS_ToInt32(ctx, &varId, argv[0])) return JS_EXCEPTION;
    return JS_NewInt32(ctx, gameGetGlobalVar(varId));
}

static JSValue js_set_global_var(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 2) return JS_EXCEPTION;
    int32_t varId, value;
    if (JS_ToInt32(ctx, &varId, argv[0])) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &value, argv[1])) return JS_EXCEPTION;
    gameSetGlobalVar(varId, value);
    return JS_UNDEFINED;
}

static JSValue js_get_local_var(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1) return JS_EXCEPTION;
    int32_t varId;
    if (JS_ToInt32(ctx, &varId, argv[0])) return JS_EXCEPTION;
    int sid = getCurrentSid(ctx);
    ProgramValue val;
    if (scriptGetLocalVar(sid, varId, val) == -1) return JS_NULL;
    return JS_NewInt32(ctx, val.integerValue);
}

static JSValue js_set_local_var(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 2) return JS_EXCEPTION;
    int32_t varId, value;
    if (JS_ToInt32(ctx, &varId, argv[0])) return JS_EXCEPTION;
    if (JS_ToInt32(ctx, &value, argv[1])) return JS_EXCEPTION;
    int sid = getCurrentSid(ctx);
    ProgramValue val(value);
    scriptSetLocalVar(sid, varId, val);
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
    JS_SetPropertyStr(ctx, fallout, "displayMsg", JS_NewCFunction(ctx, js_display_message, "displayMsg", 1));
    JS_SetPropertyStr(ctx, fallout, "getSelf", JS_NewCFunction(ctx, js_get_self, "getSelf", 0));
    JS_SetPropertyStr(ctx, fallout, "getSource", JS_NewCFunction(ctx, js_get_source, "getSource", 0));
    JS_SetPropertyStr(ctx, fallout, "getTarget", JS_NewCFunction(ctx, js_get_target, "getTarget", 0));
    JS_SetPropertyStr(ctx, fallout, "getDude", JS_NewCFunction(ctx, js_get_dude, "getDude", 0));
    JS_SetPropertyStr(ctx, fallout, "getFixedParam", JS_NewCFunction(ctx, js_get_fixed_param, "getFixedParam", 0));
    JS_SetPropertyStr(ctx, fallout, "mstr", JS_NewCFunction(ctx, js_mstr, "mstr", 1));
    JS_SetPropertyStr(ctx, fallout, "addTimerEvent", JS_NewCFunction(ctx, js_add_timer_event, "addTimerEvent", 2));
    JS_SetPropertyStr(ctx, fallout, "getGlobalVar", JS_NewCFunction(ctx, js_get_global_var, "getGlobalVar", 1));
    JS_SetPropertyStr(ctx, fallout, "setGlobalVar", JS_NewCFunction(ctx, js_set_global_var, "setGlobalVar", 2));
    JS_SetPropertyStr(ctx, fallout, "getLocalVar", JS_NewCFunction(ctx, js_get_local_var, "getLocalVar", 1));
    JS_SetPropertyStr(ctx, fallout, "setLocalVar", JS_NewCFunction(ctx, js_set_local_var, "setLocalVar", 2));
    JS_SetPropertyStr(ctx, global_obj, "fallout", fallout);

    JS_FreeValue(ctx, global_obj);
}

int jsInit() {
    jsDebugLog("jsInit() called\n");
    if (rt) {
        jsDebugLog("jsInit(): runtime already exists, skipping\n");
        return 0;
    }
    rt = JS_NewRuntime();
    if (!rt) {
        jsDebugLog("jsInit() FAILED: could not create JS runtime\n");
        return -1;
    }
    jsDebugLog("jsInit() SUCCESS: JS runtime created\n");
    return 0;
}

int jsLoadScript(int sid, const char* name) {
    jsDebugLog("jsLoadScript(sid=%d, name='%s') called\n", sid, name ? name : "(null)");

    if (!rt) {
        jsDebugLog("  FAILED: JS runtime not initialized!\n");
        jsShowMessage("JS ERROR: runtime not initialized");
        return -1;
    }

    // Check if already loaded
    auto existing = scriptContexts.find(sid);
    if (existing != scriptContexts.end()) {
        jsDebugLog("  Script sid=%d already loaded, reusing context\n", sid);
        return 0;
    }

    // Create new context for this script
    JSContext* ctx = JS_NewContext(rt);
    if (!ctx) {
        jsDebugLog("  FAILED: could not create JS context\n");
        jsShowMessage("JS ERROR: could not create context");
        return -1;
    }

    setupContext(ctx);
    JS_SetContextOpaque(ctx, (void*)(uintptr_t)sid);

    // Try multiple path strategies
    char path[256];
    size_t len;
    char* buf = nullptr;

    // Strategy 1: data/scripts/name (VFS patches path)
    snprintf(path, sizeof(path), "data/scripts/%s", name);
    jsDebugLog("  Trying path: '%s'\n", path);
    buf = readFile(path, &len);

    if (!buf) {
        // Strategy 2: data\scripts\name (Windows backslash)
        snprintf(path, sizeof(path), "data\\scripts\\%s", name);
        jsDebugLog("  Trying path: '%s'\n", path);
        buf = readFile(path, &len);
    }

    if (!buf) {
        // Strategy 3: scripts/name
        snprintf(path, sizeof(path), "scripts/%s", name);
        jsDebugLog("  Trying path: '%s'\n", path);
        buf = readFile(path, &len);
    }

    if (!buf) {
        // Strategy 4: scripts\name (Windows backslash)
        snprintf(path, sizeof(path), "scripts\\%s", name);
        jsDebugLog("  Trying path: '%s'\n", path);
        buf = readFile(path, &len);
    }

    if (!buf) {
        // Strategy 5: just name
        jsDebugLog("  Trying path: '%s'\n", name);
        buf = readFile(name, &len);
        if (buf) {
            strncpy(path, name, sizeof(path));
            path[sizeof(path) - 1] = '\0';
        }
    }

    if (!buf) {
        jsDebugLog("  FAILED: Could not open file with any path strategy!\n");
        jsShowMessage("JS ERROR: cannot find %s", name);
        debugPrint("JS: Could not load script %s\n", name);
        JS_FreeContext(ctx);
        return -1;
    }

    jsDebugLog("  SUCCESS: Read %zu bytes from '%s'\n", len, path);
    jsDebugLog("  --- JS file first lines ---\n");
    {
        // Log first 5 lines of file content
        const char* p = buf;
        for (int lineNum = 1; lineNum <= 5 && p < buf + len; lineNum++) {
            const char* eol = p;
            while (eol < buf + len && *eol != '\n') eol++;
            int lineLen = (int)(eol - p);
            if (lineLen > 120) lineLen = 120;
            jsDebugLog("  %d: %.*s\n", lineNum, lineLen, p);
            if (eol < buf + len) eol++; // skip \n
            p = eol;
        }
        jsDebugLog("  --- end ---\n");
    }

    JSValue val = JS_Eval(ctx, buf, len, path, JS_EVAL_TYPE_GLOBAL);
    free(buf);

    if (JS_IsException(val)) {
        JSValue ex = JS_GetException(ctx);
        const char* str = JS_ToCString(ctx, ex);
        jsDebugLog("  JS EVAL ERROR in %s: %s\n", name, str ? str : "(unknown)");
        jsShowMessage("JS EVAL ERROR: %s", str ? str : "unknown");
        debugPrint("JS Error in %s: %s\n", name, str);
        JS_FreeCString(ctx, str);
        JS_FreeValue(ctx, ex);
        JS_FreeContext(ctx);
        return -1;
    }

    JS_FreeValue(ctx, val);

    scriptContexts[sid] = ctx;
    jsDebugLog("  SUCCESS: Script loaded and evaluated, sid=%d\n", sid);
    jsShowMessage("JS loaded: %s (sid=%d)", name, sid);
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
    jsDebugLog("jsExecProc(sid=%d, proc='%s') called\n", sid, procName ? procName : "(null)");

    auto it = scriptContexts.find(sid);
    if (it == scriptContexts.end()) {
        jsDebugLog("  FAILED: no context found for sid=%d\n", sid);
        return -1;
    }

    JSContext* ctx = it->second;

    if (!procName) {
        jsDebugLog("  FAILED: procName is null\n");
        return -1;
    }

    JSValue global_obj = JS_GetGlobalObject(ctx);
    JSValue func = JS_GetPropertyStr(ctx, global_obj, procName);
    JS_FreeValue(ctx, global_obj);

    if (JS_IsFunction(ctx, func)) {
        jsDebugLog("  Calling JS function '%s'...\n", procName);
        JSValue ret = JS_Call(ctx, func, JS_UNDEFINED, 0, nullptr);
        if (JS_IsException(ret)) {
            JSValue ex = JS_GetException(ctx);
            const char* str = JS_ToCString(ctx, ex);
            jsDebugLog("  JS RUNTIME ERROR in '%s': %s\n", procName, str ? str : "(unknown)");
            jsShowMessage("JS ERROR in %s: %s", procName, str ? str : "unknown");
            debugPrint("JS Error exec %s: %s\n", procName, str);
            JS_FreeCString(ctx, str);
            JS_FreeValue(ctx, ex);
        } else {
            jsDebugLog("  SUCCESS: '%s' executed\n", procName);
        }
        JS_FreeValue(ctx, ret);
        JS_FreeValue(ctx, func);
        return 0;
    }

    JS_FreeValue(ctx, func);
    jsDebugLog("  Proc '%s' not found or not a function (this is normal for procs the script doesn't implement)\n", procName);
    return -1; // Proc not found or not a function
}

void jsFree() {
    jsDebugLog("jsFree() called, freeing %zu contexts\n", scriptContexts.size());
    for (auto& pair : scriptContexts) {
        jsDebugLog("  Freeing context for sid=%d\n", pair.first);
        JS_FreeContext(pair.second);
    }
    scriptContexts.clear();
    if (rt) {
        JS_FreeRuntime(rt);
        rt = nullptr;
    }
    jsDebugLog("jsFree() done\n");
    if (jsLogFile) {
        fprintf(jsLogFile, "\n=== JS Debug Log End ===\n");
        fclose(jsLogFile);
        jsLogFile = nullptr;
    }
}

} // namespace fallout
