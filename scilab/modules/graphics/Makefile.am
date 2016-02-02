#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA   - 2006 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009-2010 - Pierre MARECHAL
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#


#### target ####
modulename=graphics

#### graphics : Conf files ####
libscigraphics_la_rootdir = $(mydatadir)
libscigraphics_la_root_DATA =  license.txt

#### graphics : init scripts ####
libscigraphics_la_etcdir = $(mydatadir)/etc
libscigraphics_la_etc_DATA = \
    etc/graphics.quit \
    etc/graphics.start

#### graphics : datatips sub-module ####
MACROSDIRSEXT = macros/datatips

GRAPHICS_C_SOURCES = \
    src/c/GetCommandArg.c \
    src/c/loadTextRenderingAPI.c \
    src/c/getHandleProperty/get_figure_position_property.c \
    src/c/getHandleProperty/get_figure_size_property.c \
    src/c/getHandleProperty/get_visible_property.c \
    src/c/getHandleProperty/get_old_style_property.c \
    src/c/getHandleProperty/get_figures_id_property.c \
    src/c/getHandleProperty/get_figure_style_property.c \
    src/c/getHandleProperty/get_pixel_drawing_mode_property.c \
    src/c/getHandleProperty/get_line_mode_property.c \
    src/c/getHandleProperty/get_axes_size_property.c \
    src/c/getHandleProperty/get_thickness_property.c \
    src/c/getHandleProperty/get_font_foreground_property.c \
    src/c/getHandleProperty/get_bar_layout_property.c \
    src/c/getHandleProperty/get_current_axes_property.c \
    src/c/getHandleProperty/get_bar_width_property.c \
    src/c/getHandleProperty/get_datatips_property.c \
    src/c/getHandleProperty/get_font_color_property.c \
    src/c/getHandleProperty/set_default_axes_property.c \
    src/c/getHandleProperty/get_default_axes_property.c \
    src/c/getHandleProperty/get_y_shift_property.c \
    src/c/getHandleProperty/get_immediate_drawing_property.c \
    src/c/getHandleProperty/get_current_entity_property.c \
    src/c/getHandleProperty/get_background_property.c \
    src/c/getHandleProperty/get_rotation_style_property.c \
    src/c/getHandleProperty/get_type_property.c \
    src/c/getHandleProperty/get_current_figure_property.c \
    src/c/getHandleProperty/get_mark_foreground_property.c \
    src/c/getHandleProperty/get_default_figure_property.c \
    src/c/getHandleProperty/get_children_property.c \
    src/c/getHandleProperty/get_parent_property.c \
    src/c/getHandleProperty/get_surface_mode_property.c \
    src/c/getHandleProperty/get_figure_id_property.c \
    src/c/getHandleProperty/get_mark_mode_property.c \
    src/c/getHandleProperty/get_interp_color_mode.c \
    src/c/getHandleProperty/get_font_size_property.c \
    src/c/getHandleProperty/get_x_shift_property.c \
    src/c/getHandleProperty/get_z_shift_property.c \
    src/c/getHandleProperty/get_line_style_property.c \
    src/c/getHandleProperty/get_foreground_property.c \
    src/c/getHandleProperty/get_mark_style_property.c \
    src/c/getHandleProperty/get_color_map_property.c \
    src/c/getHandleProperty/get_polyline_style_property.c \
    src/c/getHandleProperty/get_font_angle_property.c \
    src/c/getHandleProperty/get_interp_color_vector.c \
    src/c/getHandleProperty/get_figure_name_property.c \
    src/c/getHandleProperty/get_arrow_size_factor_property.c \
    src/c/getHandleProperty/get_mark_size_unit_property.c \
    src/c/getHandleProperty/get_mark_size_property.c \
    src/c/getHandleProperty/get_mark_offset_property.c \
    src/c/getHandleProperty/get_mark_stride_property.c \
    src/c/getHandleProperty/get_mark_background_property.c \
    src/c/getHandleProperty/get_auto_resize_property.c \
    src/c/getHandleProperty/get_fill_mode_property.c \
    src/c/getHandleProperty/get_tight_limits_property.c \
    src/c/getHandleProperty/get_auto_ticks_property.c \
    src/c/getHandleProperty/get_x_label_property.c \
    src/c/getHandleProperty/get_auto_scale_property.c \
    src/c/getHandleProperty/get_auto_dimensionning_property.c \
    src/c/getHandleProperty/get_text_box_property.c \
    src/c/getHandleProperty/get_clip_box_property.c \
    src/c/getHandleProperty/get_tics_direction_property.c \
    src/c/getHandleProperty/get_position_property.c \
    src/c/getHandleProperty/get_y_label_property.c \
    src/c/getHandleProperty/get_data_property.c \
    src/c/getHandleProperty/get_auto_clear_property.c \
    src/c/getHandleProperty/get_title_property.c \
    src/c/getHandleProperty/get_zoom_state_property.c \
    src/c/getHandleProperty/get_zoom_box_property.c \
    src/c/getHandleProperty/get_log_flags_property.c \
    src/c/getHandleProperty/get_x_location_property.c \
    src/c/getHandleProperty/get_text_box_mode_property.c \
    src/c/getHandleProperty/get_y_location_property.c \
    src/c/getHandleProperty/get_z_label_property.c \
    src/c/getHandleProperty/get_alignment_property.c \
    src/c/getHandleProperty/get_font_style_property.c \
    src/c/getHandleProperty/get_auto_rotation_property.c \
    src/c/getHandleProperty/get_clip_state_property.c \
    src/c/getHandleProperty/get_callback_property.c \
    src/c/getHandleProperty/get_auto_position_property.c \
    src/c/getHandleProperty/get_closed_property.c \
    src/c/getHandleProperty/get_text_property.c \
    src/c/getHandleProperty/get_margins_property.c \
    src/c/getHandleProperty/get_view_property.c \
    src/c/getHandleProperty/get_data_bounds_property.c \
    src/c/getHandleProperty/get_axes_reverse_property.c \
    src/c/getHandleProperty/get_tics_color_property.c \
    src/c/getHandleProperty/get_axes_bounds_property.c \
    src/c/getHandleProperty/get_tics_style_property.c \
    src/c/getHandleProperty/get_user_data_property.c \
    src/c/getHandleProperty/get_xtics_coord_property.c \
    src/c/getHandleProperty/get_color_mode_property.c \
    src/c/getHandleProperty/get_z_bounds_property.c \
    src/c/getHandleProperty/get_data_mapping_property.c \
    src/c/getHandleProperty/get_arrow_size_property.c \
    src/c/getHandleProperty/get_rotation_angles_property.c \
    src/c/getHandleProperty/get_callback_type_property.c \
    src/c/getHandleProperty/get_format_n_property.c \
    src/c/getHandleProperty/get_color_flag_property.c \
    src/c/getHandleProperty/get_sub_tics_property.c \
    src/c/getHandleProperty/get_hidden_axis_color_property.c \
    src/c/getHandleProperty/get_ytics_coord_property.c \
    src/c/getHandleProperty/get_labels_font_size_property.c \
    src/c/getHandleProperty/get_isoview_property.c \
    src/c/getHandleProperty/get_grid_property.c \
    src/c/getHandleProperty/get_grid_thickness_property.c \
    src/c/getHandleProperty/get_grid_style_property.c \
    src/c/getHandleProperty/get_box_property.c \
    src/c/getHandleProperty/get_hidden_color_property.c \
    src/c/getHandleProperty/get_handle_visible_property.c \
    src/c/getHandleProperty/get_labels_font_color_property.c \
    src/c/getHandleProperty/get_surface_color_property.c \
    src/c/getHandleProperty/get_tics_label_property.c \
    src/c/getHandleProperty/get_labels_font_style_property.c \
    src/c/getHandleProperty/get_axes_visible_property.c \
    src/c/getHandleProperty/get_cube_scaling_property.c \
    src/c/getHandleProperty/get_tics_segment_property.c \
    src/c/getHandleProperty/get_cdata_mapping_property.c \
    src/c/getHandleProperty/get_colored_property.c \
    src/c/getHandleProperty/get_triangles_property.c \
    src/c/getHandleProperty/get_viewport_property.c \
    src/c/getHandleProperty/get_info_message_property.c \
    src/c/getHandleProperty/get_y_ticks_property.c \
    src/c/getHandleProperty/get_z_ticks_property.c \
    src/c/getHandleProperty/get_x_ticks_property.c \
    src/c/getHandleProperty/get_ticks_format_property.c \
    src/c/getHandleProperty/get_ticks_st_property.c \
    src/c/getHandleProperty/setGetHashTable.c \
    src/c/getHandleProperty/get_segs_color_property.c \
    src/c/getHandleProperty/get_screen_position_property.c \
    src/c/getHandleProperty/get_event_handler_property.c \
    src/c/getHandleProperty/get_event_handler_enable_property.c \
    src/c/getHandleProperty/get_arc_drawing_method_property.c \
    src/c/getHandleProperty/get_fractional_font_property.c \
    src/c/getHandleProperty/get_links_property.c \
    src/c/getHandleProperty/get_legend_location_property.c \
    src/c/getHandleProperty/get_filled_property.c \
    src/c/getHandleProperty/get_color_range_property.c \
    src/c/getHandleProperty/get_outside_colors_property.c \
    src/c/getHandleProperty/get_grid_position_property.c \
    src/c/getHandleProperty/get_anti_aliasing_property.c \
    src/c/getHandleProperty/get_tag_property.c \
    src/c/getHandleProperty/get_image_type_property.c \
    src/c/getHandleProperty/get_resize_property.c \
    src/c/getHandleProperty/get_toolbar_property.c \
    src/c/getHandleProperty/get_menubar_property.c \
    src/c/getHandleProperty/get_infobar_property.c \
    src/c/getHandleProperty/get_dockable_property.c \
    src/c/getHandleProperty/get_rect_property.c \
    src/c/getHandleProperty/GetHashTable.c \
    src/c/graphicModuleLoad.c \
    src/c/getHandleProperty/set_old_style_property.c \
    src/c/getHandleProperty/set_figure_name_property.c \
    src/c/getHandleProperty/getPropertyAssignedValue.c \
    src/c/getHandleProperty/set_visible_property.c \
    src/c/getHandleProperty/set_pixel_drawing_mode_property.c \
    src/c/getHandleProperty/set_interp_color_vector_property.c \
    src/c/getHandleProperty/set_immediate_drawing_property.c \
    src/c/getHandleProperty/set_figure_size_property.c \
    src/c/getHandleProperty/set_background_property.c \
    src/c/getHandleProperty/set_rotation_style_property.c \
    src/c/getHandleProperty/set_current_figure_property.c \
    src/c/getHandleProperty/set_axes_size_property.c \
    src/c/getHandleProperty/set_default_values_property.c \
    src/c/getHandleProperty/set_figure_position_property.c \
    src/c/getHandleProperty/set_color_map_property.c \
    src/c/getHandleProperty/set_auto_resize_property.c \
    src/c/getHandleProperty/set_current_axes_property.c \
    src/c/getHandleProperty/set_current_entity_property.c \
    src/c/getHandleProperty/set_figure_id_property.c \
    src/c/getHandleProperty/set_figure_style_property.c \
    src/c/getHandleProperty/set_mark_size_property.c \
    src/c/getHandleProperty/set_mark_offset_property.c \
    src/c/getHandleProperty/set_mark_stride_property.c \
    src/c/getHandleProperty/set_y_shift_property.c \
    src/c/getHandleProperty/set_auto_scale_property.c \
    src/c/getHandleProperty/set_auto_dimensionning_property.c \
    src/c/getHandleProperty/set_text_box_property.c \
    src/c/getHandleProperty/set_text_property.c \
    src/c/getHandleProperty/set_mark_mode_property.c \
    src/c/getHandleProperty/set_clip_box_property.c \
    src/c/getHandleProperty/set_z_shift_property.c \
    src/c/getHandleProperty/set_line_style_property.c \
    src/c/getHandleProperty/set_polyline_style_property.c \
    src/c/getHandleProperty/set_line_mode_property.c \
    src/c/getHandleProperty/set_thickness_property.c \
    src/c/getHandleProperty/set_font_foreground_property.c \
    src/c/getHandleProperty/set_text_box_mode_property.c \
    src/c/getHandleProperty/set_bar_layout_property.c \
    src/c/getHandleProperty/set_bar_width_property.c \
    src/c/getHandleProperty/set_datatips_property.c \
    src/c/getHandleProperty/SetPropertyStatus.c \
    src/c/getHandleProperty/set_font_style_property.c \
    src/c/getHandleProperty/set_surface_mode_property.c \
    src/c/getHandleProperty/set_font_size_property.c \
    src/c/getHandleProperty/set_mark_style_property.c \
    src/c/getHandleProperty/set_mark_background_property.c \
    src/c/getHandleProperty/set_interp_color_mode_property.c \
    src/c/getHandleProperty/set_font_angle_property.c \
    src/c/getHandleProperty/set_auto_clear_property.c \
    src/c/getHandleProperty/set_arrow_size_factor_property.c \
    src/c/getHandleProperty/set_mark_size_unit_property.c \
    src/c/getHandleProperty/set_zoom_state_property.c \
    src/c/getHandleProperty/set_fill_mode_property.c \
    src/c/getHandleProperty/set_zoom_box_property.c \
    src/c/getHandleProperty/set_font_color_property.c \
    src/c/getHandleProperty/set_alignment_property.c \
    src/c/getHandleProperty/set_mark_foreground_property.c \
    src/c/getHandleProperty/set_clip_state_property.c \
    src/c/getHandleProperty/set_x_shift_property.c \
    src/c/getHandleProperty/set_foreground_property.c \
    src/c/getHandleProperty/set_tics_direction_property.c \
    src/c/getHandleProperty/set_title_property.c \
    src/c/getHandleProperty/set_x_location_property.c \
    src/c/getHandleProperty/set_auto_position_property.c \
    src/c/getHandleProperty/set_tight_limits_property.c \
    src/c/getHandleProperty/set_x_label_property.c \
    src/c/getHandleProperty/set_y_label_property.c \
    src/c/getHandleProperty/set_y_location_property.c \
    src/c/getHandleProperty/set_z_label_property.c \
    src/c/getHandleProperty/set_auto_rotation_property.c \
    src/c/getHandleProperty/set_callback_property.c \
    src/c/getHandleProperty/set_closed_property.c \
    src/c/getHandleProperty/set_cdata_mapping_property.c \
    src/c/getHandleProperty/set_y_ticks_property.c \
    src/c/getHandleProperty/set_z_bounds_property.c \
    src/c/getHandleProperty/set_position_property.c \
    src/c/getHandleProperty/set_z_ticks_property.c \
    src/c/getHandleProperty/set_axes_bounds_property.c \
    src/c/getHandleProperty/set_tics_style_property.c \
    src/c/getHandleProperty/SetHashTable.c \
    src/c/getHandleProperty/set_handle_visible_property.c \
    src/c/getHandleProperty/set_labels_font_color_property.c \
    src/c/getHandleProperty/set_parent_property.c \
    src/c/getHandleProperty/set_axes_visible_property.c \
    src/c/getHandleProperty/set_triangles_property.c \
    src/c/getHandleProperty/set_tics_color_property.c \
    src/c/getHandleProperty/set_color_flag_property.c \
    src/c/getHandleProperty/set_labels_font_size_property.c \
    src/c/getHandleProperty/set_tics_segment_property.c \
    src/c/getHandleProperty/set_user_data_property.c \
    src/c/getHandleProperty/set_view_property.c \
    src/c/getHandleProperty/set_arrow_size_property.c \
    src/c/getHandleProperty/set_children_property.c \
    src/c/getHandleProperty/set_axes_reverse_property.c \
    src/c/getHandleProperty/set_grid_property.c \
    src/c/getHandleProperty/set_grid_thickness_property.c \
    src/c/getHandleProperty/set_grid_style_property.c \
    src/c/getHandleProperty/set_log_flags_property.c \
    src/c/getHandleProperty/set_cube_scaling_property.c \
    src/c/getHandleProperty/set_colored_property.c \
    src/c/getHandleProperty/set_margins_property.c \
    src/c/getHandleProperty/set_auto_ticks_property.c \
    src/c/getHandleProperty/set_xtics_coord_property.c \
    src/c/getHandleProperty/set_color_mode_property.c \
    src/c/getHandleProperty/set_data_bounds_property.c \
    src/c/getHandleProperty/set_tics_labels_property.c \
    src/c/getHandleProperty/set_data_mapping_property.c \
    src/c/getHandleProperty/set_rotation_angles_property.c \
    src/c/getHandleProperty/set_callback_type_property.c \
    src/c/getHandleProperty/set_segs_color_property.c \
    src/c/getHandleProperty/set_format_n_property.c \
    src/c/getHandleProperty/set_sub_tics_property.c \
    src/c/getHandleProperty/set_hidden_axis_color_property.c \
    src/c/getHandleProperty/set_isoview_property.c \
    src/c/getHandleProperty/set_box_property.c \
    src/c/getHandleProperty/set_hidden_color_property.c \
    src/c/getHandleProperty/set_label_font_style_property.c \
    src/c/getHandleProperty/set_surface_color_property.c \
    src/c/getHandleProperty/set_yticks_coord_property.c \
    src/c/getHandleProperty/CheckTicksProperty.c \
    src/c/getHandleProperty/set_x_ticks_property.c \
    src/c/getHandleProperty/set_ticks_format_property.c \
    src/c/getHandleProperty/set_ticks_st_property.c \
    src/c/getHandleProperty/set_viewport_property.c \
    src/c/getHandleProperty/set_info_message_property.c \
    src/c/getHandleProperty/set_screen_position_property.c \
    src/c/getHandleProperty/set_event_handler_property.c \
    src/c/getHandleProperty/set_event_handler_enable_property.c \
    src/c/getHandleProperty/set_arc_drawing_method_property.c \
    src/c/getHandleProperty/set_fractional_font_property.c \
    src/c/getHandleProperty/set_links_property.c \
    src/c/getHandleProperty/set_legend_location_property.c \
    src/c/getHandleProperty/set_filled_property.c \
    src/c/getHandleProperty/set_color_range_property.c \
    src/c/getHandleProperty/set_outside_colors_property.c \
    src/c/getHandleProperty/set_grid_position_property.c \
    src/c/getHandleProperty/set_anti_aliasing_property.c \
    src/c/getHandleProperty/set_tag_property.c \
    src/c/getHandleProperty/set_figure_resizefcn_property.c \
    src/c/getHandleProperty/get_figure_resizefcn_property.c \
    src/c/getHandleProperty/set_figure_closerequestfcn_property.c \
    src/c/getHandleProperty/get_figure_closerequestfcn_property.c \
    src/c/getHandleProperty/get_datatip_property.c \
    src/c/getHandleProperty/set_datatip_property.c \
    src/c/getHandleProperty/set_image_type_property.c \
    src/c/getHandleProperty/get_display_function_data_property.c \
    src/c/getHandleProperty/set_display_function_data_property.c \
    src/c/getHandleProperty/set_resize_property.c \
    src/c/getHandleProperty/set_toolbar_property.c \
    src/c/getHandleProperty/set_menubar_property.c \
    src/c/getHandleProperty/set_infobar_property.c \
    src/c/getHandleProperty/set_dockable_property.c \
    src/c/getHandleProperty/get_layout_property.c \
    src/c/getHandleProperty/set_layout_property.c \
    src/c/getHandleProperty/get_constraints_property.c \
    src/c/getHandleProperty/set_constraints_property.c \
    src/c/getHandleProperty/set_rect_property.c \
    src/c/getHandleProperty/get_layout_options_property.c \
    src/c/getHandleProperty/set_layout_options_property.c \
    src/c/getHandleProperty/get_border_property.cpp \
    src/c/getHandleProperty/set_border_property.c \
    src/c/getHandleProperty/get_groupname_property.c \
    src/c/getHandleProperty/set_groupname_property.c \
    src/c/getHandleProperty/get_tabtitle_property.c \
    src/c/getHandleProperty/set_tabtitle_property.c \
    src/c/getHandleProperty/get_scrollable_property.c \
    src/c/getHandleProperty/set_scrollable_property.c \
    src/c/getHandleProperty/get_marks_count_property.c \
    src/c/getHandleProperty/set_marks_count_property.c \
    src/c/getHandleProperty/get_line_width_property.c \
    src/c/getHandleProperty/set_line_width_property.c \
    src/c/getHandleProperty/get_colors_property.c \
    src/c/getHandleProperty/set_colors_property.c \
    src/c/DefaultCommandArg.c \
    src/c/Axes.c \
    src/c/sciMatrix.c \
    src/c/DrawObjects.c \
    src/c/GetProperty.c \
    src/c/Contour.c \
    src/c/SetProperty.c \
    src/c/BasicAlgos.c \
    src/c/BuildObjects.c \
    src/c/sciCall.c \
    src/c/CloneObjects.c \
    src/c/InitObjects.c \
    src/c/pixel_mode.c \
    src/c/Format.c \
    src/c/Champ.c \
    src/c/StringMatrix.c \
    src/c/FeC.c \
    src/c/Plo2dEch.c \
    src/c/Plo2dn.c \
    src/c/Gray.c \
    src/c/DestroyObjects.c \
    src/c/math_graphics.c \
    src/c/Interaction.c \
    src/c/StringBox.c \
    src/c/axesScale.c \
    src/c/TerminateGraphics.c \
    src/c/periEmpty.c \
    src/c/scitokenize.c \
    src/c/get_ticks_utils.c \
    src/c/getHandleProperty/get_ambient_color_property.c \
    src/c/getHandleProperty/get_diffuse_color_property.c \
    src/c/getHandleProperty/get_specular_color_property.c \
    src/c/getHandleProperty/get_use_color_material_property.c \
    src/c/getHandleProperty/get_material_shininess_property.c \
    src/c/getHandleProperty/get_light_type_property.c \
    src/c/getHandleProperty/get_direction_property.c \
    src/c/getHandleProperty/set_ambient_color_property.c \
    src/c/getHandleProperty/set_diffuse_color_property.c \
    src/c/getHandleProperty/set_specular_color_property.c \
    src/c/getHandleProperty/set_use_color_material_property.c \
    src/c/getHandleProperty/set_material_shininess_property.c \
    src/c/getHandleProperty/set_light_type_property.c \
    src/c/getHandleProperty/set_direction_property.c \
    src/c/getHandleProperty/set_auto_margins_property.c \
    src/c/getHandleProperty/get_auto_margins_property.c \
    src/c/name2rgb.c \
    src/c/addColor.c \
    src/c/DllmainGraphics.c

