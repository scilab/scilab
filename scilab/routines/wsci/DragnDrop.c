/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "DragnDrop.h"
/*-----------------------------------------------------------------------------------*/
extern void GetCurrentPrompt(char *CurrentPrompt);
extern int StoreCommand( char *command);
extern BOOL IsToThePrompt(void);
extern LPTSTR ScilabPathFindExtension(LPCTSTR pPath);
/*-----------------------------------------------------------------------------------*/
BOOL LaunchFilebyExtension(char *File);
/*-----------------------------------------------------------------------------------*/
void DragFunc (LPTW lptw, HDROP hdrop)
{
	static char szFile[MAX_PATH];
	int i, cFiles;
	
	cFiles = DragQueryFile (hdrop, 0xffffffff, (LPSTR) NULL, 0);

	lptw->bGetCh =FALSE;

	for (i = 0; i < cFiles; i++)
	{
		DragQueryFile (hdrop, i, szFile, MAX_PATH);
		LaunchFilebyExtension(szFile);	
	}
	DragFinish (hdrop);
}
/*-----------------------------------------------------------------------------------*/
BOOL LaunchFilebyExtension(char *File)
{
	BOOL bOK=FALSE;
	char save_prompt[10];
	char *CommandLine=NULL;
	char *ExtensionFilename=NULL;

	ExtensionFilename=ScilabPathFindExtension(File);
	GetCurrentPrompt(save_prompt);
	
	if ( (_stricmp(ExtensionFilename,".bin")==0) ||	(_stricmp(ExtensionFilename,".sav")==0) )
	{
		bOK=TRUE;
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("load('%s');printf('\n%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"load('%s');printf('\n%s');",File,save_prompt);
		else
			wsprintf(CommandLine,"load('%s');",File);
	}
	else 
	if ( (_stricmp(ExtensionFilename,".graph")==0) || (_stricmp(ExtensionFilename,".graphb")==0) )
	{
		bOK=TRUE;
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("edit_graph('%s');printf('\n%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"edit_graph('%s');printf('\n%s');",File,save_prompt);
		else
			wsprintf(CommandLine,"edit_graph('%s');",File);
	}
	else
	if ( (_stricmp(ExtensionFilename,".cos")==0) || (_stricmp(ExtensionFilename,".cosf")==0) )
	{
		bOK=TRUE;
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("scicos('%s');printf('\n%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"scicos('%s');printf('\n%s');",File,save_prompt);
		else
			wsprintf(CommandLine,"scicos('%s');printf('%s');",File);
	}
	else
	if (_stricmp(ExtensionFilename,".sci")==0)
	{
		bOK=TRUE;
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("getf('%s');printf('\n%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"getf('%s');printf('\n%s');",File,save_prompt);
		else
			wsprintf(CommandLine,"getf('%s');",File);
	}
	else
	if ( (_stricmp(ExtensionFilename,".sce")==0) || (_stricmp(ExtensionFilename,".tst")==0) || (_stricmp(ExtensionFilename,".dem")==0) )
	{
		bOK=TRUE;
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("exec('%s');printf('\n%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"exec('%s');printf('\n%s');",File,save_prompt);
		else
			wsprintf(CommandLine,"exec('%s');",File);
	}
	else
	if (_stricmp(ExtensionFilename,".scg")==0)
	{
		bOK=TRUE;
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("xload('%s');printf('\n%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"xload('%s');printf('\n%s');",File,save_prompt);
		else
			wsprintf(CommandLine,"xload('%s');",File);
	}
	else
	{
		CommandLine=(char*)MALLOC( (strlen(File)+strlen("disp('unknown file type : %s\n');printf('%s');")+strlen(save_prompt))*sizeof(char) );
		if (IsToThePrompt())
			wsprintf(CommandLine,"disp('unknown file type : %s\n');printf('%s');",ExtensionFilename,save_prompt);
		else
			wsprintf(CommandLine,"disp('unknown file type : %s\n');",ExtensionFilename);
		
		bOK=FALSE;
	}

	StoreCommand(CommandLine);
	if (CommandLine) {FREE(CommandLine);CommandLine=NULL;}
			
	FREE(ExtensionFilename);
	return bOK;

}
/*-----------------------------------------------------------------------------------*/

