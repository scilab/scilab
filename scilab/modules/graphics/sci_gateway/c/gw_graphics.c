/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_graphics.h"
#include "stack-c.h"
#include "graphicModuleLoad.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct {
	des_interf f;    /** function **/
	char *name;      /** its name **/
} MatdesTable;
/*-----------------------------------------------------------------------------------*/ 
static MatdesTable Tab[]={
	{sci_champ,"champ"},	
	{sci_champ1,"champ1"},
	{sci_driver,"driver"},
	{sci_fec,"scifec"},
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
	{sci_xclea,"xclea"},
	{sci_xclear,"xclear"},
	{sci_xclick,"xclick"},
	{sci_xdel,"xdel"},
	{sci_xend,"xend"},
	{sci_xarc,"xfarc"},
	{sci_xfarcs,"xfarcs"},
	{sci_xfpoly,"xfpoly"},
	{sci_xfpolys,"xfpolys"},
	{sci_xrect,"xfrect"},
	{sci_xg2ps,"xg2ps"},
	{sci_xget,"xget"},
	{sci_xgetech,"xgetech"},
	{sci_xgetmouse,"xgetmouse"},
	{sci_xgrid,"xgrid"},
	{sci_xinit,"xinit"},
	{sci_xlfont,"xlfont"},
	{sci_xpoly,"xpoly"},
	{sci_xpolys,"xpolys"},
	{sci_xrect,"xrect"},
	{sci_xrects,"xrects"},
	{sci_xs2fig,"xg2fig"},
	{sci_xsegs,"xsegs"},
	{sci_show_window,"show_window"},
	{sci_xset,"xset"},
	{sci_xsetech,"xsetech"},
	{sci_xstring,"xstring"},
	{sci_xstringl,"xstringl"},
	{sci_xtape,"xtape"},
	{sci_xtitle,"xtitle"},
	{sci_xgraduate,"xgraduate"},
	{sci_xname,"xname"},
	{sci_xaxis,"xaxis"},
	{sci_xs2gif,"xs2gif"},
	{sci_xs2ppm,"xs2ppm"},
	{sci_xs2ps,"xs2ps"},
	{sci_xs2bmp,"xs2bmp"},
	{sci_xs2emf,"xs2emf"},
	{sci_set_posfig_dim,"set_posfig_dim"},
	{sci_xsnative,"xsnative"},
	{sci_zoom_rect,"zoom_rect"},
	{sci_unzoom,"unzoom"},
	{sci_StringBox,"StringBox"},
	/* NG beg */
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
     /* NG end */
	{sci_xsort,"gsort"},
	{sci_help_gtk,"help_gtk"},
	{sci_UImenu,"UImenu"},
	{sci_showalluimenushandles,"showalluimenushandles"},
	{sci_readxbm,"readxbm"},
	{sci_readgif,"readgif"}
};

/*-----------------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
int C2F(gw_graphics)(void)
{  
  Rhs = Max(0, Rhs);
  if ( (getScilabMode() != SCILAB_NWNI) || (strcmp(Tab[Fin-1].name,"gsort")==0) ) /*Veru en attendant de deplacer gsort*/
  {
    /* create needed data structure if not already created */
    loadGraphicModule() ;
#ifdef _MSC_VER
#ifndef _DEBUG
	  _try
	  {
		  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	  }
	  _except (EXCEPTION_EXECUTE_HANDLER)
	  {
		  ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
	  }
#else
	  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
#else
	  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif

	  C2F(putlhsvar)();
  }
  else
  {
	  Scierror(999,"graphic interface disabled -nogui mode.\r\n");
	  return 0;
  }

  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
