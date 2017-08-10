/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __GW_FILEIO_H__
#define __GW_FILEIO_H__
#include "dynlib_fileio.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
FILEIO_IMPEXP int gw_fileio(void);
/*--------------------------------------------------------------------------*/
STACK_GATEWAY_PROTOTYPE(sci_pwd);
STACK_GATEWAY_PROTOTYPE(sci_chdir);
STACK_GATEWAY_PROTOTYPE(sci_copyfile);
STACK_GATEWAY_PROTOTYPE(sci_movefile);
STACK_GATEWAY_PROTOTYPE(sci_fscanfMat);
STACK_GATEWAY_PROTOTYPE(sci_fprintfMat);
STACK_GATEWAY_PROTOTYPE(sci_getURL);
STACK_GATEWAY_PROTOTYPE(sci_splitURL);
STACK_GATEWAY_PROTOTYPE(sci_tempname);
/*--------------------------------------------------------------------------*/
#endif /* __GW_FILEIO_H__ */
/*--------------------------------------------------------------------------*/
