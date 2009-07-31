// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA - Serge Steer
// Copyright (C) 2004-2006 - INRIA - Fabrice Leray
// Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function h=%h_load(fd)
  global init_immediate_drawing
  init_immediate_drawing = 0;

  if exists('xload_mode')==0 then xload_mode=%f,end
  version=mget(4,'uc',fd)

  // saving/loading character with 'c' is actually quite buggy
  characterFormat = 'uc';

  immediate_drawing="";

  h=[];

  if is_higher_than([3 1 0 1]) then // case 3 1 0 2 and after
    hsize = mget(2,characterFormat,fd)
    for i=1:hsize(1)
      for j=1:hsize(2)
	[htmp,immediate_drawing] = load_graphichandle(fd)
	h = [h htmp];
      end
    end
  else
    [h,immediate_drawing] = load_graphichandle(fd) // a single handle only can be loaded before 3 1 0 2
  end
  f=gcf();
  f.immediate_drawing = immediate_drawing;
  clearglobal init_immediate_drawing
  clear init_immediate_drawing
endfunction

function [h,immediate_drawing] = load_graphichandle(fd)
  global init_immediate_drawing
  typ=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))
  if typ<>'Figure'
    f=gcf();
    if init_immediate_drawing == 0
      immediate_drawing = f.immediate_drawing;
      f.immediate_drawing ='off';
      init_immediate_drawing = 1;
    end
  end
