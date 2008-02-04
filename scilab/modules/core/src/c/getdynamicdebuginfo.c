/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#ifdef HAVE_UNAME
#include <sys/utsname.h>
#endif
#include "MALLOC.h"
#include "getdynamicdebuginfo.h"

/**
 * Set a debug element
 *
 * @param msg The debug element
 * @param desc The description of the debug element
 * @param value The value of the debug element
 */
static void SetDebugMsg(debug_message *msg, char* desc, char* value){
	(*msg).value=(char*)MALLOC((strlen(value)+1)*sizeof(char));
	(*msg).description=(char*)MALLOC((strlen(desc)+1)*sizeof(char));
	strcpy((*msg).description,desc);
	strcpy((*msg).value,value);
}

char **getDynamicDebugInfo(int *sizeArray)
{
	char *value=NULL;
	char **outputDynamicList=NULL;
	int i,position=0;
	static debug_message dynamicDebug[NB_DEBUG_ELEMENT];

#ifdef HAVE_UNAME
	/* Host info */
	struct utsname name;
#endif

	/* Memory */
   	long numPages, pageSize, freePages, totalSize, totalFree, totalUsed;

	numPages = sysconf(_SC_PHYS_PAGES)/1024;
	pageSize = sysconf(_SC_PAGESIZE)/1024;
	freePages = sysconf( _SC_AVPHYS_PAGES)/1024;
	totalSize = numPages * pageSize;
	totalFree = pageSize * freePages;
	totalUsed = totalSize - totalFree;
	value=(char*)MALLOC(255*sizeof(char));
	sprintf(value,"%u MB",totalSize);
	SetDebugMsg(&dynamicDebug[position],"Total memory",value);
	position++;


	sprintf(value,"%u MB",totalFree);
	SetDebugMsg(&dynamicDebug[position],"Total free memory",value);
	position++;

	sprintf(value,"%u MB",totalUsed );
	SetDebugMsg(&dynamicDebug[position],"Total used memory",value);
	position++;
#ifdef HAVE_UNAME
	if (uname(&name) < 0) {
		sprintf(value, "Unknown OS version (uname failed - %s)", strerror(errno));
	}

	if (strcmp(name.sysname, "AIX") == 0) {
		/*
		 * Because IBM is doing something different
		 */
		sprintf(value, "%s %s.%s", name.sysname, name.version, name.release);
	} else {
		/*
		 * Get the information
		 */
		sprintf(value, "%s %s", name.sysname, name.release);
	}
	SetDebugMsg(&dynamicDebug[position],"OS version",value);
	position++;
	
#endif


	outputDynamicList=(char**)MALLOC(sizeof(char*)*(position+1));
	
	for (i=0; i<NB_DEBUG_ELEMENT; i++){
		debug_message msg=dynamicDebug[i];

		if (msg.description==NULL) /* We reach the end of the dynamic list */
			break;

		/* Create the element in the array */

		outputDynamicList[i]=(char*) MALLOC((strlen(msg.description)+strlen(msg.value)+3)*sizeof(char)); /* 3 for :, space and \0 */
		sprintf(outputDynamicList[i],"%s: %s",msg.description, msg.value);
	}
	*sizeArray=i;
	return outputDynamicList;
}
