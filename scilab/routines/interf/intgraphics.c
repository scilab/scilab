/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "intgraphics.h"
#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
#if WIN32
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
extern int GetWITH_GUI(void);
/*-----------------------------------------------------------------------------------*/ 
extern int scichamp      _PARAMS((char *fname, unsigned long fname_len));
extern int scicontour    _PARAMS((char *fname, unsigned long fname_len));
extern int sciparam3d    _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot3d     _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot3d1    _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d     _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_1  _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_2  _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_3  _PARAMS((char *fname, unsigned long fname_len));
extern int sciplot2d1_4  _PARAMS((char *fname, unsigned long fname_len));
extern int scigrayplot   _PARAMS((char *fname, unsigned long fname_len));
extern int scidriver     _PARAMS((char *fname, unsigned long fname_len));
extern int scixarc       _PARAMS((char *fname, unsigned long fname_len));
extern int scixarcs      _PARAMS((char *fname, unsigned long fname_len));
extern int scirects      _PARAMS((char *fname, unsigned long fname_len));
extern int sciarrows     _PARAMS((char *fname, unsigned long fname_len));
extern int scixsegs      _PARAMS((char *fname, unsigned long fname_len));
extern int nscixaxis     _PARAMS((char *fname, unsigned long fname_len));
extern int scixchange    _PARAMS((char *fname, unsigned long fname_len));
extern int scixclea      _PARAMS((char *fname, unsigned long fname_len));
extern int scirect       _PARAMS((char *fname, unsigned long fname_len));
extern int scixclear     _PARAMS((char *fname, unsigned long fname_len));
extern int scixclick     _PARAMS((char *fname, unsigned long fname_len));
extern int scixend       _PARAMS((char *fname, unsigned long fname_len));
extern int scixfpoly     _PARAMS((char *fname, unsigned long fname_len));
extern int scixfpolys    _PARAMS((char *fname, unsigned long fname_len));
extern int scixget       _PARAMS((char *fname, unsigned long fname_len));
extern int scixinit      _PARAMS((char *fname, unsigned long fname_len));
extern int scixlfont     _PARAMS((char *fname, unsigned long fname_len));
extern int scixnumb      _PARAMS((char *fname, unsigned long fname_len));
extern int scixpause     _PARAMS((char *fname, unsigned long fname_len));
extern int scixpoly      _PARAMS((char *fname, unsigned long fname_len));
extern int scixpolys     _PARAMS((char *fname, unsigned long fname_len));
extern int scixselect    _PARAMS((char *fname, unsigned long fname_len));
extern int scixset       _PARAMS((char *fname, unsigned long fname_len));
extern int scixstring    _PARAMS((char *fname, unsigned long fname_len));
extern int scixstringl   _PARAMS((char *fname, unsigned long fname_len));
extern int sciStringBox  _PARAMS((char *fname, unsigned long fname_len)) ;
extern int scixtape      _PARAMS((char *fname, unsigned long fname_len));
extern int scixsetech    _PARAMS((char *fname, unsigned long fname_len));
extern int scixgetech    _PARAMS((char *fname, unsigned long fname_len));
extern int scigeom3d     _PARAMS((char *fname, unsigned long fname_len));
extern int scifec        _PARAMS((char *fname, unsigned long fname_len));
extern int scixgetmouse  _PARAMS((char *fname, unsigned long fname_len));
extern int scixinfo      _PARAMS((char *fname, unsigned long fname_len));
extern int scixtitle     _PARAMS((char *fname, unsigned long fname_len));
extern int scixtitle     _PARAMS((char *fname, unsigned long fname_len));
extern int scixgrid      _PARAMS((char *fname, unsigned long fname_len));
extern int scixfarcs     _PARAMS((char *fname, unsigned long fname_len));
extern int scixsave      _PARAMS((char *fname, unsigned long fname_len));
extern int scixload      _PARAMS((char *fname, unsigned long fname_len));
extern int scichamp1     _PARAMS((char *fname, unsigned long fname_len));
extern int scidelw       _PARAMS((char *fname, unsigned long fname_len));
extern int scicontour2d  _PARAMS((char *fname, unsigned long fname_len));
extern int scixg2ps      _PARAMS((char *fname, unsigned long fname_len));
extern int scixs2ps      _PARAMS((char *fname, unsigned long fname_len));
extern int scixs2fig     _PARAMS((char *fname, unsigned long fname_len));
extern int scixsort      _PARAMS((char *fname, unsigned long fname_len));
extern int sciwinsid     _PARAMS((char *fname, unsigned long fname_len));
extern int sciparam3d1   _PARAMS((char *fname, unsigned long fname_len));
extern int scixstringb   _PARAMS((char *fname, unsigned long fname_len));
extern int scimatplot    _PARAMS((char *fname, unsigned long fname_len));
extern int scicontour2d1 _PARAMS((char *fname, unsigned long fname_len));
extern int scic2dex      _PARAMS((char *fname, unsigned long fname_len));
extern int scigray2plot  _PARAMS((char *fname, unsigned long fname_len));
extern int scixgraduate  _PARAMS((char *fname, unsigned long fname_len));
extern int scixname      _PARAMS((char *fname, unsigned long fname_len));
extern int scixaxis      _PARAMS((char *fname, unsigned long fname_len));
extern int sciseteventhandler _PARAMS((char *fname, unsigned long fname_len));
extern int int_gtkhelp   _PARAMS((char *fname, unsigned long fname_len));
extern int gset          _PARAMS((char *fname, unsigned long fname_len));
extern int gget          _PARAMS((char *fname, unsigned long fname_len));
extern int sci_delete    _PARAMS((char *fname, unsigned long fname_len));
extern int addcb         _PARAMS((char *fname, unsigned long fname_len));
extern int copy          _PARAMS((char *fname, unsigned long subwinparenttarget));
extern int move          _PARAMS((char *fname, unsigned long fname_len));
extern int glue _PARAMS((char *fname, unsigned long fname_len));
extern int unglue _PARAMS((char *fname, unsigned long fname_len));
extern int drawnow _PARAMS((char *fname, unsigned long fname_len));
extern int drawlater _PARAMS((char *fname, unsigned long fname_len));
extern int draw _PARAMS((char *fname, unsigned long fname_len));
extern int scixs2gif _PARAMS((char *fname,unsigned long fname_len));
extern int scixs2ppm _PARAMS((char *fname,unsigned long fname_len));
extern int ClearScreenConsole _PARAMS((char *fname, unsigned long fname_len));
extern int ShowWindowFunction _PARAMS((char *fname, unsigned long fname_len));
extern int HomeFunction _PARAMS((char *fname, unsigned long fname_len));
extern int scisetposfig _PARAMS((char *fname,unsigned long fname_len));
extern int XSaveNative _PARAMS((char *fname, unsigned long fname_len));
extern int scizoomrect _PARAMS((char *fname, unsigned long fname_len));
extern int sciunzoom _PARAMS((char *fname,unsigned long fname_len));
extern int intxs2bmp _PARAMS((char *fname,unsigned long fname_len));
extern int intxs2emf _PARAMS((char *fname,unsigned long fname_len));
extern int intUImenu _PARAMS((char *fname,unsigned long fname_len));
extern int scinewaxes _PARAMS((char *fname,unsigned long fname_len));
int intshowalluimenushandles _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static MatdesTable Tab[]={
  {scichamp,"champ"},
  {scicontour,"contour"},
  {sciparam3d,"param3d"},
  {sciplot3d,"plot3d"},
  {sciplot3d1,"plot3d1"},
  {sciplot2d,"plot2d"},
  {sciplot2d1_1,"plot2d1"},
  {sciplot2d1_2,"plot2d2"},
  {sciplot2d1_3,"plot2d3"},
  {sciplot2d1_4,"plot2d4"},
  {scigrayplot,"grayplot"},
  {scidriver,"driver"},
  {scixarc,"xfarc"},
  {scixarc,"xarc"},
  {scixarcs,"xarcs"},
  {scirects,"xrects"},/* NG */
  {sciarrows,"xarrows"},
  {scixsegs,"xsegs"},
  {nscixaxis,"drawaxis"},
  {scixchange,"xchange"},
  {scixclea,"xclea"},
  {scirect,"xrect"},
  {scirect,"xfrect"},
  {scixclear,"xclear"},
  {scixclick,"xclick"},
  {scixend,"xend"},
  {scixfpoly,"xfpoly"},
  {scixfpolys,"xfpolys"},
  {scixget,"xget"},
  {scixinit,"xinit"},
  {scixlfont,"xlfont"},
  {scixnumb,"xnumb"},
  {scixpause,"xpause"},
  {scixpoly,"xpoly"},
  {scixpolys,"xpolys"},
  {scixselect,"xselect"},
  {scixset,"xset"},
  {scixstring,"xstring"},
  {scixstringl,"xstringl"},
  {scixtape,"xtape"},
  {scixsetech,"xsetech"},
  {scixgetech,"xgetech"},
  {scigeom3d,"geom3d"},
  {scifec,"scifec"},
  {scixgetmouse,"xgetmouse"},
  {scixinfo,"xinfo"},
  {scixtitle,"xtitle"},
  {scixgrid,"xgrid"},
  {scixfarcs,"xfarcs"},
  {scixsave,"xsave"},
  {scixload,"xload"},
  {scichamp1,"champ1"},
  {scidelw,"xdel"},
  {scicontour2d,"contour2d"},
  {scixg2ps,"xg2ps"},
  {scixs2fig,"xg2fig"},
  {scixsort,"gsort"},
  {sciwinsid,"winsid"},
  {sciparam3d1,"param3d1"},
  {scixstringb,"xstringb"},
  {scimatplot,"Matplot"},
  {scicontour2d1,"contour2di"},
  {scic2dex,"c2dex"},
  {scigray2plot,"Matplot1"}, 
  {scixgraduate,"xgraduate"},
  {scixname,"xname"},
  {scixaxis,"xaxis"},
  {sciseteventhandler,"seteventhandler"},
  {int_gtkhelp,"help_gtk"},
  /* NG beg */
  {gset,"set"},
  {gget,"get"},
  {sci_delete,"delete"},
  {addcb,"addcb"},
  {copy,"copy"},
  {move,"move"},
  {glue,"glue"},
  {unglue,"unglue"}, 
  {drawnow,"drawnow"},
  {drawlater,"drawlater"},  
  {draw,"draw"}, /* NG end */
  {scixs2gif,"xs2gif"},
  {scixs2ppm,"xs2ppm"},
  {scixs2ps,"xs2ps"},
  {ClearScreenConsole,"clc"},
  {HomeFunction,"tohome"},
  {scisetposfig,"set_posfig_dim"},
  {ShowWindowFunction,"show_window"},
  {XSaveNative,"xsnative"},
  {scizoomrect,"zoom_rect"},
  {sciunzoom,"unzoom"},
  {intxs2bmp,"xs2bmp"},
  {intxs2emf,"xs2emf"},
  {intUImenu,"UImenu"},
  {intshowalluimenushandles,"showalluimenushandles"},
  {sciStringBox,"StringBox"},
  {scinewaxes,"newaxes"}
};

/*-----------------------------------------------------------------------------------*/ 
/* interface for the previous function Table */ 
int C2F(matdes)()
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
