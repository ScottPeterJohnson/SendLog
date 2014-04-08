#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
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
#include "proto.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif
#include "badwords.h"


DLLFUNC int m_message(aClient *cptr, aClient *sptr, int parc, char *parv[], int notice);

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
		if (!IsOper(sptr)) {
			sendnotice(sptr, "Only opers can use the sendlog command.");
			return 0;
		}
		//SENDLOG target&sender&channel message
		//It's & seperated because Unreal doesn't parse more than like 3 arguments correctly.
		//Unreal is a terrible IRCD.
		//void	sendto_message_one(aClient *to, aClient *from, char *sender,
			//char *cmd, char *nick, char *msg)
		if (parc < 3) { sendnotice(sptr,"Not enough arguments."); return 0; }
		
		char* args[3];
		args[0] = parv[1];
		int i=1;
		char* iter = parv[1];
		while(*iter != '\0' && i < 4) {
			if(*iter == '&') {
				*iter = '\0';
				args[i] = iter + 1;
				i++;
			}
			++iter;
		}
		
		if(i<3) { sendnotice(sptr,"Not enough ampersand seperated arguments."); return 0; }
		
		aClient* acptr = find_person(args[0], NULL);
		if(!acptr) sendnotice(sptr, "Target not found.");
		else {
			//DID YOU KNOW: Unreal ircd doesn't actually use char* sender in sendto_message_one?
			//It just pulls it from the from :)
			//char temp[64];
			//strncpy(temp,sptr->name,64);
			//strncpy(sptr->name, parv[1],64);
			sendto_message_one(acptr,NULL,args[1],"PRIVMSG",args[2],parv[2]);
			//strncpy(sptr->name,temp,64);
		}
		/*parv[0]=parv[1];
		parv[1]=parv[2];
		parv[2]=parv[3];
		sendnotice(sptr, "%s %s %s", parv[0], parv[1], parv[2]);
		sendnotice(sptr, "Should have received notice.");
		m_message(cptr, sptr, parc - 1, parv, 0);*/
		return 0;
}

