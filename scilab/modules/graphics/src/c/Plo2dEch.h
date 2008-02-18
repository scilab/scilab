/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre Ledru
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __PLO2DECH_H__
#define __PLO2DECH_H__

void get_frame_in_pixel(integer WIRect[]);
void get_cwindow_dims(int wdims[2]);
double Zoom3d_XPi2R(int x);
double Zoom3d_YPi2R(int y);

#endif /* __PLO2DECH_H__ */

