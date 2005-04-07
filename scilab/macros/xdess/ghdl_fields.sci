function fields=ghdl_fields(h)
//Author S. Steer May 2005, Copyright INRIA
//returns the set of fields associated with a given graphic handle
  if size(h,'*')<>1 then error('argument must be a single handle'),end
  typ=h.type
  select h.type
  case "Figure"
    fields=["visible";"figure_position";"figure_size";"axes_size";
	    "auto_resize";"figure_name";"figure_id";"color_map";"pixmap";
	    "pixel_drawing_mode";"immediate_drawing";"background";
	    "rotation_style";"children"]'
  case "Axes" 
    fields=["children";"visible";"axes_visible";"axes_reverse";"grid";"x_location";
	    "y_location";"title";"x_label";"y_label";"z_label";"auto_ticks";
	    "x_ticks.locations";"y_ticks.locations";"z_ticks.locations";
	    "x_ticks.labels";"y_ticks.labels";"z_ticks.labels";"box";"sub_ticks";
	    "font_style";"font_size";"font_color";"isoview";"cube_scaling";
	    "view";"rotation_angles";"log_flags";"tight_limits";"data_bounds" ;
	    "zoom_box";"margins";"axes_bounds";"auto_clear";"auto_scale";
	    "hiddencolor";"line_mode";"line_style";"thickness";"mark_mode";
	    "mark_style";"mark_size_unit";"mark_size";"mark_foreground";
	    "mark_background";"foreground";"background";"clip_state";"clip_box"]';
  case "Polyline"
    fields=["visible";"data";"line_mode";"line_style";"thickness";
	    "polyline_style";"foreground";"mark_mode";"mark_style";
	    "mark_size_unit";"mark_size";"mark_foreground";"mark_background";
	    "clip_state";"clip_box"]';
  case "Plot3d";
    fields=["visible";"surface_mode";"foreground";"thickness";
	    "mark_mode";"mark_style";"mark_size_unit";"mark_size";
	    "mark_foreground";"mark_background";"data";"color_mode";
	    "color_flag";"hiddencolor"]';
  case "Fac3d";
    fields=["visible";"surface_mode";"foreground";"thickness";
	    "mark_mode";"mark_style";"mark_size_unit";"mark_size";
	    "mark_foreground";"mark_background";"data.x";"data.y";"data.z";
	    "color_mode";"color_flag";"hiddencolor"]';
  case "Agregation"
    fields=["children"]';
  case "Rectangle"
   fields=["mark_mode";"mark_style";"mark_size_unit";"mark_size";
	   "mark_foreground";"mark_background";"line_mode";"line_style";
	   "thickness";"fill_mode";"foreground";"data";"visible";
	   "clip_state";"clip_box"]';
  case "Arc"
    fields=["thickness";"line_style";"fill_mode";"foreground";"data";
	    "visible";"clip_state";"clip_box"]';
  case "Segs"
    fields =["visible";"data";"line_mode";"line_style";"thickness";
	     "arrow_size";"segs_color";"mark_mode";"mark_style";
	     "mark_size_unit";"mark_size";"mark_foreground";
	     "mark_background";"clip_state";"clip_box"]';
  case "Grayplot"
    fields =["visible";"data";"data_mapping";"clip_state";"clip_box"]';
  case "Fec"
    fields =["visible";"data";"triangles";"z_bounds";"clip_state";"clip_box"]';
  case "Legend"
    fields =["visible";"text";"data";"text_box";"text_box_mode";
	     "foreground";"font_style";"font_size";"font_angle";
	     "clip_state";"clip_box"]';
  case "Text"
    fields =["visible";"text";"data";"text_box";"text_box_mode";
	     "foreground";"font_style";"font_size";"font_angle";
	     "clip_state";"clip_box"]';
  case "Label"
    fields =["visible";"text";"foreground";"font_style";"font_size"]';

  else
    disp("handle of type "+h.type+" unhandled")
    fields =[]
  end
endfunction


