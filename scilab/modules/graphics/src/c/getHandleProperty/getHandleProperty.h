/*------------------------------------------------------------------------*/
/* file: getHandleProperty.h                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : contains definitions of functions needed by sci_get             */
/*        Each function has the same signature int (sciPointObj *) and    */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_PROPERTY_H_
#define _GET_HANDLE_PROPERTY_H_

#include "ObjectStructure.h"

int get_figures_id_property( sciPointObj * pobj ) ;
int get_visible_property(    sciPointObj * pobj ) ;
int get_pixel_drawing_mode_property( sciPointObj * pobj ) ;
int get_old_style_property( sciPointObj * pobj ) ;
int get_figure_style_property( sciPointObj * pobj ) ;
int get_auto_resize_property( sciPointObj * pobj ) ;
int get_figure_position_property( sciPointObj * pobj ) ;
int get_axes_size_property( sciPointObj * pobj ) ;
int get_figure_size_property( sciPointObj * pobj ) ;
int get_figure_name_property( sciPointObj * pobj ) ;
int get_figure_id_property( sciPointObj * pobj ) ;
int get_rotation_style_property( sciPointObj * pobj ) ;
int get_immediate_drawing_property( sciPointObj * pobj ) ;
int get_pixmap_property( sciPointObj * pobj ) ;
int get_type_property( sciPointObj * pobj ) ;
int get_parent_property( sciPointObj * pobj ) ;
int get_current_axes_property( sciPointObj * pobj ) ;
int get_current_figure_property( sciPointObj * pobj ) ;
int get_current_entity_property( sciPointObj * pobj ) ;
int get_children_property( sciPointObj * pobj ) ;
int get_default_figure_property( sciPointObj * pobj ) ;
int get_default_axes_property( sciPointObj * pobj ) ;
int get_color_map_property( sciPointObj * pobj ) ;
int get_interp_color_vector_property( sciPointObj * pobj ) ;
int get_interp_color_mode_property( sciPointObj * pobj ) ;
int get_background_property( sciPointObj * pobj ) ;
int get_foreground_property( sciPointObj * pobj ) ;
int get_fill_mode_property( sciPointObj * pobj ) ;
int get_thickness_property( sciPointObj * pobj ) ;
int get_arrow_size_factor_property( sciPointObj * pobj ) ;
int get_line_style_property( sciPointObj * pobj ) ;
int get_line_mode_property( sciPointObj * pobj ) ;
int get_surface_mode_property( sciPointObj * pobj ) ;
int get_mark_style_property( sciPointObj * pobj ) ;
int get_mark_mode_property( sciPointObj * pobj ) ;
int get_mark_size_unit_property( sciPointObj * pobj ) ;
int get_mark_size_property( sciPointObj * pobj ) ;
int get_mark_foreground_property( sciPointObj * pobj ) ;
int get_mark_background_property( sciPointObj * pobj ) ;
int get_bar_layout_property( sciPointObj * pobj ) ;
int get_bar_width_property( sciPointObj * pobj ) ;
int get_x_shift_property( sciPointObj * pobj ) ;
int get_y_shift_property( sciPointObj * pobj ) ;
int get_z_shift_property( sciPointObj * pobj ) ;
int get_polyline_style_property( sciPointObj * pobj ) ;
int get_font_size_property( sciPointObj * pobj ) ;
int get_font_angle_property( sciPointObj * pobj ) ;
int get_font_foreground_property( sciPointObj * pobj ) ;
int get_font_color_property( sciPointObj * pobj ) ;

#endif /* _GET_HANDLE_PROPERTY_H_ */
