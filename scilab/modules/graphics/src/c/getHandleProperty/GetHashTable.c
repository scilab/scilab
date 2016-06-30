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
/* file: GetHashTable.c                                                   */
/* desc : implementation of the scilab hashtable for the get procedure    */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "GetHashTable.h"
#include "getHandleProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getDictionaryGetProperties.h"
#include "sci_malloc.h"
/**
 * use for the singleton to know if the hashtable has already be created.
 */
static BOOL getHashTableCreated = FALSE;

/**
 * the get hashtable
 */
static GetPropertyHashTable *getHashTable = NULL;

/**
 * a couple of key (property name) and associated value
 * (accessor name)
 */
typedef struct
{
    char *key;
    getPropertyFunc accessor;
} getHashTableCouple;

/**
 * list of all property names and associated functions in scilab
 * This is inserted in the hashTable
 */
static getHashTableCouple propertyGetTable[] =
{
    {"figures_id", get_figures_id_property},
    {"visible", get_visible_property},
    {"pixel_drawing_mode", get_pixel_drawing_mode_property},
    {"old_style", get_old_style_property},
    {"figure_style", get_figure_style_property},
    {"auto_resize", get_auto_resize_property},
    {"figure_position", get_figure_position_property},
    {"axes_size", get_axes_size_property},
    {"figure_size", get_figure_size_property},
    {"figure_name", get_figure_name_property},
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
    {"auto_clear", get_auto_clear_property},
    {"auto_scale", get_auto_scale_property},
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
    {"box", get_box_property},
    {"grid", get_grid_property},
    {"grid_thickness", get_grid_thickness_property},
    {"grid_style", get_grid_style_property},
    {"axes_visible", get_axes_visible_property},
    {"hiddencolor", get_hidden_color_property},
    {"isoview", get_isoview_property},
    {"cube_scaling", get_cube_scaling_property},
    {"arrow_size", get_arrow_size_property},
    {"colored", get_colored_property},
    {"data_mapping", get_data_mapping_property},
    {"rotation_angles", get_rotation_angles_property},
    {"color_mode", get_color_mode_property},
    {"color_flag", get_color_flag_property},
    {"cdata_mapping", get_cdata_mapping_property},
    {"surface_color", get_surface_color_property},
    {"triangles", get_triangles_property},
    {"z_bounds", get_z_bounds_property},
    {"user_data", get_user_data_property},
    {"userdata", get_user_data_property},   /* Compatibility with TK uicontrols */
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
    {"z_component", get_tip_z_component_property},
    {"display_components", get_tip_display_components_property},
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
    {"scrollable", get_scrollable_property},
    {"icon", GetUicontrolIcon},
    {"line_width", get_line_width_property},
    {"marks_count", get_marks_count_property},
    {"ticks_format", get_ticks_format_property},
    {"ticks_st", get_ticks_st_property},
    {"colors", get_colors_property}
};

/*--------------------------------------------------------------------------*/
GetPropertyHashTable *createScilabGetHashTable(void)
{
    int i = 0;

    int propertyCount = sizeof(propertyGetTable) / sizeof(getHashTableCouple);
    if (getHashTableCreated)
    {
        /* hastable already created, return */
        return getHashTable;
    }

    /* create the hash table */
    getHashTable = createGetHashTable();

    if (getHashTable == NULL)
    {
        return NULL;
    }

    /* insert every couple */
    for (i = 0; i < propertyCount; i++)
    {
        insertGetHashtable(getHashTable, propertyGetTable[i].key, propertyGetTable[i].accessor);
    }

    getHashTableCreated = TRUE;

    return getHashTable;

}

/*--------------------------------------------------------------------------*/

void* callGetProperty(void* _pvCtx, int iObjUID, char *propertyName)
{
    getPropertyFunc accessor = searchGetHashtable(getHashTable, propertyName);

    if (accessor == NULL)
    {
        Scierror(999, _("Unknown property: %s.\n"), propertyName);
        return NULL;
    }
    return accessor(_pvCtx, iObjUID);
}

/*--------------------------------------------------------------------------*/
void destroyScilabGetHashTable(void)
{
    if (!getHashTableCreated)
    {
        return;
    }

    destroyGetHashTable(getHashTable);
    getHashTableCreated = FALSE;
}

/*--------------------------------------------------------------------------*/
char **getDictionaryGetProperties(int *sizearray)
{
    char **dictionary = NULL;
    int propertyCount = sizeof(propertyGetTable) / sizeof(getHashTableCouple);

    *sizearray = 0;

    dictionary = (char **)MALLOC(sizeof(char *) * propertyCount);
    if (dictionary)
    {
        int i = 0;

        *sizearray = propertyCount;
        for (i = 0; i < propertyCount ; i++)
        {
            char *propertyname = (char *)MALLOC(sizeof(char) * (strlen(propertyGetTable[i].key) + 1));

            if (propertyname)
            {
                strcpy(propertyname, propertyGetTable[i].key);
            }
            dictionary[i] = propertyname;
        }
    }
    return dictionary;
}
/*--------------------------------------------------------------------------*/
