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
	 "polyline_style: "+string(h.polyline_style)
	 "thickness: "+string(h.thickness)
	 "mark_mode: "+string(h.mark_mode)
	 "mark_style: "+string(h.mark_style)
	 "line_style: "+string(h.line_style)
	 "fill_mode: "+string(h.fill_mode)
	 "foreground: "+string(h.foreground)
	 "data: "+d
	 "visible: "+string(h.visible)
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box)]
      case "Patch"
      d=sci2exp(h.data,0)
      if length(d)>70 then d="matrix '+strcat(string(size(h.data)),'x'),end
      t=[t;
	 "thickness: "+string(h.thickness)
	 "mark_mode: "+string(h.mark_mode)
	 "mark_style: "+string(h.mark_style)
	 "line_style: "+string(h.line_style)
	 "fill_mode: "+string(h.fill_mode)
	 "foreground: "+string(h.foreground)
	 "data: "+d
	 "visible: "+string(h.visible)
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box)]
      case "Agregation"
      t=[t;
	 "children: "+sci2exp(h.children.type,0)]
      case "Axes"
      t=[t;
	 "children: "+sci2exp(h.children.type,0)
	 "foreground: "+string(h.foreground)
	 "data: "+sci2exp(h.data,0)
	 "visible: "+string(h.visible)
	 ""
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box)
	 ""
	 "axes_visible: "+string(h.axes_visible)
	 "grid: "+sci2exp(h.grid)
	 "tight_limits: "+h.tight_limits
	 "tics_color: "+string(h.tics_color)
	 "tics_style: "+h.tics_style
	 "tics_textsize: "+string(h.tics_textsize)
	 "x_location: "+h.x_location
	 "y_location: "+h.y_location
	 "sub_tics: "+sci2exp(h.sub_tics)
	 ""
	 "log_flags: "+h.log_flags
	 "auto_clear: "+h.auto_clear
	 "auto_scale: "+h.auto_scale
	 ""
	 "mark_mode: "+string(h.mark_mode)
	 "mark_style: "+string(h.mark_style)
	 "line_style: "+string(h.line_style)
	 "fill_mode: "+string(h.fill_mode)
	 "pixel_drawing_mode: "+string(h.pixel_drawing_mode)
	]
      case "Legend"
      t=[t;
	 "foreground: "+string(h.foreground)
	 "visible: "+string(h.visible)
	 "clip_state: "+string(h.clip_state)	
	 "clip_box: "+sci2exp(h.clip_box,0)
	 "font_size:"+sci2exp(h.font_size,0)
	 "font_angle: "+string(h.font_angle)
	 "font_style: "+string(h.font_style)
	 "font_foreground: "+string(h.font_foreground)
	 "font_name: "+string(h.font_name)
	 "text: "+string(h.text)]
      case "Rectangle"
      t=[t;
	 "thickness: "+string(h.thickness)
	 "mark_mode: "+string(h.mark_mode)
	 "mark_style: "+string(h.mark_style)
	 "line_style: "+string(h.line_style)
	 "fill_mode: "+string(h.fill_mode)
	 "foreground: "+string(h.foreground)
	 "data: "+sci2exp(h.data,0)
	 "visible: "+string(h.visible)
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box,0)]
      case "Figure"
      t=[t;
	 "children: "+sci2exp(h.children.type,0)
	 "figure_position: "+sci2exp(h.figure_position,0)
	 "figure_size: "+sci2exp(h.figure_size,0)
	 "figure_name: "+h.figure_name
	 "figure_id: "+sci2exp(h.figure_id,0)
	 "axes_size: "+sci2exp(h.axes_size,0)
	 "thickness: "+string(h.thickness)
	 "mark_mode: "+string(h.mark_mode)
	 "mark_style: "+string(h.mark_style)
	 "line_style: "+string(h.line_style)
	 "background: "+string(h.background)
	 "auto_clear: "+h.auto_clear
	 "auto_scale: "+h.auto_scale
	 "visible: "+string(h.visible)
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box,0)
	 "data: "+sci2exp(h.data,0)
	 "color_map: matrix "+strcat(string(size(h.color_map)),'x')]
      case "Grayplot"
      t=[t;
	 "visible: "+string(h.visible) 
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box,0)
	 "data_mapping: "+h.data_mapping]
      case "Matplot"
      t=[t;
	 "visible: "+string(h.visible) 
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box,0)
	 "data_mapping: "+h.data_mapping]
     
      case "Fec"
      t=[t;
	 "visible: "+string(h.visible) 
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box,0)]
      case "Segs"
      t=[t;
	 "visible: "+string(h.visible) 
	 "clip_state: "+string(h.clip_state)
	 "clip_box: "+sci2exp(h.clip_box,0)]
    end
  end
  write(%io(2),t)
endfunction

