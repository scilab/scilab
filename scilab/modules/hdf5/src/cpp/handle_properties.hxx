/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include <string>
#include <unordered_map>
#include <list>
#include <vector>

#define SAVE_ONLY 0
#define SAVE_LOAD 1

extern "C"
{
#include "graphicObjectProperties.h"
#include "returnType.h"
}

enum FrameBorderType
{
    NONE = 0,
    LINE,
    BEVEL,
    SOFTBEVEL,
    ETCHED,
    TITLED,
    EMPTY,
    COMPOUND,
    MATTE
};

struct Links
{
    typedef std::list<std::vector <int> > PathList;

    static void add(int legend, PathList& path)
    {
        paths[legend] = path;
    }

    static PathList get(int legend)
    {
        std::unordered_map<int, PathList>::iterator it = paths.find(legend);
        if (it == paths.end())
        {
            return PathList();
        }

        return it->second;
    }

    static void remove(int legend)
    {
        paths.erase(legend);
    }

    static int count()
    {
        return static_cast<int>(paths.size());
    }

    static std::list<int> legends()
    {
        std::list<int> legs;
        for (auto & i : paths)
        {
            legs.push_back(i.first);
        }

        return legs;
    }

private:
    static std::unordered_map<int, PathList> paths;
};

typedef std::list<std::pair<std::string, std::vector<int> > > HandleProp;

