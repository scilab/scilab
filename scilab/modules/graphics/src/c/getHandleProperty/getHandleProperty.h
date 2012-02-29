/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: getHandleProperty.h                                              */
/* desc : contains declarations of functions needed by sci_get            */
/*        Each function has the same signature int (sciPointObj *) and    */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_PROPERTY_H_
#define _GET_HANDLE_PROPERTY_H_

#include "ObjectStructure.h"

#include "GetUimenuLabel.h"
#include "GetUicontrolString.h"
#include "GetUicontrolStyle.h"
#include "GetUicontrolPosition.h"
#include "GetUicontrolBackgroundColor.h"
#include "GetUiobjectForegroundColor.h"
#include "GetUiobjectCallback.h"
#include "GetUicontrolFontWeight.h"
#include "GetUicontrolFontUnits.h"
#include "GetUicontrolFontSize.h"
#include "GetUicontrolFontAngle.h"
#include "GetUiobjectEnable.h"
#include "GetUicontrolMin.h"
#include "GetUicontrolMax.h"
#include "GetUicontrolUnits.h"
#include "GetUicontrolListboxTop.h"
#include "GetUicontrolValue.h"
#include "GetUicontrolRelief.h"
#include "GetUicontrolHorizontalAlignment.h"
#include "GetUicontrolVerticalAlignment.h"
#include "GetUicontrolFontName.h"
#include "GetUiobjectColumnNames.h"
#include "GetUiobjectRowNames.h"
#include "GetUicontrolSliderStep.h"
#include "GetUiobjectScale.h"
#include "GetUiobjectShear.h"
#include "GetUiobjectTableData.h"
#include "GetUimenuChecked.h"
#include "GetConsoleShowHiddenHandles.h"

