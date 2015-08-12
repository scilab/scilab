/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include "stack-c.h"
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
/*------------------------------------------------------------------------*/
#define sciGetStyle(PVCTX,fname,pos,n1,opts,style) { if (get_style_arg(PVCTX,fname,pos,n1,opts,style) == 0) { return 0 ; } }
#define GetRect(PVCTX,fname,pos,opts,rect) { if (get_rect_arg(PVCTX,fname,pos,opts,rect) == 0) { return 0 ; } }
#define GetStrf(PVCTX,fname,pos,opts,strf) { if (get_strf_arg(PVCTX,fname,pos,opts,strf) == 0) { return 0 ; } }
#define GetLegend(PVCTX,fname,pos,opts,legend) { if (get_legend_arg(PVCTX,fname,pos,opts,legend) == 0) { return 0 ; } }
#define GetLabels(PVCTX,fname,pos,opts,labels) { if (get_labels_arg(PVCTX,fname,pos,opts,labels) == 0) { return 0 ; } }
#define GetNax(PVCTX,pos,opts,nax,flagNax) { if (get_nax_arg(PVCTX,pos,opts,nax,flagNax)==0) { return 0 ; } }
#define GetZminmax(PVCTX,fname,pos,opts,zminmax) { if (get_zminmax_arg(PVCTX,fname,pos,opts,zminmax) == 0) { return 0 ; } }
#define GetColminmax(PVCTX,fname,pos,opts,colminmax) { if (get_colminmax_arg(PVCTX,fname,pos,opts,colminmax)==0) { return 0 ; } }
#define GetColOut(PVCTX,fname,pos,opts,colout) { if (get_colout_arg(PVCTX,fname,pos,opts,colout)==0) { return 0 ; } }
#define GetWithMesh(PVCTX,fname,pos,opts,withMesh) { if (get_with_mesh_arg(PVCTX,fname,pos,opts,withMesh)==0) { return 0 ; } }
#define GetLogflags(PVCTX, fname,pos,opts,logFlags) { if (get_logflags_arg(PVCTX, fname,pos,opts,logFlags) == 0) { return 0 ; } }
#define GetOptionalDoubleArg(PVCTX,fname,pos,name,value,sz,opts) { if (get_optional_double_arg(PVCTX,fname,pos,name,value,sz,opts) == 0) { return 0 ; } }
#define GetOptionalIntArg(PVCTX,fname,pos,name,value,sz,opts) { if (get_optional_int_arg(PVCTX,fname,pos,name,value,sz,opts) == 0) { return 0 ; } }
/*------------------------------------------------------------------------*/

#endif /* _GET_COMMAND_ARG_H_ */
