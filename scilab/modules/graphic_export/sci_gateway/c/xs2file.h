/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * desc : interface for xs2file routine
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int xs2file(char * fname, ExportFileType fileType) ;


#endif /* _INT_XS2FILE_H_ */