int get_figures_id_property(char *pobjUID);
int get_visible_property(char *pobjUID);
int get_pixel_drawing_mode_property(char *pobjUID);
int get_old_style_property(char *pobjUID);
int get_figure_style_property(char *pobjUID);
int get_auto_resize_property(char *pobjUID);
int get_figure_position_property(char *pobjUID);
int get_axes_size_property(char *pobjUID);
int get_figure_size_property(char *pobjUID);
int get_figure_name_property(char *pobjUID);
int get_figure_id_property(char *pobjUID);
int get_rotation_style_property(char *pobjUID);
int get_immediate_drawing_property(char *pobjUID);
int get_pixmap_property(char *pobjUID);
int get_type_property(char *pobjUID);
int get_parent_property(char *pobjUID);
int get_current_axes_property(char *pobjUID);
int get_current_figure_property(char *pobjUID);
int get_current_entity_property(char *pobjUID);
int get_children_property(char *pobjUID);
int get_default_figure_property(char *pobjUID);
int get_default_axes_property(char *pobjUID);
int get_color_map_property(char *pobjUID);
int get_interp_color_vector_property(char *pobjUID);
int get_interp_color_mode_property(char *pobjUID);
int get_background_property(char *pobjUID);
int get_foreground_property(char *pobjUID);
int get_fill_mode_property(char *pobjUID);
int get_thickness_property(char *pobjUID);
int get_arrow_size_factor_property(char *pobjUID);
int get_line_style_property(char *pobjUID);
int get_line_mode_property(char *pobjUID);
int get_surface_mode_property(char *pobjUID);
int get_mark_style_property(char *pobjUID);
int get_mark_mode_property(char *pobjUID);
int get_mark_size_unit_property(char *pobjUID);
int get_mark_size_property(char *pobjUID);
int get_mark_foreground_property(char *pobjUID);
int get_mark_background_property(char *pobjUID);
int get_bar_layout_property(char *pobjUID);
int get_bar_width_property(char *pobjUID);
int get_x_shift_property(char *pobjUID);
int get_y_shift_property(char *pobjUID);
int get_z_shift_property(char *pobjUID);
int get_polyline_style_property(char *pobjUID);
int get_font_size_property(char *pobjUID);
int get_font_angle_property(char *pobjUID);
int get_font_foreground_property(char *pobjUID);
int get_font_color_property(char *pobjUID);
int get_font_style_property(char *pobjUID);
int get_text_box_mode_property(char *pobjUID);
int get_auto_dimensionning_property(char *pobjUID);
int get_alignment_property(char *pobjUID);
int get_text_box_property(char *pobjUID);
int get_text_property(char *pobjUID);
int get_auto_clear_property(char *pobjUID);
int get_auto_scale_property(char *pobjUID);
int get_zoom_box_property(char *pobjUID);
int get_zoom_state_property(char *pobjUID);
int get_clip_box_property(char *pobjUID);
int get_clip_state_property(char *pobjUID);
int get_data_property(char *pobjUID);
int get_callback_property(char *pobjUID);
int get_x_label_property(char *pobjUID);
int get_y_label_property(char *pobjUID);
int get_z_label_property(char *pobjUID);
int get_title_property(char *pobjUID);
int get_log_flags_property(char *pobjUID);
int get_tics_direction_property(char *pobjUID);
int get_x_location_property(char *pobjUID);
int get_y_location_property(char *pobjUID);
int get_tight_limits_property(char *pobjUID);
int get_closed_property(char *pobjUID);
int get_auto_position_property(char *pobjUID);
int get_auto_rotation_property(char *pobjUID);
int get_position_property(char *pobjUID);
int get_auto_ticks_property(char *pobjUID);
int get_axes_reverse_property(char *pobjUID);
int get_view_property(char *pobjUID);
int get_axes_bounds_property(char *pobjUID);
int get_data_bounds_property(char *pobjUID);
int get_margins_property(char *pobjUID);
int get_tics_color_property(char *pobjUID);
int get_tics_style_property(char *pobjUID);
int get_sub_tics_property(char *pobjUID);
int get_tics_segment_property(char *pobjUID);
int get_labels_font_size_property(char *pobjUID);
int get_labels_font_color_property(char *pobjUID);
int get_labels_font_style_property(char *pobjUID);
int get_format_n_property(char *pobjUID);
int get_xtics_coord_property(char *pobjUID);
int get_ytics_coord_property(char *pobjUID);
int get_tics_labels_property(char *pobjUID);
int get_box_property(char *pobjUID);
int get_grid_property(char *pobjUID);
int get_axes_visible_property(char *pobjUID);
int get_hidden_color_property(char *pobjUID);
int get_isoview_property(char *pobjUID);
int get_cube_scaling_property(char *pobjUID);
int get_arrow_size_property(char *pobjUID);
int get_segs_color_property(char *pobjUID);
int get_colored_property(char *pobjUID);
int get_data_mapping_property(char *pobjUID);
int get_rotation_angles_property(char *pobjUID);
int get_color_mode_property(char *pobjUID);
int get_color_flag_property(char *pobjUID);
int get_cdata_mapping_property(char *pobjUID);
int get_surface_color_property(char *pobjUID);
int get_triangles_property(char *pobjUID);
int get_z_bounds_property(char *pobjUID);
int get_user_data_property(char *pobjUID);
int get_handle_visible_property(char *pobjUID);
int get_callback_type_property(char *pobjUID);
int get_hidden_axis_color_property(char *pobjUID);
int get_x_ticks_property(char *pobjUID);
int get_y_ticks_property(char *pobjUID);
int get_z_ticks_property(char *pobjUID);
int get_viewport_property(char *pobjUID);
int get_info_message_property(char *pobjUID);
int get_screen_position_property(char *pobjUID);
int get_event_handler_enable_property(char *pobjUID);
int get_event_handler_property(char *pobjUID);
int get_arc_drawing_method_property(char *pobjUID);
int get_fractional_font_property(char *pobjUID);
int get_links_property(char *pobjUID);
int get_legend_location_property(char *pobjUID);
int get_filled_property(char *pobjUID);
int get_color_range_property(char *pobjUID);
int get_outside_colors_property(char *pobjUID);
int get_grid_position_property(char *pobjUID);
int get_anti_aliasing_property(char *pobjUID);
int get_tag_property(char *pobjUID);
int get_UID(char *pobjUID);

#endif /* _GET_HANDLE_PROPERTY_H_ */
