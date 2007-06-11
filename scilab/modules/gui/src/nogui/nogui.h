/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* @author Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/ 

/**
 * This function is a wrapper / fake when Scilab is compiled without thegraphic
 * graphic Interface
 *
 * @return 0
 */
int C2F(gw_gui)(void);


/**
 * This function is a wrapper when /fake Scilab is compiled without the 
 * graphic Interface
 *
 * @return TRUE
 */
BOOL TerminateGUI(void);

/**
 * This function is a wrapper when /fake Scilab is compiled without the 
 * graphic Interface
 * @param fname unused (just reproduce the profile)
 * @return 0
 */
int XClearScreenConsole(char *fname);
