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
		//Did you know unreal arbitrarily limits the number of parameters it parses?
		//True facts!
		//:)
		//SENDLOG target sender channel message
		//Shift all arguments one down.
		//void	sendto_message_one(aClient *to, aClient *from, char *sender,
			//char *cmd, char *nick, char *msg)
		if (parc < 5) { sendnotice(sptr,"Not enough arguments. %s", itoa(parc)); return 0; }
		
		aClient* acptr = find_person(parv[1], NULL);
		if(!acptr) sendnotice(sptr, "Target not found.");
		else {
			//DID YOU KNOW: Unreal ircd doesn't actually use char* sender in sendto_message_one?
			//It just pulls it from the from :)
			//char temp[64];
			//strncpy(temp,sptr->name,64);
			//strncpy(sptr->name, parv[1],64);
			sendto_message_one(acptr,NULL,parv[2],"PRIVMSG",parv[3],parv[4]);
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

