/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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
/* file: SetHashTable.c                                                   */
/* desc : implementation of the scilab hashtable for the set procedure    */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "SetHashTable.h"
#include "setHandleProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getDictionarySetProperties.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "BOOL.h"

/**
* use for the singleton to know if the hashtable has already be created.
*/
static BOOL setHashTableCreated = FALSE;

/**
* the get hashtable
*/
static SetPropertyHashTable *setHashTable = NULL;

/**
* a couple of key (property name) and associated value
* (accessor name)
*/
typedef struct
{
    char *key;
    setPropertyFunc accessor;
} setHashTableCouple;

/**
* list of all property names and associated functions in scilab
* This is inserted in the hashTable
*/
static setHashTableCouple propertySetTable[] =
{
    {"visible", set_visible_property},
    {"pixel_drawing_mode", set_pixel_drawing_mode_property},
    {"old_style", set_old_style_property},
    {"figure_style", set_figure_style_property},
    {"auto_resize", set_auto_resize_property},
    {"figure_position", set_figure_position_property},
    {"axes_size", set_axes_size_property},
    {"figure_size", set_figure_size_property},
    {"figure_name", set_figure_name_property},
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
    {"auto_clear", set_auto_clear_property},
    {"auto_scale", set_auto_scale_property},
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
    {"box", set_box_property},
    {"grid", set_grid_property},
    {"grid_thickness", set_grid_thickness_property},
    {"grid_style", set_grid_style_property},
    {"axes_visible", set_axes_visible_property},
    {"hiddencolor", set_hidden_color_property},
    {"isoview", set_isoview_property},
    {"cube_scaling", set_cube_scaling_property},
    {"arrow_size", set_arrow_size_property},
    {"colored", set_colored_property},
    {"data_mapping", set_data_mapping_property},
    {"rotation_angles", set_rotation_angles_property},
    {"color_mode", set_color_mode_property},
    {"color_flag", set_color_flag_property},
    {"cdata_mapping", set_cdata_mapping_property},
    {"surface_color", set_surface_color_property},
    {"triangles", set_triangles_property},
    {"z_bounds", set_z_bounds_property},
    {"user_data", set_user_data_property},
    {"userdata", set_user_data_property},   /* Compatibility with TK uicontrols */
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
    {"z_component", set_tip_z_component_property},
    {"display_components", set_tip_display_components_property},
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
    {"colors", set_colors_property}
};

/*--------------------------------------------------------------------------*/
SetPropertyHashTable *createScilabSetHashTable(void)
{
    int i = 0;

    int propertyCount = sizeof(propertySetTable) / sizeof(setHashTableCouple);
    if (setHashTableCreated)
    {
        return setHashTable;
    }

    /* create the hash table */
    setHashTable = createSetHashTable();

    if (setHashTable == NULL)
    {
        return NULL;
    }

    /* insert every couple */

    for (i = 0; i < propertyCount ; i++)
    {
        insertSetHashtable(setHashTable, propertySetTable[i].key, propertySetTable[i].accessor);
    }

    setHashTableCreated = TRUE;

    return setHashTable;

}

/*--------------------------------------------------------------------------*/
int callSetProperty(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol, const char *propertyName)
{
    setPropertyFunc accessor = searchSetHashtable(setHashTable, propertyName);

    if (accessor == NULL)
    {
        Scierror(999, _("Unknown property: %s.\n"), propertyName);
        return NULL;
    }
    return accessor(_pvCtx, iObjUID, _pvData, valueType, nbRow, nbCol);
}

/*--------------------------------------------------------------------------*/
void destroyScilabSetHashTable(void)
{
    if (!setHashTableCreated)
    {
        return;
    }

    destroySetHashTable(setHashTable);
    setHashTableCreated = FALSE;
}

/*--------------------------------------------------------------------------*/
char **getDictionarySetProperties(int *sizearray)
{
    char **dictionary = NULL;
    int propertyCount = sizeof(propertySetTable) / sizeof(setHashTableCouple);

    *sizearray = 0;
    dictionary = (char **)MALLOC(sizeof(char *) * propertyCount);
    if (dictionary)
    {
        int i = 0;

        *sizearray = propertyCount;
        for (i = 0; i < propertyCount ; i++)
        {
            dictionary[i] = os_strdup(propertySetTable[i].key);
        }
    }
    return dictionary;
}

/*--------------------------------------------------------------------------*/
#undef NB_PROPERTIES
