#ifndef JS_INTEGRATION_H
#define JS_INTEGRATION_H

namespace fallout {

int jsInit();
int jsLoadScript(int sid, const char* name);
int jsUnloadScript(int sid);
int jsExecProc(int sid, int proc);
void jsFree();

} // namespace fallout

#endif // JS_INTEGRATION_H
