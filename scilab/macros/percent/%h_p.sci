function %h_p(h)
  if size(h,'*')>1 then
    T=matrix(h.type,size(h))
    t=''
    for k=1:size(h,2)
      t=t+part(T(:,k),1:max(length(T(:,k)))+1)
    end
    t1=string(size(h,1))+' by '+string(size(h,2))+' matrix of handles:'
    t1=[t1;part('=',ones(1,length(t1)))]
    t=[t1;t]
  else
    t='Handle of type ""'+h.type+'"" with properties:'
    t=[t;part('=',ones(1,length(t)))]
    select h.type
      case "Polyline"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible)
	 "data = "+d
	 "line_style = "+string(h.line_style)
	 "thickness = "+string(h.thickness)
	 "polyline_style = "+string(h.polyline_style)
	 "mark_mode = "+sci2exp(h.mark_mode)
	 "mark_style = "+sci2exp(h.mark_style)
	 "mark_size = "+string(h.mark_size)
	 "foreground = "+string(h.foreground)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box)]
      case "Patch"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible)
	 "data = "+d
	 "foreground = "+string(h.foreground)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box)]
      case "Agregation"
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible)]
      case "Axes"
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 " "
	 "visible = "+sci2exp(h.visible)
	 "axes_visible = "+sci2exp(h.axes_visible)
	 "grid = "+sci2exp(h.grid,0)
	 "x_location = "+sci2exp(h.x_location)
	 "y_location = "+sci2exp(h.y_location)
	 "title: "+fmtchildren(h.title)
	 "x_label: "+fmtchildren(h.x_label)
	 "y_label: "+fmtchildren(h.y_label)
	 "z_label: "+fmtchildren(h.z_label)
	 "box = "+sci2exp(h.box)
	 "sub_tics = "+sci2exp(h.sub_tics,0)
	 "tics_color = "+string(h.tics_color)
         "font_style = "+string(h.font_style)
	 "font_size = "+string(h.font_size)
	 "font_color = "+string(h.font_color)
	 " "
	 "isoview = "+sci2exp(h.isoview)
	 "cube_scaling = "+sci2exp(h.cube_scaling)
	 "view = "+sci2exp(h.view)
	 "rotation_angles = "+sci2exp(h.rotation_angles,0)
	 "log_flags = "+sci2exp(h.log_flags)
	 "tight_limits = "+sci2exp(h.tight_limits)
	 "data_bounds = "+sci2exp(h.data_bounds,0)
	 "margins = "+sci2exp(h.margins,0)
	 "axes_bounds = "+sci2exp(h.axes_bounds,0)
	 " "
	 "auto_clear = "+sci2exp(h.auto_clear)
	 "auto_scale = "+sci2exp(h.auto_scale)
	 " "
	 "hiddencolor = "+string(h.hiddencolor)
	 "line_style = "+string(h.line_style)
	 "thickness = "+string(h.thickness)
	 "mark_mode = "+sci2exp(h.mark_mode)
	 "mark_style = "+sci2exp(h.mark_style,0)
	 "mark_size = "+sci2exp(h.mark_size,0)
	 "background = "+string(h.background)
	 "foreground = "+string(h.foreground)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	]
      case "Legend"
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "foreground = "+string(h.foreground)
	 "visible = "+sci2exp(h.visible)
	 "text = "+sci2exp(h.text)
	 "font_style = "+string(h.font_style)
	 "font_size = "+string(h.font_size)]
      case "Rectangle"
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "thickness = "+string(h.thickness)
	 "mark_mode = "+sci2exp(h.mark_mode)
	 "mark_style = "+string(h.mark_style)
	 "mark_size = "+string(h.mark_size)
	 "line_style = "+string(h.line_style)
	 "fill_mode = "+sci2exp(h.fill_mode)
	 "foreground = "+string(h.foreground)
	 "data = "+sci2exp(h.data,0)
	 "visible = "+sci2exp(h.visible)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)]
      case "Arc"
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "thickness = "+string(h.thickness)
	 "line_style = "+string(h.line_style)
	 "fill_mode = "+string(h.fill_mode)
	 "foreground = "+string(h.foreground)
	 "data = "+sci2exp(h.data,0)
	 "visible = "+sci2exp(h.visible)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)]
      case "Figure"
      c=sci2exp(h.children.type,0)
      if length(c)>70 then c="matrix "+strcat(string(size(h.children)),'x'),end
      t=[t;
	 "children: "+fmtchildren(h.children)
	 "figure_style = "+sci2exp(h.figure_style,0)
	 "figure_position = "+sci2exp(h.figure_position,0)
	 "figure_size = "+sci2exp(h.figure_size,0)
	 "axes_size = "+sci2exp(h.axes_size,0)
	 "auto_resize = "+sci2exp(h.auto_resize)
	 "figure_name = "+sci2exp(h.figure_name,0)
	 "figure_id = "+sci2exp(h.figure_id,0)
         "color_map= matrix "+strcat(string(size(h.color_map)),'x')
	 "pixmap = "+sci2exp(h.pixmap)
	 "pixel_drawing_mode = "+sci2exp(h.pixel_drawing_mode,0)
	 "background =  "+string(h.background)
	 " "
	 "visible = "+sci2exp(h.visible)
	 "rotation_style = "+sci2exp(h.rotation_style)
	 ]
      case "Grayplot"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible) 
	 "data = "+d
	 "data_mapping = "+sci2exp(h.data_mapping)
	]
      case "Matplot"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible)
	 "data = "+d
	 "data_mapping = "+sci2exp(h.data_mapping)]
     
      case "Fec"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'), end
      f=sci2exp(h.triangles,0)
      if length(f)>70 then f="matrix '+strcat(string(size(h.triangles)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible) 
	 "data = "+d
	 "triangles = "+f
	 "z_bounds = "+sci2exp(h.z_bounds,0)
	]
      
      case "Segs"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      c=sci2exp(h.segs_color,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.segs_color)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible) 
	 "data = "+d
	 "line_style = "+string(h.line_style)
	 "thickness = "+string(h.thickness)
	 "arrow_size = "+string(h.arrow_size)
	 "segs_color = "+c
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)]
      case "Champ"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible) 
	 "data = "+d
	 "line_style = "+string(h.line_style)
	 "thickness = "+string(h.thickness)
	 "colored = "+sci2exp(h.colored)
	 "arrow_size = "+string(h.arrow_size)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)]
      case "Text"
      T=sci2exp(h.text,0)
      if length(T)>70 then T="string array '+strcat(string(size(h.text)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible) 
	 "text = "+T
	 "data = "+sci2exp(h.data,0)
	 "text_box = "+sci2exp(h.text_box,0)
	 "text_box_mode = "+sci2exp(h.text_box_mode,0)
	 "foreground = "+string(h.foreground)
	 "font_style = "+string(h.font_style)
	 "font_size = "+string(h.font_size)
	 "font_angle = "+string(h.font_angle)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)]
      case "Title"
      T=sci2exp(h.text,0)
      if length(T)>70 then T="string array '+strcat(string(size(h.text)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "visible = "+sci2exp(h.visible) 
	 "text = "+T
	 "foreground = "+string(h.foreground)
	 "font_style = "+string(h.font_style)
	 "font_size = "+string(h.font_size)
	 "font_angle = "+string(h.font_angle)]
      case "Label"
      T=sci2exp(h.text,0)
      if length(T)>70 then T="string array '+strcat(string(size(h.text)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "visible = "+sci2exp(h.visible) 
	 "text = "+T
	 "foreground = "+string(h.foreground)
	 "font_style = "+string(h.font_style)
	 "font_size = "+string(h.font_size)]
      case "Plot3d"
      Data = h.data
      dx=sci2exp(Data.x,0)
      if length(dx)>70 then dx="matrix '+strcat(string(size(Data.x)),'x'), end

      dy=sci2exp(Data.y,0)
      if length(dy)>70 then dy="matrix '+strcat(string(size(Data.y)),'x'), end

      dz=sci2exp(Data.z,0)
      if length(dz)>70 then dz="matrix '+strcat(string(size(Data.z)),'x'), end

      if size(Data) == 5 then // There is a color
	 dcolor=sci2exp(Data.color,0)
	 if length(dcolor)>70 then dcolor="matrix '+strcat(string(size(Data.color)),'x'), end

	t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "foreground = "+string(h.foreground)
	 "thickness = "+string(h.thickness)
	 "visible = "+sci2exp(h.visible) 
	 "data.x = "+dx
	 "data.y = "+dy
	 "data.z "+dz  
	 "data.color (not used) = "+dcolor
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	 "color_mode = "+string(h.color_mode)
	 "color_flag = "+sci2exp(h.color_flag,0)
	 "hiddencolor = "+string(h.hiddencolor)
	]
        else
	  t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "foreground = "+string(h.foreground)
	 "thickness = "+string(h.thickness)
	 "visible = "+sci2exp(h.visible) 
	 "data.x = "+dx
	 "data.y = "+dy
	 "data.z = "+dz
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	 "color_mode = "+string(h.color_mode)
	 "color_flag = "+sci2exp(h.color_flag,0)
	 "hiddencolor = "+string(h.hiddencolor)
	]
      end

      case "Fac3d" 
      Data = h.data
      dx=sci2exp(Data.x,0)
      if length(dx)>70 then dx="matrix '+strcat(string(size(Data.x)),'x'), end

      dy=sci2exp(Data.y,0)
      if length(dy)>70 then dy="matrix '+strcat(string(size(Data.y)),'x'), end

      dz=sci2exp(Data.z,0)
      if length(dz)>70 then dz="matrix '+strcat(string(size(Data.z)),'x'), end

      if size(Data) == 5 then // There is a color
	 dcolor=sci2exp(Data.color,0)
	 if length(dcolor)>70 then dcolor="matrix '+strcat(string(size(Data.color)),'x'), end

	t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "foreground = "+string(h.foreground)
	 "thickness = "+string(h.thickness)
	 "visible = "+sci2exp(h.visible) 
	 "data.x = "+dx
	 "data.y = "+dy
	 "data.z = "+dz  
	 "data.color = "+dcolor
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	 "color_mode = "+string(h.color_mode)
	 "color_flag = "+sci2exp(h.color_flag,0)
	 "hiddencolor = "+string(h.hiddencolor)
	]
        else
	  t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "foreground = "+string(h.foreground)
	 "thickness = "+string(h.thickness)
	 "visible = "+sci2exp(h.visible) 
	 "data.x = "+dx
	 "data.y = "+dy
	 "data.z = "+dz
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	 "color_mode = "+string(h.color_mode)
	 "color_flag = "+sci2exp(h.color_flag,0)
	 "hiddencolor = "+string(h.hiddencolor)
	]
      end

      case "Param3d" 
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'), end
	
      c=sci2exp(h.surface_color,0)
      if length(c)>70 then c="matrix "+ strcat(string(size(h.surface_color)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "children: "+fmtchildren(h.children)
	 "foreground = "+string(h.foreground)
	 "thickness = "+string(h.thickness)
	 "visible = "+sci2exp(h.visible) 
	 "data = "+d
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	 "color_mode = "+string(h.color_mode)
	 "surface_color = "+c
	 ]
      case "Axis"
      T=sci2exp(h.tics_labels,0)
      if length(T)>70 then T="string array'+strcat(string(size(h.text)),'x'),end
      t=[t;
	 "parent: "+h.parent.type
	 "visible = "+sci2exp(h.visible)
	 "tics_direction = "+h.tics_direction
	 "xtics_coord = "+sci2exp(h.xtics_coord,0)
	 "ytics_coord = "+sci2exp(h.ytics_coord,0)
	 "tics_color = "+string(h.tics_color)
	 "tics_segment = "+string(h.tics_segment)
	 "tics_style = "+h.tics_style
	 "sub_tics = "+string(h.sub_tics)
	 "tics_labels = "+T
	 "labels_font_size = "+string(h.labels_font_size)
	 "labels_font_color = "+string(h.labels_font_color)
	 "clip_state = "+sci2exp(h.clip_state)
	 "clip_box = "+sci2exp(h.clip_box,0)
	]

    end
  end
  write(%io(2),t)
endfunction
function t=fmtchildren(c)
  if c==[] then 
    t='[]'
  else
    t=sci2exp(c.type,0)
    if length(t)>70 then t="matrix "+strcat(string(size(c)),'x'),end 
  end
endfunction
