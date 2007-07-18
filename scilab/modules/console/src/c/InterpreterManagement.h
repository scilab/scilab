/** 
 * @copyright INRIA 2007
 * @author Vincent COUVERT
 */

#include "../../../core/includes/dynamic_menus.h"
#include "../../../gui/includes/sxevents.h"

/**
 * Put a command in Scilab command queue so that Scilab executes it
 * The command is displayed in Scilab Window
 *
 * @param command the command to execute
 * @return execution status
 */

int putCommandInScilabQueue(char *command);

/**
 * Executes Scilab event loop so that queued callbacks can be executed
 * @return execution status
 */
int C2F(sxevents)(void);
