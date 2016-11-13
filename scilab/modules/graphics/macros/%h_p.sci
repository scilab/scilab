// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %h_p(h)
    if size(h,"*")>1 then
        T=matrix(h.type,size(h))
        t=""
        for k=1:size(h,2)
            t=t+part(T(:,k),1:max(length(T(:,k)))+1)
        end
        t1=string(size(h,1))+" by "+string(size(h,2))+" matrix of handles:"
        t1=[t1;part("=",ones(1,length(t1)))]
        t=[t1;t]
    else
        t="Handle of type """+h.type+""" with properties:"
        t=[t;part("=",ones(1,length(t)))]

        select h.type

            // Polyline
            // =====================================================================

        case "Polyline"

            if size(h.data,"*") > 10 then
                d="matrix "+strcat(string(size(h.data)),"x")
            else
                d=sci2exp(h.data,0)
                if length(d)>70 then d="matrix "+strcat(string(size(h.data)),"x"),end
            end

            if size(h.x_shift,"*") > 10 then
                bxs="matrix "+strcat(string(size(h.x_shift)),"x")
            else
                bxs=sci2exp(h.x_shift,0)
                if length(bxs)>70 then bxs="matrix "+strcat(string(size(h.x_shift)),"x"),end
            end

            if size(h.y_shift,"*") > 10 then
                bys="matrix "+strcat(string(size(h.y_shift)),"x")
            else
                bys=sci2exp(h.y_shift,0)
                if length(bys)>70 then bys="matrix "+strcat(string(size(h.y_shift)),"x"),end
            end

            if size(h.z_shift,"*") > 10 then
                bzs="matrix "+strcat(string(size(h.z_shift)),"x")
            else
                bzs=sci2exp(h.z_shift,0)
                if length(bzs)>70 then bzs="matrix "+strcat(string(size(h.z_shift)),"x"),end
            end

            if size(h.mark_size,"*") > 10 then
                msz="matrix "+strcat(string(size(h.mark_size)),"x")
            else
                msz=sci2exp(h.mark_size,0)
                if length(msz)>70 then msz="matrix "+strcat(string(size(h.mark_size)),"x"),end
            end

            if size(h.mark_foreground,"*") > 10 then
                mfg="matrix "+strcat(string(size(h.mark_foreground)),"x")
            else
                mfg=sci2exp(h.mark_foreground,0)
                if length(mfg)>70 then mfg="matrix "+strcat(string(size(h.mark_foreground)),"x"),end
            end

            if size(h.mark_background,"*") > 10 then
                mbg="matrix "+strcat(string(size(h.mark_background)),"x")
            else
                mbg=sci2exp(h.mark_background,0)
                if length(mbg)>70 then mbg="matrix "+strcat(string(size(h.mark_background)),"x"),end
            end

            u=h.user_data;
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "datatips: "+fmtchildren(h.datatips)
            "display_function = "++sci2exp(h.display_function)
            "display_function_data = "+fmtuser_data(h.display_function_data)
            "visible = "+sci2exp(h.visible)
            "data = "+d
            "closed = "+sci2exp(h.closed)
            "line_mode = "+sci2exp(h.line_mode)
            "fill_mode = "+sci2exp(h.fill_mode)
            "line_style = "+string(h.line_style)
            "thickness = "+string(h.thickness)
            "arrow_size_factor = "+string(h.arrow_size_factor)
            "polyline_style = "+string(h.polyline_style)
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "interp_color_vector = "+sci2exp(h.interp_color_vector)
            "interp_color_mode = "+sci2exp(h.interp_color_mode)
            "colors = "+sci2exp(h.colors)
            "mark_mode = "+sci2exp(h.mark_mode)
            "mark_style = "+sci2exp(h.mark_style)
            "mark_size_unit = "+sci2exp(h.mark_size_unit)
            "mark_size = "+msz
            "mark_foreground = "+mfg
            "mark_background = "+mbg
            "mark_offset = "+string(h.mark_offset)
            "mark_stride = "+string(h.mark_stride)
            "x_shift = "+bxs
            "y_shift = "+bys
            "z_shift = "+bzs
            "bar_width = "+sci2exp(h.bar_width)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Compound
            // =====================================================================

        case "Compound"
            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Axes
            // =====================================================================

        case "Axes"
            T=h.x_ticks
            if size(T.locations,"*")>9 then
                locx="matrix "+strcat(string(size(T.locations)),"x")
                labx="matrix "+strcat(string(size(T.labels)),"x")
            else
                locx=sci2exp(T.locations,0)
                labx=sci2exp(T.labels,0)
                if length(locx)>70 then locx="matrix "+strcat(string(size(T.locations)),"x"), end
                if length(labx)>70 then labx="matrix "+strcat(string(size(T.labels)),"x"), end
            end

            T=h.y_ticks
            if size(T.locations,"*")>9 then
                locy="matrix "+strcat(string(size(T.locations)),"x")
                laby="matrix "+strcat(string(size(T.labels)),"x")
            else
                locy=sci2exp(T.locations,0)
                laby=sci2exp(T.labels,0)
                if length(locy)>70 then locx="matrix "+strcat(string(size(T.locations)),"x"), end
                if length(laby)>70 then labx="matrix "+strcat(string(size(T.labels)),"x"), end
            end

            T=h.z_ticks
            if size(T.locations,"*")>9 then
                locz="matrix "+strcat(string(size(T.locations)),"x")
                labz="matrix "+strcat(string(size(T.labels)),"x")
            else
                locz=sci2exp(T.locations,0)
                labz=sci2exp(T.labels,0)
                if length(locz)>70 then locx="matrix "+strcat(string(size(T.locations)),"x"), end
                if length(labz)>70 then labx="matrix "+strcat(string(size(T.labels)),"x"), end
            end

            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            " "
            "visible = "+sci2exp(h.visible)
            "axes_visible = "+sci2exp(h.axes_visible)
            "axes_reverse = "+sci2exp(h.axes_reverse)
            "grid = "+sci2exp(h.grid,0)
            "grid_position = "+sci2exp(h.grid_position)
            "grid_thickness = "+sci2exp(h.grid_thickness,0)
            "grid_style = "+sci2exp(h.grid_style,0)
            "x_location = "+sci2exp(h.x_location)
            "y_location = "+sci2exp(h.y_location)
            "title: "+fmtchildren(h.title)
            "x_label: "+fmtchildren(h.x_label)
            "y_label: "+fmtchildren(h.y_label)
            "z_label: "+fmtchildren(h.z_label)
            "auto_ticks = "+sci2exp(h.auto_ticks)
            "x_ticks.locations = "+locx
            "y_ticks.locations = "+locy
            "z_ticks.locations = "+locz
            "x_ticks.labels = "+labx
            "y_ticks.labels = "+laby
            "z_ticks.labels = "+labz
            "ticks_format = "+sci2exp(h.ticks_format)
            "ticks_st = "+sci2exp(h.ticks_st, 0)
            "box = "+sci2exp(h.box)
            "filled = " + sci2exp(h.filled)
            "sub_ticks = "+sci2exp(h.sub_ticks,0)
            "font_style = "+string(h.font_style)
            "font_size = "+string(h.font_size)
            "font_color = "+string(h.font_color)
            "fractional_font = " + sci2exp(h.fractional_font)
            " "
            "isoview = "+sci2exp(h.isoview)
            "cube_scaling = "+sci2exp(h.cube_scaling)
            "view = "+sci2exp(h.view)
            "rotation_angles = "+sci2exp(h.rotation_angles,0)
            "log_flags = "+sci2exp(h.log_flags)
            "tight_limits = "+sci2exp(h.tight_limits)
            "data_bounds = "+sci2exp(h.data_bounds,0)
            "zoom_box = "+sci2exp(h.zoom_box,0)
            "margins = "+sci2exp(h.margins,0)
            "auto_margins = "+sci2exp(h.auto_margins)
            "axes_bounds = "+sci2exp(h.axes_bounds,0)
            " "
            "auto_clear = "+sci2exp(h.auto_clear)
            "auto_scale = "+sci2exp(h.auto_scale)
            " "
            "hidden_axis_color = " + string(h.hidden_axis_color);
            "hiddencolor = "+string(h.hiddencolor)
            "line_mode = "+sci2exp(h.line_mode)
            "line_style = "+string(h.line_style)
            "thickness = "+string(h.thickness)
            "mark_mode = "+sci2exp(h.mark_mode)
            "mark_style = "+sci2exp(h.mark_style,0)
            "mark_size_unit = "+sci2exp(h.mark_size_unit)
            "mark_size = "+sci2exp(h.mark_size,0)
            "mark_foreground = "+string(h.mark_foreground)
            "mark_background = "+string(h.mark_background)
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "arc_drawing_method = " + sci2exp(h.arc_drawing_method)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(h.user_data)
            "tag = "+h.tag
            ]

            // Legend
            // =====================================================================

        case "Legend"
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "text = "+sci2exp(h.text)
            "font_style = "+string(h.font_style)
            "font_size = "+string(h.font_size)
            "font_color = "+string(h.font_color)
            "fractional_font = " + sci2exp(h.fractional_font)
            "links = " +  fmtchildren(h.links)
            "legend_location = " + sci2exp(h.legend_location)
            "position = " + sci2exp(h.position)
            "line_width = "+sci2exp(h.line_width)
            "line_mode = "+sci2exp(h.line_mode)
            "thickness = "+string(h.thickness)
            "foreground = "+string(h.foreground)
            "fill_mode = "+sci2exp(h.fill_mode)
            "background = "+string(h.background)
            "marks_count = "+sci2exp(h.marks_count)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(h.user_data)
            "tag = "+h.tag
            ]

            // Rectangle
            // =====================================================================

        case "Rectangle"
            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "mark_mode = "+sci2exp(h.mark_mode)
            "mark_style = "+string(h.mark_style)
            "mark_size_unit = "+sci2exp(h.mark_size_unit)
            "mark_size = "+string(h.mark_size)
            "mark_foreground = "+string(h.mark_foreground)
            "mark_background = "+string(h.mark_background)
            "line_mode = "+sci2exp(h.line_mode)
            "fill_mode = "+sci2exp(h.fill_mode)
            "line_style = "+string(h.line_style)
            "thickness = "+string(h.thickness)
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "data = "+sci2exp(h.data,0)
            "visible = "+sci2exp(h.visible)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Arc
            // =====================================================================

        case "Arc"
            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "thickness = "+string(h.thickness)
            "line_style = "+string(h.line_style)
            "line_mode = "+sci2exp(h.line_mode)
            "fill_mode = "+sci2exp(h.fill_mode)
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "data = "+sci2exp(h.data,0)
            "visible = "+sci2exp(h.visible)
            "arc_drawing_method = " + sci2exp(h.arc_drawing_method)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Figure
            // =====================================================================

        case "Figure"
            l = h.layout_options;
            if type(l) == 16 then //tlist
                l = l(1);
                l = l(1);
            end
            u=h.user_data
            t=[t;
            "children: "+fmtchildren(h.children)
            "figure_position = "+sci2exp(h.figure_position,0)
            "figure_size = "+sci2exp(h.figure_size,0)
            "axes_size = "+sci2exp(h.axes_size,0)
            "auto_resize = "+sci2exp(h.auto_resize)
            "viewport = "+sci2exp(h.viewport)
            "figure_name = "+sci2exp(h.figure_name,0)
            "figure_id = "+sci2exp(h.figure_id,0)
            "info_message = "+sci2exp(h.info_message)
            "color_map = matrix "+strcat(string(size(h.color_map)),"x")
            "pixel_drawing_mode = "+sci2exp(h.pixel_drawing_mode,0)
            "anti_aliasing = " + sci2exp(h.anti_aliasing)
            "immediate_drawing = "+sci2exp(h.immediate_drawing)
            "background =  "+string(h.background)
            "visible = "+sci2exp(h.visible)
            "rotation_style = "+sci2exp(h.rotation_style)
            "event_handler = " + sci2exp( h.event_handler ) ;
            "event_handler_enable = " + sci2exp( h.event_handler_enable ) ;
            "user_data = "+fmtuser_data(u)
            "resizefcn = "+sci2exp(h.resizefcn)
            "closerequestfcn = "+sci2exp(h.closerequestfcn)
            "resize = "+sci2exp(h.resize)
            "toolbar = "+sci2exp(h.toolbar)
            "toolbar_visible = "+sci2exp(h.toolbar_visible)
            "menubar = "+sci2exp(h.menubar)
            "menubar_visible = "+sci2exp(h.menubar_visible)
            "infobar_visible = "+sci2exp(h.infobar_visible)
            "dockable = "+sci2exp(h.dockable)
            "layout = "+sci2exp(h.layout)
            "layout_options = "+sci2exp(l)
            "default_axes = "+sci2exp(h.default_axes)
            "icon = "+fmtuser_data(h.icon)
            "tag = "+sci2exp(h.tag)
            ]

            // Grayplot
            // =====================================================================

        case "Grayplot"
            Data = h.data
            if size(Data.x,"*") > 10 then
                dx="matrix "+strcat(string(size(Data.x)),"x")
            else
                dx=sci2exp(Data.x,0)
                if length(dx)>70 then d="matrix "+strcat(string(size(Data.x)),"x"),end
            end

            if size(Data.y,"*") > 10 then
                dy="matrix "+strcat(string(size(Data.y)),"x")
            else
                dy=sci2exp(Data.y,0)
                if length(dy)>70 then d="matrix "+strcat(string(size(Data.y)),"x"),end
            end

            if size(Data.z,"*") > 10 then
                dz="matrix "+strcat(string(size(Data.z)),"x")
            else
                dz=sci2exp(Data.z,0)
                if length(dz)>70 then d="matrix "+strcat(string(size(Data.z)),"x"),end
            end

            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "data.x = "+dx
            "data.y = "+dy
            "data.z = "+dz
            "data_mapping = "+sci2exp(h.data_mapping)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Matplot
            // =====================================================================


        case "Matplot"
            if size(h.data,"*") > 10 then
                d="matrix "+strcat(string(size(h.data)),"x")
            else
                d=sci2exp(h.data,0)
                if length(d)>70 then d="matrix "+strcat(string(size(h.data)),"x"),end
            end
            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "data = "+d
            "rect = " + sci2exp(h.rect, 0)
            "image_type = " + sci2exp(h.image_type)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Fec
            // =====================================================================
        case "Fec"
            u=h.user_data
            if size(h.data,"*") > 10 then
                d="matrix "+strcat(string(size(h.data)),"x")
            else
                d=sci2exp(h.data,0)
                if length(d)>70 then d="matrix "+strcat(string(size(h.data)),"x"),end
            end

            if size(h.triangles,"*") > 10 then
                f="matrix "+strcat(string(size(h.triangles)),"x")
            else
                f=sci2exp(h.triangles,0)
                if length(f)>70 then f="matrix "+strcat(string(size(h.triangles)),"x"),end
            end

            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "data = "+d
            "triangles = "+f
            "z_bounds = "+sci2exp(h.z_bounds,0)
            "color_range = " + sci2exp(h.color_range, 0);
            "outside_colors = " + sci2exp(h.outside_colors, 0);
            "line_mode = " + sci2exp(h.line_mode);
            "foreground = " + sci2exp(h.foreground);
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Segs
            // =====================================================================

        case "Segs"
            if size(h.data,"*") > 10 then
                d="matrix "+strcat(string(size(h.data)),"x")
            else
                d=sci2exp(h.data,0)
                if length(d)>70 then d="matrix "+strcat(string(size(h.data)),"x"),end
            end

            if size(h.segs_color,"*") > 10 then
                c="matrix "+strcat(string(size(h.segs_color)),"x")
            else
                c=sci2exp(h.segs_color,0)
                if length(c)>70 then c="matrix "+strcat(string(size(h.segs_color)),"x"),end
            end

            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "data = "+d
            "line_mode = "+sci2exp(h.line_mode)
            "line_style = "+string(h.line_style)
            "thickness = "+string(h.thickness)
            "arrow_size = "+string(h.arrow_size)
            "segs_color = "+c
            "mark_mode = "+sci2exp(h.mark_mode)
            "mark_style = "+sci2exp(h.mark_style)
            "mark_size_unit = "+sci2exp(h.mark_size_unit)
            "mark_size = "+string(h.mark_size)
            "mark_foreground = "+string(h.mark_foreground)
            "mark_background = "+string(h.mark_background)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Champ
            // =====================================================================

        case "Champ"
            Data = h.data
            if size(Data.x,"*") > 10 then
                dx="matrix "+strcat(string(size(Data.x)),"x")
            else
                dx=sci2exp(Data.x,0)
                if length(dx)>70 then d="matrix "+strcat(string(size(Data.x)),"x"),end
            end

            if size(Data.y,"*") > 10 then
                dy="matrix "+strcat(string(size(Data.y)),"x")
            else
                dy=sci2exp(Data.y,0)
                if length(dy)>70 then d="matrix "+strcat(string(size(Data.y)),"x"),end
            end

            if size(Data.fx,"*") > 10 then
                dfx="matrix "+strcat(string(size(Data.fx)),"x")
            else
                dfx=sci2exp(Data.fx,0)
                if length(dfx)>70 then d="matrix "+strcat(string(size(Data.fx)),"x"),end
            end

            if size(Data.fy,"*") > 10 then
                dfy="matrix "+strcat(string(size(Data.fy)),"x")
            else
                dfy=sci2exp(Data.fy,0)
                if length(dfy)>70 then d="matrix "+strcat(string(size(Data.fy)),"x"),end
            end

            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "data.x = "+dx
            "data.y = "+dy
            "data.fx = "+dfx
            "data.fy = "+dfy
            "line_style = "+string(h.line_style)
            "thickness = "+string(h.thickness)
            "colored = "+sci2exp(h.colored)
            "arrow_size = "+string(h.arrow_size)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Text
            // =====================================================================

        case "Text"
            u=h.user_data
            T=sci2exp(h.text,0)
            if length(T)>70 then T="string array "+strcat(string(size(h.text)),"x"),end
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "text = "+T
            "alignment = "+sci2exp(h.alignment) ;
            "data = "+sci2exp(h.data,0)
            "box = "+sci2exp(h.box)
            "line_mode = " + sci2exp(h.line_mode);
            "fill_mode = " + sci2exp(h.fill_mode);
            "text_box = "+sci2exp(h.text_box,0)
            "text_box_mode = "+sci2exp(h.text_box_mode,0)
            "font_foreground = "+string(h.font_foreground)
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "font_style = "+string(h.font_style)
            "font_size = "+string(h.font_size)
            "fractional_font = " + sci2exp(h.fractional_font)
            "auto_dimensionning = " + sci2exp(h.auto_dimensionning)
            "font_angle = "+string(h.font_angle)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Datatip
            // =====================================================================

        case "Datatip"
            u=h.user_data
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "data = "+sci2exp(h.data)
            "box_mode = "+sci2exp(h.box_mode)
            "label_mode = "+sci2exp(h.label_mode)
            "orientation = "+sci2exp(h.orientation)
            "display_components = "+sci2exp(h.display_components)
            "auto_orientation = "+sci2exp(h.auto_orientation)
            "interp_mode = "+sci2exp(h.interp_mode)
            "display_function = "+sci2exp(h.display_function)
            "detached_position = "+sci2exp(h.detached_position)
            "font_foreground = "+string(h.font_foreground)
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "line_style = "+string(h.line_style)
            "mark_mode = "+sci2exp(h.mark_mode)
            "mark_style = "+sci2exp(h.mark_style)
            "mark_size_unit = "+sci2exp(h.mark_size_unit)
            "mark_size = "+string(h.mark_size)
            "mark_foreground = "+string(h.mark_foreground)
            "mark_background = "+string(h.mark_background)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Title
            // =====================================================================

        case "Title"
            T=sci2exp(h.text,0)
            if length(T)>70 then T="string array "+strcat(string(size(h.text)),"x"),end
            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "text = "+T
            "foreground = "+string(h.foreground)
            "font_style = "+string(h.font_style)
            "font_size = "+string(h.font_size)
            "fractional_font = " + sci2exp(h.fractional_font)
            "font_angle = "+string(h.font_angle)
            "tag = "+h.tag
            ]

            // Label
            // =====================================================================

        case "Label"
            T=sci2exp(h.text,0)
            if length(T)>70 then T="string array "+strcat(string(size(h.text)),"x"),end
            t=[t;
            "parent: "+h.parent.type
            "visible = "+sci2exp(h.visible)
            "text = "+T;
            "font_foreground = " + string(h.font_foreground) ;
            "foreground = "+string(h.foreground)
            "background = "+string(h.background)
            "fill_mode = "+sci2exp(h.fill_mode)
            "font_style = "+string(h.font_style)
            "font_size = "+string(h.font_size)
            "fractional_font = " + sci2exp(h.fractional_font)
            "font_angle = "+string(h.font_angle)
            "auto_position = "+sci2exp(h.auto_position)
            "position = "+sci2exp(h.position,0)
            "auto_rotation = "+sci2exp(h.auto_rotation)
            "tag = "+h.tag
            ]

            // Plot3d
            // =====================================================================
        case "Plot3d"
            Data = h.data
            if size(Data.x,"*") > 10 then
                dx="matrix "+strcat(string(size(Data.x)),"x")
            else
                dx=sci2exp(Data.x,0)
                if length(dx)>70 then d="matrix "+strcat(string(size(Data.x)),"x"),end
            end

            if size(Data.y,"*") > 10 then
                dy="matrix "+strcat(string(size(Data.y)),"x")
            else
                dy=sci2exp(Data.y,0)
                if length(dy)>70 then d="matrix "+strcat(string(size(Data.y)),"x"),end
            end

            if size(Data.z,"*") > 10 then
                dz="matrix "+strcat(string(size(Data.z)),"x")
            else
                dz=sci2exp(Data.z,0)
                if length(dz)>70 then d="matrix "+strcat(string(size(Data.z)),"x"),end
            end

            u=h.user_data

            if size(Data) == 5 then // There is a color
                if size(Data.color,"*") > 10 then
                    dcolor="matrix "+strcat(string(size(Data.color)),"x")
                else
                    dcolor=sci2exp(Data.color,0)
                    if length(dcolor)>70 then dcolor="matrix "+strcat(string(size(Data.color)),"x"),end
                end

                t=[t;
                "parent: "+h.parent.type
                "children: "+fmtchildren(h.children)
                "visible = "+sci2exp(h.visible)
                "surface_mode = "+sci2exp(h.surface_mode)
                "foreground = "+string(h.foreground)
                "thickness = "+string(h.thickness)
                "mark_mode = "+sci2exp(h.mark_mode)
                "mark_style = "+sci2exp(h.mark_style)
                "mark_size_unit = "+sci2exp(h.mark_size_unit)
                "mark_size = "+string(h.mark_size)
                "mark_foreground = "+string(h.mark_foreground)
                "mark_background = "+string(h.mark_background)
                "data.x = "+dx
                "data.y = "+dy
                "data.z "+dz
                "data.color (not used) = "+dcolor
                "color_mode = "+string(h.color_mode)
                "color_flag = "+sci2exp(h.color_flag,0)
                "hiddencolor = "+string(h.hiddencolor)
                "clip_state = "+sci2exp(h.clip_state)
                "clip_box = "+sci2exp(h.clip_box,0)
                "ambient_color = "+sci2exp(h.ambient_color)
                "diffuse_color = "+sci2exp(h.diffuse_color)
                "specular_color = "+sci2exp(h.specular_color)
                "use_color_material = "+sci2exp(h.use_color_material)
                "material_shininess = "+sci2exp(h.material_shininess)
                "user_data = "+fmtuser_data(u)
                "tag = "+h.tag
                ]
            else
                t=[t;
                "parent: "+h.parent.type
                "children: "+fmtchildren(h.children)
                "visible = "+sci2exp(h.visible)
                "surface_mode = "+sci2exp(h.surface_mode)
                "foreground = "+string(h.foreground)
                "thickness = "+string(h.thickness)
                "mark_mode = "+sci2exp(h.mark_mode)
                "mark_style = "+sci2exp(h.mark_style)
                "mark_size_unit = "+sci2exp(h.mark_size_unit)
                "mark_size = "+string(h.mark_size)
                "mark_foreground = "+string(h.mark_foreground)
                "mark_background = "+string(h.mark_background)
                "data.x = "+dx
                "data.y = "+dy
                "data.z = "+dz
                "color_mode = "+string(h.color_mode)
                "color_flag = "+sci2exp(h.color_flag,0)
                "hiddencolor = "+string(h.hiddencolor)
                "clip_state = "+sci2exp(h.clip_state)
                "clip_box = "+sci2exp(h.clip_box,0)
                "ambient_color = "+sci2exp(h.ambient_color)
                "diffuse_color = "+sci2exp(h.diffuse_color)
                "specular_color = "+sci2exp(h.specular_color)
                "use_color_material = "+sci2exp(h.use_color_material)
                "material_shininess = "+sci2exp(h.material_shininess)
                "user_data = "+fmtuser_data(u)
                "tag = "+h.tag
                ]
            end

            // Fac3d
            // =====================================================================

        case "Fac3d"
            Data = h.data
            if size(Data.x,"*") > 10 then
                dx="matrix "+strcat(string(size(Data.x)),"x")
            else
                dx=sci2exp(Data.x,0)
                if length(dx)>70 then d="matrix "+strcat(string(size(Data.x)),"x"),end
            end

            if size(Data.y,"*") > 10 then
                dy="matrix "+strcat(string(size(Data.y)),"x")
            else
                dy=sci2exp(Data.y,0)
                if length(dy)>70 then d="matrix "+strcat(string(size(Data.y)),"x"),end
            end

            if size(Data.z,"*") > 10 then
                dz="matrix "+strcat(string(size(Data.z)),"x")
            else
                dz=sci2exp(Data.z,0)
                if length(dz)>70 then d="matrix "+strcat(string(size(Data.z)),"x"),end
            end

            u=h.user_data;

            if size(Data) == 5 then // There is a color
                if size(Data.color,"*") > 10 then
                    dcolor="matrix "+strcat(string(size(Data.color)),"x")
                else
                    dcolor=sci2exp(Data.color,0)
                    if length(dcolor)>70 then dcolor="matrix "+strcat(string(size(Data.color)),"x"),end
                end

                t=[t;
                "parent: "+h.parent.type
                "children: "+fmtchildren(h.children)
                "visible = "+sci2exp(h.visible)
                "surface_mode = "+sci2exp(h.surface_mode)
                "foreground = "+string(h.foreground)
                "thickness = "+string(h.thickness)
                "mark_mode = "+sci2exp(h.mark_mode)
                "mark_style = "+sci2exp(h.mark_style)
                "mark_size_unit = "+sci2exp(h.mark_size_unit)
                "mark_size = "+string(h.mark_size)
                "mark_foreground = "+string(h.mark_foreground)
                "mark_background = "+string(h.mark_background)
                "data.x = "+dx
                "data.y = "+dy
                "data.z = "+dz
                "data.color = "+dcolor
                "color_mode = "+string(h.color_mode)
                "color_flag = "+sci2exp(h.color_flag,0)
                "cdata_mapping = "+sci2exp(h.cdata_mapping)
                "hiddencolor = "+string(h.hiddencolor)
                "clip_state = "+sci2exp(h.clip_state)
                "clip_box = "+sci2exp(h.clip_box,0)
                "ambient_color = "+sci2exp(h.ambient_color)
                "diffuse_color = "+sci2exp(h.diffuse_color)
                "specular_color = "+sci2exp(h.specular_color)
                "use_color_material = "+sci2exp(h.use_color_material)
                "material_shininess = "+sci2exp(h.material_shininess)
                "user_data = "+fmtuser_data(u)
                "tag = "+h.tag
                ]
            else
                t=[t;
                "parent: "+h.parent.type
                "children: "+fmtchildren(h.children)
                "visible = "+sci2exp(h.visible)
                "surface_mode = "+sci2exp(h.surface_mode)
                "foreground = "+string(h.foreground)
                "thickness = "+string(h.thickness)
                "mark_mode = "+sci2exp(h.mark_mode)
                "mark_style = "+sci2exp(h.mark_style)
                "mark_size_unit = "+sci2exp(h.mark_size_unit)
                "mark_size = "+string(h.mark_size)
                "mark_foreground = "+string(h.mark_foreground)
                "mark_background = "+string(h.mark_background)
                "data.x = "+dx
                "data.y = "+dy
                "data.z = "+dz
                "color_mode = "+string(h.color_mode)
                "color_flag = "+sci2exp(h.color_flag,0)
                "hiddencolor = "+string(h.hiddencolor)
                "clip_state = "+sci2exp(h.clip_state)
                "clip_box = "+sci2exp(h.clip_box,0)
                "ambient_color = "+sci2exp(h.ambient_color)
                "diffuse_color = "+sci2exp(h.diffuse_color)
                "specular_color = "+sci2exp(h.specular_color)
                "usecolor_material = "+sci2exp(h.use_color_material)
                "material_shininess = "+sci2exp(h.material_shininess)
                "user_data = "+fmtuser_data(u)
                "tag = "+h.tag
                ]
            end

            // Param3d
            // =====================================================================

        case "Param3d"
            u=h.user_data;
            if size(h.data,"*") > 10 then
                d="matrix "+strcat(string(size(h.data)),"x")
            else
                d=sci2exp(h.data,0)
                if length(d)>70 then d="matrix "+strcat(string(size(h.data)),"x"),end
            end

            if size(h.surface_color,"*") > 10 then
                c="matrix "+strcat(string(size(h.surface_color)),"x")
            else
                c=sci2exp(h.surface_color,0)
                if length(c)>70 then c="matrix "+strcat(string(size(h.surface_color)),"x"),end
            end

            t=[t;
            "parent: "+h.parent.type
            "children: "+fmtchildren(h.children)
            "visible = "+sci2exp(h.visible)
            "line_mode = "+sci2exp(h.line_mode)
            "foreground = "+string(h.foreground)
            "thickness = "+string(h.thickness)
            "mark_mode = "+sci2exp(h.mark_mode)
            "mark_style = "+sci2exp(h.mark_style)
            "mark_size_unit = "+sci2exp(h.mark_size_unit)
            "mark_size = "+string(h.mark_size)
            "mark_foreground = "+string(h.mark_foreground)
            "mark_background = "+string(h.mark_background)
            "data = "+d
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "color_mode = "+string(h.color_mode)
            "surface_color = "+c
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // Axis
            // =====================================================================

        case "Axis"
            u=h.user_data;
            T=sci2exp(h.tics_labels,0)
            if length(T)>70 then T="string array "+strcat(string(size(h.tics_labels)),"x"),end
            t=[t;
            "parent: "+h.parent.type
            "visible = "+sci2exp(h.visible)
            "tics_direction = "+sci2exp(h.tics_direction)
            "xtics_coord = "+sci2exp(h.xtics_coord,0)
            "ytics_coord = "+sci2exp(h.ytics_coord,0)
            "tics_color = "+string(h.tics_color)
            "tics_segment = "+sci2exp(h.tics_segment)
            "tics_style = "+sci2exp(h.tics_style)
            "sub_tics = "+string(h.sub_tics)
            "tics_labels = "+T
            "format_n = "+sci2exp(h.format_n)
            "labels_font_size = "+string(h.labels_font_size)
            "labels_font_color = "+string(h.labels_font_color)
            "labels_font_style = "+string(h.labels_font_style)
            "fractional_font = " + sci2exp(h.fractional_font)
            "clip_state = "+sci2exp(h.clip_state)
            "clip_box = "+sci2exp(h.clip_box,0)
            "user_data = "+fmtuser_data(u)
            "tag = "+h.tag
            ]

            // waitbar
            // =====================================================================

        case "Waitbar"
            u=h.user_data;
            t=[t;
            "Userdata = "+fmtuser_data(u)
            "Tag = "+h.tag
            ]

            // progressionbar
            // =====================================================================

        case "Progressionbar"
            u=h.user_data;
            t=[t;
            "Userdata = "+fmtuser_data(u)
            "Tag = "+h.tag
            ]

            // uimenu
            // =====================================================================

        case "uimenu"
            u=h.user_data;
            t=[t;
            "Parent: "+h.parent.type
            "Children: "+fmtchildren(h.children)
            "Enable = "+sci2exp(h.enable)
            "Foregroundcolor = "+sci2exp(h.foregroundcolor,0)
            "Label = "+h.label
            "Handle_Visible = "+sci2exp(h.handle_visible)
            "Visible = "+sci2exp(h.visible)
            "Callback = "+h.callback
            "Callback_Type = "+sci2exp(h.callback_type,0)
            "Checked =  "+sci2exp(h.checked)
            "Icon =  "+sci2exp(h.icon)
            "Userdata = "+fmtuser_data(u)
            "Tag = "+h.tag
            ]

            // uicontextmenu
            // =====================================================================

        case "uicontextmenu"
            if isempty(h.parent) then // For an uicontextmenu, parent can be empty
                parent = "";
            else
                parent = h.parent.type;
            end
            t=[t;
            "Parent: "+parent
            "Children: "+fmtchildren(h.children)
            ]

            // uicontrol
            // =====================================================================

        case "uicontrol"

            u=h.user_data;
            c = h.constraints;
            if type(c) == 16 then //tlist
                c = c(1);
                c = c(1);
            end
            l = h.layout_options;
            if type(l) == 16 then //tlist
                l = l(1);
                l = l(1);
            end
            b = h.border;
            if type(b) == 16 then //tlist
                b = b(1);
                b = b(1);
            end

            showHiddenProperties = get(get(0), "ShowHiddenProperties") == "on";
            t($ + 1) = "Parent: " + h.parent.type;
            t($ + 1) = "Children: " + fmtchildren(h.children);
            t($ + 1) = "Style = " + h.style;
            if and(h.style <> ["popupmenu"]) | showHiddenProperties then
                t($ + 1) = "BackgroundColor = " + sci2exp(h.backgroundcolor,0);
            end
            if h.style == "frame" | showHiddenProperties then
                t($ + 1) = "Border = " + sci2exp(b);
            end
            if and(h.style <> ["frame", "layer", "text"]) | showHiddenProperties then
                t($ + 1) = "Callback = " + h.callback;
                t($ + 1) = "Callback_Type = " + sci2exp(h.callback_type,0);
            end
            t($ + 1) = "Constraints = " + sci2exp(c);
            t($ + 1) = "Enable = " + sci2exp(h.enable);
            if and(h.style <> ["image", "slider", "layer"]) | showHiddenProperties then
                t($ + 1) = "FontAngle = " + h.fontangle;
                t($ + 1) = "FontName = " + h.fontname;
                t($ + 1) = "FontSize = " + sci2exp(h.fontsize);
                t($ + 1) = "FontUnits = " + h.fontunits;
                t($ + 1) = "FontWeight = " + h.fontweight;
            end
            if and(h.style <> ["frame", "layer", "tab", "slider", "image", "popupmenu"]) | showHiddenProperties then
                t($ + 1) = "ForegroundColor = " + sci2exp(h.foregroundcolor,0);
            end
            if or(h.style == ["radiobutton", "checkbox"]) | showHiddenProperties then
                t($ + 1) = "Groupname = " + sci2exp(h.groupname);
            end
            if and(h.style <> ["frame", "layer", "tab", "listbox", "popupmenu"]) | showHiddenProperties then
                t($ + 1) = "HorizontalAlignment = " + h.horizontalalignment;
            end
            if or(h.style == ["text", "pushbutton", "frame"]) | showHiddenProperties then
                t($ + 1) = "Icon = " + fmtuser_data(h.icon);
            end
            if h.style == "frame" | showHiddenProperties then
                t($ + 1) = "Layout = " + sci2exp(h.layout);
                t($ + 1) = "Layout_options = " + sci2exp(l);
            end
            if h.style == "listbox" | showHiddenProperties then
                t($ + 1) = "ListboxTop = " + sci2exp(h.listboxtop,0);
            end
            t($ + 1) = "Margins = " + sci2exp(h.margins);
            if or(h.style == ["checkbox", "radiobutton", "slider", "spinner", "listbox", "edit"]) | showHiddenProperties then
                t($ + 1) = "Max = " + sci2exp(h.max);
                t($ + 1) = "Min = " + sci2exp(h.min);
            end
            t($ + 1) = "Position = " + sci2exp(h.position,0);
            t($ + 1) = "Relief = " + h.relief;
            if or(h.style == ["frame", "edit"]) | showHiddenProperties then
                t($ + 1) = "Scrollable = " + sci2exp(h.scrollable);
            end
            if or(h.style == ["slider", "spinner"]) | showHiddenProperties then
                t($ + 1) = "SliderStep = " + sci2exp(h.sliderstep);
            end
            if h.style <> "slider" | showHiddenProperties then
                t($ + 1) = "String = " + fmtuser_data(h.string);
            end
            t($ + 1) = "Tag = " + h.tag;
            if h.style == "tab" | showHiddenProperties then
                t($ + 1) = "Title_position = " + sci2exp(h.title_position);
                t($ + 1) = "Title_scroll = " + sci2exp(h.title_scroll);
            end
            t($ + 1) = "TooltipString = " + fmtuser_data(h.tooltipstring);
            t($ + 1) = "Units = " + h.units;
            t($ + 1) = "Userdata = " + fmtuser_data(u);
            if or(h.style == ["checkbox", "radiobutton", "slider", "spinner", "listbox", "edit", "layer", "tab", "popupmenu"]) | showHiddenProperties then
                t($ + 1) = "Value = " + sci2exp(h.value,0);
            end

            if and(h.style <> ["frame", "layer", "tab", "listbox", "popupmenu"]) | showHiddenProperties then
                t($ + 1) = "VerticalAlignment = " + h.verticalalignment;
            end
            t($ + 1) = "Visible = " + sci2exp(h.visible);

            // Console
            // =====================================================================

        case "Console"
            t=[t;
            "Children: "+fmtchildren(h.children)
            "ShowHiddenHandles = "+sci2exp(h.showhiddenhandles)
            "ShowHiddenProperties = "+sci2exp(h.showhiddenproperties)
            "UseDeprecatedSkin = "+sci2exp(h.usedeprecatedskin)
            ]
            // Light
            // =====================================================================

        case "Light"
            t=[t;
            "parent: "+h.parent.type
            "visible = "+sci2exp(h.visible)
            "light_type = "+sci2exp(h.light_type)
            "position = "+sci2exp(h.position)
            "direction = "+sci2exp(h.direction)
            "ambient_color = "+sci2exp(h.ambient_color)
            "diffuse_color = "+sci2exp(h.diffuse_color)
            "specular_color= "+sci2exp(h.specular_color)
            "user_data = "+fmtuser_data(h.userdata)
            "tag = "+sci2exp(h.tag)
            ]
        end
    end
    printf("%s\n", t);
endfunction




function t=fmtchildren(c)
    if c==[] then
        t="[]"
    else
        if size(c,"*")<10 then
            t=sci2exp(c.type,0);
            if length(t)>70 then t="matrix "+strcat(string(size(c)),"x"),end
        else
            t="matrix "+strcat(string(size(c)),"x")
        end
    end
endfunction


function t=fmtuser_data(d)
    td= type(d)
    if or(td==[1 2 3 4 8 10 15]) then
        t=sci2exp(d,0)
        if length(t)>70 then
            if type(d) <> 15 then
                t=typeof(d)+" "+strcat(string(size(d)),"x");
            else
                t=typeof(d)+"(";
                for i=1:length(d)
                    if type(d(i)) <> 9 then
                        t=t+typeof(d(i))+", ";
                    else
                        t=t+d(i).type +", ";
                    end
                end
                t=part(t,1:length(t)-2);
                t=t+")";
            end
        end
    elseif or(td==16) then
        t=typeof(d)+" "+strcat(string(size(d)),"x"),
    elseif or(td==17) then
        if typeof(d)=="ce" then
            t="cell "+strcat(string(size(d)),"x"),
        elseif typeof(d)=="st" then
            T=getfield(1,d)
            t="struct with fields: "+strcat(T(3:$)," "),
        else
            t=typeof(d)
        end
    else
        t=typeof(d)
    end
endfunction
