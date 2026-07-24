// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function fields = %h_fieldnames(h)
    fields = [];
    if is_handle_valid(h)
        showHiddenProperties = get(get(0), "ShowHiddenProperties") == "on";
        select (h.type)
        case "Polyline"
            fields = [
              "parent"
                "children"
                "datatips"
                "datatip_display_mode"
                "display_function"
                "display_function_data"
                "visible"
                "data"
                "closed"
                "line_mode"
                "fill_mode"
                "line_style"
                "thickness"
                "arrow_size_factor"
                "polyline_style"
                "foreground"
                "background"
                "interp_color_vector"
                "interp_color_mode"
                "colors"
                "mark_mode"
                "mark_style"
                "mark_size_unit"
                "mark_size"
                "mark_foreground"
                "mark_background"
                "mark_offset"
                "mark_stride"
                "x_shift"
                "y_shift"
                "z_shift"
                "bar_width"
                "clip_state"
                "clip_box"
                "user_data"
                "tag"
            ]
        case "Compound"
            fields = [
            "parent"
            "children"
            "visible"
            "user_data"
            "tag"
            ]
        case "Axes"
            fields = [
            "parent"
            "children"
            " "
            "visible"
            "axes_visible"
            "axes_reverse"
            "grid"
            "grid_position"
            "grid_thickness"
            "grid_style"
            "x_location"
            "y_location"
            "title"
            "x_label"
            "y_label"
            "z_label"
            "auto_ticks"
            "x_ticks"
            "y_ticks"
            "z_ticks"
            "ticks_format"
            "ticks_st"
            "box"
            "filled"
            "sub_ticks"
            "font_style"
            "font_size"
            "font_color"
            "fractional_font"
            " "
            "isoview"
            "cube_scaling"
            "view"
            "rotation_angles"
            "log_flags"
            "tight_limits"
            "data_bounds"
            "zoom_box"
            "margins"
            "auto_margins"
            "axes_bounds"
            " "
            "auto_clear"
            "auto_scale"
            "auto_stretch"
            " "
            "color_map"
            "hidden_axis_color"
            "hiddencolor"
            "line_mode"
            "line_style"
            "thickness"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "foreground"
            "background"
            "arc_drawing_method"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Legend"
            fields=[
            "parent"
            "children"
            "visible"
            "text"
            "interpreter"
            "font_style"
            "font_size"
            "font_color"
            "fractional_font"
            "links"
            "legend_location"
            "position"
            "line_width"
            "line_mode"
            "thickness"
            "foreground"
            "fill_mode"
            "background"
            "marks_count"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Rectangle"
            fields=[
            "parent"
            "children"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "line_mode"
            "fill_mode"
            "line_style"
            "thickness"
            "foreground"
            "background"
            "data"
            "visible"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Arc"
            fields=[
            "parent"
            "children"
            "thickness"
            "line_style"
            "line_mode"
            "fill_mode"
            "foreground"
            "background"
            "data"
            "visible"
            "arc_drawing_method"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Figure"
            fields=[
            "children"
            "figure_position"
            "figure_size"
            "axes_size"
            "auto_resize"
            "viewport"
            "figure_name"
            "figure_id"
            "info_message"
            "color_map"
            "pixel_drawing_mode"
            "anti_aliasing"
            "immediate_drawing"
            "background"
            "visible"
            "rotation_style"
            "event_handler"
            "event_handler_enable"
            "user_data"
            "resizefcn"
            "closerequestfcn"
            "resize"
            "toolbar"
            "toolbar_visible"
            "menubar"
            "menubar_visible"
            "infobar_visible"
            "dockable"
            "layout"
            "layout_options"
            "default_axes"
            "icon"
            "tag"
            ]
        case "Grayplot"
            fields=[
            "parent"
            "children"
            "visible"
            "data"
            "data_mapping"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Matplot"
            fields=[
            "parent"
            "children"
            "visible"
            "data"
            "rect"
            "image_type"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Fec"
            fields=[
            "parent"
            "children"
            "visible"
            "data"
            "triangles"
            "z_bounds"
            "color_range"
            "outside_colors"
            "line_mode"
            "foreground"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Segs"
            fields=[
            "parent"
            "children"
            "visible"
            "data"
            "line_mode"
            "line_style"
            "thickness"
            "arrow_size"
            "segs_color"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Champ"
            fields=[
            "parent"
            "children"
            "visible"
            "data"
            "line_style"
            "thickness"
            "colored"
            "arrow_size"
            "arrow_style"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Text"
            fields=[
            "parent"
            "children"
            "visible"
            "text"
            "interpreter"
            "alignment"
            "data"
            "box"
            "line_mode"
            "fill_mode"
            "text_box"
            "text_box_mode"
            "font_foreground"
            "foreground"
            "background"
            "font_style"
            "font_size"
            "fractional_font"
            "auto_dimensionning"
            "font_angle"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Datatip"
            fields=[
            "parent"
            "children"
            "visible"
            "interp_mode"
            "auto_orientation"
            "orientation"
            "label_mode"
            "data"
            "display_components"
            "display_function"
            "text"
            "interpreter"
            "font_foreground"
            "font_style"
            "font_size"
            "box_mode"
            "detached_position"
            "line_style"
            "foreground"
            "background"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "user_data"
            "tag"
            ]
        case "Label"
            fields=[
            "parent"
            "visible"
            "text"
            "interpreter"
            "font_foreground"
            "foreground"
            "background"
            "fill_mode"
            "font_style"
            "font_size"
            "fractional_font"
            "font_angle"
            "auto_position"
            "position"
            "auto_rotation"
            "user_data"
            "tag"
            ]
        case "Plot3d"
            fields=[
            "parent"
            "children"
            "visible"
            "surface_mode"
            "foreground"
            "thickness"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "data"
            "color_mode"
            "color_flag"
            "hiddencolor"
            "clip_state"
            "clip_box"
            "ambient_color"
            "diffuse_color"
            "specular_color"
            "use_color_material"
            "material_shininess"
            "user_data"
            "tag"
            ]
        case "Fac3d"
            fields=[
            "parent"
            "children"
            "visible"
            "surface_mode"
            "foreground"
            "thickness"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "data"
            "color_mode"
            "color_flag"
            "cdata_mapping"
            "cdata_bounds"
            "color_range"
            "hiddencolor"
            "clip_state"
            "clip_box"
            "ambient_color"
            "diffuse_color"
            "specular_color"
            "use_color_material"
            "material_shininess"
            "user_data"
            "tag"
            ]
        case "Param3d"
            fields=[
            "parent"
            "children"
            "visible"
            "line_mode"
            "foreground"
            "thickness"
            "mark_mode"
            "mark_style"
            "mark_size_unit"
            "mark_size"
            "mark_foreground"
            "mark_background"
            "data"
            "clip_state"
            "clip_box"
            "color_mode"
            "surface_color"
            "user_data"
            "tag"
            ]
        case "Axis"
            fields=[
            "parent"
            "visible"
            "tics_direction"
            "xtics_coord"
            "ytics_coord"
            "tics_color"
            "tics_segment"
            "tics_style"
            "sub_tics"
            "tics_labels"
            "tics_interpreters"
            "format_n"
            "labels_font_size"
            "labels_font_color"
            "labels_font_style"
            "fractional_font"
            "clip_state"
            "clip_box"
            "user_data"
            "tag"
            ]
        case "Waitbar"
            fields=[
            "Userdata"
            "Tag"
            ]
        case "Progressionbar"
            fields=[
            "Userdata"
            "Tag"
            ]
        case "uimenu"
            fields=[
            "Parent"
            "Children"
            "Enable"
            "Foregroundcolor"
            "Label"
            "Handle_Visible"
            "Visible"
            "Callback"
            "Callback_Type"
            "Checked"
            "Icon"
            "TooltipString"
            "Userdata"
            "Tag"
            ]
        case "uicontextmenu"
            fields=[
            "Parent"
            "Children"
            ]
        case "uicontrol"
            fields=[];
            fields($ + 1)="Parent"
            fields($ + 1)="Children"
            fields($ + 1)="Style"
            if and(h.style <> ["popupmenu", "browser"]) | showHiddenProperties then
                fields($ + 1)="BackgroundColor"
            end

            if h.style == "frame" | showHiddenProperties then
                fields($ + 1)="Border"
            end

            if and(h.style <> ["frame", "layer", "text"]) | showHiddenProperties then
                fields($ + 1)="Callback"
                fields($ + 1)="Callback_Type"
            end

            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="Constraints"
            end

            if h.style =="browser" | showHiddenProperties then
                fields($ + 1)="Data"
            end

            if h.style =="browser" | showHiddenProperties then
                if h.debug == "on" then
                    fields($ + 1)="Debug"
                end
            end

            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="Enable"
            end

            if and(h.style <> ["image", "slider", "layer", "browser"]) | showHiddenProperties then
                fields($ + 1)="FontAngle"
                fields($ + 1)="FontName"
                fields($ + 1)="FontSize"
                fields($ + 1)="FontUnits"
                fields($ + 1)="FontWeight"
            end

            if and(h.style <> ["frame", "layer", "tab", "slider", "image", "popupmenu", "browser"]) | showHiddenProperties then
                fields($ + 1)="ForegroundColor"
            end
            if or(h.style == ["radiobutton", "checkbox"]) | showHiddenProperties then
                fields($ + 1)="Groupname"
            end
            if and(h.style <> ["frame", "layer", "tab", "listbox", "popupmenu", "browser"]) | showHiddenProperties then
                fields($ + 1)="HorizontalAlignment"
            end
            if or(h.style == ["text", "pushbutton", "frame"]) | showHiddenProperties then
                fields($ + 1)="Icon"
            end
            if h.style == "frame" | showHiddenProperties then
                fields($ + 1)="Layout"
                fields($ + 1)="Layout_options"
            end
            if h.style == "listbox" | showHiddenProperties then
                fields($ + 1)="ListboxTop"
            end


            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="Margins"
            end

            if or(h.style == ["checkbox", "radiobutton", "slider", "spinner", "listbox", "edit"]) | showHiddenProperties then
                fields($ + 1)="Max"
                fields($ + 1)="Min"
            end

            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="Position"
            end

            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="Relief"
            end

            if or(h.style == ["frame", "edit"]) | showHiddenProperties then
                fields($ + 1)="Scrollable"
            end
            if or(h.style == ["slider", "spinner"]) | showHiddenProperties then
                fields($ + 1)="SliderStep"
            end

            if h.style=="slider" | showHiddenProperties then
                fields($ + 1)="SnapToTicks"
            end

            if h.style <> "slider" | showHiddenProperties then
                fields($ + 1)="String"
            end

            fields($ + 1)="Tag"

            if h.style == "tab" | showHiddenProperties then
                fields($ + 1)="Title_position"
                fields($ + 1)="Title_scroll"
            end

            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="TooltipString"
            end

            if and(h.style <> ["browser"]) | showHiddenProperties then
                fields($ + 1)="Units"
            end

            fields($ + 1)="Userdata"

            if or(h.style == ["checkbox", "radiobutton", "slider", "spinner", "listbox", "edit", "layer", "tab", "popupmenu"]) | showHiddenProperties then
                fields($ + 1)="Value"
            end

            if and(h.style <> ["frame", "layer", "tab", "listbox", "popupmenu", "browser"]) | showHiddenProperties then
                fields($ + 1)="VerticalAlignment"
            end

            fields($ + 1)="Visible"
        case "Console"
            fields=[
            "Children"
            "ShowHiddenHandles"
            "ShowHiddenProperties"
            "UseDeprecatedSkin"
            "user_data"
            "tag"
            ]
        case "Light"
            fields=[
            "parent"
            "visible"
            "light_type"
            "position"
            "direction"
            "ambient_color"
            "diffuse_color"
            "specular_color"
            "user_data"
            "tag"]
        end
        if showHiddenProperties
            fields= ["UID"; fields];
        end
    end
endfunction
