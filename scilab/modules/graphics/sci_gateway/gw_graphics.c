/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_graphics.h"
#include <string.h>
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
extern int GetWITH_GUI(void);
/*-----------------------------------------------------------------------------------*/ 
extern int scichamp                       _PARAMS((char *fname, unsigned long fname_len));
extern int sci_contour                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_param3d                    _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot3d                      _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot3d1                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_plot2d                     _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_1                   _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_2                   _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_3                   _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_4                   _PARAMS((char *fname, unsigned long fname_len));
extern int sci_grayplot                   _PARAMS((char *fname, unsigned long fname_len));
extern int sci_driver                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xarc                       _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xarcs                      _PARAMS((char *fname, unsigned long fname_len));
extern int scirects                       _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xarrows                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xsegs                      _PARAMS((char *fname, unsigned long fname_len));
extern int nscixaxis                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xchange                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xclea                      _PARAMS((char *fname, unsigned long fname_len));
extern int scirect                        _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xclear                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xclick                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xend                       _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xfpoly                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xfpolys                    _PARAMS((char *fname, unsigned long fname_len));
extern int scixget                        _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xinit                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xlfont                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xpoly                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xpolys                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xselect                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xset                       _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xstring                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xstringl                   _PARAMS((char *fname, unsigned long fname_len));
extern int sci_StringBox                  _PARAMS((char *fname, unsigned long fname_len)) ;
extern int sci_xtape                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xsetech                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xgetech                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_geom3d                     _PARAMS((char *fname, unsigned long fname_len));
extern int sci_fec                        _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xgetmouse                  _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xinfo                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xtitle                     _PARAMS((char *fname, unsigned long fname_len));
extern int scixtitle                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xgrid                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xfarcs                     _PARAMS((char *fname, unsigned long fname_len));
extern int scixsave                       _PARAMS((char *fname, unsigned long fname_len));
extern int scixload                       _PARAMS((char *fname, unsigned long fname_len));
extern int scichamp1                      _PARAMS((char *fname, unsigned long fname_len));
extern int scidelw                        _PARAMS((char *fname, unsigned long fname_len));
extern int scicontour2d                   _PARAMS((char *fname, unsigned long fname_len));
extern int scixg2ps                       _PARAMS((char *fname, unsigned long fname_len));
extern int scixs2ps                       _PARAMS((char *fname, unsigned long fname_len));
extern int scixs2fig                      _PARAMS((char *fname, unsigned long fname_len));
extern int scixsort                       _PARAMS((char *fname, unsigned long fname_len));
extern int sciwinsid                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_param3d1                   _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xstringb                   _PARAMS((char *fname, unsigned long fname_len));
extern int sci_matplot                    _PARAMS((char *fname, unsigned long fname_len));
extern int sci_contour2d1                 _PARAMS((char *fname, unsigned long fname_len));
extern int sci_matplot1                   _PARAMS((char *fname, unsigned long fname_len));
extern int scixgraduate                   _PARAMS((char *fname, unsigned long fname_len));
extern int scixname                       _PARAMS((char *fname, unsigned long fname_len));
extern int sci_xaxis                      _PARAMS((char *fname, unsigned long fname_len));
extern int sciseteventhandler             _PARAMS((char *fname, unsigned long fname_len));
extern int int_gtkhelp                    _PARAMS((char *fname, unsigned long fname_len));
extern int gset                           _PARAMS((char *fname, unsigned long fname_len));
extern int gget                           _PARAMS((char *fname, unsigned long fname_len));
extern int sci_delete                     _PARAMS((char *fname, unsigned long fname_len));
extern int addcb                          _PARAMS((char *fname, unsigned long fname_len));
extern int copy                           _PARAMS((char *fname, unsigned long subwinparenttarget));
extern int move                           _PARAMS((char *fname, unsigned long fname_len));
extern int glue                           _PARAMS((char *fname, unsigned long fname_len));
extern int unglue                         _PARAMS((char *fname, unsigned long fname_len));
extern int drawnow                        _PARAMS((char *fname, unsigned long fname_len));
extern int drawlater                      _PARAMS((char *fname, unsigned long fname_len));
extern int sci_draw                       _PARAMS((char *fname, unsigned long fname_len));
extern int scixs2gif                      _PARAMS((char *fname,unsigned long fname_len));
extern int scixs2ppm                      _PARAMS((char *fname,unsigned long fname_len));
extern int ClearScreenConsole             _PARAMS((char *fname, unsigned long fname_len));
extern int ShowWindowFunction             _PARAMS((char *fname, unsigned long fname_len));
extern int HomeFunction                   _PARAMS((char *fname, unsigned long fname_len));
extern int scisetposfig                   _PARAMS((char *fname,unsigned long fname_len));
extern int XSaveNative                    _PARAMS((char *fname, unsigned long fname_len));
extern int scizoomrect                    _PARAMS((char *fname, unsigned long fname_len));
extern int sciunzoom                      _PARAMS((char *fname,unsigned long fname_len));
extern int intxs2bmp                      _PARAMS((char *fname,unsigned long fname_len));
extern int intxs2emf                      _PARAMS((char *fname,unsigned long fname_len));
extern int intUImenu                      _PARAMS((char *fname,unsigned long fname_len));
extern int sci_newaxes                     _PARAMS((char *fname,unsigned long fname_len));
extern int intshowalluimenushandles       _PARAMS(( char * fname, unsigned long fname_len ) ) ;
extern int sciRelocateHandle              _PARAMS(( char * fname, unsigned long fname_len ) ) ;
extern int sciSwapHandles                 _PARAMS(( char * fname, unsigned long fname_len ) ) ;
extern int sci_readxbm	                  _PARAMS(( char * fname, unsigned long fname_len ) ) ;
extern int sci_readgif	                  _PARAMS(( char * fname, unsigned long fname_len ) ) ;
/*-----------------------------------------------------------------------------------*/ 
static MatdesTable Tab[]={
	{scichamp,"champ"},	
	{scichamp1,"champ1"},
	{sci_contour,"contour"},
	{sci_driver,"driver"},
	{sci_fec,"scifec"},
	{sci_geom3d,"geom3d"},
	{sci_grayplot,"grayplot"},
	{sci_matplot,"Matplot"},
	{sci_matplot1,"Matplot1"}, 
	{sci_contour2d1,"contour2di"},
	{sci_param3d,"param3d"},
	{sci_param3d1,"param3d1"},
	{sci_xstringb,"xstringb"},
	{sci_plot2d,"plot2d"},
	{sciplot2d1_1,"plot2d1"},
	{sciplot2d1_2,"plot2d2"},
	{sciplot2d1_3,"plot2d3"},
	{sciplot2d1_4,"plot2d4"},
	{sciplot3d,"plot3d"},
	{sciplot3d1,"plot3d1"},
	{sciwinsid,"winsid"},
	{sci_xarc,"xarc"},
	{sci_xarcs,"xarcs"},
	{sci_xarrows,"xarrows"},
	{nscixaxis,"drawaxis"},
	{sci_xchange,"xchange"},
	{sci_xclea,"xclea"},
	{sci_xclear,"xclear"},
	{sci_xclick,"xclick"},
	{scidelw,"xdel"},
	{sci_xend,"xend"},
	{sci_xarc,"xfarc"},
	{sci_xfarcs,"xfarcs"},
	{sci_xfpoly,"xfpoly"},
	{sci_xfpolys,"xfpolys"},
	{scirect,"xfrect"},
	{scixg2ps,"xg2ps"},
	{scixget,"xget"},
	{sci_xgetech,"xgetech"},
	{sci_xgetmouse,"xgetmouse"},
	{sci_xgrid,"xgrid"},
	{sci_xinfo,"xinfo"},
	{sci_xinit,"xinit"},
	{sci_xlfont,"xlfont"},
	{scixload,"xload"},
	{sci_xpoly,"xpoly"},
	{sci_xpolys,"xpolys"},
	{scirect,"xrect"},
	{scirects,"xrects"},
	{scixs2fig,"xg2fig"},
	{scixsave,"xsave"},
	{sci_xsegs,"xsegs"},
	{sci_xselect,"xselect"},
	{sci_xset,"xset"},
	{sci_xsetech,"xsetech"},
	{sci_xstring,"xstring"},
	{sci_xstringl,"xstringl"},
	{sci_xtape,"xtape"},
	{sci_xtitle,"xtitle"},
	{scixgraduate,"xgraduate"},
	{scixname,"xname"},
	{sci_xaxis,"xaxis"},
	{sciseteventhandler,"seteventhandler"},
	{scixs2gif,"xs2gif"},
	{scixs2ppm,"xs2ppm"},
	{scixs2ps,"xs2ps"},
	{intxs2bmp,"xs2bmp"},
	{intxs2emf,"xs2emf"},
	{scisetposfig,"set_posfig_dim"},
	{XSaveNative,"xsnative"},
	{ShowWindowFunction,"show_window"},
	{scizoomrect,"zoom_rect"},
	{sciunzoom,"unzoom"},
	{sci_StringBox,"StringBox"},
	/* NG beg */
	{move,"move"},
	{glue,"glue"},
	{unglue,"unglue"}, 
	{drawnow,"drawnow"},
	{drawlater,"drawlater"},  
	{sci_draw,"draw"},
	{addcb,"addcb"},
	{copy,"copy"},
	{sci_delete,"delete"},
	{gget,"get"},
	{gset,"set"},
	{sci_newaxes,"newaxes"},
  {sciRelocateHandle,"relocate_handle"},
  {sciSwapHandles,"swap_handles"},
     /* NG end */
	{scixsort,"gsort"},
	{int_gtkhelp,"help_gtk"},
	{ClearScreenConsole,"clc"},
	{HomeFunction,"tohome"},
	{intUImenu,"UImenu"},
	{intshowalluimenushandles,"showalluimenushandles"},
	{sci_readxbm,"readxbm"},
	{sci_readgif,"readgif"}
};

/*-----------------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
int C2F(gw_graphics)()
{  
  Rhs = Max(0, Rhs);
  if ( GetWITH_GUI() || (strcmp(Tab[Fin-1].name,"gsort")==0) ) /*Veru en attendant de deplacer gsort*/
  {
#if _MSC_VER
#ifndef _DEBUG
	  _try
	  {
		  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	  }
	  _except (EXCEPTION_EXECUTE_HANDLER)
	  {
		  char *ExceptionString=GetExceptionString(GetExceptionCode());
		  sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
		  if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
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
