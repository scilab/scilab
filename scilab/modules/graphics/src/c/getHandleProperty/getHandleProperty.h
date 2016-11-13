/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: getHandleProperty.h                                              */
/* desc : contains declarations of functions needed by sci_get            */
/*        Each function has the same signature int (void*, int) and    */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_PROPERTY_H_
#define _GET_HANDLE_PROPERTY_H_

#include "GetUicontrol.h"

GRAPHICS_IMPEXP void* get_figures_id_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_visible_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_pixel_drawing_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_old_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_resize_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_position_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_axes_size_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_size_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_name_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_id_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_resizefcn_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_figure_closerequestfcn_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_rotation_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_immediate_drawing_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_type_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_parent_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_current_axes_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_current_figure_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_current_entity_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_children_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_default_figure_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_default_axes_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_color_map_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_interp_color_vector_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_interp_color_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_background_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_foreground_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_fill_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_thickness_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_arrow_size_factor_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_line_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_line_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_surface_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_size_unit_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_size_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_offset_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_stride_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_foreground_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_mark_background_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_bar_layout_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_bar_width_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_x_shift_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_y_shift_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_z_shift_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_polyline_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_font_size_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_font_angle_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_font_foreground_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_font_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_font_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_text_box_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_dimensionning_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_alignment_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_text_box_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_text_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_clear_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_scale_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_zoom_box_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_zoom_state_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_clip_box_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_clip_state_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_data_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_callback_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_x_label_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_y_label_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_z_label_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_title_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_log_flags_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tics_direction_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_x_location_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_y_location_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tight_limits_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_closed_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_position_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_rotation_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_position_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_ticks_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_axes_reverse_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_view_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_axes_bounds_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_data_bounds_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_margins_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tics_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tics_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_sub_tics_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tics_segment_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_labels_font_size_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_labels_font_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_labels_font_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_format_n_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_xtics_coord_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_ytics_coord_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tics_labels_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_box_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_grid_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_grid_thickness_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_grid_style_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_axes_visible_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_hidden_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_isoview_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_cube_scaling_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_arrow_size_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_segs_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_colored_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_data_mapping_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_rotation_angles_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_color_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_color_flag_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_cdata_mapping_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_surface_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_triangles_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_z_bounds_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_user_data_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_handle_visible_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_callback_type_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_hidden_axis_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_x_ticks_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_y_ticks_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_z_ticks_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_viewport_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_info_message_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_screen_position_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_event_handler_enable_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_event_handler_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_arc_drawing_method_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_fractional_font_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_links_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_legend_location_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_filled_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_color_range_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_outside_colors_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_grid_position_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_anti_aliasing_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tag_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_UID(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_orientation_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_z_component_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_display_components_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_auto_orientation_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_interp_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_box_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_label_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_disp_function_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_tip_detached_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_ambient_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_diffuse_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_specular_color_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_use_color_material_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_material_shininess_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_light_type_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_direction_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_image_type_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_video_mode_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_datatips_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_display_function_data_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_resize_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_toolbar_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_toolbar_visible_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_menubar_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_menubar_visible_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_infobar_visible_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_dockable_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_layout_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_constraints_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_rect_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_layout_options_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_border_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_groupname_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_title_position_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_title_scroll_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_scrollable_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_line_width_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_marks_count_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_auto_margins_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_ticks_format_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_ticks_st_property(void* _pvCtx, int iObjUID);
GRAPHICS_IMPEXP void* get_colors_property(void* _pvCtx, int pobjUID);
GRAPHICS_IMPEXP void* get_colors_property(void* _pvCtx, int iObjUID);

#endif /* _GET_HANDLE_PROPERTY_H_ */