//  mprintf('----------------------------- %s ----------------------\n',typ)
  select typ
  case "Figure"
    if xload_mode then
      h=gcf()
      visible=toggle(mget(1,characterFormat,fd)); // visible
      figure_position=mget(2,'sl',fd); // figure_position
      figure_size=mget(2,'sl',fd); // figure_size
      axes_size=mget(2,'sl',fd); //axes_size
      if ( is_higher_than([4 1 2 0]) ) then
        viewport = mget(2,'sl',fd) ; // viewport
        info_message = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // info_message
        tag = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // tag
      end
      auto_resize=toggle(mget(1,characterFormat,fd)); // auto_resize
      figure_name=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // figure_name
      figure_id=mget(1,'sl',fd); // figure_id
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3) // color_map
      pixmap=toggle(mget(1,characterFormat,fd)); // pixmap
      pixel_drawing_mode=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // pixel_drawing_mode
	  if (is_higher_than([5 1 0 0])) then
	    anti_aliasing=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)); // anti_aliasing
	  end
      immediate_drawing=toggle(mget(1,characterFormat,fd));// immediate drawing // init. global variable immediate_drawing
      h.immediate_drawing = 'off';  // set it to 'off' to pass useless redraw due to several 'set' calls
      h.background=mget(1,'il',fd) // background
      rotation_style=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // rotation_style
    else
      visible=toggle(mget(1,characterFormat,fd)); // visible
      figure_position=mget(2,'sl',fd); // figure_position
      // if figure is iconified in old scilab version, its position is -32000, -32000]
      figure_position = max(figure_position, [0,0]);
      figure_size=mget(2,'sl',fd); // figure_size
      axes_size=mget(2,'sl',fd); // axes_size
      if ( is_higher_than([4 1 2 0]) ) then
        viewport = mget(2,'sl',fd) ; // viewport
        info_message = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // info_message
        tag = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // tag
      end
      auto_resize=toggle(mget(1,characterFormat,fd)); // auto_resize
      figure_name=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // figure_name
      figure_id=mget(1,'sl',fd); // figure_id
      // create the figure
      h=scf(figure_id);
       h.visible=visible;  // can be set now as we act on immediate_drawing everywhere else F.Leray 18.02.05
      h.figure_position=figure_position
	  // set auto_resize first otherwise viewport modification may not have any effect.
	  h.auto_resize = auto_resize;
	  h.figure_size = figure_size;
	  // set axes_size last because it's more important than figure_size
	  h.axes_size = axes_size;

      if ( is_higher_than([4 1 2 0]) ) then
        h.viewport = viewport;
        h.info_message = info_message ;
        h.tag = tag ;
      end

      h.figure_name=figure_name
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3) // color_map
      h.pixmap=toggle(mget(1,characterFormat,fd)); // pixmap
      h.pixel_drawing_mode=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // pixel_drawing_mode
	  if (is_higher_than([5 1 0 0])) then
	    h.anti_aliasing=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)); // anti_aliasing
	  end
      immediate_drawing=toggle(mget(1,characterFormat,fd)); // immediate_drawing  // init. global variable immediate_drawing
      h.immediate_drawing = 'off'; // set it to 'off' to pass useless redraw due to several 'set' calls
      h.background=mget(1,'il',fd); // background
      h.rotation_style = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // rotation_style

    end

    if ( is_higher_than([4 1 2 0]) ) then
      h.event_handler = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // event_handler
      h.event_handler_enable = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // event_handler_enable
    end

    // children
    n_axes=mget(1,'il',fd);
    if n_axes==1 then
      load_graphichandle(fd)
    else
      load_graphichandle(fd);
      for k=2:n_axes
	xsetech(wrect=[0 0 .1 .1])
	load_graphichandle(fd)
      end
    end
    load_user_data(fd); // user_data
  case "Axes"
    a=gca() ;
    titl=a.title ;
    x_label=a.x_label ;
    y_label=a.y_label ;
    set(a,"visible"              , toggle(mget(1,characterFormat,fd))) // visible
    if and(version==[3 0 0 0]) then // axes_visible
      axes_visible= toggle(mget(1,characterFormat,fd));
      axes_visible=emptystr(1,3)+axes_visible
    else
      axes_visible= toggle(mget(mget(1,characterFormat,fd),characterFormat,fd)) ;
    end

    if is_higher_than( [3 1 0 1] ) then // axes_reverse
      axes_reverse = toggle(mget(mget(1,characterFormat,fd),characterFormat,fd)) ;
    end

    set(a,"axes_visible", axes_visible)
    set(a,"axes_reverse", axes_reverse)
    set(a,"grid"        , mget(mget(1,characterFormat,fd),'il',fd)) //grid
	if (is_higher_than([5 0 3 0])) then
	  set(a,"grid_position",ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))); // grid_position
	else
	  set(a,"grid_position","background"); // grid_position
	end
    set(a,"x_location"  , ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))) // x_location
    set(a,"y_location"  , ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))) // y_location

    view            =  ascii(mget(2,characterFormat,fd)); // view

    // title
    set(titl,"visible"   , toggle(mget(1,characterFormat,fd))) // title.visible
    if is_higher_than( [4 1 2 0] ) then
      set(titl, "text", load_text_matrix( fd ) ) ;
    else
      set(titl,"text"      , ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))) // title.text
    end
    if is_higher_than([4 1 2 0]) then
      set(titl,"font_foreground", mget(1,'il',fd)); // title.font_foreground
      set(titl,"fractional_font", toggle(mget(1,characterFormat,fd))); //title.fractional_font
    end
    set(titl,"foreground", mget(1,'il',fd)); // title.foreground
    if is_higher_than([3 1 0 0]) then
      set(titl,"background"       , mget(1,'il',fd)); // title.background
      set(titl,"fill_mode"        , toggle(mget(1,characterFormat,fd))); //title.fill_mode
    end

    set(titl,"font_style"       , mget(1,characterFormat,fd)); // title.font_style
    set(titl,"font_size"        , mget(1,characterFormat,fd)); // title.font_size

    if is_higher_than([3 1 0 0]) then
      auto_rotation =  toggle(mget(1,characterFormat,fd)) ; // title.auto_rotation
      set(titl,"font_angle"   , mget(1,'dl',fd)); // title.font_angle
      auto_position = toggle(mget(1,characterFormat,fd)) ;  // title.auto_position
      set(titl,"position"     , mget(2,'dl',fd)); // title.position
      set( titl, "auto_rotation", auto_rotation ) ;
      set( titl, "auto_position", auto_position ) ;
    end

    // x_label
    set(x_label,"visible"   , toggle(mget(1,characterFormat,fd))) // x_label.visible
    if is_higher_than( [4 1 2 0] ) then
      set(x_label, "text", load_text_matrix( fd ) ) ;
    else
      set(x_label,"text"      , ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))) // title.text
    end
    if is_higher_than([4 1 2 0]) then
      set(x_label,"font_foreground", mget(1,'il',fd)); // x_label.font_foreground
      set(x_label,"fractional_font", toggle(mget(1,characterFormat,fd))); //x_label.fractional_font
    end
    set(x_label,"foreground", mget(1,'il',fd)); // x_label.foreground
    if is_higher_than([3 0 0 0]) then
      set(x_label,"background"       , mget(1,'il',fd)); // x_label.background
      set(x_label,"fill_mode"        , toggle(mget(1,characterFormat,fd))); // x_label.fill_mode
    end
    set(x_label,"font_style"     , mget(1,characterFormat,fd)); // x_label.font_style
    set(x_label,"font_size"      , mget(1,characterFormat,fd)); // x_label.font_size

    if is_higher_than([3 0 0 0]) then
      auto_rotation =  toggle(mget(1,characterFormat,fd)) ; // x_label.auto_rotation
      set(x_label,"font_angle"   , mget(1,'dl',fd)); // x_label.font_angle
      auto_position = toggle(mget(1,characterFormat,fd)) ; // x_label.auto_position
      set( x_label,"position"     , mget(2,'dl',fd)); // x_label.position
      set( x_label, "auto_rotation", auto_rotation ) ;
      set( x_label, "auto_position", auto_position ) ;
    end

    // y_label
    set(y_label,"visible"        , toggle(mget(1,characterFormat,fd)))
    if is_higher_than( [4 1 2 0] ) then
      set(y_label, "text", load_text_matrix( fd ) ) ;
    else
      set(y_label,"text"      , ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))) // title.text
    end
    if is_higher_than([4 1 2 0]) then
      set(y_label,"font_foreground", mget(1,'il',fd)); // y_label.font_foreground
      set(y_label,"fractional_font", toggle(mget(1,characterFormat,fd))); //y_label.fractional_font
    end
    set(y_label,"foreground"     , mget(1,'il',fd));
    if is_higher_than([3 0 0 0]) then
      set(y_label,"background"       , mget(1,'il',fd));
      set(y_label,"fill_mode"        , toggle(mget(1,characterFormat,fd)));
    end
    set(y_label,"font_style"     , mget(1,characterFormat,fd));
    set(y_label,"font_size"      , mget(1,characterFormat,fd));

    if is_higher_than([3 0 0 0]) then
      auto_rotation =  toggle(mget(1,characterFormat,fd)) ; // y_label.auto_rotation
      set(y_label,"font_angle"   , mget(1,'dl',fd)); // y_label.font_angle
      auto_position = toggle(mget(1,characterFormat,fd)) ; // y_label.auto_position
      set( y_label,"position"     , mget(2,'dl',fd)); // y_label.position
      set( y_label, "auto_rotation", auto_rotation ) ;
      set( y_label, "auto_position", auto_position ) ;
    end

    if view=='3d' then
      // z_label
      z_label=a.z_label
      set(z_label,"visible"        , toggle(mget(1,characterFormat,fd)))
      if is_higher_than( [4 1 2 0] ) then
	set(z_label, "text", load_text_matrix( fd ) ) ;
      else
	set(z_label,"text"      , ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))) // title.text
      end
      if is_higher_than([4 1 2 0]) then
	set(z_label,"font_foreground", mget(1,'il',fd)); // z_label.font_foreground
        set(z_label,"fractional_font", toggle(mget(1,characterFormat,fd))); //z_label.fractional_font
      end
      set(z_label,"foreground"     , mget(1,'il',fd));
      if is_higher_than([3 0 0 0]) then
	set(z_label,"background"       , mget(1,'il',fd));
	set(z_label,"fill_mode"        , toggle(mget(1,characterFormat,fd)));
      end
      set(z_label,"font_style"     , mget(1,characterFormat,fd));
      set(z_label,"font_size"      , mget(1,characterFormat,fd));

      if is_higher_than([3 0 0 0]) then
	auto_rotation =  toggle(mget(1,characterFormat,fd)) ; // z_label.auto_rotation
        set(z_label,"font_angle"   , mget(1,'dl',fd)); // z_label.font_angle
        auto_position = toggle(mget(1,characterFormat,fd)) ; // z_label.auto_position
        set( z_label,"position"     , mget(2,'dl',fd)); // z_label.position
        set( z_label, "auto_rotation", auto_rotation ) ;
        set( z_label, "auto_position", auto_position ) ;
      end
    end



    if is_higher_than([3 0 0 0]) then
      auto_ticks=toggle(mget(mget(1,characterFormat,fd),characterFormat,fd)); // auto_ticks

      ticks=['ticks','locations','labels']
      sz=mget(1,'sl',fd) // x_ticks.locations
      if sz>0 then
	x_ticks_locations=mget(sz,'dl',fd)'
	lz=mget(sz,characterFormat,fd) // x_ticks.label
	x_ticks_labels=[];for ks=1:sz,x_ticks_labels(ks)=ascii(mget(lz(ks),characterFormat,fd));end
      else
	x_ticks_labels=[];
	x_ticks_locations=[];
      end
	set(a,'x_ticks',tlist(ticks,x_ticks_locations,x_ticks_labels))

      sz=mget(1,'sl',fd) // y_ticks.locations
      if sz>0 then
	y_ticks_locations=mget(sz,'dl',fd)'
	lz=mget(sz,characterFormat,fd) // y_ticks.label
	y_ticks_labels=[];for ks=1:sz,y_ticks_labels(ks)=ascii(mget(lz(ks),characterFormat,fd));end
      else
	y_ticks_labels=[];
	y_ticks_locations=[];
      end
      set(a,'y_ticks',tlist(ticks,y_ticks_locations,y_ticks_labels))

      sz=mget(1,'sl',fd) // z_ticks.locations
      if sz>0 then
	z_ticks_locations=mget(sz,'dl',fd)'
	lz=mget(sz,characterFormat,fd) // z_ticks.labels
	z_ticks_labels=[];for ks=1:sz,z_ticks_labels(ks)=ascii(mget(lz(ks),characterFormat,fd));end
      else
	z_ticks_labels=[];
	z_ticks_locations=[];
      end
      set(a,'z_ticks',tlist(ticks,z_ticks_locations,z_ticks_labels))

      set(a,"auto_ticks"           , auto_ticks)
    end
    if is_higher_than([4 1 2 0]) then
      // migth be now 'off','hidden_axis','back_half' or 'on'
      boxtype = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ;
      set( a, "box", boxtype  ) // box
      set(a,"filled",  toggle(mget(1,characterFormat,fd) )); // filled
    else
      set(a, "box", toggle(mget(1,characterFormat,fd) ) ) // box
    end

    set(a,"sub_tics"             , mget(mget(1,characterFormat,fd),characterFormat,fd)) // sub_tics
    if ~(is_higher_than([3 1 0 1]) ) then
      mget(1,'il',fd); // tics_color is removed F.Leray 15.03.05
    end
    set(a,"font_style"           , mget(1,characterFormat,fd));  // font_style
    set(a,"font_size"            , mget(1,characterFormat,fd));  // font_size
    set(a,"font_color"           , mget(1,'il',fd)); // font_color
    if is_higher_than([4 1 2 0]) then
      set(a,"fractional_font", toggle(mget(1,characterFormat,fd))); // fractional_font
    end
    set(a,"isoview"              , toggle(mget(1,characterFormat,fd))) // isoview

    cube_scaling    = toggle(mget(1,characterFormat,fd)) // cube_scaling
    rotation_angles = mget(2,'dl',fd); // rotation_angles

    if a.view=='2d' then
      set(a,"view"               , view);
    end
    if a.view=='3d' then
      set(a,"cube_scaling"      , cube_scaling)
      if view=='3d' then
	set(a,"rotation_angles"   , rotation_angles)
      end
    end

    if is_higher_than([3 0 0 0]) then // log_flags
      log_flags= ascii(mget(3,characterFormat,fd));
    else
      log_flags= ascii(mget(2,characterFormat,fd));
    end


    set(a,"tight_limits"         , toggle(mget(1,characterFormat,fd))) // tight_limits
    data_bounds = matrix(mget(mget(1,characterFormat,fd),'dl',fd),2,-1) // data_bounds


    if view=='2d'& a.view=='3d' then
      data_bounds(2,3)=0;
    end
    if xload_mode
      // check if a had at least a child previously
      // if not the axes is considered unused
      // and we don't merge the data_bounds.
      if a.children <> []  then
	old_bounds=a.data_bounds;
	for k=1:size(old_bounds,2)
	  data_bounds(1,k)=min(data_bounds(1,k),old_bounds(1,k));
	  data_bounds(2,k)=max(data_bounds(2,k),old_bounds(2,k));
	end
      end
    end
    if is_higher_than([3 0 0 0]) then
      zoom_box_size = mget(1,characterFormat,fd);
      if zoom_box_size<>0 then
	set(a,"zoom_box"          , mget(zoom_box_size,'dl',fd))  // zoom_box
      end
    end
    if is_higher_than([3 1 0 1]) then
      set(a,"margins"              , mget(4,'dl',fd))  // margins
    end

    set(a,"axes_bounds"          , mget(4,'dl',fd))  // axes_bounds
    set(a,"auto_clear"           , toggle(mget(1,characterFormat,fd))) // auto_clear
    set(a,"auto_scale"           , toggle(mget(1,characterFormat,fd))) // auto_scale

    if is_higher_than([4 1 2 0] ) then // 4 0 0 0 and after
      set(a,"hidden_axis_color", mget(1,'il',fd)) ; // hidden_axis_color
      set(a, "arc_drawing_method", ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))); // arc_drawing_method
    else
      set(a, "arc_drawing_method", "nurbs"); // default value, real circle
    end

    set(a,"hiddencolor"          , mget(1,'il',fd)), // hidden_color
    set(a,"line_mode"            , toggle(mget(1,characterFormat,fd))), // line_mode
    set(a,"line_style"           , mget(1,characterFormat,fd)) // line_style
    set(a,"thickness"            , mget(1,'sl',fd)), // thickness
    set(a,"mark_mode"            , toggle(mget(1,characterFormat,fd))), //mark_mode
    set(a,"mark_style"           , mget(1,characterFormat,fd)) // mark_style
    set(a,"mark_size"            , mget(1,characterFormat,fd)) // mark_size
    if is_higher_than([3 0 0 0]) then

      if ascii(mget(1,characterFormat,fd))=='t' then // mark_size_unit
	msu='tabulated' ;
      else
	msu='point';
      end

      set(a,"mark_size_unit"   , msu)
      set(a,"mark_foreground"      , mget(1,'il',fd)) ; // mark_foreground
      set(a,"mark_background"      , mget(1,'il',fd)) ; // mark_background
    else
      set(a,"mark_size_unit"     , 'tabulated')
    end
    set(a,"foreground"           , mget(1,'il',fd)), // foreground
    set(a,"background"           , mget(1,'il',fd)), // background
    clip_state                   = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then // clip_box
      set(a,"clip_box",mget(4,'dl',fd)) ;
    end
    set(a,"clip_state"           ,clip_state);

    // children
    nbChildren = mget(1,'il',fd) ;
    for k = 1 : nbChildren
       load_graphichandle(fd) ;
    end
    //next lines because tools used to rebuild children change the
    //data_bounds an axes_visible properties
    set(a,"data_bounds"          , data_bounds) ;
    set(a,"log_flags"            , log_flags);

    set(a,"axes_visible"          , axes_visible) ;
    if is_higher_than([4 1 2 0] ) then
      set(a, "box", boxtype ) ;
    end

    h=a;
    load_user_data(fd) ; // user_data
    global %LEG

    if %LEG<>[] then
      //get handles from paths
      links=get_links_from_path(a,%LEG.paths)
      if links<>[] then
	L=captions(links,%LEG.text)
	L.visible         = %LEG.visible
	L.font_style      = %LEG.font_style
	L.font_size       = %LEG.font_size
	L.font_color      = %LEG.font_color
	L.fractional_font = %LEG.fractional_font
	L.mark_mode       = 'off';
	L.legend_location = %LEG.legend_location
	L.position        = %LEG.position
	L.line_mode       = %LEG.line_mode
	L.thickness       = %LEG.thickness
	L.foreground      = %LEG.foreground
	L.fill_mode       = %LEG.fill_mode
	L.background      = %LEG.background
	L.clip_state      = %LEG.clip_state
	if %LEG.clip_state=='on' then
	  L.clip_box      = %LEG.clip_box
	end
	L.user_data       = %LEG.user_data
      else
	 warning(msprintf(_("%s: Legend does not fit with the current context. Skipped\n"),"load"));
      end
    end
    clearglobal %LEG

  case "Polyline"

    visible=toggle(mget(1,characterFormat,fd)) // visible
    sz=mget(2,'il',fd); // data
    data=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    if is_higher_than([3 1 0 0]) then
      closed      = toggle(mget(1,characterFormat,fd)) // closed
    end
    line_mode      = toggle(mget(1,characterFormat,fd)) // line_mode
    if is_higher_than([3 1 0 0]) then
      fill_mode      = toggle(mget(1,characterFormat,fd)) // fill_mode
    end
    line_style     = mget(1,characterFormat,fd); // line_style
    thickness      = mget(1,'sl',fd); // thickness
    if is_higher_than([3 1 0 1]) then
      arrow_size_factor = mget(1,'sl',fd); // arrow_size_factor
    end
    polyline_style = mget(1,characterFormat,fd); // polyline_style

    if is_higher_than([3 1 0 1] ) then
      size_interp_color = mget(1,'sl',fd) ; // interp_color_vector
      interp_color_vector = mget( size_interp_color, 'dl', fd ) ;
      interp_color_mode   = toggle( mget( 1, characterFormat, fd ) ) ; // interp_color_mode
    end

    mark_mode      = toggle(mget(1,characterFormat,fd)) // mark_mode
    mark_style     = mget(1,characterFormat,fd); // mark_style
    mark_size      = mget(1,characterFormat,fd); // mark_size

    msu='tabulated'
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,characterFormat,fd))=='t' then // mark_size_unit
	msu='tabulated' ;
      else
	msu='point';
      end
    end

    foreground     = mget(1,'il',fd); // foreground

    if is_higher_than([3 1 0 0]) then
      background     = mget(1,'il',fd); // background
    end

    if is_higher_than([3 0 0 0]) then
      mark_foreground=mget(1,'il',fd) // mark_foreground
      mark_background=mget(1,'il',fd) // mark_background
    end

    if is_higher_than([3 1 0 0]) then
      sz_x_shift=mget(1,'sl',fd) // x_shift
      x_shift=mget(sz_x_shift,'dl',fd)'

      sz_y_shift=mget(1,'sl',fd) // y_shift
      y_shift=mget(sz_y_shift,'dl',fd)'

      sz_z_shift=mget(1,'sl',fd) // z_shift
      z_shift=mget(sz_z_shift,'dl',fd)'
    end

    if is_higher_than([3 1 0 1]) then
      bar_width = mget( 1, 'dl', fd ) ; // bar_width
    end

    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state

    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) ; // clip_box
    else
      clip_box = [] ;
    end
    // draw the polyline and set properties
    xpoly(data(:,1),data(:,2))
    //plot2d( data(:,1),data(:,2));
    h=get('hdl')
    set(h,"data",data)
    set(h,"visible",visible)
    set(h,"line_mode",line_mode),
    set(h,"line_style",line_style)
    set(h,"thickness",thickness)
    set(h,"arrow_size_factor",arrow_size_factor);
    set(h,"polyline_style",max(1,polyline_style)),
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_mode",mark_mode),
    set(h,"foreground",foreground),
    if is_higher_than([3 0 0 0]) then
      set(h,"mark_size_unit",msu)
      set(h,"mark_foreground",mark_foreground),
      set(h,"mark_background",mark_background)
    end
    if is_higher_than([3 1 0 0]) then
      set(h,"background",background)
      set(h,"fill_mode",fill_mode)
      set(h,"closed",closed);
      set(h,'x_shift',x_shift);
      set(h,'y_shift',y_shift);
      set(h,'z_shift',z_shift);
    end

    if is_higher_than([3 1 0 1]) then
      set(h,"interp_color_mode",interp_color_mode);
      if interp_color_mode == 'on' then
	set(h,"interp_color_vector",interp_color_vector);
      end
      set(h,"bar_width",bar_width);
    end

    if clip_state =='' then clip_state='clipgrf',end
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);

    load_user_data(fd) // user_data

  case "Plot3d" then
    visible=toggle(mget(1,characterFormat,fd)) // visible
    surface_mode   = toggle(mget(1,characterFormat,fd)) // surface_mode
    foreground     = mget(1,'il',fd); // foreground
    thickness      = mget(1,'sl',fd); // thickness
    mark_mode      = toggle(mget(1,characterFormat,fd)) // mark_mode
    mark_style     = mget(1,characterFormat,fd); // mark_style
    mark_size      = mget(1,characterFormat,fd); // mark_size

    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,characterFormat,fd))=='t' then // mark_size_unit
	msu='tabulated' ;
      else
	msu='point';
      end
      mark_foreground=mget(1,'il',fd) // mark_foreground
      mark_background=mget(1,'il',fd) // mark_background
    else
      msu='tabulated'
    end

    if is_higher_than([5 1 1 0]) then
	  color_mode     = mget(1,'il',fd); // color_mode
	  color_flag     = mget(1,'il',fd); // color_flag
	else
	  color_mode     = mget(1,characterFormat,fd);
	  color_flag     = mget(1,characterFormat,fd);
	end

    sz=mget(2,'il',fd); // data.x
    x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.y
    y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.z
    z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);

    if or(color_flag==[2,5]) then
         sz=mget(2,'il',fd); // data.color
	 clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
    end
    hiddencolor    = mget(1,'il',fd); // hidden_color

    // plot3d modify the given rotation angles
    // trick to force keeping the good rotation angles F.Leray 18.02.05
    // same issue with axes properties... B.Jofret 21.04.09
    a=gca();
    rotation_angles = a.rotation_angles;
    axes_visible = a.axes_visible;
    box = a.box;
    margins = a.margins;
    x_label_visible = a.x_label.visible;
    y_label_visible = a.y_label.visible;
    z_label_visible = a.z_label.visible;
	x_label_text = a.x_label.text;
	y_label_text = a.y_label.text;
	z_label_text = a.z_label.text;
	axes_isoview = a.isoview;


    if or(color_flag==[2 5]) then
      plot3d1(x,y,list(z,clr))
    else
      plot3d(x,y,z)
    end

    // Restore this properties after plot3d.
    a.rotation_angles = rotation_angles;
    a.axes_visible = axes_visible;
    a.box = box;
    a.margins = margins;
    a.x_label.visible = x_label_visible;
    a.y_label.visible = y_label_visible;
    a.z_label.visible = z_label_visible;
	a.x_label.text = x_label_text;
	a.y_label.text = y_label_text;
	a.z_label.text = z_label_text;
	a.isoview = axes_isoview;


    h=gce();
    set(h,"visible",visible)
    set(h,"surface_mode",surface_mode)
    set(h,"thickness",thickness)
    set(h,"foreground",foreground),
    set(h,"color_mode",color_mode),
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    if is_higher_than([3 0 0 0]) then
      set(h,"mark_size_unit",msu),
      set(h,"mark_foreground",mark_foreground),
      set(h,"mark_background",mark_background)
    end
    set(h,"mark_mode",mark_mode)
    set(h,"color_flag",color_flag),
    set(h,"hiddencolor",hiddencolor),

    if is_higher_than([4 1 2 0])
      clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
      if clip_state=='on' then
        set(h,"clip_box", mget(4,'dl',fd)) // clip_box
      end
      set(h,"clip_state",clip_state);
    end

    load_user_data(fd)

  case "Fac3d" then

    visible=toggle(mget(1,characterFormat,fd)) // visible
    surface_mode   = toggle(mget(1,characterFormat,fd)) // surface_mode
    foreground     = mget(1,'il',fd); // foreground
    thickness      = mget(1,'sl',fd); // thickness
    mark_mode      = toggle(mget(1,characterFormat,fd)) // mark_mode
    mark_style     = mget(1,characterFormat,fd); // mark_style
    mark_size      = mget(1,characterFormat,fd); // mark_size
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,characterFormat,fd))=='t' then // mark_size_unit
	msu='tabulated';
      else
	  msu='point';
      end
      mark_foreground=mget(1,'il',fd) ; // mark_foreground
      mark_background=mget(1,'il',fd) ; // mark_background
    end
    if is_higher_than([5 1 1 0]) then
	  color_mode     = mget(1,'il',fd); // color_mode
	  color_flag     = mget(1,'il',fd); // color_flag
	else
	  color_mode     = mget(1,characterFormat,fd);
	  color_flag     = mget(1,characterFormat,fd);
	end

    sz=mget(2,'il',fd); // data.x
    x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.y
    y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.z
    z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);

    if is_higher_than([3 1 0 1]) & color_flag >= 2 then
      sz=mget(2,'il',fd); // data.z
      clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);

      if ascii(mget(1,characterFormat,fd)) == 's' then // cdata_mapping
	cdata_mapping = 'scaled' ;
      else
	cdata_mapping = 'direct'  ;
      end

    elseif or(color_flag==[2 5]) then
      // compatibility with old version
      sz=mget(2,'il',fd); // data.z
      clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
    end
    hiddencolor    = mget(1,'il',fd); // hiddencolor

    // plot3d modify the given rotation angles
    // trick to force keeping the good rotation angles F.Leray 18.02.05
    // same issue with axes properties... B.Jofret 21.04.09
	// and labels text and isoview
    a=gca();
    rotation_angles = a.rotation_angles;
    axes_visible = a.axes_visible;
    box = a.box;
    margins = a.margins;
    x_label_visible = a.x_label.visible;
    y_label_visible = a.y_label.visible;
    z_label_visible = a.z_label.visible;
	x_label_text = a.x_label.text;
	y_label_text = a.y_label.text;
	z_label_text = a.z_label.text;
	axes_isoview = a.isoview;

    if is_higher_than([3 1 0 1]) & color_flag >= 2 then
      plot3d1(x,y,list(z,clr))
    elseif or(color_flag==[2 5]) then
      plot3d1(x,y,list(z,clr))
    else
      plot3d(x,y,z)
    end

   // Restore this properties after plot3d.
    a.rotation_angles = rotation_angles;
    a.axes_visible = axes_visible;
    a.box = box;
    a.margins = margins;
    a.x_label.visible = x_label_visible;
    a.y_label.visible = y_label_visible;
    a.z_label.visible = z_label_visible;
	a.x_label.text = x_label_text;
	a.y_label.text = y_label_text;
	a.z_label.text = z_label_text;
	a.isoview = axes_isoview;

    h=gce();
    set(h,"visible",visible)
    set(h,"surface_mode",surface_mode)
    set(h,"thickness",thickness)
    set(h,"foreground",foreground),
    set(h,"color_mode",color_mode),
    set(h,"color_flag",color_flag),
    set(h,"hiddencolor",hiddencolor),
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_mode",mark_mode)
    if is_higher_than([3 0 0 0]) then
      set(h,"mark_size_unit",msu),
      set(h,"mark_foreground",mark_foreground),
      set(h,"mark_background",mark_background)
    end

    if is_higher_than([3 1 0 1]) & color_flag >= 2 then
      set(h,"cdata_mapping",cdata_mapping);
    end

    if is_higher_than([4 1 2 0])
      clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
      if clip_state=='on' then
        set(h,"clip_box", mget(4,'dl',fd)) // clip_box
      end
      set(h,"clip_state",clip_state);
    end

    load_user_data(fd) ; // user_data

  case "Compound"
    // children
    n=mget(1,'il',fd)
    H=[]
    for k=1:n
      htmp = load_graphichandle(fd)
      H=[htmp H]
    end
    h=glue(H)
    if is_higher_than([3 1 0 1]) then // visible
     h.visible = toggle(mget(1,characterFormat,fd)) ;
    end

    load_user_data(fd) // user_data


  case "Agregation" // for compatibility with old save

    // children
    n=mget(1,'il',fd)
    H=[]
    for k=1:n
      htmp = load_graphichandle(fd)
      H=[htmp H]
    end
    h=glue(H)

    if is_higher_than([3 1 0 1]) then // visible
      h.visible = toggle(mget(1,characterFormat,fd)) ;
    end

    load_user_data(fd) // user_data

  case "Rectangle"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    thickness      = mget(1,'sl',fd); // thickness
    mark_mode      = toggle(mget(1,characterFormat,fd)) // mark_mode
    mark_style     = mget(1,characterFormat,fd); // mark_style
    mark_size      = mget(1,characterFormat,fd); // mark_size
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,characterFormat,fd))=='t' then // mark_size_unit
	msu='tabulated' ;
      else
	msu='point';
      end
      mark_foreground=mget(1,'il',fd) // mark_foreground
      mark_background=mget(1,'il',fd) // mark_background
    else
      msu='tabulated'
    end

    line_mode      = toggle(mget(1,characterFormat,fd)) ; // line_mode
    line_style     = mget(1,characterFormat,fd); // line_style
    fill_mode      = toggle(mget(1,characterFormat,fd)) ; // fill_mode
    foreground     = mget(1,'il',fd); // foreground

    if is_higher_than([3 1 0 1]) then
      background = mget(1,'il',fd); // background
    end
    if (is_higher_than([5 0 3 0])) then
	  // data size might be 4 or 5
	  data = mget(mget(1,'il',fd),'dl',fd); // data
	else
	  parentAxes = gca();
	  if (parentAxes.view == "2d") then
        data = mget(4,'dl',fd);
	  else
	    data = mget(5,'dl',fd);
	  end
	end
    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ; // clip_stata
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
    else
      clip_box=[]
    end

    // draw the rectangle
    xrect(0,1,1,1); // create the rectangle with dummy values
    h=get('hdl')
	set(h,"data",data);
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_size_unit",msu),
    if is_higher_than([3 0 0 0]) then
      set(h,"mark_foreground",mark_foreground) ;
      set(h,"mark_background",mark_background) ;
    end
    set(h,"mark_mode",mark_mode)
    set(h,"line_style",line_style)
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground) ;
    if is_higher_than([3 1 0 1]) then
      set(h,"background",background) ;
    end
    set(h,"line_mode",line_mode)
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);

    load_user_data(fd) ; // user_data

  case "Arc"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    thickness      = mget(1,'sl',fd); // thickness
    line_style     = mget(1,characterFormat,fd);  // line_style

    if is_higher_than([3 1 0 1])
      line_mode = toggle(mget(1,characterFormat,fd)) ; // line_mode
    end

    fill_mode      = toggle(mget(1,characterFormat,fd)) // fill_mode
    foreground     = mget(1,'il',fd); // foreground

    if is_higher_than([3 1 0 1]) then
      background = mget(1,'il',fd) ; // background
    end

    if (is_higher_than([5 0 3 0])) then
	  // data size might be 6 or 7
	  data = mget(mget(1,'il',fd),'dl',fd); // data
	else
      parentAxes = gca();
	  if (parentAxes.view == "2d") then
        data = mget(6,'dl',fd);
	  else
	    data = mget(7,'dl',fd);
	  end
	end

    if is_higher_than([4 1 2 0]) then
      drawing_method = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)); // drawing_method
    else
      drawing_method = "nurbs";
    end

    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
    else
      clip_box=[]
    end
	xarc(0,1,1,1,0,360); // create the arc dummy values
    h=get('hdl')
	if ~is_higher_than([4 1 2 0]) then
      // angle wass stored by 64th of degree
      data($) = data($) / 64;
	  data($-1) = data($-1) / 64;
    end
	set(h,"data",data);
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"line_style",line_style)
    set(h,"line_mode",line_mode);
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground) ;
    set(h,"background",background) ;
    set(h,"arc_drawing_method", drawing_method) ;
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);

    load_user_data(fd) // user_data

  case "Champ"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    sz=mget(2,'il',fd); // data.x
    x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.y
    y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.fx
    fx=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.fy
    fy=matrix(mget(prod(sz),'dl',fd),sz(1),-1);

    // draw the champ
    champ(x,y,fx,fy);
    h=gce();

    set(h,"visible",visible);
    set(h,"line_style",mget(1,characterFormat,fd)); // line_style
    set(h,"thickness",mget(1,'sl',fd)) // thickness
    set(h,"colored",toggle(mget(1,characterFormat,fd))) // colored
    set(h,"arrow_size",mget(1,'dl',fd)) // arrow_size

    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd)) // clip_box
    end
    set(h,"clip_state",clip_state);
    load_user_data(fd) // user_data

  case "Segs"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    sz             = mget(2,'il',fd) // data
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)

    // draw the segs
    xsegs(data(:,1),data(:,2))
    h=gce()
    if size(data,2)==3 then
      h.data=data
    end
    set(h,"visible",visible);
    set(h,"line_mode" ,toggle(mget(1,characterFormat,fd))) // line_mode
    set(h,"line_style",mget(1,characterFormat,fd)); // line_style
    set(h,"thickness",mget(1,'sl',fd)) // thickness
    set(h,"arrow_size",mget(1,'dl',fd)) // arrow_size

    n=mget(1,'il',fd) // segs_color
    set(h,"segs_color",mget(n,'il',fd))
    // it is needed to set it at the end, ut I don't know why
    mark_mode = toggle(mget(1,characterFormat,fd)) ; // mark_mode
    set(h,"mark_style"           , mget(1,characterFormat,fd)) // mark_style
    set(h,"mark_size"            , mget(1,characterFormat,fd)) // mark_size
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,characterFormat,fd))=='t' then // mark_size_unit
	msu='tabulated'
      else
	msu='point';
      end
      set(h,"mark_size_unit"     , msu) ;
      set(h,"mark_foreground"    , mget(1,'il',fd)) ; // mark_foreground
      set(h,"mark_background"    , mget(1,'il',fd)) ; // mark_background
    else
      set(h,"mark_size_unit"     , 'tabulated') ; // mark_size_unit
    end

    set(h,"mark_mode", mark_mode ) ;

    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd)) // clip_box
    end
    set(h,"clip_state",clip_state);
    load_user_data(fd) // user_data

  case "Grayplot"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    if is_higher_than([3 0 0 0]) then
      sz=mget(2,'il',fd); // data.x
      x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
      sz=mget(2,'il',fd); // data.y
      y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
      sz=mget(2,'il',fd); // data.z
      z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    else
      sz = mget(2,'il',fd) // data
      data = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    end

    data_mapping   = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // data_mapping
    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
    else
      clip_box=[]
    end

    // draw the grayplot
    if is_higher_than([3 0 0 0]) then
      grayplot(x,y,z)
    else
      grayplot(data(2:$,1),data(1,2:$),data(2:$,2:$))
    end

    h=get('hdl')
    set(h,"visible",visible)
    set(h,"data_mapping",data_mapping)
    if clip_state=='on' then
      set(h,"clip_box",  clip_box)
    end
    set(h,"clip_state",clip_state);

    load_user_data(fd) // user_data

  case "Matplot"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    sz=mget(2,'il',fd); // data
    data=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
