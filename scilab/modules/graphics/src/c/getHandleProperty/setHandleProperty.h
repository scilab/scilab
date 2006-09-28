/*------------------------------------------------------------------------*/
/* file: setHandleProperty.h                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : contains declarations of functions needed by sci_set            */
/*        Each function has the same signature and :                      */
/*        int (sciPointobj *, int, int, int)                              */
/*        parameters stands for respectively, the object to modify,       */
/*        the pointer of the object on the scilab stack, and the number   */
/*        of rows and columns of the new value                            */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _SET_HANDLE_PROPERTY_H_
#define _SET_HANDLE_PROPERTY_H_

#include "ObjectStructure.h"

int set_color_map_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_old_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_pixel_drawing_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_default_values_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_visible_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_auto_resize_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_current_entity_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_current_axes_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_current_figure_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_axes_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_position_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_name_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_id_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_rotation_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_immediate_drawing_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_pixmap_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_background_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_interp_color_vector_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_interp_color_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_foreground_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_fill_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_thickness_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_arrow_size_factor_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_line_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_line_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_surface_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_mark_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_mark_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_mark_size_unit_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_mark_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_mark_foreground_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_mark_background_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_bar_width_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_bar_layout_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_x_shift_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_y_shift_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_z_shift_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_polyline_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_font_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_font_angle_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_font_foreground_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_font_color_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_font_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_font_name_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_text_box_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_auto_dimensionning_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_alignment_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_text_box_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_text_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_auto_clear_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_auto_scale_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_zoom_box_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_zoom_state_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_clip_box_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_clip_state_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_data_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;

#endif /* _SET_HANDLE_PROPERTY_H_ */
