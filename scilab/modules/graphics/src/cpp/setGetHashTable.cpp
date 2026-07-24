/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Clément DAVID
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: setGetHashTable.h                                                */
/* desc : define two hash table to be used in sci_set and sci_get         */
/*        These hash table are based on the Scilab hashTable              */
/*------------------------------------------------------------------------*/
#include <string>
#include <unordered_map>

#include "setGetHashTable.h"
extern "C"
{
#include "setHandleProperty.h"
#include "getHandleProperty.h"
}

using get_map_t = std::unordered_map<std::string, getPropertyFunc>;
using set_map_t = std::unordered_map<std::string, setPropertyFunc>;

static get_map_t getMap;
static set_map_t setMap;
/*--------------------------------------------------------------------------*/
static std::string normalizeKey(const char* key)
{
    std::string lowerKey(key ? key : "");
    for (char& c : lowerKey)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c = static_cast<char>(c + 32);
        }
    }
    return lowerKey;
}
/*--------------------------------------------------------------------------*/
static const GetPropertyEntry getEntries[] =
{
    {"figures_id", get_figures_id_property},
    {"visible", get_visible_property},
    {"pixel_drawing_mode", get_pixel_drawing_mode_property},
    {"old_style", get_old_style_property},
    {"auto_resize", get_auto_resize_property},
    {"figure_position", get_figure_position_property},
    {"axes_size", get_axes_size_property},
    {"figure_size", get_figure_size_property},
    {"figure_name", get_figure_name_property},
    {"name", get_figure_name_property},
    {"figure_id", get_figure_id_property},
    {"rotation_style", get_rotation_style_property},
    {"immediate_drawing", get_immediate_drawing_property},
    {"type", get_type_property},
    {"parent", get_parent_property},
    {"current_axes", get_current_axes_property},
    {"current_figure", get_current_figure_property},
    {"current_obj", get_current_entity_property},
    {"current_entity", get_current_entity_property},
    {"hdl", get_current_entity_property},
    {"children", get_children_property},
    {"default_figure", get_default_figure_property},
    {"default_axes", get_default_axes_property},
    {"color_map", get_color_map_property},
    {"interp_color_vector", get_interp_color_vector_property},
    {"interp_color_mode", get_interp_color_mode_property},
    {"background", get_background_property},
    {"foreground", get_foreground_property},
    {"fill_mode", get_fill_mode_property},
    {"thickness", get_thickness_property},
    {"arrow_size_factor", get_arrow_size_factor_property},
    {"segs_color", get_segs_color_property},
    {"line_style", get_line_style_property},
    {"line_mode", get_line_mode_property},
    {"surface_mode", get_surface_mode_property},
    {"mark_style", get_mark_style_property},
    {"mark_mode", get_mark_mode_property},
    {"mark_size_unit", get_mark_size_unit_property},
    {"mark_size", get_mark_size_property},
    {"mark_foreground", get_mark_foreground_property},
    {"mark_background", get_mark_background_property},
    {"mark_offset", get_mark_offset_property},
    {"mark_stride", get_mark_stride_property},
    {"bar_layout", get_bar_layout_property},
    {"bar_width", get_bar_width_property},
    {"x_shift", get_x_shift_property},
    {"y_shift", get_y_shift_property},
    {"z_shift", get_z_shift_property},
    {"polyline_style", get_polyline_style_property},
    {"font_size", get_font_size_property},
    {"font_angle", get_font_angle_property},
    {"font_foreground", get_font_foreground_property},
    {"font_color", get_font_color_property},
    {"font_style", get_font_style_property},
    {"text_box_mode", get_text_box_mode_property},
    {"auto_dimensionning", get_auto_dimensionning_property},
    {"alignment", get_alignment_property},
    {"text_box", get_text_box_property},
    {"text", get_text_property},
    {"interpreter", get_interpreter_property},
    {"auto_clear", get_auto_clear_property},
    {"auto_scale", get_auto_scale_property},
    {"auto_stretch", get_auto_stretch_property},
    {"zoom_box", get_zoom_box_property},
    {"zoom_state", get_zoom_state_property},
    {"clip_box", get_clip_box_property},
    {"clip_state", get_clip_state_property},
    {"data", get_data_property},
    {"callback", get_callback_property},
    {"x_label", get_x_label_property},
    {"y_label", get_y_label_property},
    {"z_label", get_z_label_property},
    {"title", get_title_property},
    {"log_flags", get_log_flags_property},
    {"tics_direction", get_tics_direction_property},
    {"x_location", get_x_location_property},
    {"y_location", get_y_location_property},
    {"tight_limits", get_tight_limits_property},
    {"closed", get_closed_property},
    {"auto_position", get_auto_position_property},
    {"auto_rotation", get_auto_rotation_property},
    {"position", get_position_property},
    {"auto_ticks", get_auto_ticks_property},
    {"axes_reverse", get_axes_reverse_property},
    {"view", get_view_property},
    {"axes_bounds", get_axes_bounds_property},
    {"data_bounds", get_data_bounds_property},
    {"margins", get_margins_property},
    {"auto_margins", get_auto_margins_property},
    {"tics_color", get_tics_color_property},
    {"tics_style", get_tics_style_property},
    {"sub_tics", get_sub_tics_property},
    {"sub_ticks", get_sub_tics_property},
    {"tics_segment", get_tics_segment_property},
    {"labels_font_size", get_labels_font_size_property},
    {"labels_font_color", get_labels_font_color_property},
    {"labels_font_style", get_labels_font_style_property},
    {"format_n", get_format_n_property},
    {"xtics_coord", get_xtics_coord_property},
    {"ytics_coord", get_ytics_coord_property},
    {"tics_labels", get_tics_labels_property},
    {"tics_interpreters", get_tics_interpreters_property},
    {"box", get_box_property},
    {"grid", get_grid_property},
    {"grid_thickness", get_grid_thickness_property},
    {"grid_style", get_grid_style_property},
    {"axes_visible", get_axes_visible_property},
    {"hiddencolor", get_hidden_color_property},
    {"isoview", get_isoview_property},
    {"cube_scaling", get_cube_scaling_property},
    {"arrow_size", get_arrow_size_property},
    {"arrow_style", get_arrow_style_property},
    {"colored", get_colored_property},
    {"data_mapping", get_data_mapping_property},
    {"rotation_angles", get_rotation_angles_property},
    {"color_mode", get_color_mode_property},
    {"color_flag", get_color_flag_property},
    {"cdata_mapping", get_cdata_mapping_property},
    {"cdata_bounds", get_cdata_bounds_property},
    {"surface_color", get_surface_color_property},
    {"triangles", get_triangles_property},
    {"z_bounds", get_z_bounds_property},
    {"user_data", get_user_data_property},
    {"userdata", get_user_data_property},
    {"handle_visible", get_handle_visible_property},
    {"callback_type", get_callback_type_property},
    {"enable", GetUiobjectEnable},
    {"hidden_axis_color", get_hidden_axis_color_property},
    {"x_ticks", get_x_ticks_property},
    {"y_ticks", get_y_ticks_property},
    {"z_ticks", get_z_ticks_property},
    {"viewport", get_viewport_property},
    {"info_message", get_info_message_property},
    {"screen_position", get_screen_position_property},
    {"event_handler_enable", get_event_handler_enable_property},
    {"event_handler", get_event_handler_property},
    {"label", GetUimenuLabel},
    {"string", GetUicontrolString},
    {"style", GetUicontrolStyle},
    {"backgroundcolor", GetUicontrolBackgroundColor},
    {"foregroundcolor", GetUiobjectForegroundColor},
    {"fontweight", GetUicontrolFontWeight},
    {"fontunits", GetUicontrolFontUnits},
    {"fontsize", GetUicontrolFontSize},
    {"fontangle", GetUicontrolFontAngle},
    {"min", GetUicontrolMin},
    {"max", GetUicontrolMax},
    {"tag", get_tag_property},
    {"listboxtop", GetUicontrolListboxTop},
    {"value", GetUicontrolValue},
    {"units", GetUicontrolUnits},
    {"relief", GetUicontrolRelief},
    {"horizontalalignment", GetUicontrolHorizontalAlignment},
    {"verticalalignment", GetUicontrolVerticalAlignment},
    {"fontname", GetUicontrolFontName},
    {"sliderstep", GetUicontrolSliderStep},
    {"snaptoticks", GetUicontrolSnapToTicks},
    {"checked", GetUimenuChecked},
    {"arc_drawing_method", get_arc_drawing_method_property},
    {"fractional_font", get_fractional_font_property},
    {"links", get_links_property},
    {"legend_location", get_legend_location_property},
    {"filled", get_filled_property},
    {"outside_colors", get_outside_colors_property},
    {"color_range", get_color_range_property},
    {"grid_position", get_grid_position_property},
    {"anti_aliasing", get_anti_aliasing_property},
    {"UID", get_UID},
    {"showhiddenhandles", GetConsoleShowHiddenHandles},
    {"showhiddenproperties", GetConsoleShowHiddenProperties},
    {"usedeprecatedskin", GetConsoleUseDeprecatedLF},
    {"resizefcn", get_figure_resizefcn_property},
    {"tooltipstring", GetUicontrolTooltipString},
    {"closerequestfcn", get_figure_closerequestfcn_property},
    {"orientation", get_tip_orientation_property},
    {"display_components", get_tip_display_components_property},
    {"datatip_display_mode", get_datatip_display_mode_property},
    {"auto_orientation", get_tip_auto_orientation_property},
    {"interp_mode", get_tip_interp_mode_property},
    {"box_mode", get_tip_box_mode_property},
    {"label_mode", get_tip_label_mode_property},
    {"display_function", get_tip_disp_function_property},
    {"detached_position", get_tip_detached_property},
    {"ambient_color", get_ambient_color_property},
    {"diffuse_color", get_diffuse_color_property},
    {"specular_color", get_specular_color_property},
    {"use_color_material", get_use_color_material_property},
    {"material_shininess", get_material_shininess_property},
    {"light_type", get_light_type_property},
    {"direction", get_direction_property},
    {"image_type", get_image_type_property},
    {"datatips", get_datatips_property},
    {"display_function_data", get_display_function_data_property},
    {"resize", get_resize_property},
    {"toolbar", get_toolbar_property},
    {"toolbar_visible", get_toolbar_visible_property},
    {"menubar", get_menubar_property},
    {"menubar_visible", get_menubar_visible_property},
    {"infobar_visible", get_infobar_visible_property},
    {"dockable", get_dockable_property},
    {"layout", get_layout_property},
    {"constraints", get_constraints_property},
    {"rect", get_rect_property},
    {"layout_options", get_layout_options_property},
    {"border", get_border_property},
    {"groupname", get_groupname_property},
    {"title_position", get_title_position_property},
    {"title_scroll", get_title_scroll_property},
    {"default_axes", get_default_axes_property},
    {"scrollable", get_scrollable_property},
    {"icon", GetUicontrolIcon},
    {"line_width", get_line_width_property},
    {"marks_count", get_marks_count_property},
    {"ticks_format", get_ticks_format_property},
    {"ticks_st", get_ticks_st_property},
    {"colors", get_colors_property},
    {"debug", GetUicontrolDebug}
};