//    data_mapping   = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))
    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
    else
      clip_box=[]
    end

    // draw the matplot
    Matplot(data);

    h=get('hdl')
    set(h,"visible",visible)
//    set(h,"data_mapping",data_mapping)
    if clip_state=='on' then
      set(h,"clip_box",  clip_box)
    end
    set(h,"clip_state",clip_state);
    // user_data
    load_user_data(fd)

  case "Fec"
    visible        = toggle(mget(1,characterFormat,fd)) // visible
    sz             = mget(2,'il',fd) // data
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    sz             = mget(2,'il',fd) // triangles
    triangles      = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    z_bounds       = mget(2,'dl',fd) // z_bounds
    // draw the fec
    fec(data(:,1),data(:,2),triangles,data(:,3))
    h=unglue(get('hdl'))
    set(h,"visible",visible)
    set(h,"z_bounds",z_bounds)
	if is_higher_than( [5 0 3 0] ) then
	  set(h,"color_range",mget(2,'dl',fd)); // color_range
	  set(h,"outside_colors",mget(2,'dl',fd)); // color_range
	  set(h,"line_mode" ,toggle(mget(1,characterFormat,fd))) // line_mode
	  set(h,"foreground", mget(1,'il',fd)); // foreground
	end
    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd)) // clip_box
    end
    set(h,"clip_state",clip_state);
    load_user_data(fd) // user_data

   case "Legend"
    if is_higher_than( [5 0 0 0] ) then
      global %LEG
      %LEG=[];
      %LEG.visible         = toggle(mget(1,characterFormat,fd)) // visible
      %LEG.text            = load_text_vector(fd); // text
      %LEG.font_style      = mget(1,characterFormat,fd); // font_style
      %LEG.font_size       = mget(1,characterFormat,fd); // font_size
      %LEG.font_color      = mget(1,'il',fd); // font_size
      %LEG.fractional_font = toggle(mget(1,characterFormat,fd)); // fractional_font
      nlegends             = mget(1,characterFormat,fd);
      paths = list()
      for kl=1:nlegends
	paths($+1)         = mget(mget(1,'il',fd),'il',fd);
      end
      %LEG.paths           = paths
      %LEG.legend_location = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd))
      %LEG.position        = mget(2,'dl',fd)
      %LEG.line_mode       = toggle(mget(1,characterFormat,fd))
      %LEG.thickness       = mget(1,'sl',fd)
      %LEG.foreground      = mget(1,'il',fd)
      %LEG.fill_mode       = toggle(mget(1,characterFormat,fd))
      %LEG.background      = mget(1,'il',fd)

      %LEG.clip_state      = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
      if %LEG.clip_state=='on' then
	%LEG.clip_box      = mget(4,'dl',fd); // clip_box
      end
      load(fd,"user_data")
      %LEG.user_data       = user_data;
    else
      visible         = toggle(mget(1,characterFormat,fd)) // visible
      line_mode       = toggle(mget(1,characterFormat,fd)) // line_mode
      mark_mode       = toggle(mget(1,characterFormat,fd)) // mark_mode
      mark_foreground = mget(1,'il',fd) ; // mark_foreground
      mark_background = mget(1,'il',fd) ; // mark_background

      //text=ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // text

      text = load_text_vector(fd); // text

      //create the legend
      //get the number of lines of the legend
      lineFeedPosition = strindex(text,'@')
      nbLines = size( lineFeedPosition ) ;
      nbLines = nbLines(2) + 1
      //create as many curves as lines in the text
      nullVector = zeros(1,nbLines);
      //draw the legend
      plot2d(0,nullVector,leg=text) ;
      H=unglue(get('hdl'));
      h=H(1);
      delete(H(2));

      set(h,"visible",visible)
      set(h,"line_mode",line_mode);
      set(h,"mark_mode",mark_mode);
      set(h,"mark_foreground",mark_foreground) ;
      set(h,"mark_background",mark_background) ;
      set(h,"foreground", mget(1,'il',fd)); // foreground

      set(h,"font_style", mget(1,characterFormat,fd)); // font_style
      set(h,"font_size" , mget(1,characterFormat,fd)); // font_size
      if is_higher_than( [4 1 2 0] ) then
	set(h,"fractional_font" , toggle(mget(1,characterFormat,fd))); // fractional_font
      end
      clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
      if clip_state=='on' then
	set(h,"clip_box",mget(4,'dl',fd)); // clip_box
      end
      set(h,"clip_state",clip_state);
    end

  case "Text"
    visible         = toggle(mget(1,characterFormat,fd)) // visible

    if is_higher_than( [4 1 2 0] ) then
      text            = load_text_matrix( fd ) ;
    else
      text            = load_text_vector(fd) // text
    end
    sz              = mget(2,characterFormat,fd)
    data            = matrix(mget(prod(sz),'dl',fd),sz(1),-1) // data
    text_box        = mget(2,'dl',fd) // text_box
    text_box_mode   = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)); // text_box_mode

    // draw the text
    if text_box_mode == 'off' then
      xstring(data(1),data(2),text)
    else
      xstringb(data(1),data(2),text,text_box(1),text_box(2))
    end

    h=get('hdl');
	set(h,"data",data);
    set(h,"visible",visible) ;
    set(h,"text_box_mode",text_box_mode)
    set(h,"foreground"           , mget(1,'il',fd)); // foreground
    set(h,"font_style"           , mget(1,characterFormat,fd)); // font_style

    if text_box_mode == 'filled' then // font_size
      mget(1,characterFormat,fd) ;
    else
      set(h,"font_size", mget(1,characterFormat,fd));
    end

    set(h,"font_angle"           , mget(1,'dl',fd)); // font_angle

    //adding JB Silvy 28/11/05
    // box drawing
    if is_higher_than([3 1 0 1]) then
      set( h, "box"      , toggle( mget( 1, characterFormat, fd ) ) ) ; // box
      set( h, "line_mode", toggle( mget( 1, characterFormat, fd ) ) ) ; // line_mode
      set( h, "fill_mode", toggle( mget( 1, characterFormat, fd ) ) ) ; // fill_mode

      set( h, "font_foreground", mget( 1, 'il', fd ) ) ; // font_foreground
      set( h, "background"     , mget( 1, 'il', fd ) ) ; // background
    end

    if is_higher_than( [4 1 2 0] ) then
      set( h, "alignment", ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)  ) ) ; // alignment
      set( h, "fractional_font", toggle( mget( 1, characterFormat, fd ) ) ) ; // fractional_font
    end

    clip_state     = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
      set(h,"clip_box",clip_box) ; // clip_box
    else
      clip_box=[]
    end
    set(h,"clip_state",clip_state);
    load_user_data(fd) // user_data
  case 'Axis'
    if is_higher_than([3 1 0 0]) then

      visible          = toggle(mget(1,characterFormat,fd)) // visible
      n                = mget(1,'il',fd) // tics_direction
      tics_direction   = ascii(mget(n,characterFormat,fd));
      nx               = mget(1,'il',fd) // xtics_coord
      xtics_coord      = mget(nx,'dl',fd)'
      ny               = mget(1,'il',fd) // ytics_coord
      ytics_coord      = mget(ny,'dl',fd)'
      if nx>1 then axisdir='u',else axisdir='l',end
      drawaxis(x=xtics_coord,y=ytics_coord,dir=axisdir);
      h=gce()

      h.tics_color       = mget(1,'il',fd) // tics_color
      h.tics_segment     = toggle(mget(1,characterFormat,fd)) // tics_segment
      h.tics_style       = ascii(mget(1,characterFormat,fd)) // tics_style
      h.sub_tics         = mget(1,'il',fd) // sub_tics
      h.tics_labels     = load_text_vector(fd)' // tics_label
      labelfontsize = mget(1,'il',fd);
      // Bug fix: there was a bug in Scilab <=4.1.2 which used -1 as default value for labels_font_size
      // Scilab 5 needs font size to be >= 0 so we change the value to avoid an error message due to a Scilab bug...
      if labelfontsize == -1 then
	//labelfontsize = 0;
      end
      h.labels_font_size = labelfontsize // label_font_size
      h.labels_font_color= mget(1,'il',fd); // labels_font_color
      if is_higher_than( [4 1 2 0] ) then
        set( h, "fractional_font", toggle( mget( 1, characterFormat, fd ) ) ) ; // fractional_font
      end
      // h.tics_style=tics_style // jb Silvy apparently strange

      clip_state       = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) // clip_state
      if clip_state == 'on' then
	set(h,"clip_box",clip_box)
      end
      set(h,"clip_state",clip_state);
      load_user_data(fd) // user_data
    end

  case "uimenu"
    if is_higher_than( [4 1 2 0] ) then
      h = uimenu("parent", gcf());
      h.enable = toggle(mget(1,"c",fd)); // Enable
      ncolors = mget(1,"il",fd); // Foregroundcolor (size)
      h.foregroundcolor = mget(ncolors,"dl",fd); // ForegroundColor (data)
      h.label = ascii(mget(mget(1,"c",fd),"c",fd)); // Label
      h.visible = toggle(mget(1,"c",fd)); // Visible
      h.callback = ascii(mget(mget(1,"c",fd),"c",fd)); // Callback
      h.callback_type = mget(1,"il",fd); // Callback Type
      h.tag = ascii(mget(mget(1,"c",fd),"c",fd)); // Tag
    end
    if is_higher_than( [5 1 2 0] ) then // 5.2 and higher
      h.checked = toggle(mget(1,"c",fd)); // Checked
    end

  case "uicontrol"
    if is_higher_than( [4 1 2 0] ) then
      uistyle = ascii(mget(mget(1,"c",fd),"c",fd)); // Style
      h = uicontrol("parent",gcf(), "style", uistyle);
      ncolors = mget(1,"il",fd); // BackgroundColor (size)
      h.backgroundcolor = mget(ncolors,"dl",fd); // BackgroundColor (data)
      h.enable = toggle(mget(1,"c",fd)); // Enable
      h.fontangle = ascii(mget(mget(1,"c",fd),"c",fd)); // FontAngle
      h.fontname = ascii(mget(mget(1,"c",fd),"c",fd)); // FontName
      fontsize_in_units = mget(1,"dl",fd); // FontSize
      h.fontunits = ascii(mget(mget(1,"c",fd),"c",fd)); // FontUnits
      h.fontsize = fontsize_in_units; // FontSize written after 'FontUnits' to avoid them to be computed again
      h.fontweight = ascii(mget(mget(1,"c",fd),"c",fd)); // FontWeight
      ncolors = mget(1,"il",fd); // Foregroundcolor (size)
      h.foregroundcolor = mget(ncolors,"dl",fd); // ForegroundColor (data)
      h.horizontalalignment = ascii(mget(mget(1,"c",fd),"c",fd)); // HorizontalAlignment
      ndata = mget(1,"il",fd); // ListboxTop (size)
      h.listboxtop = mget(ndata,"dl",fd); // ListboxTop (data)
      h.max = mget(1,"dl",fd); // Max
      h.min = mget(1,"dl",fd); // Min
      ndata = mget(1,"il",fd); // Position (size)
      position_in_units = mget(ndata,"dl",fd); // Position (data)
      h.relief = ascii(mget(mget(1,"c",fd),"c",fd)); // Relief
      ndata = mget(1,"il",fd); // SliderStep (size)
      h.sliderstep = mget(ndata,"dl",fd); // SliderStep (data)
      h.string = load_text_matrix(fd) ; // String
      h.units = ascii(mget(mget(1,"c",fd),"c",fd)); // Units
      h.position = position_in_units; // Position written after 'Units' to avoid them to be computed again
      ndata = mget(1,"il",fd); // Value (size)
      h.value = mget(ndata,"dl",fd); // Value (data)
      h.verticalalignment = ascii(mget(mget(1,"c",fd),"c",fd)); // VerticalAlignment
      h.visible = toggle(mget(1,"c",fd)); // Visible
      h.callback = ascii(mget(mget(1,"c",fd),"c",fd)); // Callback
      h.callback_type = mget(1,"il",fd); // Callback Type
      load_user_data(fd); // Userdata
      h.tag = ascii(mget(mget(1,"c",fd),"c",fd)); // Tag
    end

  else
      warning("type " +typ+" unhandled");
  end

