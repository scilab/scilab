/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#ifndef _GET_DATA_PROPERTY_H_
#define _GET_DATA_PROPERTY_H_

/* the grayplot data is now given as a tlist (like for surface and champ objects) */

int getgrayplotdata (char *);

/* the champ data is now given as a tlist (like for surface objects) */
int getchampdata(char *pobjUID);

int get3ddata(char *pobjUID);

#endif /* _GET_DATA_PROPERTY_H_ */
