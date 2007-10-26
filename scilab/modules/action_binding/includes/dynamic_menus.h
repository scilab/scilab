/**
 *
 * @author INRIA 2007
 */
#ifndef __DYNAMIC_MENUS_H__
#define __DYNAMIC_MENUS_H__

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

/**
 * @TODO add comment
 *
 * @param str   
 * @return <ReturnValue>
 */
int GetCommand (char *str);

#endif /* __DYNAMIC_MENUS_H__ */
