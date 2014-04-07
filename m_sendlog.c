#include "config.h"
#include "struct.h"
#include "common.h"
#include "channel.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "proto.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif

DLLFUNC int m_sendlog(aClient* cptr, aClient* sptr, int parc, char* parv[]);

ModuleHeader MOD_HEADER(m_sendlog)
  = {
	"send_logs",
	"v1",
	"/SENDLOG target msg",
	"3.2-b8-1",
	NULL 
    };
    
DLLFUNC int MOD_INIT(m_sendlog)(ModuleInfo* modinfo) {
	ModuleSetOptions(modinfo->handle, MOD_OPT_PERM);
	CommandAdd(modinfo->handle, "SENDLOG", "Sl", m_sendlog, 3, M_USER);
	return MOD_SUCCESS;
}

DLLFUNC int MOD_LOAD(m_sendlog)(int module_load) {
		return MOD_SUCCESS;
}

DLLFUNC int MOD_UNLOAD(m_sendlog)(int module_unload) {
	return MOD_SUCCESS;
}

DLLFUNC int m_sendlog(aClient* cptr, aClient* sptr, int parc, char* parv[]) {
		if (!isOper(sPtr)) {
			sendnotice(sptr, "Only opers can use the sendlog command.");
			return 0;
		}
		//Shift all arguments one down.
		parv[0]=parv[1];
		parv[1]=parv[2];
		parv[2]=parv[1];
		m_message(cptr, sptr, parc - 1, parv, notice);
}