GRAPHICS_CXX_SOURCES = \
    src/cpp/configgraphicvariable.cpp \
    src/cpp/set_data_property.cpp \
    src/cpp/ContourOnMesh.cpp

GATEWAY_C_SOURCES = \
    sci_gateway/c/sci_swap_handles.c \
    sci_gateway/c/sci_grayplot.c \
    sci_gateway/c/sci_xfpolys.c \
    sci_gateway/c/sci_newaxes.c \
    sci_gateway/c/sci_xlfont.c \
    sci_gateway/c/sci_xgraduate.c \
    sci_gateway/c/sci_xgrid.c \
    sci_gateway/c/sci_xclick.c \
    sci_gateway/c/sci_geom3d.c \
    sci_gateway/c/sci_unzoom.c \
    sci_gateway/c/sci_xsegs.c \
    sci_gateway/c/sci_glue.c \
    sci_gateway/c/sci_xarc.c \
    sci_gateway/c/sci_xpolys.c \
    sci_gateway/c/sci_xstring.c \
    sci_gateway/c/sci_xstringb.c \
    sci_gateway/c/sci_show_window.c \
    sci_gateway/c/sci_param3d1.c \
    sci_gateway/c/sci_xname.c \
    sci_gateway/c/sci_xpoly.c \
    sci_gateway/c/sci_xchange.c \
    sci_gateway/c/sci_drawaxis.c \
    sci_gateway/c/sci_demo.c \
    sci_gateway/c/sci_plot2d1.c \
    sci_gateway/c/sci_xrect.c \
    sci_gateway/c/sci_xarrows.c \
    sci_gateway/c/sci_plot3d.c \
    sci_gateway/c/sci_move.c \
    sci_gateway/c/sci_copy.c \
    sci_gateway/c/sci_xarcs.c \
    sci_gateway/c/sci_xfpoly.c \
    sci_gateway/c/sci_drawlater.c \
    sci_gateway/c/sci_champ.c \
    sci_gateway/c/sci_unglue.c \
    sci_gateway/c/sci_zoom_rect.c \
    sci_gateway/c/sci_delete.c \
    sci_gateway/c/sci_plot2d.c \
    sci_gateway/c/sci_xrects.c \
    sci_gateway/c/sci_xtitle.c \
    sci_gateway/c/sci_StringBox.c \
    sci_gateway/c/sci_xgetmouse.c \
    sci_gateway/c/sci_drawnow.c \
    sci_gateway/c/sci_xfarcs.c \
    sci_gateway/c/sci_param3d.c \
    sci_gateway/c/sci_contour2di.c \
    sci_gateway/c/sci_xdel.c \
    sci_gateway/c/sci_fec.c \
    sci_gateway/c/sci_set.c \
    sci_gateway/c/sci_rubberbox.c \
    sci_gateway/c/sci_rotate_axes.c \
    sci_gateway/c/sci_is_handle_valid.c \
    sci_gateway/c/sci_Legend.c \
    sci_gateway/c/sci_light.c \
    sci_gateway/c/sci_color.c \
    sci_gateway/c/sci_name2rgb.c \
    sci_gateway/c/sci_addcolor.c

