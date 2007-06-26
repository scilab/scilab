/**
 *
 * @author INRIA 2007
 */

#include "machine.h"

/*
 * Checks if there's something on the 
 * commandQueue 
 * @return
 */
integer ismenu(void);

/**
 * @TODO : add comment
 *
 * @param flag  
 */
void SetCommandflag(int flag);

/**
 * try to execute a command or add it to the end of command queue 
 *
 * @param command the command
 * @return <ReturnValue>
 */
int StoreCommand (char *command);

/**
 * try to execute a command or add it to the end of command queue 
 * 
 * @param command the command
 * @param flag set to 1 to show the command in Scilab console
 * @return <ReturnValue>
 */
int StoreCommand1 (char *command, int flag);