static const size_t getEntriesCount = sizeof(getEntries) / sizeof(getEntries[0]);

struct GetMapInitializer
{
    GetMapInitializer()
    {
        getMap.reserve(getEntriesCount);
        for (auto&& entry : getEntries)
        {
            getMap[normalizeKey(entry.key)] = entry.func;
        }
    }
};

static GetMapInitializer getMapInitializer;
/*--------------------------------------------------------------------------*/
static const SetPropertyEntry setEntries[] =
{
    {"visible", set_visible_property},
    {"pixel_drawing_mode", set_pixel_drawing_mode_property},
    {"old_style", set_old_style_property},
    {"auto_resize", set_auto_resize_property},
    {"figure_position", set_figure_position_property},
    {"axes_size", set_axes_size_property},
    {"figure_size", set_figure_size_property},
    {"figure_name", set_figure_name_property},
    {"name", set_figure_name_property},
    {"figure_id", set_figure_id_property},
    {"rotation_style", set_rotation_style_property},
    {"immediate_drawing", set_immediate_drawing_property},
    {"parent", set_parent_property},
    {"current_axes", set_current_axes_property},
    {"current_figure", set_current_figure_property},
    {"current_obj", set_current_entity_property},
    {"current_entity", set_current_entity_property},
    {"hdl", set_current_entity_property},
    {"children", set_children_property},
    {"default_values", set_default_values_property},
    {"color_map", set_color_map_property},
    {"interp_color_vector", set_interp_color_vector_property},
    {"interp_color_mode", set_interp_color_mode_property},
    {"background", set_background_property},
    {"foreground", set_foreground_property},
    {"fill_mode", set_fill_mode_property},
    {"thickness", set_thickness_property},
    {"arrow_size_factor", set_arrow_size_factor_property},
    {"segs_color", set_segs_color_property},
    {"line_style", set_line_style_property},
    {"line_mode", set_line_mode_property},
    {"surface_mode", set_surface_mode_property},
    {"mark_style", set_mark_style_property},
    {"mark_mode", set_mark_mode_property},
    {"mark_size_unit", set_mark_size_unit_property},
    {"mark_size", set_mark_size_property},
    {"mark_foreground", set_mark_foreground_property},
    {"mark_background", set_mark_background_property},
    {"mark_offset", set_mark_offset_property},
    {"mark_stride", set_mark_stride_property},
    {"bar_layout", set_bar_layout_property},
    {"bar_width", set_bar_width_property},
    {"x_shift", set_x_shift_property},
    {"y_shift", set_y_shift_property},
    {"z_shift", set_z_shift_property},
    {"polyline_style", set_polyline_style_property},
    {"font_size", set_font_size_property},
    {"font_angle", set_font_angle_property},
    {"font_foreground", set_font_foreground_property},
    {"font_color", set_font_color_property},
    {"font_style", set_font_style_property},
    {"text_box_mode", set_text_box_mode_property},
    {"auto_dimensionning", set_auto_dimensionning_property},
    {"alignment", set_alignment_property},
    {"text_box", set_text_box_property},
    {"text", set_text_property},
    {"interpreter", set_interpreter_property},
    {"auto_clear", set_auto_clear_property},
    {"auto_scale", set_auto_scale_property},
    {"auto_stretch", set_auto_stretch_property},
    {"zoom_box", set_zoom_box_property},
    {"zoom_state", set_zoom_state_property},
    {"clip_box", set_clip_box_property},
    {"clip_state", set_clip_state_property},
    {"data", set_data_property},
    {"callback", set_callback_property},
    {"x_label", set_x_label_property},
    {"y_label", set_y_label_property},
    {"z_label", set_z_label_property},
    {"title", set_title_property},
    {"log_flags", set_log_flags_property},
    {"tics_direction", set_tics_direction_property},
    {"x_location", set_x_location_property},
    {"y_location", set_y_location_property},
    {"tight_limits", set_tight_limits_property},
    {"closed", set_closed_property},
    {"auto_position", set_auto_position_property},
    {"auto_rotation", set_auto_rotation_property},
    {"position", set_position_property},
    {"auto_ticks", set_auto_ticks_property},
    {"axes_reverse", set_axes_reverse_property},
    {"view", set_view_property},
    {"axes_bounds", set_axes_bounds_property},
    {"data_bounds", set_data_bounds_property},
    {"margins", set_margins_property},
    {"auto_margins", set_auto_margins_property},
    {"tics_color", set_tics_color_property},
    {"tics_style", set_tics_style_property},
    {"sub_tics", set_sub_tics_property},
    {"sub_ticks", set_sub_tics_property},
    {"tics_segment", set_tics_segment_property},
    {"labels_font_size", set_labels_font_size_property},
    {"labels_font_color", set_labels_font_color_property},
    {"labels_font_style", set_labels_font_style_property},
    {"format_n", set_format_n_property},
    {"xtics_coord", set_xtics_coord_property},
    {"ytics_coord", set_ytics_coord_property},
    {"tics_labels", set_tics_labels_property},
    {"tics_interpreters", set_tics_interpreters_property},
    {"box", set_box_property},
    {"grid", set_grid_property},
    {"grid_thickness", set_grid_thickness_property},
    {"grid_style", set_grid_style_property},
    {"axes_visible", set_axes_visible_property},
    {"hiddencolor", set_hidden_color_property},
    {"isoview", set_isoview_property},
    {"cube_scaling", set_cube_scaling_property},
    {"arrow_size", set_arrow_size_property},
    {"arrow_style", set_arrow_style_property},
    {"colored", set_colored_property},
    {"data_mapping", set_data_mapping_property},
    {"rotation_angles", set_rotation_angles_property},
    {"color_mode", set_color_mode_property},
    {"color_flag", set_color_flag_property},
    {"cdata_mapping", set_cdata_mapping_property},
    {"cdata_bounds", set_cdata_bounds_property},
    {"surface_color", set_surface_color_property},
    {"triangles", set_triangles_property},
    {"z_bounds", set_z_bounds_property},
    {"user_data", set_user_data_property},
    {"userdata", set_user_data_property},
    {"handle_visible", set_handle_visible_property},
    {"callback_type", set_callback_type_property},
    {"enable", SetUiobjectEnable},
    {"hidden_axis_color", set_hidden_axis_color_property},
    {"x_ticks", set_x_ticks_property},
    {"y_ticks", set_y_ticks_property},
    {"z_ticks", set_z_ticks_property},
    {"viewport", set_viewport_property},
    {"info_message", set_info_message_property},
    {"screen_position", set_screen_position_property},
    {"event_handler_enable", set_event_handler_enable_property},
    {"event_handler", set_event_handler_property},
    {"label", SetUimenuLabel},
    {"string", SetUicontrolString},
    {"backgroundcolor", SetUicontrolBackgroundColor},
    {"foregroundcolor", SetUiobjectForegroundColor},
    {"fontweight", SetUicontrolFontWeight},
    {"fontunits", SetUicontrolFontUnits},
    {"fontsize", SetUicontrolFontSize},
    {"fontangle", SetUicontrolFontAngle},
    {"min", SetUicontrolMin},
    {"max", SetUicontrolMax},
    {"tag", set_tag_property},
    {"listboxtop", SetUicontrolListboxTop},
    {"value", SetUicontrolValue},
    {"units", SetUicontrolUnits},
    {"relief", SetUicontrolRelief},
    {"horizontalalignment", SetUicontrolHorizontalAlignment},
    {"verticalalignment", SetUicontrolVerticalAlignment},
    {"fontname", SetUicontrolFontName},
    {"sliderstep", SetUicontrolSliderStep},
    {"snaptoticks", SetUicontrolSnapToTicks},
    {"checked", SetUimenuChecked},
    {"arc_drawing_method", set_arc_drawing_method_property},
    {"fractional_font", set_fractional_font_property},
    {"links", set_links_property},
    {"legend_location", set_legend_location_property},
    {"filled", set_filled_property},
    {"outside_colors", set_outside_colors_property},
    {"color_range", set_color_range_property},
    {"grid_position", set_grid_position_property},
    {"anti_aliasing", set_anti_aliasing_property},
    {"showhiddenhandles", SetConsoleShowHiddenHandles},
    {"showhiddenproperties", SetConsoleShowHiddenProperties},
    {"usedeprecatedskin", SetConsoleUseDeprecatedLF},
    {"resizefcn", set_figure_resizefcn_property},
    {"tooltipstring", SetUicontrolTooltipString},
    {"closerequestfcn", set_figure_closerequestfcn_property},
    {"orientation", set_tip_orientation_property},
    {"display_components", set_tip_display_components_property},
    {"datatip_display_mode", set_datatip_display_mode_property},
    {"auto_orientation", set_tip_auto_orientation_property},
    {"interp_mode", set_tip_interp_mode_property},
    {"box_mode", set_tip_box_mode_property},
    {"label_mode", set_tip_label_mode_property},
    {"display_function", set_tip_disp_function_property},
    {"detached_position", set_tip_detached_property},
    {"ambient_color", set_ambient_color_property},
    {"diffuse_color", set_diffuse_color_property},
    {"specular_color", set_specular_color_property},
    {"use_color_material", set_use_color_material_property},
    {"material_shininess", set_material_shininess_property},
    {"light_type", set_light_type_property},
    {"direction", set_direction_property},
    {"image_type", set_image_type_property},
    {"datatips", set_datatips_property},
    {"display_function_data", set_display_function_data_property},
    {"resize", set_resize_property},
    {"toolbar", set_toolbar_property},
    {"toolbar_visible", set_toolbar_visible_property},
    {"menubar", set_menubar_property},
    {"menubar_visible", set_menubar_visible_property},
    {"infobar_visible", set_infobar_visible_property},
    {"dockable", set_dockable_property},
    {"layout", set_layout_property},
    {"constraints", set_constraints_property},
    {"rect", set_rect_property},
    {"layout_options", set_layout_options_property},
    {"border", set_border_property},
    {"groupname", set_groupname_property},
    {"title_position", set_title_position_property},
    {"title_scroll", set_title_scroll_property},
    {"default_axes", set_default_axes_property},
    {"scrollable", set_scrollable_property},
    {"icon", SetUicontrolIcon},
    {"line_width", set_line_width_property},
    {"marks_count", set_marks_count_property},
    {"ticks_format", set_ticks_format_property},
    {"ticks_st", set_ticks_st_property},
    {"colors", set_colors_property},
    {"debug", SetUicontrolDebug}
};