endfunction

function r=toggle(k)
  r=emptystr(k)+'on'
  r(k==0)='off'
endfunction

function load_user_data(fd)
  if is_higher_than([3 1 0 0]) then
    h; //make a copy of the calling context h here
    load(fd,'user_data')
    if user_data<>[] then
      h.user_data=user_data;
    end
  end
endfunction

function r=is_higher_than(v)
//check if current version is strictly higher than the given one
  r=%f
  for k=1:4
    if version(k)>v(k) then r=%t,break,end
    if version(k)<v(k) then r=%f,break,end
  end
endfunction

function text=load_text_vector(fd)
  T=mget(mget(1,'il',fd),characterFormat,fd)
  newline=[find(T==10) size(T,'*')+1]
  text=[]
  p=1
  for k=1:size(newline,'*')
    text=[text;ascii(T(p:newline(k)-1))]
    p=newline(k)+1
  end
endfunction

// retrieve a string matrix saved by save_text_matrix
function strMat = load_text_matrix( fd )
  nbRow = mget( 1, 'il', fd ) ;
  nbCol = mget( 1, 'il', fd ) ;
  for i = 1:nbRow
    for j = 1:nbCol
      strMat(i,j) = ascii(mget(mget(1,characterFormat,fd),characterFormat,fd)) ;
    end
  end
endfunction

function links=get_links_from_path(ax,paths)
//  ax is a  handle on an axes entity
//  paths a list or row vector which gives the set of paths relative to
//  the axes
  links=[];ok=%t
  for p=paths
    e=ax;
    for kp=1:size(p,'*'),
      if or(e.type==['Axes','Compound'])&p(kp)<=size(e.children,'*') then
	e=e.children(p(kp)),
      else
	ok=%f
	break
      end
    end
    if ~ok then break,end
    links=[links,e]
  end
  if ~ok then links=[],end
endfunction
