/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __FLAGS_H__
#define __FLAGS_H__

/**
 * set the echo mode of typed scilab commands 
 * @param mode the mode wanted
*/
void set_echo_mode(int mode);

/**
 * return the echo mode of the typed scilab commands
 * @return the mode
 */ 
int get_echo_mode(void);

/**
 * handle function when scilab waits for commands 
 * set the mode 
 * @param mode the mode wanted
*/
void set_is_reading(int mode);

/**
 * handle function when scilab waits for commands 
 * get the mode 
 * @return the reading mode
*/
int get_is_reading(void);

#endif /* __FLAGS_H__ */