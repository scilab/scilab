/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
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

/*------------------------------------------------------------------------*/
/* file: GetCommandArg.h                                                  */
/* desc : tools to retrieve parameters within the command line for        */
/*        graphic routines.                                               */
/*------------------------------------------------------------------------*/

#ifndef _GET_COMMAND_ARG_H_
#define _GET_COMMAND_ARG_H_

#include "dynlib_graphics.h"
#include "api_scilab.h"
#include "BOOL.h"

/*------------------------------------------------------------------------*/
/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param n1
 * @param rhs_opts opts[]
 * @param style
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_style_arg(void* _pvCtx, char *fname, int pos, int n1, rhs_opts opts[], int ** style);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param rect
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_rect_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], double ** rect);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param strf
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_strf_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** strf);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param legend
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_legend_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** legend);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param labels
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_labels_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** labels);

/**
 * @TODO add comment
 *
 * @param pos
 * @param rhs_opts opts[]
 * @param nax
 * @param flagNax
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_nax_arg(void* _pvCtx, int pos, rhs_opts opts[], int ** nax, BOOL * flagNax);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param zminmax
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_zminmax_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], double ** zminmax);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param colminmax
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_colminmax_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], int ** colminmax);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param colout
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_colout_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], int ** colout);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param withMesh
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_with_mesh_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], BOOL * withMesh);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param rhs_opts opts[]
 * @param logFlags
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_logflags_arg(void* _pvCtx, char *fname, int pos, rhs_opts opts[], char ** logFlags);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param name
 * @param value
 * @param sz
 * @param  rhs_opts opts[]
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_optional_double_arg(void* _pvCtx, char * fname, int pos, char * name, double ** value, int sz, rhs_opts opts[]);

/**
 * @TODO add comment
 *
 * @param fname
 * @param pos
 * @param name
 * @param value
 * @param sz
 * @param  rhs_opts opts[]
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int get_optional_int_arg(void* _pvCtx, char * fname, int pos, char * name, int ** value, int sz, rhs_opts opts[]);

#endif /* _GET_COMMAND_ARG_H_ */
