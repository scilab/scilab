/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged;

import org.scilab.modules.localization.Messages;

/**
 * All messages used in GED.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class MessagesGED {
    //MenuBar
    public static final String close = Messages.gettext("Close");
    public static final String file = Messages.gettext("File");
    //Inspector - Labels
    public static final String quick_ged = Messages.gettext("Quick GED");
    public static final String property_list = Messages.gettext("Property List");
    public static final String hide = Messages.gettext("Hide");
    public static final String show = Messages.gettext("Show");
    public static final String arc = Messages.gettext("Arc");
    public static final String axes = Messages.gettext("Axes");
    public static final String champ = Messages.gettext("Champ");
    public static final String datatip = Messages.gettext("Datatip");
    public static final String polyline = Messages.gettext("Polyline");
    public static final String fec = Messages.gettext("Fec");
    public static final String figure = Messages.gettext("Figure");
    public static final String legend = Messages.gettext("Legend");
    public static final String segs = Messages.gettext("Segs");
    public static final String surface = Messages.gettext("Surface");
    public static final String plot3d = Messages.gettext("Plot3D");
    public static final String rectangle = Messages.gettext("Rectangle");
    public static final String fac3d = Messages.gettext("Fac3D");
    public static final String grayplot = Messages.gettext("Grayplot");
    public static final String matplot = Messages.gettext("Matplot");

    //Base Properties - Labels
    public static final String arc_drawing_method = Messages.gettext("Arc Drawing Method");
    public static final String auto_rotation = Messages.gettext("Auto Rotation");
    public static final String auto_position = Messages.gettext("Auto Position");
    public static final String auto_ticks = Messages.gettext("Auto Ticks");
    public static final String tick = Messages.gettext("Tick");
    public static final String lines = Messages.gettext("Lines");
    public static final String nurbs = Messages.gettext("Nurbs");
    public static final String anti_alising = Messages.gettext("Anti-alising");
    public static final String auto_clear = Messages.gettext("Auto Clear");
    public static final String auto_orientation = Messages.gettext("Auto Orientation");
    public static final String auto_scale = Messages.gettext("Auto Scale");
    public static final String auto_resize = Messages.gettext("Auto Resize");
    public static final String axes_visible = Messages.gettext("Axes Visible");
    public static final String axes_reverse = Messages.gettext("Axes Reverse");
    public static final String closed = Messages.gettext("Closed");
    public static final String cube_scaling = Messages.gettext("Cube Scaling");
    public static final String fill_mode = Messages.gettext("Fill Mode");
    public static final String immediate_drawing = Messages.gettext("Immediate Drawing");
    public static final String isometric_view = Messages.gettext("Isometric View");
    public static final String label_mode = Messages.gettext("Label Mode");
    public static final String line_mode = Messages.gettext("Line Mode");
    public static final String mark_mode = Messages.gettext("Mark Mode");
    public static final String orientation = Messages.gettext("Orientation");
    public static final String visible = Messages.gettext("Visible");
    public static final String base_properties = Messages.gettext("Base Properties");
    public static final String on = Messages.gettext("On");
    public static final String off = Messages.gettext("Off");
    public static final String pixmap = Messages.gettext("Pixmap");
    public static final String tag = Messages.gettext("Tag");
    public static final String text = Messages.gettext("Text");
    public static final String surface_mode = Messages.gettext("Surface Mode");

    public static final String box = Messages.gettext("Box");
    public static final String hidden_axis_color = Messages.gettext("Hidden Axis Color");

    public static final String camera = Messages.gettext("Camera");
    public static final String alpha = Messages.gettext("Alpha");
    public static final String theta = Messages.gettext("Theta");

    public static final String ticks = Messages.gettext("Ticks");

    public static final String warning = Messages.gettext("Warning");
    public static final String warning_logscale = Messages.gettext("The data bounds from the axis must be strictly positive to be changed to logarithmic scale.");

    //ColorDialog
    public static final String choose_background_color = Messages.gettext("Choose Background Color");
    public static final String choose_foreground_color = Messages.gettext("Choose Foreground Color");
    public static final String choose_font_color = Messages.gettext("Choose Font Color");
    public static final String choose_color = Messages.gettext("Choose a Color");

    //Style/Appearance - Labels
    public static final String arrow_size = Messages.gettext("Arrow Size");
    public static final String colored = Messages.gettext("Colored");
    public static final String colors = Messages.gettext("Colors");
    public static final String background = Messages.gettext("Background");
    public static final String foreground = Messages.gettext("Foreground");
    public static final String grid_position = Messages.gettext("Grid Position");
    public static final String x_grid_color = Messages.gettext("X Grid Color");
    public static final String y_grid_color = Messages.gettext("Y Grid Color");
    public static final String back_half = Messages.gettext("Back Half");
    public static final String hidden_axes = Messages.gettext("Hidden Axes");
    public static final String style_appearance = Messages.gettext("Style/Appearance");
    public static final String background_color = Messages.gettext("Background Color");
    public static final String foreground_color = Messages.gettext("Foreground Color");
    public static final String line_style = Messages.gettext("Line Style");
    public static final String solid = Messages.gettext("solid");
    public static final String dash = Messages.gettext("dash");
    public static final String dash_dot = Messages.gettext("dash dot");
    public static final String longdash_dot = Messages.gettext("longdash dot");
    public static final String bigdash_dot = Messages.gettext("bigdash dot");
    public static final String bigdash_longdash = Messages.gettext("bigdash longdash");
    public static final String dot = Messages.gettext("dot");
    public static final String double_dot = Messages.gettext("double dot");
    public static final String polyline_style = Messages.gettext("Polyline Style");
    public static final String interpolated = Messages.gettext("interpolated");
    public static final String staircase = Messages.gettext("staircase");
    public static final String barplot = Messages.gettext("barplot");
    public static final String bar = Messages.gettext("bar");
    public static final String arrowed = Messages.gettext("arrowed");
    public static final String filled = Messages.gettext("filled");
    public static final String min = Messages.gettext("Min");
    public static final String max = Messages.gettext("Max");
    public static final String color_range = Messages.gettext("Color Range");
    public static final String outside_color = Messages.gettext("Outside Color");
    public static final String z_bounds = Messages.gettext("Z Bounds");

    public static final String pixel_drawing_mode = Messages.gettext("Pixel Drawing Mode");
    public static final String rotation_style = Messages.gettext("Rotation Style");
    public static final String unary = Messages.gettext("unary");
    public static final String multiple = Messages.gettext("multiple");

    public static final String arrow_size_factor = Messages.gettext("Arrow Size Factor");
    public static final String bar_width = Messages.gettext("Bar Width");
    public static final String color_mode = Messages.gettext("Color Mode");
    public static final String color_flag = Messages.gettext("Color Flag");
    public static final String hidden_color = Messages.gettext("Hidden Color");
    public static final String thickness = Messages.gettext("Thickness");
    public static final String mark = Messages.gettext("Mark");
    public static final String mark_background = Messages.gettext("Mark Background");
    public static final String mark_foreground = Messages.gettext("Mark Foreground");
    public static final String mark_size = Messages.gettext("Mark Size");
    public static final String mark_style = Messages.gettext("Mark Style");

    //Data - Labels
    public static final String display_components = Messages.gettext("Components display");
    public static final String data_properties = Messages.gettext("Data Properties");
    public static final String data_mapping = Messages.gettext("Data Mapping");
    public static final String direct = Messages.gettext("Direct");
    public static final String clip_box = Messages.gettext("Clip Box");
    public static final String upper_left = Messages.gettext("Upper-left");
    public static final String point = Messages.gettext("Point");
    public static final String clip_state = Messages.gettext("Clip State");
    public static final String clipgrf = Messages.gettext("clipgrf");
    public static final String data = Messages.gettext("Data");
    public static final String figure_id = Messages.gettext("Figure ID");
    public static final String figure_name = Messages.gettext("Figure Name");
    public static final String info_message = Messages.gettext("Info Message");
    public static final String units = Messages.gettext("Units");
    public static final String upper_left_point = Messages.gettext("Upper Left Point");
    public static final String user_data = Messages.gettext("User Data");
    public static final String x_label = Messages.gettext("X Label");
    public static final String y_label = Messages.gettext("Y Label");
    public static final String z_label = Messages.gettext("Z Label");
    public static final String scaled = Messages.gettext("Scaled");
    public static final String title = Messages.gettext("Title");
    public static final String start_angle = Messages.gettext("Start Angle");
    public static final String end_angle = Messages.gettext("End Angle");

    //Data - Tooltip
    public static final String figure_id_tooltip = Messages.gettext("Number of current figure.");
    public static final String figure_name_tooltip = Messages.gettext("Enter a figure name.");
    public static final String refresh = Messages.gettext("Refresh");
    public static final String submit_close = Messages.gettext("Submit and close");

    //Data Table
    public static final String append = Messages.gettext("Append");
    public static final String data_editor = Messages.gettext("Data Editor");
    public static final String delete = Messages.gettext("Delete");

    //Control - Labels
    public static final String _2d = Messages.gettext("2D");
    public static final String _3d = Messages.gettext("3D");
    public static final String control = Messages.gettext("Control");
    public static final String axes_size = Messages.gettext("Axes Size");
    public static final String close_request_fcn = Messages.gettext("CloseRequestFcn");
    public static final String resize_fcn = Messages.gettext("ResizeFcn");
    public static final String figure_position = Messages.gettext("Figure Position");
    public static final String figure_size = Messages.gettext("Figure Size");
    public static final String event_handler = Messages.gettext("Event Handler");
    public static final String function = Messages.gettext("Function");
    public static final String view = Messages.gettext("View");
    public static final String viewport = Messages.gettext("Viewport");
    public static final String x_left = Messages.gettext("X Left");
    public static final String y_up = Messages.gettext("Y Up");
    public static final String log_flags = Messages.gettext("Log Flags");
    public static final String x_tight_limits = Messages.gettext("X Tight Limits");
    public static final String y_tight_limits = Messages.gettext("Y Tight Limits");
    public static final String z_tight_limits = Messages.gettext("Z Tight Limits");

    //Control - Tooltip
    public static final String x_position_tooltip = Messages.gettext("Enter a x position.");
    public static final String y_position_tooltip = Messages.gettext("Enter a y position.");
    public static final String x_size_tooltip = Messages.gettext("Enter a x size.");
    public static final String y_size_tooltip = Messages.gettext("Enter a y size.");

    //Axis Rulers - Labels
    public static final String axis_rulers = Messages.gettext("Axis Rulers");
    public static final String x_location = Messages.gettext("X Location");
    public static final String y_location = Messages.gettext("Y Location");
    public static final String location = Messages.gettext("Location");
    public static final String bottom = Messages.gettext("bottom");
    public static final String middle = Messages.gettext("middle");
    public static final String origin = Messages.gettext("origin");
    public static final String top = Messages.gettext("top");
    public static final String left = Messages.gettext("left");
    public static final String right = Messages.gettext("right");
    public static final String reverse = Messages.gettext("Reverse");

    //Label - Labels
    public static final String box_mode = Messages.gettext("Box Mode");
    public static final String label = Messages.gettext("Label");
    public static final String title_page = Messages.gettext("Title Page");
    public static final String axis_title = Messages.gettext("Axis Title");

    //Font - Labels
    public static final String font = Messages.gettext("Font");
    public static final String font_angle = Messages.gettext("Font Angle");
    public static final String font_color = Messages.gettext("Font Color");
    public static final String font_size = Messages.gettext("Font Size");
    public static final String font_style = Messages.gettext("Font Style");
    public static final String fractional_font = Messages.gettext("Fractional Font");

    //Position - Labels
    public static final String sub_ticks = Messages.gettext("Sub Ticks");
    public static final String coordinates = Messages.gettext("Coordinates");
    public static final String x_coordinate = Messages.gettext("X Coordinate");
    public static final String y_coordinate = Messages.gettext("Y Coordinate");
    public static final String position = Messages.gettext("Position");
    public static final String axes_bounds = Messages.gettext("Axes Bounds");
    public static final String mark_size_unit = Messages.gettext("Mark Size Unit");
    public static final String shift = Messages.gettext("Shift");
    public static final String up = Messages.gettext("Up");
    public static final String width = Messages.gettext("Width");
    public static final String height = Messages.gettext("Height");
    public static final String margins = Messages.gettext("Margins");
    public static final String rotation_angles = Messages.gettext("Rotation Angles");
    public static final String tabulated = Messages.gettext("Tabulated");
    public static final String x_shift = Messages.gettext("X Shift");
    public static final String y_shift = Messages.gettext("Y Shift");
    public static final String z_shift = Messages.gettext("Z Shift");
}
