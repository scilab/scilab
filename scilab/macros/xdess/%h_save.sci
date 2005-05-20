function %h_save(h,fd)
  //Author S. Steer Sept 2004, Copyright INRIA
  version=[3 1 0 0]
  mput(version,'c',fd)
  save_graphichandle(h,fd)
endfunction

function save_graphichandle(h,fd)
//Author S. Steer Sept 2004, Copyright INRIA
  typ=h.type
  select h.type
  case "Figure"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(h.figure_position,'sl',fd)
    mput(h.figure_size,'sl',fd)
    mput(h.axes_size,'sl',fd)
    mput(bool2s(h.auto_resize=='on'),'c',fd)
    mput(length(h.figure_name),'c',fd);mput(ascii(h.figure_name),'c',fd);
    mput(h.figure_id,'sl',fd);
    mput(size(h.color_map,'*'),'il',fd);mput(h.color_map,'dl',fd)
    mput(bool2s(h.pixmap=='on'),'c',fd)
    mput(length(h.pixel_drawing_mode),'c',fd);
    mput(ascii(h.pixel_drawing_mode),'c',fd);
    mput(bool2s(h.immediate_drawing=='on'),'c',fd)
    mput(h.background,'il',fd)
    mput(length(h.rotation_style),'c',fd);mput(ascii(h.rotation_style),'c',fd);
    c=h.children;
    n=size(c,'*')
    mput(n,'il',fd)
    for k=n:-1:1
      save_graphichandle(c(k),fd)
    end
  case "Axes" 
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(size(h.axes_visible,'*'),'c',fd);mput(bool2s(h.axes_visible=='on'),'c',fd)
    mput(size(h.grid,'*'),'c',fd); mput(h.grid,'il',fd);
    mput(length(h.x_location),'c',fd);mput(ascii(h.x_location),'c',fd);
    mput(length(h.y_location),'c',fd);mput(ascii(h.y_location),'c',fd);
    mput(ascii(h.view),'c',fd)
    l=h.title;
    mput(bool2s(l.visible=='on'),'c',fd)
    mput(length(l.text),'c',fd);mput(ascii(l.text),'c',fd);
    mput(l.foreground,'il',fd)
    mput(l.font_style,'c',fd)
    mput(l.font_size,'c',fd)
    l=h.x_label
    mput(bool2s(l.visible=='on'),'c',fd)
    mput(length(l.text),'c',fd);mput(ascii(l.text),'c',fd);
    mput(l.foreground,'il',fd)
    mput(l.font_style,'c',fd)
    mput(l.font_size,'c',fd)
    l=h.y_label
    mput(bool2s(l.visible=='on'),'c',fd)
    mput(length(l.text),'c',fd);mput(ascii(l.text),'c',fd);
    mput(l.foreground,'il',fd)
    mput(l.font_style,'c',fd)
    mput(l.font_size,'c',fd)
    if h.view=='3d' then
      l=h.z_label
      mput(bool2s(l.visible=='on'),'c',fd)
      mput(length(l.text),'c',fd);mput(ascii(l.text),'c',fd);
      mput(l.foreground,'il',fd)
      mput(l.font_style,'c',fd)
      mput(l.font_size,'c',fd)
    end
    mput(size(h.auto_ticks,'*'),'c',fd);mput(bool2s(h.auto_ticks=='on'),'c',fd)
    
    mput(size(h.x_ticks.locations,'*'),'sl',fd); mput(h.x_ticks.locations,'dl',fd)
    mput(length(h.x_ticks.labels),'c',fd);
    mput(ascii(strcat(h.x_ticks.labels)),'c',fd);
  
    mput(size(h.y_ticks.locations,'*'),'sl',fd);mput(h.y_ticks.locations,'dl',fd)
    mput(length(h.y_ticks.labels),'c',fd);
    mput(ascii(strcat(h.y_ticks.labels)),'c',fd);

    mput(size(h.z_ticks.locations,'*'),'sl',fd);mput(h.z_ticks.locations,'dl',fd)
    mput(length(h.z_ticks.labels),'c',fd);
    mput(ascii(strcat(h.z_ticks.labels)),'c',fd);
    
    
    mput(bool2s(h.box=='on'),'c',fd)
    mput(size(h.sub_tics,'*'),'c',fd);mput(h.sub_tics,'c',fd);
    mput(-1,'il',fd) // tics_color is removed F.Leray 15.03.05
    mput(h.font_style,'c',fd)
    mput(h.font_size,'c',fd)
    mput(h.font_color,'il',fd)
    mput(bool2s(h.isoview=='on'),'c',fd)
    mput(bool2s(h.cube_scaling=='on'),'c',fd)
    mput(h.rotation_angles,'dl',fd)
    mput(ascii(h.log_flags),'c',fd)
    mput(bool2s(h.tight_limits=='on'),'c',fd)
    mput(size(h.data_bounds,'*'),'c',fd); mput(h.data_bounds,'dl',fd);
    mput(size(h.zoom_box,'*'),'c',fd); 
    if size(h.zoom_box,'*')>0 then mput(h.zoom_box,'dl',fd);end
    mput(h.axes_bounds,'dl',fd);
    mput(bool2s(h.auto_clear=='on'),'c',fd)
    mput(bool2s(h.auto_scale=='on'),'c',fd)
    mput(h.hiddencolor,'il',fd)
    mput(bool2s(h.line_mode=='on'),'c',fd)
    mput(h.line_style,'c',fd)
    mput(h.thickness,'sl',fd)
    mput(bool2s(h.mark_mode=='on'),'c',fd)
    mput(h.mark_style,'c',fd)
    mput(h.mark_size,'c',fd)
    mput(ascii(part(h.mark_size_unit,1)),'c',fd)
    mput(h.mark_foreground,'il',fd)
    mput(h.mark_background,'il',fd)
    mput(h.foreground,'il',fd)
    mput(h.background,'il',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
       mput(h.clip_box,'dl',fd)
    end
    c=h.children;
    n=size(c,'*')
    mput(n,'il',fd)
    for k=n:-1:1
      save_graphichandle(c(k),fd)
    end
  case "Polyline"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(size(h.data),'il',fd);mput(h.data,'dl',fd)
    mput(bool2s(h.line_mode=='on'),'c',fd)
    mput(h.line_style,'c',fd)
    mput(h.thickness,'sl',fd)
    mput(max(1,h.polyline_style),'c',fd)
    mput(bool2s(h.mark_mode=='on'),'c',fd)
    mput(h.mark_style,'c',fd)
    mput(h.mark_size,'c',fd)
    mput(ascii(part(h.mark_size_unit,1)),'c',fd)
    mput(h.foreground,'il',fd)
    mput(h.mark_foreground,'il',fd)
    mput(h.mark_background,'il',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
       mput(h.clip_box,'dl',fd)
    end
  case "Plot3d";
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(bool2s(h.surface_mode=='on'),'c',fd)
    mput(h.foreground,'il',fd)
    mput(h.thickness,'sl',fd)
    mput(bool2s(h.mark_mode=='on'),'c',fd)
    mput(h.mark_style,'c',fd)
    mput(h.mark_size,'c',fd)
    mput(ascii(part(h.mark_size_unit,1)),'c',fd)
    mput(h.mark_foreground,'il',fd)
    mput(h.mark_background,'il',fd)
    mput(h.color_mode,'c',fd)
    mput(h.color_flag,'c',fd)
    x=h.data.x;sz=size(x)
    mput(sz,'il',fd)
    mput(x,'dl',fd)
    y=h.data.y;sz=size(y)
    mput(sz,'il',fd)
    mput(y,'dl',fd)
    z=h.data.z;sz=size(z)
    mput(sz,'il',fd)
    mput(z,'dl',fd)
    if or(h.color_flag==[2 5]) then
      clr=h.data.color;sz=size(clr)
      mput(sz,'il',fd)
      mput(clr,'il',fd)
    end
    mput(h.hiddencolor,'il',fd)
  case "Fac3d";
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(bool2s(h.surface_mode=='on'),'c',fd)
    mput(h.foreground,'il',fd)
    mput(h.thickness,'sl',fd)
    mput(bool2s(h.mark_mode=='on'),'c',fd)
    mput(h.mark_style,'c',fd)
    mput(h.mark_size,'c',fd)
    mput(ascii(part(h.mark_size_unit,1)),'c',fd)
    mput(h.mark_foreground,'il',fd)
    mput(h.mark_background,'il',fd)
    mput(h.color_mode,'c',fd)
    mput(h.color_flag,'c',fd)
    x=h.data.x;sz=size(x)
    mput(sz,'il',fd)
    mput(x,'dl',fd)
    y=h.data.y;sz=size(y)
    mput(sz,'il',fd)
    mput(y,'dl',fd)
    z=h.data.z;sz=size(z)
    mput(sz,'il',fd)
    mput(z,'dl',fd)
    if or(h.color_flag==[2 5]) then
      clr=h.data.color;sz=size(clr)
      mput(sz,'il',fd)
      mput(clr,'il',fd)
    end
    mput(h.hiddencolor,'il',fd)
  case "Agregation"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);

    h=h.children
    n=size(h,'*')
    mput(n,'il',fd)
    for k=1:n
      save_graphichandle(h(k),fd)
    end
  case "Rectangle"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(h.thickness,'sl',fd)
    mput(bool2s(h.mark_mode=='on'),'c',fd)
    mput(h.mark_style,'c',fd)
    mput(h.mark_size,'c',fd)
    mput(ascii(part(h.mark_size_unit,1)),'c',fd)
    mput(h.mark_foreground,'il',fd)
    mput(h.mark_background,'il',fd)
    mput(bool2s(h.line_mode=='on'),'c',fd)
    mput(h.line_style,'c',fd)
    mput(bool2s(h.fill_mode=='on'),'c',fd)
    mput(h.foreground,'il',fd)
    mput(h.data,'dl',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
       mput(h.clip_box,'dl',fd)
    end
  case "Arc"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(h.thickness,'sl',fd)
    mput(h.line_style,'c',fd)
    mput(bool2s(h.fill_mode=='on'),'c',fd)
    mput(h.foreground,'il',fd)
    mput(h.data,'dl',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
  case "Champ"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    x=h.data.x;sz=size(x)
    mput(sz,'il',fd)
    mput(x,'dl',fd)
    y=h.data.y;sz=size(y)
    mput(sz,'il',fd)
    mput(y,'dl',fd)
    fx=h.data.fx;sz=size(fx)
    mput(sz,'il',fd)
    mput(fx,'dl',fd)
    fy=h.data.fy;sz=size(fy)
    mput(sz,'il',fd)
    mput(fy,'dl',fd)
    mput(h.line_style,'c',fd)
    mput(h.thickness,'sl',fd)
    mput(bool2s(h.colored=='on'),'c',fd)
    mput(h.arrow_size,'dl',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
  case "Segs"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(size(h.data),'il',fd);mput(h.data,'dl',fd)
    mput(bool2s(h.line_mode=='on'),'c',fd)
    mput(h.line_style,'c',fd)
    mput(h.thickness,'sl',fd)
    mput(h.arrow_size,'dl',fd)
    mput(size(h.segs_color,'*'),'il',fd);mput(h.segs_color,'il',fd)
    mput(bool2s(h.mark_mode=='on'),'c',fd)
    mput(h.mark_style,'c',fd)
    mput(h.mark_size,'c',fd)
    mput(ascii(part(h.mark_size_unit,1)),'c',fd)
    mput(h.mark_foreground,'il',fd)
    mput(h.mark_background,'il',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end  
  case "Grayplot"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    x=h.data.x;sz=size(x)
    mput(sz,'il',fd)
    mput(x,'dl',fd)
    y=h.data.y;sz=size(y)
    mput(sz,'il',fd)
    mput(y,'dl',fd)
    z=h.data.z;sz=size(z)
    mput(sz,'il',fd)
    mput(z,'dl',fd)
    mput(length(h.data_mapping),'c',fd);mput(ascii(h.data_mapping),'c',fd);
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
  case "Matplot"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(size(h.data),'il',fd);mput(h.data,'dl',fd)
//    mput(length(h.data_mapping),'c',fd);mput(ascii(h.data_mapping),'c',fd);
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
  case "Fec"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(size(h.data),'il',fd);mput(h.data,'dl',fd)
    mput(size(h.triangles),'il',fd);mput(h.triangles,'dl',fd)
    mput(h.z_bounds,'dl',fd);
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
   case "Legend"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(length(h.text),'c',fd);mput(ascii(h.text),'c',fd);
    mput(h.foreground,'il',fd)
    mput(h.font_style,'c',fd)
    mput(h.font_size,'c',fd)
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
  case "Text"
    mput(length(h.type),'c',fd);mput(ascii(h.type),'c',fd);
    mput(bool2s(h.visible=='on'),'c',fd)
    mput(length(h.text),'il',fd);mput(ascii(h.text),'c',fd);
    mput(size(h.data),'c',fd);mput(h.data,'dl',fd);
    mput(h.text_box,'dl',fd);
    mput(length(h.text_box_mode),'c',fd);mput(ascii(h.text_box_mode),'c',fd);
    mput(h.foreground,'il',fd)
    mput(h.font_style,'c',fd)
    mput(h.font_size,'c',fd)
    mput(h.font_angle,'dl',fd)
    
    mput(length(h.clip_state),'c',fd);mput(ascii(h.clip_state),'c',fd);
    if h.clip_state=='on' then
      mput(h.clip_box,'dl',fd)
    end
   
  else
    disp("handle of type "+h.type+" unhandled")
  end

endfunction

