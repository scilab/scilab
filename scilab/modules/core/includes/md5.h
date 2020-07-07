/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2021 - ESI Group - Clément DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __MD5_H__
#define __MD5_H__

#include "dynlib_core.h"

#include <stdio.h>

/*
** Compute md5 of a file
*/
CORE_IMPEXP char* md5_file(FILE* f);

#endif /* !__BANNER_HXX__ */
