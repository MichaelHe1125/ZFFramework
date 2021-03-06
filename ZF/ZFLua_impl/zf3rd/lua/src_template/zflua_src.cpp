
#include "../../../../zf3rd/_tmp/lua/lapi.c"
#include "../../../../zf3rd/_tmp/lua/lauxlib.c"
#include "../../../../zf3rd/_tmp/lua/lbaselib.c"
#include "../../../../zf3rd/_tmp/lua/lbitlib.c"
#include "../../../../zf3rd/_tmp/lua/lcode.c"
#include "../../../../zf3rd/_tmp/lua/lcorolib.c"
#include "../../../../zf3rd/_tmp/lua/lctype.c"
#include "../../../../zf3rd/_tmp/lua/ldblib.c"
#include "../../../../zf3rd/_tmp/lua/ldebug.c"
#include "../../../../zf3rd/_tmp/lua/ldo.c"
#include "../../../../zf3rd/_tmp/lua/ldump.c"
#include "../../../../zf3rd/_tmp/lua/lfunc.c"
#include "../../../../zf3rd/_tmp/lua/lgc.c"
#include "../../../../zf3rd/_tmp/lua/linit.c"
#include "../../../../zf3rd/_tmp/lua/liolib.c"
#include "../../../../zf3rd/_tmp/lua/llex.c"
#include "../../../../zf3rd/_tmp/lua/lmathlib.c"
#include "../../../../zf3rd/_tmp/lua/lmem.c"
#include "../../../../zf3rd/_tmp/lua/loadlib.c"
#include "../../../../zf3rd/_tmp/lua/lobject.c"
#include "../../../../zf3rd/_tmp/lua/lopcodes.c"

// system() not available since iOS11
#define system(...) (-1)
#include "../../../../zf3rd/_tmp/lua/loslib.c"
#undef system

#include "../../../../zf3rd/_tmp/lua/lparser.c"
#include "../../../../zf3rd/_tmp/lua/lstate.c"
#include "../../../../zf3rd/_tmp/lua/lstring.c"
#include "../../../../zf3rd/_tmp/lua/lstrlib.c"
#include "../../../../zf3rd/_tmp/lua/ltable.c"
#include "../../../../zf3rd/_tmp/lua/ltablib.c"
#include "../../../../zf3rd/_tmp/lua/ltm.c"
#include "../../../../zf3rd/_tmp/lua/lundump.c"
#include "../../../../zf3rd/_tmp/lua/lutf8lib.c"
#include "../../../../zf3rd/_tmp/lua/lvm.c"
#include "../../../../zf3rd/_tmp/lua/lzio.c"
#include "zflua_reset.h"

