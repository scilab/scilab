/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
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
