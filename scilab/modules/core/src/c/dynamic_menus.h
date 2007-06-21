/**
 *
 * @author INRIA 2007
 */

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
