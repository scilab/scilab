/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Bruno JOFRET
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

#ifndef __ADDCOLOR_H__
#define __ADDCOLOR_H__

#define COLOR_COMPONENT 3

#include "dynlib_graphics.h"

/*
** Add color into figure colormap
** _iFig : the figure UID
** _pdblColor[3] : [R,G,B] color
*/
GRAPHICS_IMPEXP int addColor(int _iFig, double* _pdblColor);

#endif /* !__ADDCOLOR_H__ */