struct FigureHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("toolbar", std::vector<int>({SAVE_ONLY, __GO_TOOLBAR__, jni_int}));
        m.emplace_back("toolbar_visible", std::vector<int>({SAVE_ONLY, __GO_TOOLBAR_VISIBLE__, jni_bool}));
        m.emplace_back("menubar", std::vector<int>({SAVE_ONLY, __GO_MENUBAR__, jni_int}));
        m.emplace_back("menubar_visible", std::vector<int>({SAVE_ONLY, __GO_MENUBAR_VISIBLE__, jni_bool}));
        m.emplace_back("infobar_visible", std::vector<int>({SAVE_ONLY, __GO_INFOBAR_VISIBLE__, jni_bool}));
        m.emplace_back("dockable", std::vector<int>({SAVE_ONLY, __GO_DOCKABLE__, jni_bool}));
        m.emplace_back("default_axes", std::vector<int>({SAVE_ONLY, __GO_DEFAULT_AXES__, jni_bool}));

        //@start
        m.emplace_back("figure_position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_int_vector, -1, -2}));
        m.emplace_back("auto_resize", std::vector<int>({SAVE_LOAD, __GO_AUTORESIZE__, jni_bool}));
        m.emplace_back("figure_size", std::vector<int>({SAVE_LOAD, __GO_SIZE__, jni_int_vector, -1, -2}));
        m.emplace_back("axes_size", std::vector<int>({SAVE_LOAD, __GO_AXES_SIZE__, jni_int_vector, -1, -2}));

        //@ don't care
        m.emplace_back("viewport", std::vector<int>({SAVE_LOAD, __GO_VIEWPORT__, jni_int_vector, -1, -2}));
        m.emplace_back("info_message", std::vector<int>({SAVE_LOAD, __GO_INFO_MESSAGE__, jni_string}));
        m.emplace_back("figure_name", std::vector<int>({SAVE_LOAD, __GO_NAME__, jni_string}));
        m.emplace_back("color_map", std::vector<int>({SAVE_LOAD, __GO_COLORMAP__, jni_double_vector, __GO_COLORMAP_SIZE__, -3}));
        m.emplace_back("pixel_drawing_mode", std::vector<int>({SAVE_LOAD, __GO_PIXEL_DRAWING_MODE__, jni_int}));
        m.emplace_back("anti_aliasing", std::vector<int>({SAVE_LOAD, __GO_ANTIALIASING__, jni_int}));
        m.emplace_back("immediate_drawing", std::vector<int>({SAVE_LOAD, __GO_IMMEDIATE_DRAWING__, jni_bool}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("rotation_style", std::vector<int>({SAVE_LOAD, __GO_ROTATION_TYPE__, jni_int}));
        m.emplace_back("event_handler_enable", std::vector<int>({SAVE_LOAD, __GO_EVENTHANDLER_ENABLE__, jni_bool}));
        m.emplace_back("closerequestfcn", std::vector<int>({SAVE_LOAD, __GO_CLOSEREQUESTFCN__, jni_string}));
        m.emplace_back("resize", std::vector<int>({SAVE_LOAD, __GO_RESIZE__, jni_bool}));
        m.emplace_back("layout", std::vector<int>({SAVE_LOAD, __GO_LAYOUT__, jni_int}));
        m.emplace_back("icon", std::vector<int>({SAVE_LOAD, __GO_UI_ICON__, jni_string}));

        //@end
        m.emplace_back("event_handler", std::vector<int>({SAVE_LOAD, __GO_EVENTHANDLER_NAME__, jni_string}));
        m.emplace_back("resizefcn", std::vector<int>({SAVE_LOAD, __GO_RESIZEFCN__, jni_string}));
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct AxesHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("visible", std::vector<int>({SAVE_ONLY, __GO_VISIBLE__, jni_bool}));

        //@start

        //@ don't care
        m.emplace_back("axes_visible_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_VISIBLE__, jni_bool}));
        m.emplace_back("axes_visible_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_VISIBLE__, jni_bool}));
        m.emplace_back("axes_visible_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_VISIBLE__, jni_bool}));
        m.emplace_back("axes_reverse_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_REVERSE__, jni_bool}));
        m.emplace_back("axes_reverse_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_REVERSE__, jni_bool}));
        m.emplace_back("axes_reverse_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_REVERSE__, jni_bool}));
        m.emplace_back("grid_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_GRID_COLOR__, jni_int}));
        m.emplace_back("grid_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_GRID_COLOR__, jni_int}));
        m.emplace_back("grid_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_GRID_COLOR__, jni_int}));
        m.emplace_back("grid_position", std::vector<int>({SAVE_LOAD, __GO_GRID_POSITION__, jni_int}));
        m.emplace_back("grid_thickness_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_GRID_THICKNESS__, jni_double}));
        m.emplace_back("grid_thickness_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_GRID_THICKNESS__, jni_double}));
        m.emplace_back("grid_thickness_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_GRID_THICKNESS__, jni_double}));
        m.emplace_back("grid_style_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_GRID_STYLE__, jni_int}));
        m.emplace_back("grid_style_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_GRID_STYLE__, jni_int}));
        m.emplace_back("grid_style_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_GRID_STYLE__, jni_int}));
        m.emplace_back("x_location", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_LOCATION__, jni_int}));
        m.emplace_back("y_location", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_LOCATION__, jni_int}));
        m.emplace_back("view", std::vector<int>({SAVE_LOAD, __GO_VIEW__, jni_int}));
        m.emplace_back("ticks_format_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_FORMAT__, jni_string}));
        m.emplace_back("ticks_format_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_FORMAT__, jni_string}));
        m.emplace_back("ticks_format_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_FORMAT__, jni_string}));
        m.emplace_back("ticks_st_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_ST_FACTORS__, jni_double_vector, -1, -2}));
        m.emplace_back("ticks_st_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_ST_FACTORS__, jni_double_vector, -1, -2}));
        m.emplace_back("ticks_st_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_ST_FACTORS__, jni_double_vector, -1, -2}));
        m.emplace_back("box", std::vector<int>({SAVE_LOAD, __GO_BOX_TYPE__, jni_int}));
        m.emplace_back("filled", std::vector<int>({SAVE_LOAD, __GO_FILLED__, jni_bool}));
        m.emplace_back("sub_tics_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_SUBTICKS__, jni_int}));
        m.emplace_back("sub_tics_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_SUBTICKS__, jni_int}));
        m.emplace_back("sub_tics_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_SUBTICKS__, jni_int}));
        m.emplace_back("font_style", std::vector<int>({SAVE_LOAD, __GO_FONT_STYLE__, jni_int}));
        m.emplace_back("font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("font_color", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));
        m.emplace_back("isoview", std::vector<int>({SAVE_LOAD, __GO_ISOVIEW__, jni_bool}));
        m.emplace_back("cube_scaling", std::vector<int>({SAVE_LOAD, __GO_CUBE_SCALING__, jni_bool}));
        m.emplace_back("rotation_angles", std::vector<int>({SAVE_LOAD, __GO_ROTATION_ANGLES__, jni_double_vector, -1, -2}));
        m.emplace_back("tight_limits_x", std::vector<int>({SAVE_LOAD, __GO_X_TIGHT_LIMITS__, jni_bool}));
        m.emplace_back("tight_limits_y", std::vector<int>({SAVE_LOAD, __GO_Y_TIGHT_LIMITS__, jni_bool}));
        m.emplace_back("tight_limits_z", std::vector<int>({SAVE_LOAD, __GO_Z_TIGHT_LIMITS__, jni_bool}));
        m.emplace_back("data_bounds", std::vector<int>({SAVE_LOAD, __GO_DATA_BOUNDS__, jni_double_vector, -2, -3}));
        m.emplace_back("zoom_box", std::vector<int>({SAVE_LOAD, __GO_ZOOM_BOX__, jni_double_vector, -1, -6}));
        m.emplace_back("margins", std::vector<int>({SAVE_LOAD, __GO_MARGINS__, jni_double_vector, -1, -4}));
        m.emplace_back("auto_margins", std::vector<int>({SAVE_LOAD, __GO_AUTO_MARGINS__, jni_bool}));
        m.emplace_back("axes_bounds", std::vector<int>({SAVE_LOAD, __GO_AXES_BOUNDS__, jni_double_vector, -1, -4}));
        m.emplace_back("auto_clear", std::vector<int>({SAVE_LOAD, __GO_AUTO_CLEAR__, jni_bool}));
        m.emplace_back("auto_scale", std::vector<int>({SAVE_LOAD, __GO_AUTO_SCALE__, jni_bool}));
        m.emplace_back("hidden_axis_color", std::vector<int>({SAVE_LOAD, __GO_HIDDEN_AXIS_COLOR__, jni_int}));
        m.emplace_back("arc_drawing_method", std::vector<int>({SAVE_LOAD, __GO_ARC_DRAWING_METHOD__, jni_int}));
        m.emplace_back("hiddencolor", std::vector<int>({SAVE_LOAD, __GO_HIDDEN_COLOR__, jni_int}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));

        m.emplace_back("ticks_locations_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_TICKS_LOCATIONS__, jni_double_vector, -1, __GO_X_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_labels_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_TICKS_LABELS__, jni_string_vector, -1, __GO_X_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_locations_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_TICKS_LOCATIONS__, jni_double_vector, -1, __GO_Y_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_labels_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_TICKS_LABELS__, jni_string_vector, -1, __GO_Y_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_locations_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_TICKS_LOCATIONS__, jni_double_vector, -1, __GO_Z_AXIS_NUMBER_TICKS__}));
        m.emplace_back("ticks_labels_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_TICKS_LABELS__, jni_string_vector, -1, __GO_Z_AXIS_NUMBER_TICKS__}));

        //@end

        //must be set after data_bounds
        m.emplace_back("log_flags_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_LOG_FLAG__, jni_bool}));
        m.emplace_back("log_flags_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_LOG_FLAG__, jni_bool}));
        m.emplace_back("log_flags_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_LOG_FLAG__, jni_bool}));

        //must be set mark_style
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));

        //must be set after ticks labels
        m.emplace_back("auto_ticks_x", std::vector<int>({SAVE_LOAD, __GO_X_AXIS_AUTO_TICKS__, jni_bool}));
        m.emplace_back("auto_ticks_y", std::vector<int>({SAVE_LOAD, __GO_Y_AXIS_AUTO_TICKS__, jni_bool}));
        m.emplace_back("auto_ticks_z", std::vector<int>({SAVE_LOAD, __GO_Z_AXIS_AUTO_TICKS__, jni_bool}));

        return m;
    }
};

struct LabelHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_double_vector, -1, -3}));
        m.emplace_back("font_foreground", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("font_style", std::vector<int>({SAVE_LOAD, __GO_FONT_STYLE__, jni_int}));
        m.emplace_back("font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));
        m.emplace_back("font_angle", std::vector<int>({SAVE_LOAD, __GO_FONT_ANGLE__, jni_double}));
        m.emplace_back("auto_rotation", std::vector<int>({SAVE_LOAD, __GO_AUTO_ROTATION__, jni_bool}));
        m.emplace_back("auto_position", std::vector<int>({SAVE_LOAD, __GO_AUTO_POSITION__, jni_bool}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));

        return m;
    }
};

struct ChampHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@ don't care
        m.emplace_back("arrow_size", std::vector<int>({SAVE_LOAD, __GO_ARROW_SIZE__, jni_double}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("colored", std::vector<int>({SAVE_LOAD, __GO_COLORED__, jni_bool}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct SurfaceHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("surface_mode", std::vector<int>({SAVE_LOAD, __GO_SURFACE_MODE__, jni_bool}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("color_mode", std::vector<int>({SAVE_LOAD, __GO_COLOR_MODE__, jni_int}));
        m.emplace_back("color_flag", std::vector<int>({SAVE_LOAD, __GO_COLOR_FLAG__, jni_int}));
        m.emplace_back("ambient_color", std::vector<int>({SAVE_LOAD, __GO_AMBIENTCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("diffuse_color", std::vector<int>({SAVE_LOAD, __GO_DIFFUSECOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("specular_color", std::vector<int>({SAVE_LOAD, __GO_SPECULARCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("use_color_material", std::vector<int>({SAVE_LOAD, __GO_COLOR_MATERIAL__, jni_bool}));
        m.emplace_back("material_shininess", std::vector<int>({SAVE_LOAD, __GO_MATERIAL_SHININESS__, jni_double}));
        m.emplace_back("hiddencolor", std::vector<int>({SAVE_LOAD, __GO_HIDDEN_COLOR__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //only for Fac3d
        //m.emplace_back("cdata_mapping", std::vector<int>({SAVE_LOAD, __GO_COLORED__, jni_bool}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct PolylineHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("display_function", std::vector<int>({SAVE_LOAD, __GO_DATATIP_DISPLAY_FNC__, jni_string}));
        m.emplace_back("display_function_data", std::vector<int>({SAVE_LOAD, __GO_DATA_MODEL_DISPLAY_FUNCTION__, jni_int_vector, -1, __GO_DATA_MODEL_DISPLAY_FUNCTION_SIZE__}));
        m.emplace_back("closed", std::vector<int>({SAVE_LOAD, __GO_CLOSED__, jni_bool}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("arrow_size_factor", std::vector<int>({SAVE_LOAD, __GO_ARROW_SIZE_FACTOR__, jni_double}));
        m.emplace_back("polyline_style", std::vector<int>({SAVE_LOAD, __GO_POLYLINE_STYLE__, jni_int}));
        m.emplace_back("interp_color_mode", std::vector<int>({SAVE_LOAD, __GO_INTERP_COLOR_MODE__, jni_bool}));
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("mark_offset", std::vector<int>({SAVE_LOAD, __GO_MARK_OFFSET__, jni_int}));
        m.emplace_back("mark_stride", std::vector<int>({SAVE_LOAD, __GO_MARK_STRIDE__, jni_int}));
        m.emplace_back("bar_width", std::vector<int>({SAVE_LOAD, __GO_BAR_WIDTH__, jni_double}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct CompoundHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        //@start

        //@ don't care

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct DatatipHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("data_index", std::vector<int>({SAVE_ONLY, __GO_DATATIP_INDEXES__, jni_int}));

        //@start
        m.emplace_back("box_mode", std::vector<int>({SAVE_LOAD, __GO_DATATIP_BOX_MODE__, jni_bool}));
        m.emplace_back("label_mode", std::vector<int>({SAVE_LOAD, __GO_DATATIP_LABEL_MODE__, jni_bool}));
        m.emplace_back("orientation", std::vector<int>({SAVE_LOAD, __GO_DATATIP_ORIENTATION__, jni_int}));
        m.emplace_back("display_components", std::vector<int>({SAVE_LOAD, __GO_DATATIP_DISPLAY_COMPONENTS__, jni_string}));
        m.emplace_back("auto_orientation", std::vector<int>({SAVE_LOAD, __GO_DATATIP_AUTOORIENTATION__, jni_bool}));
        m.emplace_back("interp_mode", std::vector<int>({SAVE_LOAD, __GO_DATATIP_INTERP_MODE__, jni_bool}));
        m.emplace_back("display_function", std::vector<int>({SAVE_LOAD, __GO_DATATIP_DISPLAY_FNC__, jni_string}));
        m.emplace_back("font_foreground", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("detached_position", std::vector<int>({SAVE_LOAD, __GO_DATATIP_DETACHED_POSITION__, jni_double_vector, -1, -3}));
        m.emplace_back("detached_mode", std::vector<int>({SAVE_LOAD, __GO_DATATIP_DETACHED_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));

        //@ don't care

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct RectangleHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("upper_left_point", std::vector<int>({SAVE_LOAD, __GO_UPPER_LEFT_POINT__, jni_double_vector, -1, -3}));
        m.emplace_back("width", std::vector<int>({SAVE_LOAD, __GO_WIDTH__, jni_double}));
        m.emplace_back("height", std::vector<int>({SAVE_LOAD, __GO_HEIGHT__, jni_double}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct ArcHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("upper_left_point", std::vector<int>({SAVE_LOAD, __GO_UPPER_LEFT_POINT__, jni_double_vector, -1, -3}));
        m.emplace_back("width", std::vector<int>({SAVE_LOAD, __GO_WIDTH__, jni_double}));
        m.emplace_back("height", std::vector<int>({SAVE_LOAD, __GO_HEIGHT__, jni_double}));
        m.emplace_back("start", std::vector<int>({SAVE_LOAD, __GO_START_ANGLE__, jni_double}));
        m.emplace_back("end", std::vector<int>({SAVE_LOAD, __GO_END_ANGLE__, jni_double}));
        m.emplace_back("arc_drawing_method", std::vector<int>({SAVE_LOAD, __GO_ARC_DRAWING_METHOD__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct SegsHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("number_arrows", std::vector<int>({SAVE_LOAD, __GO_NUMBER_ARROWS__, jni_int}));
        m.emplace_back("segs_color", std::vector<int>({SAVE_LOAD, __GO_SEGS_COLORS__, jni_int_vector, -1, __GO_NUMBER_ARROWS__}));
        m.emplace_back("base", std::vector<int>({SAVE_LOAD, __GO_BASE__, jni_double_vector, -3, __GO_NUMBER_ARROWS__}));
        m.emplace_back("direction", std::vector<int>({SAVE_LOAD, __GO_DIRECTION__, jni_double_vector, -3, __GO_NUMBER_ARROWS__}));
        m.emplace_back("arrow_size", std::vector<int>({SAVE_LOAD, __GO_ARROW_SIZE__, jni_double}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("line_style", std::vector<int>({SAVE_LOAD, __GO_LINE_STYLE__, jni_int}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("mark_style", std::vector<int>({SAVE_LOAD, __GO_MARK_STYLE__, jni_int}));
        m.emplace_back("mark_mode", std::vector<int>({SAVE_LOAD, __GO_MARK_MODE__, jni_bool}));
        m.emplace_back("mark_size", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE__, jni_int}));
        m.emplace_back("mark_size_unit", std::vector<int>({SAVE_LOAD, __GO_MARK_SIZE_UNIT__, jni_int}));
        m.emplace_back("mark_foreground", std::vector<int>({SAVE_LOAD, __GO_MARK_FOREGROUND__, jni_int}));
        m.emplace_back("mark_background", std::vector<int>({SAVE_LOAD, __GO_MARK_BACKGROUND__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));
        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct GrayplotHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("data_mapping", std::vector<int>({SAVE_LOAD, __GO_DATA_MAPPING__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct MatplotHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("rect", std::vector<int>({SAVE_ONLY, __GO_DATA_MODEL_MATPLOT_BOUNDS__, jni_double_vector, -1, -4}));

        //@start

        //@ don't care
        m.emplace_back("image_type", std::vector<int>({SAVE_LOAD, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int}));
        m.emplace_back("data_type", std::vector<int>({SAVE_LOAD, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int}));
        m.emplace_back("data_order", std::vector<int>({SAVE_LOAD, __GO_DATA_MODEL_MATPLOT_DATA_ORDER__, jni_int}));
        m.emplace_back("num_x", std::vector<int>({SAVE_LOAD, __GO_DATA_MODEL_NUM_X__, jni_int}));
        m.emplace_back("num_y", std::vector<int>({SAVE_LOAD, __GO_DATA_MODEL_NUM_Y__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct FecHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("coords", std::vector<int>({SAVE_ONLY, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, -3, __GO_DATA_MODEL_NUM_VERTICES__}));
        m.emplace_back("values", std::vector<int>({SAVE_ONLY, __GO_DATA_MODEL_VALUES__, jni_double_vector, -1, __GO_DATA_MODEL_NUM_VERTICES__}));

        //@start

        //@ don't care
        m.emplace_back("z_bounds", std::vector<int>({SAVE_LOAD, __GO_Z_BOUNDS__, jni_double_vector, -1, -2}));
        m.emplace_back("color_range", std::vector<int>({SAVE_LOAD, __GO_COLOR_RANGE__, jni_int_vector, -1, -2}));
        m.emplace_back("outside_colors", std::vector<int>({SAVE_LOAD, __GO_OUTSIDE_COLOR__, jni_int_vector, -1, -2}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct LegendHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("font_style", std::vector<int>({SAVE_LOAD, __GO_FONT_STYLE__, jni_int}));
        m.emplace_back("font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("font_color", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));

        //links recurtion to find axes
        m.emplace_back("legend_location", std::vector<int>({SAVE_LOAD, __GO_LEGEND_LOCATION__, jni_int}));
        m.emplace_back("position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_double_vector, -1, -2}));
        m.emplace_back("line_width", std::vector<int>({SAVE_LOAD, __GO_LINE_WIDTH__, jni_double}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("thickness", std::vector<int>({SAVE_LOAD, __GO_LINE_THICKNESS__, jni_double}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("marks_count", std::vector<int>({SAVE_LOAD, __GO_MARKS_COUNT__, jni_int}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct TextHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_double_vector, -1, -3}));
        m.emplace_back("text_box", std::vector<int>({SAVE_LOAD, __GO_TEXT_BOX__, jni_double_vector, -1, -2}));
        m.emplace_back("text_box_mode", std::vector<int>({SAVE_LOAD, __GO_TEXT_BOX_MODE__, jni_int}));
        m.emplace_back("foreground", std::vector<int>({SAVE_LOAD, __GO_LINE_COLOR__, jni_int}));
        m.emplace_back("font_style", std::vector<int>({SAVE_LOAD, __GO_FONT_STYLE__, jni_int}));
        m.emplace_back("font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("font_angle", std::vector<int>({SAVE_LOAD, __GO_FONT_ANGLE__, jni_double}));
        m.emplace_back("box", std::vector<int>({SAVE_LOAD, __GO_BOX__, jni_bool}));
        m.emplace_back("line_mode", std::vector<int>({SAVE_LOAD, __GO_LINE_MODE__, jni_bool}));
        m.emplace_back("fill_mode", std::vector<int>({SAVE_LOAD, __GO_FILL_MODE__, jni_bool}));
        m.emplace_back("font_foreground", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("background", std::vector<int>({SAVE_LOAD, __GO_BACKGROUND__, jni_int}));
        m.emplace_back("alignment", std::vector<int>({SAVE_LOAD, __GO_ALIGNMENT__, jni_int}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct AxisHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("tics_direction", std::vector<int>({SAVE_LOAD, __GO_TICKS_DIRECTION__, jni_int}));
        m.emplace_back("xtics_coord", std::vector<int>({SAVE_LOAD, __GO_X_TICKS_COORDS__, jni_double_vector, -1, __GO_X_NUMBER_TICKS__}));
        m.emplace_back("ytics_coord", std::vector<int>({SAVE_LOAD, __GO_Y_TICKS_COORDS__, jni_double_vector, -1, __GO_Y_NUMBER_TICKS__}));
        m.emplace_back("tics_color", std::vector<int>({SAVE_LOAD, __GO_TICKS_COLOR__, jni_int}));
        m.emplace_back("tics_segment", std::vector<int>({SAVE_LOAD, __GO_TICKS_SEGMENT__, jni_bool}));
        m.emplace_back("tics_style", std::vector<int>({SAVE_LOAD, __GO_TICKS_STYLE__, jni_int}));
        m.emplace_back("sub_tics", std::vector<int>({SAVE_LOAD, __GO_SUBTICKS__, jni_int}));
        m.emplace_back("tics_labels", std::vector<int>({SAVE_LOAD, __GO_TICKS_LABELS__, jni_string_vector, -1, __GO_NUMBER_TICKS_LABELS__}));
        m.emplace_back("labels_font_size", std::vector<int>({SAVE_LOAD, __GO_FONT_SIZE__, jni_double}));
        m.emplace_back("labels_font_color", std::vector<int>({SAVE_LOAD, __GO_FONT_COLOR__, jni_int}));
        m.emplace_back("fractional_font", std::vector<int>({SAVE_LOAD, __GO_FONT_FRACTIONAL__, jni_bool}));
        m.emplace_back("clip_box", std::vector<int>({SAVE_LOAD, __GO_CLIP_BOX__, jni_double_vector, -1, -4}));
        m.emplace_back("clip_state", std::vector<int>({SAVE_LOAD, __GO_CLIP_STATE__, jni_int}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct LightHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("light_type", std::vector<int>({SAVE_LOAD, __GO_LIGHT_TYPE__, jni_int}));
        m.emplace_back("position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_double_vector, -1, -3}));
        m.emplace_back("direction", std::vector<int>({SAVE_LOAD, __GO_DIRECTION__, jni_double_vector, -1, -3}));
        m.emplace_back("ambient_color", std::vector<int>({SAVE_LOAD, __GO_AMBIENTCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("diffuse_color", std::vector<int>({SAVE_LOAD, __GO_DIFFUSECOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("specular_color", std::vector<int>({SAVE_LOAD, __GO_SPECULARCOLOR__, jni_double_vector, -1, -3}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct UimenuHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));

        //@start

        //@ don't care
        m.emplace_back("enable", std::vector<int>({SAVE_LOAD, __GO_UI_ENABLE__, jni_bool}));
        m.emplace_back("foregroundcolor", std::vector<int>({SAVE_LOAD, __GO_UI_FOREGROUNDCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("label", std::vector<int>({SAVE_LOAD, __GO_UI_LABEL__, jni_string}));
        m.emplace_back("callback", std::vector<int>({SAVE_LOAD, __GO_CALLBACK__, jni_string}));
        m.emplace_back("callback_type", std::vector<int>({SAVE_LOAD, __GO_CALLBACKTYPE__, jni_int}));
        m.emplace_back("checked", std::vector<int>({SAVE_LOAD, __GO_UI_CHECKED__, jni_bool}));
        m.emplace_back("icon", std::vector<int>({SAVE_LOAD, __GO_UI_ICON__, jni_string}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

struct UicontextmenuHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        return m;
    }
};

struct UicontrolHandle
{
    static HandleProp getPropertyList()
    {
        HandleProp m;

        //SAVE_ONLY, so don't care of reload ordering
        m.emplace_back("type", std::vector<int>({SAVE_ONLY, __GO_TYPE__, jni_int}));
        m.emplace_back("style", std::vector<int>({SAVE_ONLY, __GO_STYLE__, jni_int}));
        m.emplace_back("scrollable", std::vector<int>({SAVE_LOAD, __GO_UI_SCROLLABLE__, jni_bool}));
        m.emplace_back("margins", std::vector<int>({SAVE_LOAD, __GO_MARGINS__, jni_double_vector, -1, -4}));
        //constraint
        m.emplace_back("border_position", std::vector<int>({SAVE_ONLY, __GO_UI_BORDER_POSITION__, jni_int}));
        m.emplace_back("border_size", std::vector<int>({SAVE_ONLY, __GO_UI_BORDER_PREFERREDSIZE__, jni_int_vector, -1, -2}));
        m.emplace_back("gridbad_grid", std::vector<int>({SAVE_ONLY, __GO_UI_GRIDBAG_GRID__, jni_int_vector, -1, -4}));
        m.emplace_back("gridbad_weight", std::vector<int>({SAVE_ONLY, __GO_UI_GRIDBAG_WEIGHT__, jni_double_vector, -1, -2}));
        m.emplace_back("gridbad_fill", std::vector<int>({SAVE_ONLY, __GO_UI_GRIDBAG_FILL__, jni_int}));
        m.emplace_back("gridbad_anchor", std::vector<int>({SAVE_ONLY, __GO_UI_GRIDBAG_ANCHOR__, jni_int}));
        m.emplace_back("gridbad_padding", std::vector<int>({SAVE_ONLY, __GO_UI_GRIDBAG_PADDING__, jni_int_vector, -1, -2}));
        m.emplace_back("gridbad_size", std::vector<int>({SAVE_ONLY, __GO_UI_GRIDBAG_PREFERREDSIZE__, jni_int_vector, -1, -2}));
        m.emplace_back("value", std::vector<int>({SAVE_ONLY, __GO_UI_VALUE__, jni_double_vector, -1, __GO_UI_VALUE_SIZE__}));

        //@start

        //@ don't care
        m.emplace_back("backgroundcolor", std::vector<int>({SAVE_LOAD, __GO_UI_BACKGROUNDCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("enable", std::vector<int>({SAVE_LOAD, __GO_UI_ENABLE__, jni_bool}));
        m.emplace_back("fontangle", std::vector<int>({SAVE_LOAD, __GO_UI_FONTANGLE__, jni_string}));
        m.emplace_back("fontname", std::vector<int>({SAVE_LOAD, __GO_UI_FONTNAME__, jni_string}));
        m.emplace_back("fontsize", std::vector<int>({SAVE_LOAD, __GO_UI_FONTSIZE__, jni_double}));
        m.emplace_back("fontunits", std::vector<int>({SAVE_LOAD, __GO_UI_FONTUNITS__, jni_string}));
        m.emplace_back("fontweight", std::vector<int>({SAVE_LOAD, __GO_UI_FONTWEIGHT__, jni_string}));
        m.emplace_back("foregroundcolor", std::vector<int>({SAVE_LOAD, __GO_UI_FOREGROUNDCOLOR__, jni_double_vector, -1, -3}));
        m.emplace_back("horizontalalignment", std::vector<int>({SAVE_LOAD, __GO_UI_HORIZONTALALIGNMENT__, jni_string}));
        m.emplace_back("listboxtop", std::vector<int>({SAVE_LOAD, __GO_UI_LISTBOXTOP__, jni_int_vector, -1, __GO_UI_LISTBOXTOP_SIZE__}));
        m.emplace_back("max", std::vector<int>({SAVE_LOAD, __GO_UI_MAX__, jni_double}));
        m.emplace_back("min", std::vector<int>({SAVE_LOAD, __GO_UI_MIN__, jni_double}));
        m.emplace_back("position", std::vector<int>({SAVE_LOAD, __GO_POSITION__, jni_double_vector, -1, -4}));
        m.emplace_back("relief", std::vector<int>({SAVE_LOAD, __GO_UI_RELIEF__, jni_string}));
        m.emplace_back("sliderstep", std::vector<int>({SAVE_LOAD, __GO_UI_SLIDERSTEP__, jni_double_vector, -1, -2}));
        m.emplace_back("tooltipstring", std::vector<int>({SAVE_LOAD, __GO_UI_TOOLTIPSTRING__, jni_string_vector, -1, __GO_UI_TOOLTIPSTRING_SIZE__}));
        m.emplace_back("units", std::vector<int>({SAVE_LOAD, __GO_UI_UNITS__, jni_string}));
        m.emplace_back("verticalalignment", std::vector<int>({SAVE_LOAD, __GO_UI_VERTICALALIGNMENT__, jni_string}));
        m.emplace_back("callback", std::vector<int>({SAVE_LOAD, __GO_CALLBACK__, jni_string}));
        m.emplace_back("callback_type", std::vector<int>({SAVE_LOAD, __GO_CALLBACKTYPE__, jni_int}));
        m.emplace_back("layout", std::vector<int>({SAVE_LOAD, __GO_LAYOUT__, jni_int}));
        //layout_options
        m.emplace_back("layout_grid", std::vector<int>({SAVE_LOAD, __GO_GRID_OPT_GRID__, jni_int_vector, -1, -2}));
        m.emplace_back("layout_padding", std::vector<int>({SAVE_LOAD, __GO_GRID_OPT_PADDING__, jni_int_vector, -1, -2}));
        m.emplace_back("border_padding", std::vector<int>({SAVE_LOAD, __GO_BORDER_OPT_PADDING__, jni_int_vector, -1, -2}));

        m.emplace_back("groupname", std::vector<int>({SAVE_LOAD, __GO_UI_GROUP_NAME__, jni_string}));
        m.emplace_back("icon", std::vector<int>({SAVE_LOAD, __GO_UI_ICON__, jni_string}));

        //@end
        m.emplace_back("visible", std::vector<int>({SAVE_LOAD, __GO_VISIBLE__, jni_bool}));
        return m;
    }
};

void update_link_path(int legend, Links::PathList& paths);
int import_handle(int dataset, int parent);
bool export_handle(int parent, const std::string& name, int uid);
int add_current_entity(int handle);
