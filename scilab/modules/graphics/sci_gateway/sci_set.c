/*------------------------------------------------------------------------*/
/* file: sci_set.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : interface for sci_set routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_set.h"
/*-----------------------------------------------------------------------------------*/
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

#include "Xcall1.h"
#include "Format.h"
#include "pixel_mode.h"
#include "getHandleProperty/setHandleProperty.h"
#include "ColorMapManagement.h"
#include "getHandleProperty/SetPropertyStatus.h"


#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/**@name int sciset(sciPointObj *pobj,char *marker, long *x, long *y, long *w, long *h)
* Sets the value to the object
*/

int sciSet(sciPointObj *pobj, char *marker, int *value, int valueType, int *numrow, int *numcol)
{

  /***************** graphics mode *******************************/ 
  if (strcmp(marker,"color_map") == 0)
  {
    return set_color_map_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"old_style") == 0)
  {
    return set_old_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"figure_style") == 0)
  {
    return set_figure_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"pixel_drawing_mode") == 0)
  {
    return set_pixel_drawing_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  
  else if ( strcmp(marker,"default_values") == 0 )
  {
    return set_default_values_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"visible") == 0) 
  {
    return set_visible_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if ( strcmp(marker,"auto_resize") == 0 )
  {
    return set_auto_resize_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /*************************** Handles Properties ********/
  else if ((strcmp(marker,"current_obj") == 0) || (strcmp(marker,"current_entity") == 0))
  {
    return set_current_entity_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"current_axes") == 0) 
  {
    return set_current_axes_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"current_figure") == 0) 
  {
    return set_current_figure_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /************************  figure Properties *****************************/ 
  else if (strcmp(marker,"figure_position") == 0)
  {
    return set_figure_position_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"axes_size") == 0)
  {
    return set_axes_size_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"figure_size") == 0)
  {
    return set_figure_size_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"figure_name") == 0)
  {
    return set_figure_name_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"figure_id") == 0)
  {
    return set_figure_id_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"rotation_style") == 0)
  {
    return set_rotation_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"immediate_drawing") == 0)
  {
    return set_immediate_drawing_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"pixmap") == 0)
  {
    return set_pixmap_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /********************** context graphique ******************************/
  else if (strcmp(marker,"background") == 0)
  {
    return set_background_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"interp_color_vector") == 0)
  {
    return set_interp_color_vector_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"interp_color_mode") == 0)
  {
    return set_interp_color_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"foreground") == 0)
  {
    return set_foreground_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"fill_mode") == 0)
  {
    return set_fill_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"thickness") == 0) 
  {
    return set_thickness_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"arrow_size_factor") == 0)
  {
    return set_arrow_size_factor_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"line_style") == 0)
  {
    return set_line_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"line_mode") == 0)
  {
    return set_line_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"surface_mode") == 0)
  {
    return set_surface_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_style") == 0)
  {
    return set_mark_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_mode") == 0)
  {
    return set_mark_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_size_unit") == 0)
  {
    return set_mark_size_unit_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_size") == 0)
  {
    return set_mark_size_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_foreground") == 0)
  {
    return set_mark_foreground_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"mark_background") == 0)
  {
    return set_mark_background_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"bar_width") == 0)
  {
    return set_bar_width_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"bar_layout") == 0)
  {
    return set_bar_layout_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"x_shift") == 0)
  {
    return set_x_shift_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"y_shift") == 0)
  {
    return set_y_shift_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"z_shift") == 0)
  {
    return set_z_shift_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"polyline_style") == 0)
  {
    return set_polyline_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /************* font properties *********/
  else if (strcmp(marker,"font_size") == 0)
  {
    return set_font_size_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_angle") == 0)
  {
    return set_font_angle_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_foreground") == 0)
  {
    return set_font_foreground_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_color") == 0)
  {
    return set_font_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }	
  else if (strcmp(marker,"font_style") == 0)
  {
    return set_font_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"font_name") == 0)
  {
    return set_font_name_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"text_box_mode") == 0)
  {
    return set_text_box_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"auto_dimensionning") == 0 )
  {
    return set_auto_dimensionning_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"alignment") == 0 )
  {
    return set_alignment_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"text_box") == 0)
  {
    return set_text_box_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"text") == 0)
  {
    return set_text_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /******************/
  else if (strcmp(marker,"auto_clear") == 0)
  {
    return set_auto_clear_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"auto_scale") == 0)
  {
    return set_auto_scale_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"zoom_box") == 0 )
  {
    return set_zoom_box_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if ( strcmp(marker,"zoom_state") == 0 )
  {
    return set_zoom_state_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /***********************************************/
  else if (strcmp(marker,"clip_box") == 0)
  {
    return set_clip_box_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"clip_state") == 0)
  {
    return set_clip_state_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }		
  else if (strcmp(marker,"data") == 0)
  {
    return set_data_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }

  /**************** callback *********************/
  else if (strcmp(marker,"callbackmevent") == 0)
  {
    return set_callbackmevent_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"callback") == 0)
  {
    return set_callback_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /******** AXES *******************************/
  else if (strcmp(marker,"x_label") == 0)
  {
    return set_x_label_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"y_label") == 0)
  {
    return set_y_label_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"z_label") == 0)
  {
    return set_z_label_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"title") == 0)
  {
    return set_title_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tics_direction") == 0)
  {
    return set_tics_direction_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"x_location") == 0)
  {
    return set_x_location_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"y_location") == 0)
  {
    return set_y_location_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tight_limits") == 0) 
  {
    return set_tight_limits_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"closed") == 0)
  {
    return set_closed_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"auto_position") == 0)
  {
    return set_auto_position_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  
  else if (strcmp(marker,"auto_rotation") == 0)
  {
    return set_auto_rotation_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"position") == 0)
  {
    return set_position_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /* F.Leray adding auto_ticks flags */
  else if (strcmp(marker,"auto_ticks") == 0) 
  {
    return set_auto_ticks_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"axes_reverse") == 0) 
  {
    return set_axes_reverse_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"view") == 0) 
  {
    return set_view_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if (strcmp(marker,"axes_bounds") == 0)
  {
    return set_axes_bounds_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"data_bounds") == 0)
  {
    return set_data_bounds_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"margins") == 0)
  {
    return set_margins_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tics_color") == 0)
  {
    return set_tics_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tics_style") == 0)
  {
    return set_tics_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /*Dj.A 17/12/2003*/
  /* modified jb Silvy 01/2006 */
  else if ((strcmp(marker,"sub_tics") == 0) || (strcmp(marker,"sub_ticks") == 0))
  {
    return set_sub_tics_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"format_n") == 0)
  {
    return set_format_n_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"tics_segment") == 0) 
  {
    return set_tics_segment_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"labels_font_size") == 0)
  {
    return set_labels_font_size_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"labels_font_color") == 0)
  {
    return set_labels_font_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }	
  else if (strcmp(marker,"labels_font_style") == 0)
  { /* Adding F.Leray 09.04.04 : For the moment sciAxes have no style property*/
    return set_labels_font_style_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }	
  else if (strcmp(marker,"tics_labels") == 0) 
  {
    return set_tics_labels_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"xtics_coord") == 0) 
  {
    return set_xtics_coord_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"ytics_coord") == 0) 
  {
    return set_ytics_coord_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  else if  (strcmp(marker,"box") == 0) 
  {
    return set_box_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"grid") == 0)
  {
    return set_grid_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if  (strcmp(marker,"axes_visible") == 0) 
  {
    return set_axes_visible_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"hiddencolor") == 0)
  {
    return set_hidden_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /**DJ.Abdemouche 2003**/
  else if  (strcmp(marker,"isoview") == 0) 
  {  /* DJ.A 2003 */
    return set_isoview_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  /**/
  else if  (strcmp(marker,"cube_scaling") == 0)  /* F.Leray 22.04.04 */
  {
    return set_cube_scaling_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  /**/
  else if (strcmp(marker,"log_flags") == 0)
  {
    return set_log_flags_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"arrow_size") == 0)
  {
    return set_arrow_size_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }  
  else if ( strcmp(marker,"segs_color") == 0 )
  {
    return set_segs_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ((strcmp(marker,"colored") == 0) && (sciGetEntityType (pobj) == SCI_SEGS))
  {
    return set_colored_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /**************** Matplot Grayplot *********************/
  else if (strcmp(marker,"data_mapping") == 0)
  {
    return set_data_mapping_property( pobj, *value, valueType, *numrow, *numcol ) ;
  } 
  /**************** Surface *******************************/
  else if (strcmp(marker,"rotation_angles") == 0)
  {
    return set_rotation_angles_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  /*DJ.A merge*/
  else if (strcmp(marker,"color_mode") == 0)
  {
    return set_color_mode_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"color_flag") == 0)
  {
    return set_color_flag_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"cdata_mapping") == 0)
  {
    return set_cdata_mapping_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"surface_color") == 0)
  {
    return set_surface_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"triangles") == 0)
  {
    return set_triangles_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"z_bounds") == 0)
  {
    return set_z_bounds_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"handle_visible") == 0)
  {
    return set_handle_visible_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"callback_type") == 0)
  {
    return set_callback_type_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if (strcmp(marker,"menu_enable") == 0)
  {
    return set_menu_enable_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if( strcmp(marker,"hidden_axis_color") == 0 )
  {
    return set_hidden_axis_color_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"user_data") == 0 )
  {
    return set_user_data_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"x_ticks") == 0 )
  {
    return set_x_ticks_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"y_ticks") == 0 )
  {
    return set_x_ticks_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else if ( strcmp(marker,"z_ticks") == 0 )
  {
    return set_x_ticks_property( pobj, *value, valueType, *numrow, *numcol ) ;
  }
  else 
  {
    sciprint( "Unknown  property %s.\n", marker ) ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------
 * sciset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/
int sci_set(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,numrow3,numcol3,l3,num,cur,na,verb=0;
  unsigned long hdl; 
  int lw,t2;
  BOOL vis_save = FALSE ;
  sciPointObj *pobj;

  int valueType = 0 ; /* type of the rhs */

  int numrow[4],i;
  int numcol[4], lxyzcol[4];
  int ptrindex[2];
  int flagc = -1;
  int setStatus = 0 ; /* after the call to sciSet get the status : 0 <=> OK,          */ 
                      /*                                          -1 <=> Error,       */
                      /*                                           1 <=> nothing done */

  /* F.Leray Init. to 0*/
  for(i=0;i<4;i++){
    numrow[i] = 0;
    numcol[i] = 0;
    lxyzcol[i] = 0;
  }
  ptrindex[0] = 0;
  ptrindex[1] = 0;


  CheckRhs(2,3);
  CheckLhs(0,1);
  /*  set or create a graphic window */
  switch(VarType(1)) 
    {
    case 1: /* tclsci handle */
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"set",3);return 0;
      return 0;
      break;
    case 9: /* first is a scalar argument so it's a gset(hdl,"command",[param]) */
      /* F.Leray; INFO: case 9 is considered for a matrix of graphic handles*/
      CheckRhs(3,3);
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */
      if ( *hstk(l1) != sciGetHandle(getFigureModel()) && *hstk(l1) != sciGetHandle(getAxesModel())
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_z_label))
	{
	  if (m1!=1||n1!=1) { 
	    lw = 1 + Top - Rhs;
	    C2F(overload)(&lw,"set",3);return 0;
	  }
	  if (version_flag() ==0)
	    hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */ 
	  else
	    hdl = (unsigned long)0;
	  if (hdl == (unsigned long)0 )
	    pobj = (sciPointObj *) NULL;
	  else
	    pobj = sciGetPointerFromHandle(hdl);
	} 
      else 
	{
	  hdl = (unsigned long)*hstk(l1);
	  pobj = sciGetPointerFromHandle(hdl);
	}

      GetRhsVar(2,"c",&m2,&n2,&l2); /* Gets the command name */  
      if ( *hstk(l1) != sciGetHandle(getFigureModel()) && *hstk(l1) != sciGetHandle(getAxesModel())
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_title)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_x_label)
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_y_label) 
	   &&  *hstk(l1) != sciGetHandle(pSUBWIN_FEATURE(getAxesModel())->mon_z_label))
      {
	if ((strcmp(cstk(l2),"old_style") !=0) 
	    &&(strcmp(cstk(l2),"default_figure") !=0) 
            && (strcmp(cstk(l2),"default_axes") !=0) ) { SciWin() ; }
      }

      valueType = VarType(3) ;
      
      if ( valueType == 1 )
      {
        GetRhsVar(3,"d",&numrow3,&numcol3,&l3) ;
      }
      else if ( valueType == 9 )
      {
        GetRhsVar(3,"h",&numrow3,&numcol3,&l3);
      }
      else if ( valueType == 10 ) 
      { 
        if (    strcmp( cstk(l2), "tics_labels"  ) != 0
             && strcmp( cstk(l2), "auto_ticks"   ) != 0
             && strcmp( cstk(l2), "axes_visible" ) != 0
             && strcmp( cstk(l2), "axes_reverse" ) != 0
             && strcmp( cstk(l2), "text"         ) != 0 )
        {
          GetRhsVar(3,"c",&numrow3,&numcol3,&l3);
        } 
	else
        {
	  GetRhsVar(3,"S",&numrow3,&numcol3,&l3);
        }
      }
      break;
    case 10:/* first is a string argument so it's a gset("command",[param]) */ 
      CheckRhs(2,2);
      GetRhsVar(1,"c",&m2,&n2,&l2);
      if (strcmp(cstk(l2),"default_figure") !=0 && strcmp(cstk(l2),"default_axes") !=0 )
	{
	  if ((strcmp(cstk(l2),"old_style") ==0) || 
	      (strcmp(cstk(l2),"current_figure") ==0)) {
	    hdl = (unsigned long)0;
	    pobj = (sciPointObj *) NULL;
	  }
	  else
	    {
	      SciWin();
	      if (version_flag() == 0 )
		if ((strcmp(cstk(l2),"zoom_") !=0) && 
		    (strcmp(cstk(l2),"auto_") !=0) && 
		    (strcmp(cstk(l2),"clip_box") !=0) )   
		  hdl = (unsigned long ) sciGetHandle(sciGetCurrentObj ()) ; 
		else  
		  hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin (sciGetCurrentFigure ()));
	      else
		hdl = (unsigned long)0;
	      if (hdl == (unsigned long)0 )
		pobj = (sciPointObj *) NULL;
	      else
		pobj = sciGetPointerFromHandle(hdl);
	    }
	}
      else
	{
	  hdl = (unsigned long)0;
	  pobj = (sciPointObj *) NULL;
	}
      valueType = VarType(2) ;
      t2=sciType(cstk(l2),pobj);
      if (t2<0) {
	Scierror(999,"%s: unknown property name '%s' \r\n",fname,cstk(l2));
	return 0;} 
      if ( valueType != t2 )
	if(strcmp(cstk(l2),"current_figure") != 0 && VarType(2) !=1 ){  
	  /* F.Leray : special unique case here set("current_figure", HANDLE);*/
	  /* HANDLE type is 9 */
	  Scierror(999,"%s: uncompatible values for property type  '%s' \r\n",fname,cstk(l2));
	  return 0;
	}

      if (valueType == 1 )
      {
        GetRhsVar(2,"d",&numrow3,&numcol3,&l3);
      }
      else if ( valueType == 9 )
      {
        GetRhsVar(2,"h",&numrow3,&numcol3,&l3);
      }
      else if ( valueType == 10 )
      {
	if (   strcmp( cstk(l2), "tics_labels"  ) != 0
	    && strcmp( cstk(l2), "auto_ticks"   ) != 0
	    && strcmp( cstk(l2), "axes_visible" ) != 0
	    && strcmp( cstk(l2), "axes_reverse" ) != 0
               && strcmp( cstk(l2), "text"      ) != 0 )
        {
          GetRhsVar(2,"c",&numrow3,&numcol3,&l3);
        } 
	else
        {
	  GetRhsVar(2,"S",&numrow3,&numcol3,&l3);
        }
      }
      break;
    default:
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"set",3);return 0;
      break;
    }

  if ( (hdl != (unsigned long)0) ) { /* F.Leray 16.03.04*/
    pobj = sciGetPointerFromHandle(hdl);
    if ( pobj == NULL )
    {
      Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
      return 0;
    }
    vis_save = sciGetVisibility(pobj) ; /*used not to redraw the figure is object remains invisible SS 20.04.04*/
    if ( (setStatus = sciSet(pobj, cstk(l2), &l3, valueType, &numrow3, &numcol3)) < 0 )
    {
      return 0 ;
    }

    if ( !( vis_save == 0 && sciGetVisibility(pobj)==0 ) && setStatus == 0 ) 
    {
      /* do not redraw figure if object remains invisible */
      if ((strcmp(cstk(l2),"figure_style") !=0) &&
	  (strcmp(cstk(l2),"old_style") !=0 ) && 
	  (strcmp(cstk(l2),"current_axes") !=0) &&
	  (strcmp(cstk(l2),"default_figure") !=0) && 
	  (strcmp(cstk(l2),"default_axes") !=0) &&
	  (pobj != getFigureModel()) && (pobj != getAxesModel())
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_title
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_x_label
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_y_label
	  && pobj != pSUBWIN_FEATURE(getAxesModel())->mon_z_label )
      { 
        /* Addings F.Leray 10.06.04 */
	num= sciGetNumFigure (pobj);    
	C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  

	sciDrawObj(sciGetParentFigure(pobj)); /* F.Leray we redraw here*/
	/* 	EraseAndOrRedraw(pobj); */  /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */


	C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  }
  else if ( ( setStatus = sciSet( NULL, cstk(l2), &l3, valueType, &numrow3, &numcol3) ) < 0 )
  {
    return 0;
  }

  LhsVar(1)=0;
  return 0;
}
