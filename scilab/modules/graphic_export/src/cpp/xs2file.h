/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * desc : interface for xs2file routine
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

#ifndef _XS2FILE_H_
#define _XS2FILE_H_

#include "exportToFile.hxx"

/**
 * Generic function to create either ps, fig, gif or ppm files.
 *
 * @param[in] fname     name of the routine (ie xs2ps, xs2fig,...)
 * @param fileType type of the export (ie JPG_EXPORT, BMP_EXPORT, PS_EXPORT, ...).
 * @return 0 if success, -1 otherwise
 */
int xs2file(char * fname, ExportFileType fileType, void* pvApiCtx);


#endif /* _INT_XS2FILE_H_ */
