/*--------------------------------------------------------------------------*/
/**
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */
/*--------------------------------------------------------------------------*/
#ifndef __INTERPRETERMANAGEMENT_H__
#define __INTERPRETERMANAGEMENT_H__

#include "BOOL.h" /* BOOL */

/**
 * Put a command in Scilab command queue so that Scilab executes it
 * The command is displayed in Scilab Window
 *
 * @param command the command to execute
 * @return execution status
 */
int putCommandInScilabQueue(char *command);

/*
* Stops Scilab current work
* @return execution status
*/
int interruptScilab(void);

/*--------------------------------------------------------------------------*/

#endif /* __INTERPRETERMANAGEMENT_H__ */
