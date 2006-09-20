/*------------------------------------------------------------------------*/
/* file: sci_get.c                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : interface for sci_get routine                                   */
/*------------------------------------------------------------------------*/
#include "sci_get.h"
/*-----------------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>

#include "bcg.h"
#include "Graphics.h" 
#include "stack-c.h"
#include "CloneObjects.h"
#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"

/* Constructors should NOT be called at this level (not inside matdes.c) */
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "StringMatrix.h"
#include "Xcall1.h"
#include "Format.h"
#include "pixel_mode.h"
#include "../src/c/getHandleProperty/returnProperty.h"

#include "../src/c/getHandleProperty/getHandleProperty.h"



#include "MALLOC.h" /* MALLOC */

/*-----------------------------------------------------------------------------------*/
extern int versionflag;
/*-----------------------------------------------------------------------------------*/
static char error_message[128];
/*-----------------------------------------------------------------------------------*/
int sciGet(sciPointObj *pobj,char *marker);
int getgrayplotdata(sciPointObj *pobj);
int getchampdata(sciPointObj *pobj);
int get3ddata(sciPointObj *pobj);
int getticks(char * xyztick, sciPointObj* psubwin);
int ChooseGoodFormat(char * c_format,char logflag, double *_grads,int n_grads);
int BuildTListForTicks(double * locations, char ** labels, int nbtics);
double * ReBuildTicksLog2Lin(char logflag, int nbtics, double *grads);
BOOL GetHandleVisibilityOnUimenu(sciPointObj * pobj);
/*-----------------------------------------------------------------------------------*/
int sci_get(char *fname,unsigned long fname_len)
{
  integer m1,n1,numrow2,numcol2,l2 ;
  int l1;
  unsigned long hdl;

  int lw;
  sciPointObj *pobj;

  CheckRhs(1,2);
  CheckLhs(0,1);
  
  /*  set or create a graphic window */

  /* le premier argument peut etre soit un Id, ou un marker(auquel cas, get renvoi la valeur de 
     l'objet courant ) */
  switch(VarType(1))
    {
    case 1: /* tclsci handle */
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"get",3);return 0;
      return 0;
      break;
    case 9: /* scalar argument (hdl + string) */
      CheckRhs(2,2);
      GetRhsVar(1,"h",&m1,&n1,&l1);
      if (m1!=1||n1!=1) { 
	lw = 1 + Top - Rhs;
	C2F(overload)(&lw,"get",3);return 0;
      }
      GetRhsVar(2,"c",&numrow2,&numcol2,&l2);
      if ( *hstk(l1) != sciGetHandle(getFigureModel()) && *hstk(l1) != sciGetHandle(getAxesModel())
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_z_label))
	{
	  if ((strcmp(cstk(l2),"old_style") !=0)
	      &&(strcmp(cstk(l2),"default_figure") !=0) 
	      && (strcmp(cstk(l2),"default_axes") !=0) ) SciWin();
	  if (version_flag() ==0)
	    hdl = (unsigned long)*hstk(l1); /* on recupere le pointeur d'objet par le handle */
	  else
	    hdl = (unsigned long)0;
	}/* DJ.A 08/01/04 */
      else 
	hdl = (unsigned long)*hstk(l1);
      break;
    case 10:/* string argument (string) */
      CheckRhs(1,1);
      GetRhsVar(1,"c",&numrow2,&numcol2,&l2);
      if (strcmp(cstk(l2),"default_figure") !=0 && strcmp(cstk(l2),"default_axes") !=0 )
	{
	  if (strcmp(cstk(l2),"old_style") == 0 || strcmp(cstk(l2),"current_figure") ==0) {
	    hdl = (unsigned long)0;
	  }
	  else
	    {
	      SciWin();
	      if (version_flag() == 0)
		{
		  /* Test debug F.Leray 13.04.04 */
		  sciPointObj * ppobj;
		  if ((strcmp(cstk(l2),"children") != 0) &&  
		      (strcmp(cstk(l2),"zoom_") !=0) && 
		      (strcmp(cstk(l2),"clip_box") !=0) && 
		      (strcmp(cstk(l2),"auto_") !=0)) 
		    {
		      ppobj = sciGetCurrentObj();
		      hdl = (unsigned long ) sciGetHandle(sciGetCurrentObj ());}
		  else  
		    hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin (sciGetCurrentFigure ()));/* on recupere le pointeur d'objet par le handle */
		}
	      else
		hdl = (unsigned long)0;
	    }/* DJ.A 08/01/04 */
	}
      else
	hdl = (unsigned long)0;
    
      break;
    default:
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"get",3);return 0;
      /*Scierror(999,"%s: Incorrect argument\r\n",fname);*/
      return 0;
      break;
    }
  /* cstk(l2) est la commande, l3 l'indice sur les parametres de la commande */
  CheckLhs(0,1);
  if (hdl == (unsigned long)0 ) {
    if (sciGet((sciPointObj *) NULL, cstk(l2))!=0) {
      Scierror(999,"%s: %s\r\n",fname,error_message);
      return 0;
    }
  }
  else
  {
    if ( (pobj = sciGetPointerFromHandle(hdl) ) )
    {

      if (sciGet(pobj, cstk(l2))!=0)
      {
        Scierror(999,"%s: %s\r\n",fname,error_message);
        return 0;
      }
    }
    else
    {
      Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
    }
  }
  LhsVar(1)=Rhs+1;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* @name sciGet(sciPointObj *pobj,) */
