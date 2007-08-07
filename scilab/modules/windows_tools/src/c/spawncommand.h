/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __SPAWNCOMMAND_H__
#define __SPAWNCOMMAND_H__
/*-----------------------------------------------------------------------------------*/
#include <Windows.h>
#include "machine.h" /* BOOL */
/*-----------------------------------------------------------------------------------*/
typedef struct pipeinfo
{
	HANDLE pipe;
	char *OutputBuffer;
	int NumberOfLines;
} pipeinfo;
/*-----------------------------------------------------------------------------------*/
/**
* spawn a command
* @param[in] command
* @param[in] DetachProcess (if we want detach to scilab process)
* @return 0
*/
int spawncommand(char *command,BOOL DetachProcess);

/**
* ReadFromPipe (in or out)
* @param[in] pipe handle
* @return 0
*/
DWORD WINAPI ReadFromPipe (LPVOID args);

/**
* check if we have a '&' (detach process)
* @param[in] command
* @return TRUE or FALSE
*/
BOOL DetectDetachProcessInCommandLine(char *command);

/**
* CreateOuput
* @param[in] pipe
* @param[in] DetachProcess
* @return output
*/
char **CreateOuput(pipeinfo *pipe,BOOL DetachProcess);

/**
* Close pipe
* @param[in] pipe
* @return 0
*/
int ClosePipeInfo (pipeinfo pipe);

#endif /* __SPAWNCOMMAND_H__ */
/*-----------------------------------------------------------------------------------*/
