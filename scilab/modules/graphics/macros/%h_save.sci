// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA - Serge Steer
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function %h_save(h,fd)
    version=[5 5 2 0]
    mput(version,"uc",fd)

    // There are issues when saving characters with 'c' format
    // characterFormat format behave smoothly
    characterFormat = "uc";
    stringFormat = "il";

    hsize = size(h);
    mput(hsize,characterFormat,fd);
    // introduced in version 3 1 0 2 to handle
    // the case where we have a matrix of handles to save

    if or(hsize>1)
        for i=1:hsize(1)
            for j=1:hsize(2)
                save_graphichandle(h(i,j),fd)
            end
        end
    else
        save_graphichandle(h,fd)
    end
endfunction

function save_graphichandle(h,fd)
    if ~is_handle_valid(h) then
        warning(msprintf(gettext("%s: handle no more valid ignored.\n"),"save_graphichandle"));
        return
    end
    typ=h.type
    select h.type

    case "Figure"
        mput( length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        // handle => new style
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(h.figure_position,"sl",fd) // figure_position
        mput(h.figure_size,"sl",fd) // figure size
        mput(h.axes_size,"sl",fd) // axes_size
        mput(h.viewport,"sl",fd) // viewport
        mput(length(ascii(h.info_message)),stringFormat,fd) ; // info_message
        mput(ascii(h.info_message),characterFormat,fd) ;
        mput(length(ascii(h.tag)),characterFormat,fd) ; // tag
        mput(ascii(h.tag),characterFormat,fd) ;
        mput(bool2s(h.auto_resize=="on"),characterFormat,fd) // auto_resize
        mput(length(ascii(h.figure_name)),stringFormat,fd); // figure_name
        mput(ascii(h.figure_name),characterFormat,fd);
        mput(h.figure_id,"sl",fd); // figure_id

        mput(size(h.color_map,"*"),"il",fd); // color_map
        mput(h.color_map,"dl",fd) ;
        mput(length(h.pixel_drawing_mode),characterFormat,fd); // pixel_drawing_mode
        mput(ascii(h.pixel_drawing_mode),characterFormat,fd);
        mput(length(h.anti_aliasing),characterFormat,fd); // anti_aliasing
        mput(ascii(h.anti_aliasing),characterFormat,fd);
        mput(bool2s(h.immediate_drawing=="on"),characterFormat,fd) // immediate_drawing
        mput(h.background,"il",fd) // background
        mput(length(h.rotation_style),characterFormat,fd); // rotation style
        mput(ascii(h.rotation_style),characterFormat,fd);

        mput(length(ascii(h.event_handler)),characterFormat,fd); // event_handler
        mput(ascii(h.event_handler),characterFormat,fd);
        mput(length(h.event_handler_enable),characterFormat,fd); // even_handler_enable
        mput(ascii(h.event_handler_enable),characterFormat,fd); // need to be put after event_handler

        mput(length(ascii(h.resizefcn)),characterFormat,fd) ; // resizefcn
        mput(ascii(h.resizefcn),characterFormat,fd) ;
        mput(length(ascii(h.closerequestfcn)),characterFormat,fd) ; // closerequestfcn
        mput(ascii(h.closerequestfcn),characterFormat,fd) ;

        // children
        c=h.children;
        n=size(c,"*")
        mput(n,"il",fd)
        for k=n:-1:1
            save_graphichandle(c(k),fd)
        end

        user_data=h.user_data; // user_data
        save(fd,user_data) ;


    case "Axes"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(size(h.axes_visible,"*"),characterFormat,fd); // axes_visible
        mput(bool2s(h.axes_visible=="on"),characterFormat,fd) ;
        mput(size(h.axes_reverse,"*"),characterFormat,fd); // axes_reverse
        mput(bool2s(h.axes_reverse=="on"),characterFormat,fd) ;
        mput(size(h.grid,"*"),characterFormat,fd); mput(h.grid,"il",fd); // grid
        mput(length(h.grid_position),characterFormat,fd); // grid_position
        mput(ascii(h.grid_position),characterFormat,fd);
        mput(length(h.x_location),characterFormat,fd); // x_location
        mput(ascii(h.x_location),characterFormat,fd);
        mput(length(h.y_location),characterFormat,fd); // y_location
        mput(ascii(h.y_location),characterFormat,fd);
        mput(ascii(h.view),characterFormat,fd) ; // view

        // title
        l=h.title;
        mput(bool2s(l.visible=="on"),characterFormat,fd) ; // title.visible
        save_text_matrix( l.text, fd ) ;
        mput(l.font_foreground,"il",fd); // title_label.font_foreground
        mput(bool2s(l.fractional_font=="on"),characterFormat,fd) // title.fractional_font
        mput(l.foreground,"il",fd) // title.foreground
        mput(l.background,"il",fd) // title.background
        mput(bool2s(l.fill_mode=="on"),characterFormat,fd) // title.fill_mode
        mput(l.font_style,characterFormat,fd) ; // title.font_style
        mput(l.font_size,characterFormat,fd)  ; // title.font_size

        mput(bool2s(l.auto_rotation=="on"),characterFormat,fd) // title.auto_rotation
        mput(l.font_angle,"dl",fd) // title.font_angle
        mput(bool2s(l.auto_position=="on"),characterFormat,fd) // title.auto_position
        mput(l.position,"dl",fd) // title.position

        // x_label
        l=h.x_label
        mput(bool2s(l.visible=="on"),characterFormat,fd) // x_label.visible
        save_text_matrix( l.text, fd ) ;
        mput(l.font_foreground,"il",fd); // x_label.font_foreground
        mput(bool2s(l.fractional_font=="on"),characterFormat,fd) // x_label.fractional_font
        mput(l.foreground,"il",fd) // x_label.foreground
        mput(l.background,"il",fd) // x_label.background
        mput(bool2s(l.fill_mode=="on"),characterFormat,fd) // x_label.fill_mode
        mput(l.font_style,characterFormat,fd) // x_label.font_style
        mput(l.font_size,characterFormat,fd)  // x_label.font_size

        mput(bool2s(l.auto_rotation=="on"),characterFormat,fd) // x_label.auto_rotation
        mput(l.font_angle,"dl",fd) // x_label.font_angle
        mput(bool2s(l.auto_position=="on"),characterFormat,fd) // x_label.auto_position
        mput(l.position,"dl",fd) // x_label.position

        // y_label
        l=h.y_label
        mput(bool2s(l.visible=="on"),characterFormat,fd) // y_label.visible
        save_text_matrix( l.text, fd ) ;
        mput(l.font_foreground,"il",fd); // y_label.font_foreground
        mput(bool2s(l.fractional_font=="on"),characterFormat,fd) // y_label.fractional_font
        mput(l.foreground,"il",fd) // y_label.foreground
        mput(l.background,"il",fd) // y_label.background
        mput(bool2s(l.fill_mode=="on"),characterFormat,fd) // y_label.fill_mode
        mput(l.font_style,characterFormat,fd) // y_label.font_style
        mput(l.font_size,characterFormat,fd) // y_label.font_size

        mput(bool2s(l.auto_rotation=="on"),characterFormat,fd) // y_label.auto_rotation
        mput(l.font_angle,"dl",fd) // y_label.font_angle
        mput(bool2s(l.auto_position=="on"),characterFormat,fd) // y_label.auto_position
        mput(l.position,"dl",fd) // y_label.position

        if h.view=="3d" then
            // z_label
            l=h.z_label
            mput(bool2s(l.visible=="on"),characterFormat,fd) // z_label.visible
            save_text_matrix( l.text, fd ) ;
            mput(l.font_foreground,"il",fd); // z_label.font_foreground
            mput(bool2s(l.fractional_font=="on"),characterFormat,fd) // z_label.fractional_font
            mput(l.foreground,"il",fd) // z_label.foreground
            mput(l.background,"il",fd) // z_label.background
            mput(bool2s(l.fill_mode=="on"),characterFormat,fd) // z_label.fill_mode
            mput(l.font_style,characterFormat,fd) // z_label.font_style
            mput(l.font_size,characterFormat,fd) // z_label.font_size
            mput(bool2s(l.auto_rotation=="on"),characterFormat,fd) // z_label.auto_rotation
            mput(l.font_angle,"dl",fd) // z_label.font_angle
            mput(bool2s(l.auto_position=="on"),characterFormat,fd) // z_label.auto_position
            mput(l.position,"dl",fd) // z_label.position
        end

        mput(size(h.auto_ticks,"*"),characterFormat,fd); // auto_ticks
        mput(bool2s(h.auto_ticks=="on"),characterFormat,fd)
        // x_ticks
        sz=size(h.x_ticks.locations,"*");
        mput(sz,"sl",fd);
        if sz>0 then
            mput(h.x_ticks.locations,"dl",fd)
            mput(length(h.x_ticks.labels),characterFormat,fd); // x_ticks.labels
            mput(ascii(strcat(h.x_ticks.labels)),characterFormat,fd);
        end
        // y_ticks
        sz=size(h.y_ticks.locations,"*");
        mput(sz,"sl",fd);
        if sz>0 then
            mput(h.y_ticks.locations,"dl",fd)
            mput(length(h.y_ticks.labels),characterFormat,fd); // y_ticks.labels
            mput(ascii(strcat(h.y_ticks.labels)),characterFormat,fd);
        end
        // z_ticks
        sz=size(h.z_ticks.locations,"*");
        mput(sz,"sl",fd);
        if sz>0 then
            mput(h.z_ticks.locations,"dl",fd)
            mput(length(h.z_ticks.labels),characterFormat,fd); // z_ticks.labels
            mput(ascii(strcat(h.z_ticks.labels)),characterFormat,fd);
        end
        mput(length(h.box), characterFormat, fd ) ; // box
        mput(ascii(h.box),  characterFormat, fd ) ;
        mput(bool2s(h.filled == "on"),characterFormat,fd); // filled
        mput(size(h.sub_tics,"*"),characterFormat,fd);mput(h.sub_tics,characterFormat,fd); // sub_ticks
        //mput(-1,'il',fd) // tics_color is removed F.Leray 15.03.05
        mput(h.font_style,characterFormat,fd) // font_style
        mput(h.font_size,characterFormat,fd) //font_size
        mput(h.font_color,"il",fd) // font_color
        mput(bool2s(h.fractional_font=="on"),characterFormat,fd) // fractional_font
        mput(bool2s(h.isoview=="on"),characterFormat,fd)    // isoview
        mput(bool2s(h.cube_scaling=="on"),characterFormat,fd) // cube_scaling
        mput(h.rotation_angles,"dl",fd) // rotation_angles
        mput(ascii(h.log_flags),characterFormat,fd) // log_flags
        mput(size(h.tight_limits,"*"),characterFormat,fd); // tight_limits
        mput(bool2s(h.tight_limits=="on"),characterFormat,fd);
        mput(size(h.data_bounds,"*"),characterFormat,fd); // data_bounds
        mput(h.data_bounds,"dl",fd);
        mput(size(h.zoom_box,"*"),characterFormat,fd);  // zoom_box
        if size(h.zoom_box,"*")>0 then mput(h.zoom_box,"dl",fd);end
        mput(h.margins,"dl",fd); // margins
        mput(h.axes_bounds,"dl",fd); // axes_bounds
        mput(bool2s(h.auto_clear=="on"),characterFormat,fd) // auto_clear
        mput(bool2s(h.auto_scale=="on"),characterFormat,fd) // auto_scale
        mput(h.hidden_axis_color,"il",fd); // hidden_axis_color
        mput(length(h.arc_drawing_method),characterFormat,fd); // arc_drawing_method
        mput(ascii(h.arc_drawing_method),characterFormat,fd);
        mput(h.hiddencolor,"il",fd) // hidden_color
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) // line_mode
        mput(h.line_style,characterFormat,fd) // line_style
        mput(h.thickness,"sl",fd) // thickness
        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background

        mput(h.foreground,"il",fd) // foreground
        mput(h.background,"il",fd) // background

        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end

        // children
        c=h.children;
        n=size(c,"*")
        mput(n,"il",fd)
        for k=n:-1:1
            save_graphichandle(c(k),fd)
        end

        user_data=h.user_data;save(fd,user_data) // user_data

    case "Polyline"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type

        mput(bool2s(h.visible=="on"),characterFormat,fd) ; // visible
        mput(size(h.data),"il",fd);mput(h.data,"dl",fd) // data
        mput(bool2s(h.closed=="on"),characterFormat,fd) // closed
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) // line_mode
        mput(bool2s(h.fill_mode=="on"),characterFormat,fd) // fill_mode
        mput(h.line_style,characterFormat,fd) // line_style
        mput(h.thickness,"sl",fd) // thickness
        mput(h.arrow_size_factor,"sl",fd) // arrow_size_factor
        mput(max(1,h.polyline_style),characterFormat,fd) // ployline_style

        mput( size( h.interp_color_vector, "*" ), "sl", fd ) ; // interp_color_vector
        mput( h.interp_color_vector, "dl", fd ) ;
        mput( bool2s(h.interp_color_mode=="on"), characterFormat, fd ) ; // interp_color_mode

        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.foreground,"il",fd) // foreground
        mput(h.background,"il",fd) // background
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background
        mput(h.mark_offset,"il",fd) // mark_offset
        mput(h.mark_stride,"il",fd) // mark_stride

        mput(size(h.x_shift,"*"),"sl",fd); mput(h.x_shift,"dl",fd); // x_shift
        mput(size(h.y_shift,"*"),"sl",fd); mput(h.y_shift,"dl",fd); // y_shift
        mput(size(h.z_shift,"*"),"sl",fd); mput(h.z_shift,"dl",fd); // z_shift

        mput( h.bar_width, "dl", fd ) ; // bar_width
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Datatip"
        mput(length(h.type),characterFormat,fd); // type
        mput(ascii(h.type),characterFormat,fd);
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(size(h.data),characterFormat,fd); // data
        mput(h.data,"dl",fd);
        mput(length(h.box_mode),characterFormat,fd); // box_mode
        mput(ascii(h.box_mode),characterFormat,fd);
        mput(length(h.label_mode),characterFormat,fd); // label_mode
        mput(ascii(h.label_mode),characterFormat,fd);
        mput(h.orientation,characterFormat,fd) // orientation
        mput(length(h.display_components),characterFormat,fd);
        mput(h.display_components,characterFormat,fd) // display_components
        mput(bool2s(h.auto_orientation=="on"),characterFormat,fd) // auto_orientation
        mput(bool2s(h.interp_mode=="on"),characterFormat,fd) // interp_mode
        mput(length(ascii(h.display_function)),stringFormat,fd); // display_function
        mput(h.font_foreground, "il", fd) ; // font_foreground
        mput(h.foreground,"il",fd) // foreground
        mput(h.background, "il", fd) ; // background
        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background
        user_data=h.user_data;save(fd,user_data) // user_data
        mput(length(h.tag),"c",fd); // tag
        mput(ascii(h.tag),"c",fd);

    case "Plot3d";
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(bool2s(h.surface_mode=="on"),characterFormat,fd) // surface_mode
        mput(h.foreground,"il",fd) // foreground
        mput(h.thickness,"sl",fd) // thickness
        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background
        mput(h.color_mode,"il",fd) // color_mode
        mput(h.color_flag,"il",fd) // color_flag
        x=h.data.x;sz=size(x)
        mput(sz,"il",fd) // data.x
        mput(x,"dl",fd)
        y=h.data.y;sz=size(y)
        mput(sz,"il",fd) // data.y
        mput(y,"dl",fd)
        z=h.data.z;sz=size(z)
        mput(sz,"il",fd) // data.z
        mput(z,"dl",fd)
        if or(h.color_flag==[2 5]) then
            clr=h.data.color;sz=size(clr)
            mput(sz,"il",fd) // data.color
            mput(clr,"il",fd)
        end
        mput(h.hiddencolor,"il",fd) // hidden_color
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Fac3d";
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd) ; // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) ; // visible
        mput(bool2s(h.surface_mode=="on"),characterFormat,fd) // surface_mode
        mput(h.foreground,"il",fd) // foreground
        mput(h.thickness,"sl",fd) // thickness
        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background
        mput(h.color_mode,"il",fd) // color_mode
        mput(h.color_flag,"il",fd) // color_flag
        x=h.data.x;sz=size(x)
        mput(sz,"il",fd) // data.x
        mput(x,"dl",fd)
        y=h.data.y;sz=size(y)
        mput(sz,"il",fd) // data.y
        mput(y,"dl",fd)
        z=h.data.z;sz=size(z)
        mput(sz,"il",fd) // data.z
        mput(z,"dl",fd)
        if h.color_flag >=2 then
            clr=h.data.color;sz=size(clr) // data.color
            mput(sz,"il",fd)
            mput(clr,"il",fd)
            mput(ascii(part(h.cdata_mapping,1)),characterFormat,fd) ; // cdata_mapping
        end
        mput(h.hiddencolor,"il",fd) // hidden_color
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Compound"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type

        // children
        children=h.children
        n=size(children,"*")
        mput(n,"il",fd)
        for k=1:n
            save_graphichandle(children(k),fd)
        end
        mput( bool2s( h.visible=="on"), characterFormat, fd ) ; // visible

        user_data=h.user_data;save(fd,user_data) // user_data

    case "Rectangle"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(h.thickness,"sl",fd) // thickness
        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) // line_mode
        mput(h.line_style,characterFormat,fd) // line_style
        mput(bool2s(h.fill_mode=="on"),characterFormat,fd) // fill_mode
        mput(h.foreground,"il",fd) // foreground
        mput(h.background,"il",fd) // background
        mput(size(h.data, "*"), "il", fd); // data
        mput(h.data,"dl",fd);
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end

        user_data=h.user_data;save(fd,user_data) ; // user_data

    case "Arc"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(h.thickness,"sl",fd) // thickness
        mput(h.line_style,characterFormat,fd) // line_style
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) ;// line_mode
        mput(bool2s(h.fill_mode=="on"),characterFormat,fd) // fill_mode
        mput(h.foreground,"il",fd) // foreground
        mput(h.background,"il",fd) ; // background
        mput(size(h.data, "*"), "il", fd); // data
        mput(h.data,"dl",fd);
        mput(length(h.arc_drawing_method),characterFormat,fd); // arc_drawing_method
        mput(ascii(h.arc_drawing_method),characterFormat,fd);
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Champ"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        x=h.data.x;sz=size(x)
        mput(sz,"il",fd) // data.x
        mput(x,"dl",fd)
        y=h.data.y;sz=size(y)
        mput(sz,"il",fd) // data.y
        mput(y,"dl",fd)
        fx=h.data.fx;sz=size(fx)
        mput(sz,"il",fd) // data.fx
        mput(fx,"dl",fd)
        fy=h.data.fy;sz=size(fy)
        mput(sz,"il",fd) // data.fz
        mput(fy,"dl",fd)
        mput(h.line_style,characterFormat,fd) // line_style
        mput(h.thickness,"sl",fd) // thickness
        mput(bool2s(h.colored=="on"),characterFormat,fd) // colored
        mput(h.arrow_size,"dl",fd) // arrow_size
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Segs"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(size(h.data),"il",fd);mput(h.data,"dl",fd) // data
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) // line_mode
        mput(h.line_style,characterFormat,fd) // line_style
        mput(h.thickness,"sl",fd) // thickness
        mput(h.arrow_size,"dl",fd) // arrow_size
        mput(size(h.segs_color,"*"),"il",fd); // segs_color
        mput(h.segs_color,"il",fd)
        mput(bool2s(h.mark_mode=="on"),characterFormat,fd) // mark_mode
        mput(h.mark_style,characterFormat,fd) // mark_style
        mput(h.mark_size,characterFormat,fd) // mark_size
        mput(ascii(part(h.mark_size_unit,1)),characterFormat,fd) // mark_size_unit
        mput(h.mark_foreground,"il",fd) // mark_foreground
        mput(h.mark_background,"il",fd) // mark_background
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd); // clip_state
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Grayplot"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        x=h.data.x;sz=size(x)
        mput(sz,"il",fd) // data.x
        mput(x,"dl",fd)
        y=h.data.y;sz=size(y)
        mput(sz,"il",fd) // data.y
        mput(y,"dl",fd)
        z=h.data.z;sz=size(z)
        mput(sz,"il",fd) // data.z
        mput(z,"dl",fd)
        mput(length(h.data_mapping),characterFormat,fd); // data_mapping
        mput(ascii(h.data_mapping),characterFormat,fd);
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Matplot"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(size(h.data),"il",fd);mput(h.data,"dl",fd) // datat
        //    mput(length(h.data_mapping),characterFormat,fd);mput(ascii(h.data_mapping),characterFormat,fd);
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Fec"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(size(h.data),"il",fd);mput(h.data,"dl",fd) // data
        mput(size(h.triangles),"il",fd);mput(h.triangles,"dl",fd) // triangles
        mput(h.z_bounds,"dl",fd); // z_bounds
        mput(h.color_range,"dl",fd); // color_range
        mput(h.outside_colors,"dl",fd); // outside_colors
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) // line_mode
        mput(h.foreground,"il",fd) // foreground
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Legend"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); //type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        save_text_vector(h.text,fd); // text
        mput(h.font_style,characterFormat,fd) // font_style
        mput(h.font_size,characterFormat,fd) // font_size
        mput(h.font_color,"il",fd) // font_color
        mput(bool2s(h.fractional_font=="on"),characterFormat,fd) // fractional_font
        //replace links by a path relative to the parent axes
        links=h.links;nlegends=size(links,"*");
        mput(nlegends,characterFormat,fd)
        for kl=1:nlegends
            p=get_entity_path(links(kl))
            mput(size(p,"*"),"il",fd)
            mput(p,"il",fd)
        end
        mput(length(h.legend_location),characterFormat,fd);mput(ascii(h.legend_location),characterFormat,fd); //legend_location
        mput(h.position,"dl",fd);
        mput(bool2s(h.line_mode=="on"),characterFormat,fd) // line_mode
        mput(h.thickness,"sl",fd) // thickness
        mput(h.foreground,"il",fd) // foreground
        mput(bool2s(h.fill_mode=="on"),characterFormat,fd) // fill_mode
        mput(h.background,"il",fd) ; // background

        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Text"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        //save_text_vector(h.text,fd) // text
        save_text_matrix( h.text, fd ) ;
        mput(size(h.data),characterFormat,fd); // data // size could be 2 or 3
        mput(h.data,"dl",fd);
        mput(h.text_box,"dl",fd); // text_box
        mput(length(h.text_box_mode),characterFormat,fd);
        mput(ascii(h.text_box_mode),characterFormat,fd); // text_box_mode

        mput(h.foreground,"il",fd) // foreground
        mput(h.font_style,characterFormat,fd) // font_style
        mput(h.font_size,characterFormat,fd) // font_size
        mput(h.font_angle,"dl",fd) // font_angle

        //adding JB Silvy 28/11/05
        // box drawing
        mput( bool2s( h.box       == "on" ), characterFormat, fd ) ; // box
        mput( bool2s( h.line_mode == "on" ), characterFormat, fd ) ; // line_mode
        mput( bool2s( h.fill_mode == "on" ), characterFormat, fd ) ; // fill_mode

        mput( h.font_foreground, "il", fd ) ; // font_foreground
        mput( h.background     , "il", fd ) ; // background

        mput(length(h.alignment),characterFormat,fd);
        mput(ascii(h.alignment),characterFormat,fd) ; // alignment
        mput(bool2s(h.fractional_font=="on"),characterFormat,fd) // fractional_font

        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "Axis"
        mput(length(h.type),characterFormat,fd);mput(ascii(h.type),characterFormat,fd); // type
        mput(bool2s(h.visible=="on"),characterFormat,fd) // visible
        mput(length(h.tics_direction),"il",fd); // ticks_direction
        mput(ascii(h.tics_direction),characterFormat,fd);
        mput(size(h.xtics_coord,"*"),"il",fd); // xtics_coord
        mput(h.xtics_coord,"dl",fd)
        mput(size(h.ytics_coord,"*"),"il",fd); // ytics_coord
        mput(h.ytics_coord,"dl",fd)
        mput(h.tics_color,"il",fd) // tics_color
        mput(bool2s(h.tics_segment=="on"),characterFormat,fd) // tics_segment
        mput(ascii(h.tics_style),characterFormat,fd) // tics_style
        mput(h.sub_tics,"il",fd) // sub_tics
        save_text_vector(h.tics_labels,fd) // tics_labels
        mput(h.labels_font_size,"il",fd) // label_font_size
        mput(h.labels_font_color,"il",fd) // labels_font_color
        mput(h.labels_font_style,"il",fd) // labels_font_style
        mput(bool2s(h.fractional_font=="on"),characterFormat,fd) // fractional_font
        mput(length(h.clip_state),characterFormat,fd); // clip_state
        mput(ascii(h.clip_state),characterFormat,fd);
        if h.clip_state=="on" then
            mput(h.clip_box,"dl",fd) // clip_box
        end
        user_data=h.user_data;save(fd,user_data) // user_data

    case "uimenu"
        mput(length(h.type),"c",fd);mput(ascii(h.type),"c",fd); // Type
        mput(bool2s(h.enable=="on"),"c",fd); // Enable
        mput(size(h.foregroundcolor,"*"),"il",fd); // ForegroundColor (size)
        mput(h.foregroundcolor,"dl",fd); // ForegroundColor (data)
        mput(length(h.label),"c",fd);mput(ascii(h.label),"c",fd); // Label
        mput(bool2s(h.visible=="on"),"c",fd); // Visible
        mput(length(h.callback),stringFormat,fd);mput(ascii(h.callback),"c",fd); // Callback
        mput(h.callback_type,"il",fd); // Callback Type
        mput(length(h.tag),"c",fd);mput(ascii(h.tag),"c",fd); // Tag
        mput(bool2s(h.checked=="on"),"c",fd); // Checked
        // children
        c=h.children;
        n=size(c,"*")
        mput(n,"il",fd)
        for k=n:-1:1
            save_graphichandle(c(k),fd)
        end

    case "uicontextmenu"
        mput(length(h.type),"c",fd);mput(ascii(h.type),"c",fd); // Type
        // children
        c=h.children;
        n=size(c,"*")
        mput(n,"il",fd)
        for k=n:-1:1
            save_graphichandle(c(k),fd)
        end

    case "uicontrol"
        mput(length(h.type),"c",fd);mput(ascii(h.type),"c",fd); // Type
        mput(length(h.style),"c",fd);mput(ascii(h.style),"c",fd); // Style
        mput(size(h.backgroundcolor,"*"),"il",fd); // BackgroundColor (size)
        mput(h.backgroundcolor,"dl",fd); // BackgroundColor (data)
        mput(bool2s(h.enable=="on"),"c",fd); // Enable
        mput(length(h.fontangle),"c",fd);mput(ascii(h.fontangle),"c",fd); // FontAngle
        mput(length(h.fontname),"c",fd);mput(ascii(h.fontname),"c",fd); // FontName
        mput(h.fontsize,"dl",fd); // FontSize
        mput(length(h.fontunits),"c",fd);mput(ascii(h.fontunits),"c",fd); // FontUnits
        mput(length(h.fontweight),"c",fd);mput(ascii(h.fontweight),"c",fd); // FontWeight
        mput(size(h.foregroundcolor,"*"),"il",fd); // ForegroundColor (size)
        mput(h.foregroundcolor,"dl",fd); // ForegroundColor (data)
        mput(length(h.horizontalalignment),"c",fd);mput(ascii(h.horizontalalignment),"c",fd); // HorizontalAlignment
        mput(size(h.listboxtop,"*"),"il",fd); // ListboxTop (size)
        mput(h.listboxtop,"dl",fd); // ListboxTop (data)
        mput(h.max,"dl",fd); // Max
        mput(h.min,"dl",fd); // Min
        mput(size(h.position,"*"),"il",fd); // Position (size)
        mput(h.position,"dl",fd); // Position (data)
        mput(length(h.relief),"c",fd);mput(ascii(h.relief),"c",fd); // Relief
        mput(size(h.sliderstep,"*"),"il",fd); // SliderStep (size)
        mput(h.sliderstep,"dl",fd); // SliderStep (data)
        save_text_matrix( h.string, fd ) ; // String
        save_text_matrix( h.tooltipstring, fd ) ; // TooltipString
        mput(length(h.units),"c",fd);mput(ascii(h.units),"c",fd); // Units
        mput(size(h.value,"*"),"il",fd); // Value (size)
        mput(h.value,"dl",fd); // Value (data)
        mput(length(h.verticalalignment),"c",fd);mput(ascii(h.verticalalignment),"c",fd); // VerticalAlignment
        mput(bool2s(h.visible=="on"),"c",fd); // Visible
        mput(length(h.callback),stringFormat,fd);mput(ascii(h.callback),"c",fd); // Callback
        mput(h.callback_type,"il",fd); // Callback Type
        user_data=h.user_data;save(fd,user_data); // Userdata
        mput(length(h.tag),"c",fd);mput(ascii(h.tag),"c",fd); // Tag

    else
        warning("handle of type "+h.type+" unhandled")
    end

endfunction

function save_text_vector(t,fd)
    t=ascii(strcat(t,ascii(10)));
    mput(length(t),"il",fd);mput(t,characterFormat,fd);
endfunction

// save a text matrix
function save_text_matrix(strMat,fd)
    // put nbRow and nbCol
    nbRow = size( strMat, 1 ) ;
    nbCol = size( strMat, 2 ) ;
    mput( nbRow, "il", fd ) ;
    mput( nbCol, "il", fd ) ;
    for i = 1:nbRow
        for j = 1:nbCol
            mput(length(ascii(strMat(i,j))), stringFormat, fd) ;
            mput(ascii(strMat(i,j)),characterFormat,fd) ;
        end
    end
endfunction

function p=get_entity_path(e)
    // given a handle e on an entity this function returns its path relative
    // to its parent axes.
    // the path is a vector of child index.
    p=[];parent=e.parent;

    while  %t
        pos=find(parent.children==e,1)
        if pos==[] then
            error(msprintf(_("%s : Invalid entity %s\n"),"save","Legend"))
        end
        p=[pos p]
        if parent.type=="Axes" then break,end
        e=parent
        parent=e.parent;
    end
endfunction
