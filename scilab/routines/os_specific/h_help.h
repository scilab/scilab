#ifndef __SCIHELP
#define __SCIHELP

#include "../machine.h" 

/**************************************
 * extern data 
 **************************************/

extern char   **helpTopicInfo ;
extern int      nTopicInfo ;

#define MAX_HELP_CHAPTERS 100

extern char* helpInfo[MAX_HELP_CHAPTERS];
extern char* helpPaths[MAX_HELP_CHAPTERS];
extern int   nInfo;

#define APROPOSMAX 100
#define MAXTOPIC 56

typedef struct {
  char name[MAXTOPIC];
  char *HelpTopic[APROPOSMAX];
  int  Where[APROPOSMAX];
  int  nTopic;
} Apropos;

extern  Apropos AP ;

#ifndef WIN32

extern void initHelpActions();
extern void initHelpPanel();
extern void setHelpShellState();

#else

extern void SciCallHelp  __PARAMS((char *helpPath,char *Topic));
extern int HelpGetPath  __PARAMS((char* line,char *Path,char *Tit));

#endif /* WIN32 */

extern int Sci_Help __PARAMS((char *name));
extern int Sci_Apropos __PARAMS((char *name));
extern int Help_Init   __PARAMS((void));
extern int setHelpTopicInfo  __PARAMS((int n));
extern void HelpActivate __PARAMS((int ntopic));
extern int initHelpDatas  __PARAMS((void));
extern void SciCallHelp  __PARAMS((char *helpPath,char *Topic));
extern int HelpGetPath  __PARAMS((char* line,char *Path,char *Tit));
extern int SetAproposTopics  __PARAMS((char *));

#endif /*  __SCIHELP */

