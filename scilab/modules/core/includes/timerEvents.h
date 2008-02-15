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

#ifndef __TIMEREVENTS_H__
#define __TIMEREVENTS_H__
/**
 * a time interval of dt microsec (dt=10000)
 * @return 1 if interval from last call is greater than 

 */
int scilab_timer_check(void);

#endif /* __TIMEREVENTS_H__ */
