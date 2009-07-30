/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vicent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: setHandleProperty.h                                              */
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

#include <stdlib.h>

#include "ObjectStructure.h"

#include "SetUimenuLabel.h"
#include "SetUicontrolString.h"
#include "SetUicontrolBackgroundColor.h"
#include "SetUiobjectForegroundColor.h"
#include "SetUiobjectCallback.h"
#include "SetUicontrolFontWeight.h"
#include "SetUicontrolFontUnits.h"
#include "SetUicontrolFontSize.h"
#include "SetUicontrolFontAngle.h"
#include "SetUiobjectTag.h"
#include "SetUiobjectEnable.h"
#include "SetUicontrolMin.h"
#include "SetUicontrolMax.h"
#include "SetUicontrolUnits.h"
#include "SetUicontrolListboxTop.h"
#include "SetUicontrolValue.h"
#include "SetUicontrolRelief.h"
#include "SetUicontrolHorizontalAlignment.h"
#include "SetUicontrolVerticalAlignment.h"
#include "SetUicontrolFontName.h"
#include "SetUicontrolSliderStep.h"
#include "SetUimenuChecked.h"


int set_color_map_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_old_style_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_figure_style_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_pixel_drawing_mode_property(   sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_default_values_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_visible_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_resize_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_current_entity_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_current_axes_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_current_figure_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_axes_size_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_figure_position_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_figure_name_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_figure_size_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_figure_id_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_rotation_style_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_immediate_drawing_property(    sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_pixmap_property(               sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_background_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_interp_color_vector_property(  sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_interp_color_mode_property(    sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_foreground_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_fill_mode_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_thickness_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_arrow_size_factor_property(    sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_line_style_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_line_mode_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_surface_mode_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_mark_style_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_mark_mode_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_mark_size_unit_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_mark_size_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_mark_foreground_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_mark_background_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_bar_width_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_bar_layout_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_x_shift_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_y_shift_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_z_shift_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_polyline_style_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_font_size_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_font_angle_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_font_foreground_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_font_color_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_font_style_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_text_box_mode_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_dimensionning_property(   sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_alignment_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_text_box_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_text_property(                 sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_clear_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_scale_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_zoom_box_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_zoom_state_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_clip_box_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_clip_state_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_data_property(                 sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_x_label_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_y_label_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_z_label_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_callbackmevent_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_callback_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_title_property(                sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_tics_direction_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_x_location_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_y_location_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_tight_limits_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_closed_property(               sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_position_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_rotation_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_position_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_auto_ticks_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_axes_reverse_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_view_property(                 sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_axes_bounds_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_data_bounds_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_margins_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_tics_color_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_tics_style_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_sub_tics_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_format_n_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_tics_segment_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_labels_font_size_property(     sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_labels_font_color_property(    sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_labels_font_style_property(    sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_tics_labels_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_xtics_coord_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_ytics_coord_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_box_property(                  sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_grid_property(                 sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_axes_visible_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_hidden_color_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_isoview_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_cube_scaling_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_log_flags_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_arrow_size_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_segs_color_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_colored_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_data_mapping_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_rotation_angles_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_color_mode_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_color_flag_property(           sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_cdata_mapping_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_surface_color_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_triangles_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_z_bounds_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_handle_visible_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_callback_type_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_hidden_axis_color_property(    sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_user_data_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_x_ticks_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_y_ticks_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_z_ticks_property(              sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_parent_property(               sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_children_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_viewport_property(             sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_info_message_property(         sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_screen_position_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_dimension_property(            sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_event_handler_enable_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_event_handler_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_arc_drawing_method_property(   sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_fractional_font_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_links_property(                sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_legend_location_property(      sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_filled_property(               sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_outside_colors_property(       sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_color_range_property(          sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_grid_position_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;
int set_anti_aliasing_property(        sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol ) ;

#endif /* _SET_HANDLE_PROPERTY_H_ */
