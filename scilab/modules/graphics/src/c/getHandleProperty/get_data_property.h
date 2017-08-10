/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef _GET_DATA_PROPERTY_H_
#define _GET_DATA_PROPERTY_H_

void* getmatplotdata (int iObjUID);

/* the grayplot data is now given as a tlist (like for surface and champ objects) */

void* getgrayplotdata (int iObjUID);

/* the champ data is now given as a tlist (like for surface objects) */
void* getchampdata(int iObjUID);

void* get3ddata(int iObjUID);

#endif /* _GET_DATA_PROPERTY_H_ */