static const size_t setEntriesCount = sizeof(setEntries) / sizeof(setEntries[0]);

struct SetMapInitializer
{
    SetMapInitializer()
    {
        setMap.reserve(setEntriesCount);
        for (auto&& entry : setEntries)
        {
            setMap[normalizeKey(entry.key)] = entry.func;
        }
    }
};

static SetMapInitializer setMapInitializer;
/*--------------------------------------------------------------------------*/
const GetPropertyEntry* getGetPropertyEntries(size_t* count)
{
    if (count)
    {
        *count = getEntriesCount;
    }
    return getEntries;
}
/*--------------------------------------------------------------------------*/
const SetPropertyEntry* getSetPropertyEntries(size_t* count)
{
    if (count)
    {
        *count = setEntriesCount;
    }
    return setEntries;
}
/*--------------------------------------------------------------------------*/
getPropertyFunc searchGetHashtable(const char* key)
{
    auto it = getMap.find(normalizeKey(key));
    if (it == getMap.end())
    {
        return NULL;
    }
    return it->second;
}
/*--------------------------------------------------------------------------*/
setPropertyFunc searchSetHashtable(const char* key)
{
    auto it = setMap.find(normalizeKey(key));
    if (it == setMap.end())
    {
        return NULL;
    }
    return it->second;
}
/*--------------------------------------------------------------------------*/
