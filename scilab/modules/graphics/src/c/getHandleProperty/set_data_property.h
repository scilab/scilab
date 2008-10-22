/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

int setchampdata( sciPointObj * pobj, AssignedList * tlist );

int setgrayplotdata( sciPointObj * pobj, AssignedList * tlist );

int set3ddata( sciPointObj * pobj, AssignedList * tlist );

int CheckAndUpdate_x_shift(sciPointObj * pobj, int numrow);

int CheckAndUpdate_y_shift(sciPointObj * pobj, int numrow);

int CheckAndUpdate_z_shift(sciPointObj * pobj, int numrow);
