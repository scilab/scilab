/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include "gw_graphics.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"
#include "rendererBasicAlgos.h"
/*--------------------------------------------------------------------------*/ 
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
	{sci_champ,"champ"},	
	{sci_champ1,"champ1"},
	{sci_fec,"fec"},
	{sci_geom3d,"geom3d"},
	{sci_grayplot,"grayplot"},
	{sci_matplot,"Matplot"},
	{sci_matplot1,"Matplot1"}, 
	{sci_contour2di,"contour2di"},
	{sci_param3d,"param3d"},
	{sci_param3d1,"param3d1"},
	{sci_xstringb,"xstringb"},
	{sci_plot2d,"plot2d"},
	{sci_plot2d1_1,"plot2d1"},
	{sci_plot2d1_2,"plot2d2"},
	{sci_plot2d1_3,"plot2d3"},
	{sci_plot2d1_4,"plot2d4"},
	{sci_plot3d,"plot3d"},
	{sci_plot3d,"plot3d1"},
	{sci_winsid,"winsid"},
	{sci_xarc,"xarc"},
	{sci_xarcs,"xarcs"},
	{sci_xarrows,"xarrows"},
	{sci_drawaxis,"drawaxis"},
	{sci_xchange,"xchange"},
	{sci_show_pixmap,"show_pixmap"},
	{sci_xclick,"xclick"},
	{sci_xdel,"xdel"},
	{sci_xarc,"xfarc"},
	{sci_xfarcs,"xfarcs"},
	{sci_xfpoly,"xfpoly"},
	{sci_xfpolys,"xfpolys"},
	{sci_xrect,"xfrect"},
	{sci_xget,"xget"},
	{sci_xgetech,"xgetech"},
	{sci_xgetmouse,"xgetmouse"},
	{sci_xgrid,"xgrid"},
	{sci_xlfont,"xlfont"},
	{sci_xpoly,"xpoly"},
	{sci_xpolys,"xpolys"},
	{sci_xrect,"xrect"},
	{sci_xrects,"xrects"},
	{sci_xsegs,"xsegs"},
	{sci_show_window,"show_window"},
	{sci_xset,"xset"},
	{sci_xsetech,"xsetech"},
	{sci_xstring,"xstring"},
	{sci_xtitle,"xtitle"},
	{sci_xgraduate,"xgraduate"},
	{sci_xname,"xname"},
	{sci_clear_pixmap,"clear_pixmap"},
	{sci_zoom_rect,"zoom_rect"},
	{sci_unzoom,"unzoom"},
	{sci_stringbox,"stringbox"},
	{sci_move,"move"},
	{sci_glue,"glue"},
	{sci_unglue,"unglue"}, 
	{sci_drawnow,"drawnow"},
	{sci_drawlater,"drawlater"},  
	{sci_draw,"draw"},
	{sci_addcb,"addcb"},
	{sci_copy,"copy"},
	{sci_delete,"delete"},
	{sci_get,"get"},
	{sci_set,"set"},
	{sci_newaxes,"newaxes"},
	{sci_relocate_handle,"relocate_handle"},
	{sci_swap_handles,"swap_handles"},
	{sci_showalluimenushandles,"showalluimenushandles"},
	{sci_rubberbox,"rubberbox"},
	{sci_rotate_axes,"rotate_axes"},
	{sci_is_handle_valid,"is_handle_valid"},
	{sci_Legend,"captions"}
};

/*--------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
int gw_graphics(void)
{  
	Rhs = Max(0, Rhs);

	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (!loadedDep) 
		{
			loadOnUseClassPath("graphics");
			loadedDep = TRUE;
		}
		callFunctionFromGatewayWithExceptions(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	}
	else
	{
		Scierror(999,_("Scilab graphic module disabled -nogui or -nwni mode.\n"));
	}

	return 0;
}
/*--------------------------------------------------------------------------*/ 
