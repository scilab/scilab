/**
 *
 * @author INRIA 2007
 */

/**
 * @TODO : add comment
 *
 * @param 
 * @return <ReturnValue>
 */
int iswaitingforinputend(void);

/*
 * Checks if there's something on the 
 * commandQueue 
 * @return
 */
integer C2F(ismenu)(void);

/**
 * @TODO : add comment
 *
 * @param flag  
 */
void SetCommandflag(int flag);

/**
 * @TODO : add comment
 *
 * @param flag  
 */
void C2F(waitforinputend)( int *flag);


/**
 * try to execute a command or add it to the end of command queue 
 *
 * @param command the command
 * @return <ReturnValue>
 */
int StoreCommand (char *command);
