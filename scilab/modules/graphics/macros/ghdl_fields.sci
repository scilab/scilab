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
	    "rotation_style";"children";"user_data"]'
  case "Axes" 
    fields=["children";"visible";"axes_visible";"axes_reverse";"grid";"x_location";
	    "y_location";"title";"x_label";"y_label";"z_label";"auto_ticks";
	    "x_ticks";"y_ticks";"z_ticks";"box";"sub_ticks";
	    "font_style";"font_size";"font_color";"isoview";"cube_scaling";
	    "view";"rotation_angles";"log_flags";"tight_limits";"data_bounds" ;
	    "zoom_box";"margins";"axes_bounds";"auto_clear";"auto_scale";
	    "hiddencolor";"line_mode";"line_style";"thickness";"mark_mode";
	    "mark_style";"mark_size_unit";"mark_size";"mark_foreground";
	    "mark_background";"foreground";"background";"clip_state";"clip_box";
	    "user_data"]';
  case "Polyline"
    fields=["visible";"data";"line_mode";"line_style";"thickness";
	    "polyline_style";"foreground";"mark_mode";"mark_style";
	    "mark_size_unit";"mark_size";"mark_foreground";"mark_background";
	    "clip_state";"clip_box";"user_data"]';
  case "Plot3d";
    fields=["visible";"surface_mode";"foreground";"thickness";
	    "mark_mode";"mark_style";"mark_size_unit";"mark_size";
	    "mark_foreground";"mark_background";"data";"color_mode";
	    "color_flag";"hiddencolor";"user_data"]';
  case "Fac3d";
    fields=["visible";"surface_mode";"foreground";"thickness";
	    "mark_mode";"mark_style";"mark_size_unit";"mark_size";
	    "mark_foreground";"mark_background";"data";
	    "color_mode";"color_flag";"hiddencolor";"user_data"]';
  case "Compound"
    fields=["children";"user_data"]';
  case "Rectangle"
   fields=["mark_mode";"mark_style";"mark_size_unit";"mark_size";
	   "mark_foreground";"mark_background";"line_mode";"line_style";
	   "thickness";"fill_mode";"foreground";"data";"visible";
	   "clip_state";"clip_box";"user_data"]';
  case "Arc"
    fields=["thickness";"line_style";"fill_mode";"foreground";"data";
	    "visible";"clip_state";"clip_box";"user_data"]';
  case "Segs"
    fields =["visible";"data";"line_mode";"line_style";"thickness";
	     "arrow_size";"segs_color";"mark_mode";"mark_style";
	     "mark_size_unit";"mark_size";"mark_foreground";
	     "mark_background";"clip_state";"clip_box";"user_data"]';
  case "Grayplot"
    fields =["visible";"data";"data_mapping";"clip_state";"clip_box";"user_data"]';
  case "Fec"
    fields =["visible";"data";"triangles";"z_bounds";"clip_state";"clip_box";"user_data"]';
  case "Legend"
    fields =["visible";"text";
	     "foreground";"font_style";"font_size";"font_angle";
	     "clip_state";"clip_box"]';
  case "Text"
    fields =["visible";"text";"data";"text_box";"text_box_mode";
	     "foreground";"font_style";"font_size";"font_angle";
	     "clip_state";"clip_box"]';
  case "Label"
    fields =["visible";"text";"foreground";"font_style";"font_size"]';

  case "Axis"
    fields =["visible";"tics_direction";"xtics_coord";"ytics_coord";
	    "tics_color";"tics_segment";"tics_style";"sub_tics";
	     "tics_labels";"labels_font_size";"labels_font_color";
	     "clip_state";"clip_box";"user_data"]';

  else
    disp("handle of type "+h.type+" unhandled")
    fields =[]
  end
endfunction