GATEWAY_CXX_SOURCES = \
    sci_gateway/cpp/graphics_gw.cpp \
    sci_gateway/cpp/sci_get.cpp \
    sci_gateway/cpp/sci_xget.cpp \
    sci_gateway/cpp/sci_xset.cpp \
    sci_gateway/cpp/sci_matplot.cpp \
    sci_gateway/cpp/sci_matplot1.cpp \
    sci_gateway/cpp/sci_winsid.cpp \
    sci_gateway/cpp/sci_contour2dm.cpp

libscigraphics_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/getHandleProperty/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/action_binding/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/graphic_export/includes/ \
    -I$(top_srcdir)/modules/graphic_objects/includes/ \
    -I$(top_srcdir)/modules/gui/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/renderer/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(TCL_INC_PATH) \
    $(TK_INC_PATH) \
    $(AM_CPPFLAGS)

libscigraphics_algo_la_SOURCES = $(GRAPHICS_C_SOURCES) $(GRAPHICS_CXX_SOURCES)
libscigraphics_la_SOURCES = $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libscigraphics_algo_la_CPPFLAGS = $(libscigraphics_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(GRAPHICS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscigraphics_la_CPPFLAGS)

libscigraphics_la_LIBADD = libscigraphics-algo.la



#### graphics : gateway declaration ####
libscigraphics_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscigraphics_la_sci_gateway_DATA = sci_gateway/graphics_gateway.xml

### graphics : ged files ###
libscigraphics_la_geddir = $(mydatadir)
nobase_libscigraphics_la_ged_DATA = tcl/ged/Arc.tcl \
tcl/ged/Axes.tcl \
tcl/ged/Axis.tcl \
tcl/ged/Champ.tcl \
tcl/ged/Compound.tcl \
tcl/ged/Fac3d.tcl \
tcl/ged/Fec.tcl \
tcl/ged/Figure.tcl \
tcl/ged/Grayplot.tcl \
tcl/ged/Legend.tcl \
tcl/ged/Matplot.tcl \
tcl/ged/ObjectsBrowser.tcl \
tcl/ged/Plot3d.tcl \
tcl/ged/Polyline.tcl \
tcl/ged/Rectangle.tcl \
tcl/ged/Segs.tcl \
tcl/ged/Text.tcl \
tcl/ged/images/arc.gif \
tcl/ged/images/axes.gif \
tcl/ged/images/axis.gif \
tcl/ged/images/champ.gif \
tcl/ged/images/compound.gif \
tcl/ged/images/fac3d.gif \
tcl/ged/images/fec.gif \
tcl/ged/images/figure.gif \
tcl/ged/images/grayplot.gif \
tcl/ged/images/leaf.gif \
tcl/ged/images/legend.gif \
tcl/ged/images/matplot.gif \
tcl/ged/images/passwd.gif \
tcl/ged/images/plot3d.gif \
tcl/ged/images/polyline.gif \
tcl/ged/images/rectangle.gif \
tcl/ged/images/segs.gif \
tcl/ged/images/text.gif

libscigraphics_disable_la_SOURCES = \
	src/nographics/nographics.c \
	src/nographics/nographics_gw.cpp

libscigraphics_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/c/getHandleProperty/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/libs/hashtable/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/core/includes/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/dynamic_link/includes \
    $(AM_CPPFLAGS)

# endif

pkglib_LTLIBRARIES = libscigraphics-disable.la

if GUI
USEANT=1
pkglib_LTLIBRARIES += libscigraphics.la
noinst_LTLIBRARIES = libscigraphics-algo.la
endif



include $(top_srcdir)/Makefile.incl.am

