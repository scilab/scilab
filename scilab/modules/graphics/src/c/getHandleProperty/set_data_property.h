/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
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
#ifndef __SET_DATA_PROPERTY_H__
#define __SET_DATA_PROPERTY_H__

#include "getPropertyAssignedValue.h"

int setchampdata(void* _pvCtx, int iObjUID, AssignedList * tlist);

int setgrayplotdata(void* _pvCtx, int iObjUID, AssignedList * tlist);

int set3ddata(void* _pvCtx, int iObjUID, AssignedList * tlist);

#endif /* ! __SET_DATA_PROPERTY_H__ */
