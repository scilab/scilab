/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan Cornet
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

/*--------------------------------------------------------------------------*/
#ifndef __INTGRAPHICS__
#define __INTGRAPHICS__

#include "dynlib_graphics.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
GRAPHICS_IMPEXP int sci_addcb(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_champ(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_champ1(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_champ_G(char *fname,
                                int (*func) (double *, double *, double *, double *, int *, int *, char *, double *, double *, int),
                                void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xarc(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xarcs(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_zoom_rect(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xtitle(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xstringb(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xstring(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xsetech(char *fname, void *pvApiCtx);
//GRAPHICS_IMPEXP int sci_xset(char *fname, void *pvApiCtx); => c++
GRAPHICS_IMPEXP int sci_xsegs(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xrects(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xrect(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xpolys(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xpoly(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xname(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xlfont(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xgrid(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xgraduate(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xgetmouse(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xgetech(char *fname, void *pvApiCtx);
//GRAPHICS_IMPEXP int sci_xget(char *fname, void *pvApiCtx); => c++
GRAPHICS_IMPEXP int sci_xfpolys(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xfpoly(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xfarcs(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xdel(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xclick(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xchange(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_xarrows(char *fname, void *pvApiCtx);
//GRAPHICS_IMPEXP int sci_winsid(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_unzoom(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_unglue(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_swap_handles(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_stringbox(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_show_window(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_set(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_rubberbox(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_rotate_axes(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot3d(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d1(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d1_2(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d1_3(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d1_4(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d2(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d3(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d4(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d1_G(char *fname, int ptype, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_plot2d(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_param3d1(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_param3d(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_newaxes(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_move(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_matlot1(char *fname, void *pvApiCtx);
//GRAPHICS_IMPEXP int sci_matplot1(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_matlot(char *fname, void *pvApiCtx);
//GRAPHICS_IMPEXP int sci_matplot(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_Legend(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_is_handle_valid(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_grayplot(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_glue(char *fname, void *pvApiCtx);
//GRAPHICS_IMPEXP int sci_get(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_geom3d(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_fec(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_drawnow(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_drawlater(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_drawaxis(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_demo(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_delete(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_copy(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_contour2di(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_addcolor(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_name2rgb(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_color(char *fname, void *pvApiCtx);
GRAPHICS_IMPEXP int sci_contour2dm(char *fname, void *pvApiCtx);
/*--------------------------------------------------------------------------*/
GRAPHICS_IMPEXP  C_GATEWAY_PROTOTYPE(sci_light);

#endif /* __INTGRAPHICS__ */
/*--------------------------------------------------------------------------*/