/* Sets the value to the object */

extern int get_cf_type(void);

int sciGet(sciPointObj *pobj,char *marker)
{
  integer itmp = 0 ;
  sciPointObj *psubwin;
  int Etype,iflag=0;

  if (pobj != (sciPointObj *)NULL && pobj  != getFigureModel()  && pobj  != getAxesModel()
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_title
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_x_label
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_y_label
      && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_z_label ){ /* Addings F.Leray 10.06.04 */
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    Etype=sciGetEntityType (pobj);}

  if ((pobj == (sciPointObj *)NULL) && 
      (strcmp(marker,"old_style") !=0 ) && 
      (strcmp(marker,"figure_style") != 0) && 
      (strcmp(marker,"current_figure") != 0) &&
      (strcmp(marker,"default_figure") !=0) && 
      (strcmp(marker,"default_axes") !=0))
    {
      if (version_flag() == 0)
	{strcpy(error_message,"handle is not valid");return -1;}
      else
	{strcpy(error_message,"function not valid under old graphics style");return -1;}
    }
  else if (strcmp(marker,"figures_id") == 0)
  { 
    return get_figures_id_property( pobj ) ;
  }
  /***************** graphics mode *******************************/ 
  else if (strcmp(marker,"visible") == 0)
  {
    return get_visible_property( pobj ) ;
  }
  else if (strcmp(marker,"pixel_drawing_mode") == 0) 
  {
    return get_pixel_drawing_mode_property( pobj ) ;
  }  
  else if (strcmp(marker,"old_style") == 0)
  {
    return get_old_style_property( pobj ) ;
  }
  else if (strcmp(marker,"figure_style") == 0)
  {
    return get_figure_style_property( pobj ) ;
  }
  else if (strcmp(marker,"auto_resize") == 0)
  {
    return get_auto_resize_property( pobj ) ;
  }
  /************************  figure Properties *****************************/ 
  else if (strcmp(marker,"figure_position") == 0)
  {
    return get_figure_position_property( pobj ) ;
  }  
  else if (strcmp(marker,"axes_size") == 0)
  {
    return get_axes_size_property( pobj ) ;
  } 
  else if (strcmp(marker,"figure_size") == 0)
  {
    return get_figure_size_property( pobj ) ;
  }
  else if (strcmp(marker,"figure_name") == 0)
  {
    return get_figure_name_property( pobj ) ;
  }
  else if (strcmp(marker,"figure_id") == 0)
  {
    return get_figure_id_property( pobj ) ;
  }
  else if (strcmp(marker,"rotation_style") == 0) 
  {
    return get_rotation_style_property( pobj ) ;
  }
  else if (strcmp(marker,"immediate_drawing") == 0) 
  {
    return get_immediate_drawing_property( pobj ) ;
  }
  /*Ajout A.Djalel*/
  else if (strcmp(marker,"pixmap") == 0)
  {
    return get_pixmap_property( pobj ) ;    
  }
  /********** Handles Properties *********************************************/       
  else if (strcmp(marker,"type") == 0)
  {
    return get_type_property( pobj ) ;
  }
  else if (strcmp(marker,"parent") == 0)
  {
    return get_parent_property( pobj ) ;    
  }
  else if (strcmp(marker,"current_axes") == 0)
  {
    return get_current_axes_property( pobj ) ;
  }
  else if (strcmp(marker,"current_figure") == 0)
  {
    return get_current_figure_property( pobj ) ;
  }
  else if( (strcmp(marker,"current_obj") == 0) || (strcmp(marker,"current_entity") == 0) || (strcmp(marker,"hdl") == 0) )
  {
    return get_current_entity_property( pobj ) ;
  }
  else if (strcmp(marker,"children") == 0)
  {
    return get_children_property( pobj ) ;
  }
  /* DJ.A 08/01/04 */
  else if (strcmp(marker,"default_figure") == 0)
  {
    return get_default_figure_property( pobj ) ;
  }
  else if (strcmp(marker,"default_axes") == 0)
  {
    return get_default_axes_property( pobj ) ;  
  }
  /******************************** context graphique  ******************************************/
  else if ( strcmp(marker,"color_map") == 0 )
  {
    return get_color_map_property( pobj ) ;
  }
  else if (strcmp(marker,"interp_color_vector") == 0)
  {
    return get_interp_color_vector_property( pobj ) ;
  }
  else if (strcmp(marker,"interp_color_mode") == 0)
  {
    return get_interp_color_mode_property( pobj ) ;
  }
  else if (strcmp(marker,"background") == 0) /**DJ.Abdemouche 2003**/
  {
    return get_background_property( pobj ) ;
  }
  else if (strcmp(marker,"foreground") == 0) 
  {
    return get_foreground_property( pobj ) ;
  }
  else if (strcmp(marker,"fill_mode") == 0) 
  {
    return get_fill_mode_property( pobj ) ;    
  }
  else if (strcmp(marker,"thickness") == 0) 
  {
    return get_thickness_property( pobj ) ;
  }
  else if (strcmp(marker,"arrow_size_factor") == 0) 
  {
    return get_arrow_size_factor_property( pobj ) ;
  }
  else if (strcmp(marker,"line_style") == 0)
  {
    return get_line_style_property( pobj ) ;
  }
  else if (strcmp(marker,"line_mode") == 0)
  {
    return get_line_mode_property( pobj ) ;
  }
  else if (strcmp(marker,"surface_mode") == 0)
  {
    return get_surface_mode_property( pobj ) ;    
  }
  else if (strcmp(marker,"mark_style") == 0)
  {
    return get_mark_style_property( pobj ) ;
  }
  else if (strcmp(marker,"mark_mode") == 0)
  {
    return get_mark_mode_property( pobj ) ;
  }
  else if (strcmp(marker,"mark_size_unit") == 0)
  {
    return get_mark_size_unit_property( pobj ) ;
  }
  else if (strcmp(marker,"mark_size") == 0)
  {
    return get_mark_size_property( pobj ) ;
  }
  else if (strcmp(marker,"mark_foreground") == 0)
  {
    return get_mark_foreground_property( pobj ) ;
  }
  else if (strcmp(marker,"mark_background") == 0)
  {
    return get_mark_background_property( pobj ) ;
  }
  else if (strcmp(marker,"bar_layout") == 0)
  {
    return get_bar_layout_property( pobj ) ;    
  }
  else if (strcmp(marker,"bar_width") == 0)
  {
    return get_bar_width_property( pobj ) ;  
  }
  else if (strcmp(marker,"x_shift") == 0)
  {
    return get_x_shift_property( pobj ) ;    
  } 
  else if (strcmp(marker,"y_shift") == 0)
  {
    return get_y_shift_property( pobj ) ;    
  }  
  else if (strcmp(marker,"z_shift") == 0)
  {
    return get_z_shift_property( pobj ) ;    
  }   
  else if (strcmp(marker,"polyline_style") == 0)
  {
    return get_polyline_style_property( pobj ) ;
  }
  /****************************************************/
  else if (strcmp(marker,"font_size") == 0)
  {
    return get_font_size_property( pobj ) ;
  }
  else if (strcmp(marker,"font_angle") == 0)
  {
    return get_font_angle_property( pobj ) ;
  }
  else if (strcmp(marker,"font_foreground") == 0)
  {
    return get_font_foreground_property( pobj ) ;
  }
  else if (strcmp(marker,"font_color") == 0)
  {
    /* F.Leray 09.04.04 : Added for FIGURE and SUBWIN objects */
    return get_font_color_property( pobj ) ;
  }
  else if (strcmp(marker,"font_style") == 0)
  {
    return get_font_style_property( pobj ) ;
  }
  else if (strcmp(marker,"font_name") == 0)
  {
    return get_font_name_property( pobj ) ;
  }
  else if (strcmp(marker,"text_box_mode") == 0)
  {
    return get_text_box_mode_property( pobj ) ;
  }
  else if ( strcmp(marker,"auto_dimensionning") == 0 )
  {
    return get_auto_dimensionning_property( pobj ) ;
  }
  else if ( strcmp(marker,"alignment") == 0 )
  {
    return get_alignment_property( pobj ) ;
  }
  else if (strcmp(marker,"text_box") == 0)
  {
    return get_text_box_property( pobj ) ;
  }
  else if (strcmp(marker,"text") == 0)
  {
    return get_text_property( pobj ) ;
  }
  else if (strcmp(marker,"auto_clear") == 0)
  {
    return get_auto_clear_property( pobj ) ;
  }
  else if (strcmp(marker,"auto_scale") == 0)
  {
    return get_auto_scale_property( pobj ) ;
  }
  else if ( strcmp(marker,"zoom_box") == 0 )
  {
    return get_zoom_box_property( pobj ) ;
  }
  else if ( strcmp(marker,"zoom_state") == 0 )
  {
    return get_zoom_state_property( pobj ) ;
  }
  else if (strcmp(marker,"clip_box") == 0) 
  {
    return get_clip_box_property( pobj ) ;
  }
  else if (strcmp(marker,"clip_state") == 0) 
  {
    return get_clip_state_property( pobj ) ;
  }
  else if (strcmp(marker,"data") == 0)
  {
    return get_data_property( pobj ) ;
  }
  /**************** callback *********************/
  else if (strcmp(marker,"callbackmevent") == 0)
  {
    return get_callbackmevent_property( pobj ) ;
  }
  else if (strcmp(marker,"callback") == 0)
  {
    return get_callback_property( pobj ) ;
  }
  /**************************** AXES *************/
  else if (strcmp(marker,"x_label") == 0) /* we send back a handle on the x_label object */
  {
    return get_x_label_property( pobj ) ;
  }
  else if (strcmp(marker,"y_label") == 0)
  {
    return get_y_label_property( pobj ) ;
  }
  else if (strcmp(marker,"z_label") == 0)
  {
    return get_z_label_property( pobj ) ;
  }
  else if (strcmp(marker,"title") == 0)
  {
    return get_title_property( pobj ) ;
  }
  else if (strcmp(marker,"log_flags") == 0)
  {
    return get_log_flags_property( pobj ) ;
  }
  else if (strcmp(marker,"tics_direction") == 0)
  {
    return get_tics_direction_property( pobj ) ;
  }
  else if (strcmp(marker,"x_location") == 0) 
  {
    return get_x_location_property( pobj ) ;
  }
  else if (strcmp(marker,"y_location") == 0)
  {
    return get_y_location_property( pobj ) ;
  }
  else if (strcmp(marker,"tight_limits") == 0)
  {
    return get_tight_limits_property( pobj ) ;
  }
  else if (strcmp(marker,"closed") == 0) 
  {
    return get_closed_property( pobj ) ;
  }
  else if (strcmp(marker,"auto_position") == 0) 
  {
    return get_auto_position_property( pobj ) ;
  }
  else if (strcmp(marker,"auto_rotation") == 0) 
  {
    return get_auto_rotation_property( pobj ) ;
  }
  else if (strcmp(marker,"position") == 0) 
  {
    return get_position_property( pobj ) ;
  }
  else if ( strcmp(marker,"auto_ticks") == 0 )
  {
    return get_auto_ticks_property( pobj ) ;
  }
  else if ( strcmp(marker,"axes_reverse") == 0 )
  {
    return get_axes_reverse_property( pobj ) ;
  }
  else if (strcmp(marker,"view") == 0)
  {
    return get_view_property( pobj ) ;
  }
  else if (strcmp(marker,"axes_bounds") == 0)
  {
    return get_axes_bounds_property( pobj ) ;
  }
  else if (strcmp(marker,"data_bounds") == 0)
  {
    return get_data_bounds_property( pobj ) ;
  } 
  else if (strcmp(marker,"margins") == 0)
  {
    return get_margins_property( pobj ) ;
  } 
  else if (strcmp(marker,"tics_color") == 0) 
  {
    return get_tics_color_property( pobj ) ;
  }
  else if (strcmp(marker,"tics_style") == 0)
  {
    return get_tics_style_property( pobj ) ;
  }
  else if ((strcmp(marker,"sub_tics") == 0) || (strcmp(marker,"sub_ticks") == 0))
  {
    return get_sub_tics_property( pobj ) ;
  }
  else if (strcmp(marker,"tics_segment") == 0) 
  {
    return get_tics_segment_property( pobj ) ;
  }
  else if (strcmp(marker,"labels_font_size") == 0) 
  {
    return get_labels_font_size_property( pobj ) ;
  }
  else if (strcmp(marker,"labels_font_color") == 0 )
  {
    return get_labels_font_color_property( pobj ) ;
  }
  else if (strcmp(marker,"labels_font_style") == 0 )
  {
    /* Adding F.Leray 09.04.04 */
    return get_labels_font_style_property( pobj ) ;
  }
  else if (strcmp(marker,"format_n") == 0)
  {
    return get_labels_format_n_property( pobj ) ;
  }
  else if (strcmp(marker,"xtics_coord") == 0)
  {
    return get_xtics_coord_property( pobj ) ;
  }
  else if (strcmp(marker,"ytics_coord") == 0)
  {
    return get_ytics_coord_property( pobj ) ;
  }
  else if (strcmp(marker,"tics_labels") == 0)
  {
    return get_tics_label_property( pobj ) ;
  }
  else if ((strcmp(marker,"box") == 0))
  {
    return get_box_property( pobj ) ;
  }
  /**DJ.Abdemouche 2003**/
  else if (strcmp(marker,"grid") == 0) 
  {
    return get_grid_property( pobj ) ;
  }
  else if (strcmp(marker,"axes_visible") == 0)
  {
    return get_axes_visible_property( pobj ) ;
  }
  else if (strcmp(marker,"hiddencolor") == 0) 
  {
    return get_hidden_color_property( pobj ) ;    
  }
  /**DJ.Abdemouche 2003**/
  else if (strcmp(marker,"isoview") == 0)
  {
    return get_isoview_property( pobj ) ;
  }
  else if (strcmp(marker,"cube_scaling") == 0)
  {
    return get_cube_scaling_property( pobj ) ;
  }
  /**************** SEGS  *********************/
  else if (strcmp(marker,"arrow_size") == 0)
  {
    return get_arrow_size_property( pobj ) ;
  }
  else if ( strcmp(marker,"colored") == 0 )
  {
    return get_colored_property( pobj ) ;
  }
  /**************** Matplot Grayplot *********************/
  else if (strcmp(marker,"data_mapping") == 0)
  {
    return get_data_mapping_property( pobj ) ;
  } 
  /**************** Surface *******************************/
  else if (strcmp(marker,"rotation_angles") == 0)
  {
    return get_rotation_angles_property( pobj ) ;
  } 
  else if (strcmp(marker,"color_mode") == 0)
  {
    return get_color_mode_property( pobj ) ;
  } 
  else if (strcmp(marker,"color_flag") == 0)
  {
    return get_color_flag_property( pobj ) ;
  } 
  else if (strcmp(marker,"cdata_mapping") == 0)
  {
    return get_cdata_mapping_property( pobj ) ;
  }
  else if (strcmp(marker,"surface_color") == 0)
  {
    return get_surface_color_property( pobj ) ;
  } 
  /*  ===================================== FEC ======================================== */
  else if (strcmp(marker,"triangles") == 0)
  {
    return get_triangles_property( pobj ) ;
  }
  else if ( strcmp(marker,"z_bounds") == 0 )
  {
    return get_z_bounds_property( pobj ) ;
  }
  else if (strcmp(marker,"user_data") == 0)
  {
    return get_user_data_property( pobj ) ;
  }
  else if (strcmp(marker,"handle_visible") == 0)
  {
    return get_handle_visible_property( pobj ) ;
  }
  else if (strcmp(marker,"callback_type") == 0)
  {
    return get_callback_type_property( pobj ) ;
  }
  else if (strcmp(marker,"menu_enable") == 0)
  {
    return get_menu_enable_property( pobj ) ;
  }
  else if ( strcmp(marker,"hidden_axis_color") == 0 )
  {
    return get_hidden_axis_color_property( pobj ) ;
  }
  else if ( strcmp(marker,"x_ticks") == 0 )
  {
    return get_x_ticks_property( pobj ) ;
  }
  else if ( strcmp(marker,"y_ticks") == 0 )
  {
    return get_y_ticks_property( pobj ) ;
  }
  else if ( strcmp(marker,"z_ticks") == 0 )
  {
    return get_z_ticks_property( pobj ) ;
  }
  else 
  {
    sprintf(error_message,"Unknown  property %s",marker) ;
    return -1 ;
  }

  return 0;
}


/*-----------------------------------------------------------------------------------*/
