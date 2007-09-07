function h=%h_load(fd)
//Author S. Steer Sept 2004, Copyright INRIA
  global init_immediate_drawing
  init_immediate_drawing = 0;
  
  if exists('xload_mode')==0 then xload_mode=%f,end
  version=mget(4,'c',fd)
  immediate_drawing="";
  
  h=[];
  
  if is_higher_than([3 1 0 1]) then // case 3 1 0 2 and after
    hsize = mget(2,'c',fd)
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
//Author S. Steer Sept 2004, Copyright INRIA
  global init_immediate_drawing
  typ=ascii(mget(mget(1,'c',fd),'c',fd))
  if typ<>'Figure'
    f=gcf();
    if init_immediate_drawing == 0
      immediate_drawing = f.immediate_drawing;
      f.immediate_drawing ='off';
      init_immediate_drawing = 1;
    end
  end
 
  select typ
  case "Figure"
    
    if xload_mode then
      h=gcf()
      visible=toggle(mget(1,'c',fd)); // visible
      figure_position=mget(2,'sl',fd); // figure_position
      figure_size=mget(2,'sl',fd); // figure_size
      axes_size=mget(2,'sl',fd); //axes_size
      auto_resize=toggle(mget(1,'c',fd)); // auto_resize
      figure_name=ascii(mget(mget(1,'c',fd),'c',fd)) // figure_name
      figure_id=mget(1,'sl',fd); // figure_id
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3) // color_map
      pixmap=toggle(mget(1,'c',fd)); // pixmap
      pixel_drawing_mode=ascii(mget(mget(1,'c',fd),'c',fd)) // pixel_drawing_mode
      immediate_drawing=toggle(mget(1,'c',fd));// immediate drawing // init. global variable immediate_drawing
      h.immediate_drawing = 'off';  // set it to 'off' to pass useless redraw due to several 'set' calls
      h.background=mget(1,'il',fd) // background
      rotation_style=ascii(mget(mget(1,'c',fd),'c',fd)) // rotation_style
    else
      visible=toggle(mget(1,'c',fd)); // visible
      figure_position=mget(2,'sl',fd); // figure_position
      figure_size=mget(2,'sl',fd); // figure_size
      axes_size=mget(2,'sl',fd); // axes_size
      auto_resize=toggle(mget(1,'c',fd)); // auto_resize
      figure_name=ascii(mget(mget(1,'c',fd),'c',fd)) // figure_name
      figure_id=mget(1,'sl',fd); // figure_id
      
      // create the figure
      h=scf(figure_id)
       h.visible=visible;  // can be set now as we act on immediate_drawing everywhere else F.Leray 18.02.05
      h.figure_position=figure_position
      h.figure_size=figure_size
      h.axes_size=axes_size
      h.auto_resize=auto_resize
      h.figure_name=figure_name
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3) // color_map
      h.pixmap=toggle(mget(1,'c',fd)); // pixmap
      h.pixel_drawing_mode=ascii(mget(mget(1,'c',fd),'c',fd)) // pixel_drawing_mode
      immediate_drawing=toggle(mget(1,'c',fd)); // immediate_drawing  // init. global variable immediate_drawing
      h.immediate_drawing = 'off'; // set it to 'off' to pass useless redraw due to several 'set' calls
      h.background=mget(1,'il',fd); // background
      h.rotation_style=ascii(mget(mget(1,'c',fd),'c',fd)) // rotation_style
      
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
    Title=a.title ;
    x_label=a.x_label ;
    y_label=a.y_label ;
    set(a,"visible"              , toggle(mget(1,'c',fd))) // visible
    if and(version==[3 0 0 0]) then // axes_visible
      axes_visible= toggle(mget(1,'c',fd));
      axes_visible=emptystr(1,3)+axes_visible
    else
      axes_visible= toggle(mget(mget(1,'c',fd),'c',fd)) ;
    end
    
    if is_higher_than( [3 1 0 1] ) then // axes_reverse
      axes_reverse = toggle(mget(mget(1,'c',fd),'c',fd)) ;
    end
    
    set(a,"axes_visible", axes_visible)
    set(a,"axes_reverse", axes_reverse)
    set(a,"grid"        , mget(mget(1,'c',fd),'il',fd)) //grid
    set(a,"x_location"  , ascii(mget(mget(1,'c',fd),'c',fd))) // x_location
    set(a,"y_location"  , ascii(mget(mget(1,'c',fd),'c',fd))) // y_location
    view            =  ascii(mget(2,'c',fd)); // view

    // title
    set(Title,"visible"   , toggle(mget(1,'c',fd))) // title.visible
    set(Title,"text"      , ascii(mget(mget(1,'c',fd),'c',fd))) // title.text
    set(Title,"foreground", mget(1,'il',fd)); // title.foreground
    if is_higher_than([3 1 0 0]) then
      set(Title,"background"       , mget(1,'il',fd)); // title.background
      set(Title,"fill_mode"        , toggle(mget(1,'c',fd))); //title.fill_mode
    end
    set(Title,"font_style"       , mget(1,'c',fd)); // title.font_style
    set(Title,"font_size"        , mget(1,'c',fd)); // title.font_size

    if is_higher_than([3 1 0 0]) then
      set(Title,"auto_rotation", toggle(mget(1,'c',fd))) // title.auto_rotation
      set(Title,"font_angle"   , mget(1,'dl',fd)); // title.font_angle
      set(Title,"auto_position", toggle(mget(1,'c',fd))) // title.auto_position
      set(Title,"position"     , mget(2,'dl',fd)); // title.position
    end
    
    // x_label
    set(x_label,"visible"   , toggle(mget(1,'c',fd))) // x_label.visible
    set(x_label,"text"      , ascii(mget(mget(1,'c',fd),'c',fd))) // x_label.text
    set(x_label,"foreground", mget(1,'il',fd)); // x_label.foreground
    if is_higher_than([3 0 0 0]) then
      set(x_label,"background"       , mget(1,'il',fd)); // x_label.background
      set(x_label,"fill_mode"        , toggle(mget(1,'c',fd))); // x_label.fill_mode
    end
    set(x_label,"font_style"     , mget(1,'c',fd)); // x_label.font_style
    set(x_label,"font_size"      , mget(1,'c',fd)); // x_label.font_size
    
    if is_higher_than([3 0 0 0]) then
      set(x_label,"auto_rotation"         , toggle(mget(1,'c',fd))) // x_label.auto_rotation
      set(x_label,"font_angle"            , mget(1,'dl',fd)); // x_label.font_angle
      set(x_label,"auto_position"         , toggle(mget(1,'c',fd))) // x_label.auto_position
      set(x_label,"position"              , mget(2,'dl',fd)); // x_label.position
    end
    
    // y_label
    set(y_label,"visible"        , toggle(mget(1,'c',fd)))
    set(y_label,"text"           , ascii(mget(mget(1,'c',fd),'c',fd)))
    
    set(y_label,"foreground"     , mget(1,'il',fd));
    if is_higher_than([3 0 0 0]) then
      set(y_label,"background"       , mget(1,'il',fd));
      set(y_label,"fill_mode"        , toggle(mget(1,'c',fd)));
    end
    set(y_label,"font_style"     , mget(1,'c',fd));
    set(y_label,"font_size"      , mget(1,'c',fd));
    
    if is_higher_than([3 0 0 0]) then
      set(y_label,"auto_rotation"         , toggle(mget(1,'c',fd)))
      set(y_label,"font_angle"            , mget(1,'dl',fd));
      set(y_label,"auto_position"         , toggle(mget(1,'c',fd)))
      set(y_label,"position"              , mget(2,'dl',fd));
    end
    
    if view=='3d' then
      // z_label
      z_label=a.z_label
      set(z_label,"visible"        , toggle(mget(1,'c',fd)))
      set(z_label,"text"           , ascii(mget(mget(1,'c',fd),'c',fd)))
      set(z_label,"foreground"     , mget(1,'il',fd));
      if is_higher_than([3 0 0 0]) then
	set(z_label,"background"       , mget(1,'il',fd));
	set(z_label,"fill_mode"        , toggle(mget(1,'c',fd)));
      end
      set(z_label,"font_style"     , mget(1,'c',fd));
      set(z_label,"font_size"      , mget(1,'c',fd));
      
      if is_higher_than([3 0 0 0]) then
	set(z_label,"auto_rotation"         , toggle(mget(1,'c',fd)))
	set(z_label,"font_angle"            , mget(1,'dl',fd));
	set(z_label,"auto_position"         , toggle(mget(1,'c',fd)))
	set(z_label,"position"              , mget(2,'dl',fd));
      end
    end
    
    
    
    if is_higher_than([3 0 0 0]) then
      auto_ticks=toggle(mget(mget(1,'c',fd),'c',fd)); // auto_ticks
      
      ticks=['ticks','locations','labels']
      sz=mget(1,'sl',fd) // x_ticks.locations
      x_ticks_locations=mget(sz,'dl',fd)'
      lz=mget(sz,'c',fd) // x_ticks.label
      x_ticks_labels=[];for ks=1:sz,x_ticks_labels(ks)=ascii(mget(lz(ks),'c',fd));end
      set(a,'x_ticks',tlist(ticks,x_ticks_locations,x_ticks_labels))
      
      sz=mget(1,'sl',fd) // y_ticks.locations
      y_ticks_locations=mget(sz,'dl',fd)'
      lz=mget(sz,'c',fd) // y_ticks.label
      y_ticks_labels=[];for ks=1:sz,y_ticks_labels(ks)=ascii(mget(lz(ks),'c',fd));end
      set(a,'y_ticks',tlist(ticks,y_ticks_locations,y_ticks_labels))

      sz=mget(1,'sl',fd) // z_ticks.locations
      z_ticks_locations=mget(sz,'dl',fd)'
      lz=mget(sz,'c',fd) // z_ticks.labels
      z_ticks_labels=[];for ks=1:sz,z_ticks_labels(ks)=ascii(mget(lz(ks),'c',fd));end
      set(a,'z_ticks',tlist(ticks,z_ticks_locations,z_ticks_labels))

      set(a,"auto_ticks"           , auto_ticks)
    end
    set(a,"box"                  , toggle(mget(1,'c',fd))) // box
    set(a,"sub_tics"             , mget(mget(1,'c',fd),'c',fd)) // sub_tics
    if ~(is_higher_than([3 1 0 1]) ) then 
      mget(1,'il',fd); // tics_color is removed F.Leray 15.03.05
    end
    set(a,"font_style"           , mget(1,'c',fd));  // font_style
    set(a,"font_size"            , mget(1,'c',fd));  // font_size
    set(a,"font_color"           , mget(1,'il',fd)); // font_color
    set(a,"isoview"              , toggle(mget(1,'c',fd))) // isoview
 
    cube_scaling    = toggle(mget(1,'c',fd)) // cube_scaling
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
      set(a,"log_flags"            , ascii(mget(3,'c',fd)));
    else
      set(a,"log_flags"            , ascii(mget(2,'c',fd)));
    end
    
    set(a,"tight_limits"         , toggle(mget(1,'c',fd))) // tight_limits
    data_bounds = matrix(mget(mget(1,'c',fd),'dl',fd),2,-1) // data_bounds
    
    
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
      if mget(1,'c',fd)<>0 then
	set(a,"zoom_box"          , mget(4,'dl',fd))  // zoom_box
      end
    end
    if is_higher_than([3 1 0 1]) then
      set(a,"margins"              , mget(4,'dl',fd))  // margins
    end
    
    set(a,"axes_bounds"          , mget(4,'dl',fd))  // axes_bounds
    set(a,"auto_clear"           , toggle(mget(1,'c',fd))) // auto_clear
    set(a,"auto_scale"           , toggle(mget(1,'c',fd))) // auto_scale
    set(a,"hiddencolor"          , mget(1,'il',fd)), // hidden_color
    set(a,"line_mode"            , toggle(mget(1,'c',fd))), // line_mode
    set(a,"line_style"           , mget(1,'c',fd)) // line_style
    set(a,"thickness"            , mget(1,'sl',fd)), // thickness
    set(a,"mark_mode"            , toggle(mget(1,'c',fd))), //mark_mode
    set(a,"mark_style"           , mget(1,'c',fd)) // mark_style
    set(a,"mark_size"            , mget(1,'c',fd)) // mark_size
    if is_higher_than([3 0 0 0]) then  
      
      if ascii(mget(1,'c',fd))=='t' then // mark_size_unit 
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
    clip_state                   = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
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
    set(a,"axes_visible"          , axes_visible) ;  
    
    h=a;
    load_user_data(fd) ; // user_data
    
  case "Polyline"
    visible=toggle(mget(1,'c',fd)) // visible
    sz=mget(2,'il',fd); // data
    data=matrix(mget(prod(sz),'dl',fd),sz(1),-1); 
    if is_higher_than([3 1 0 0]) then  
      closed      = toggle(mget(1,'c',fd)) // closed
    end
    line_mode      = toggle(mget(1,'c',fd)) // line_mode
    if is_higher_than([3 1 0 0]) then  
      fill_mode      = toggle(mget(1,'c',fd)) // fill_mode
    end
    line_style     = mget(1,'c',fd); // line_style
    thickness      = mget(1,'sl',fd); // thickness
    if is_higher_than([3 1 0 1]) then  
      arrow_size_factor = mget(1,'sl',fd); // arrow_size_factor
    end
    polyline_style = mget(1,'c',fd); // polyline_style
    
    if is_higher_than([3 1 0 1] ) then
      size_interp_color = mget(1,'sl',fd) ; // interp_color_vector
      interp_color_vector = mget( size_interp_color, 'dl', fd ) ;
      interp_color_mode   = toggle( mget( 1, 'c', fd ) ) ; // interp_color_mode
    end
    
    mark_mode      = toggle(mget(1,'c',fd)) // mark_mode
    mark_style     = mget(1,'c',fd); // mark_style
    mark_size      = mget(1,'c',fd); // mark_size
    msu='tabulated'
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,'c',fd))=='t' then // mark_size_unit
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
    
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
    
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
    visible=toggle(mget(1,'c',fd)) // visible
    surface_mode   = toggle(mget(1,'c',fd)) // surface_mode
    foreground     = mget(1,'il',fd); // foreground
    thickness      = mget(1,'sl',fd); // thickness
    mark_mode      = toggle(mget(1,'c',fd)) // mark_mode
    mark_style     = mget(1,'c',fd); // mark_style
    mark_size      = mget(1,'c',fd); // mark_size
    
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,'c',fd))=='t' then // mark_size_unit
	msu='tabulated' ;
      else 
	msu='point';
      end
      mark_foreground=mget(1,'il',fd) // mark_foreground
      mark_background=mget(1,'il',fd) // mark_background
    else
      msu='tabulated'  
    end
    
    color_mode     = mget(1,'c',fd); // color_mode
    color_flag     = mget(1,'c',fd); // color_flag

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
    a=gca();
    rotation_angles = a.rotation_angles;
    
    if or(color_flag==[2 5]) then
      plot3d1(x,y,list(z,clr))
    else
      plot3d(x,y,z)
    end
    
    a.rotation_angles = rotation_angles;
    
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
    load_user_data(fd)
    
  case "Fac3d" then
    
    visible=toggle(mget(1,'c',fd)) // visible
    surface_mode   = toggle(mget(1,'c',fd)) // surface_mode
    foreground     = mget(1,'il',fd); // foreground
    thickness      = mget(1,'sl',fd); // thickness
    mark_mode      = toggle(mget(1,'c',fd)) // mark_mode
    mark_style     = mget(1,'c',fd); // mark_style
    mark_size      = mget(1,'c',fd); // mark_size
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,'c',fd))=='t' then // mark_size_unit
	msu='tabulated';
      else 
	  msu='point';
      end
      mark_foreground=mget(1,'il',fd) ; // mark_foreground
      mark_background=mget(1,'il',fd) ; // mark_background
    end
    color_mode     = mget(1,'c',fd); // color_mode
    color_flag     = mget(1,'c',fd); // color_flag
 
    sz=mget(2,'il',fd); // data.x
    x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.y
    y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd); // data.z
    z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    
    if is_higher_than([3 1 0 1]) & color_flag >= 2 then
      sz=mget(2,'il',fd); // data.z
      clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
      
      if ascii(mget(1,'c',fd)) == 's' then // cdata_mapping
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
    a=gca();
    rotation_angles = a.rotation_angles;
    if is_higher_than([3 1 0 1]) & color_flag >= 2 then
      plot3d1(x,y,list(z,clr))
    elseif or(color_flag==[2 5]) then
      plot3d1(x,y,list(z,clr))
    else
      plot3d(x,y,z)
    end
    
    a.rotation_angles = rotation_angles;
    
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
     h.visible = toggle(mget(1,'c',fd)) ;
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
      h.visible = toggle(mget(1,'c',fd)) ;
    end
    
    load_user_data(fd) // user_data
  
  case "Rectangle"
    visible        = toggle(mget(1,'c',fd)) // visible
    thickness      = mget(1,'sl',fd); // thickness
    mark_mode      = toggle(mget(1,'c',fd)) // mark_mode
    mark_style     = mget(1,'c',fd); // mark_style
    mark_size      = mget(1,'c',fd); // mark_size
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,'c',fd))=='t' then // mark_size_unit
	msu='tabulated' ;
      else 
	msu='point';
      end
      mark_foreground=mget(1,'il',fd) // mark_foreground
      mark_background=mget(1,'il',fd) // mark_background
    else
      msu='tabulated'
    end
    
    line_mode      = toggle(mget(1,'c',fd)) ; // line_mode
    line_style     = mget(1,'c',fd); // line_style
    fill_mode      = toggle(mget(1,'c',fd)) ; // fill_mode
    foreground     = mget(1,'il',fd); // foreground
    
    if is_higher_than([3 1 0 1]) then
      background = mget(1,'il',fd); // background
    end
    
    data           = mget(4,'dl',fd); // data
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) ; // clip_stata
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
    else
      clip_box=[]
    end
    
    // draw the rectangle
    xrect(data);
    h=get('hdl')
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
    visible        = toggle(mget(1,'c',fd)) // visible
    thickness      = mget(1,'sl',fd); // thickness
    line_style     = mget(1,'c',fd);  // line_style
    
    if is_higher_than([3 1 0 1])
      line_mode = toggle(mget(1,'c',fd)) ; // line_mode
    end
    
    fill_mode      = toggle(mget(1,'c',fd)) // fill_mode
    foreground     = mget(1,'il',fd); // foreground
    
    if is_higher_than([3 1 0 1]) then
      background = mget(1,'il',fd) ; // background
    end
    
    data           = mget(6,'dl',fd); // data
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd) // clip_box
    else
      clip_box=[]
    end
    xarc(data(1),data(2),data(3),data(4),data(5),data(6));
    h=get('hdl')
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"line_style",line_style)
    set(h,"line_mode",line_mode);
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground) ;
    set(h,"background",background) ;
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
    
    load_user_data(fd) // user_data
    
  case "Champ"
    visible        = toggle(mget(1,'c',fd)) // visible
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
    set(h,"line_style",mget(1,'c',fd)); // line_style
    set(h,"thickness",mget(1,'sl',fd)) // thickness
    set(h,"colored",toggle(mget(1,'c',fd))) // colored
    set(h,"arrow_size",mget(1,'dl',fd)) // arrow_size
    
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd)) // clip_box
    end
    set(h,"clip_state",clip_state);  
    load_user_data(fd) // user_data
    
  case "Segs"
    visible        = toggle(mget(1,'c',fd)) // visible
    sz             = mget(2,'il',fd) // data
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    
    // draw the segs
    xsegs(data(:,1),data(:,2))
    h=gce()
    if size(data,2)==3 then
      h.data=data
    end
    set(h,"visible",visible);
    set(h,"line_mode" ,toggle(mget(1,'c',fd))) // line_mode
    set(h,"line_style",mget(1,'c',fd)); // line_style
    set(h,"thickness",mget(1,'sl',fd)) // thickness
    set(h,"arrow_size",mget(1,'dl',fd)) // arrow_size
    
    n=mget(1,'il',fd) // segs_color
    set(h,"segs_color",mget(n,'il',fd))
    set(h,"mark_mode" ,toggle(mget(1,'c',fd))) // mark_mode
    set(h,"mark_style"           , mget(1,'c',fd)) // mark_style
    set(h,"mark_size"            , mget(1,'c',fd)) // mark_size
    if is_higher_than([3 0 0 0]) then
      if ascii(mget(1,'c',fd))=='t' then // mark_size_unit 
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
    
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd)) // clip_box
    end
    set(h,"clip_state",clip_state);
    load_user_data(fd) // user_data
    
  case "Grayplot"
    visible        = toggle(mget(1,'c',fd)) // visible
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
    
    data_mapping   = ascii(mget(mget(1,'c',fd),'c',fd)) // data_mapping
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
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
    visible        = toggle(mget(1,'c',fd)) // visible
    sz=mget(2,'il',fd); // data
    data=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
//    data_mapping   = ascii(mget(mget(1,'c',fd),'c',fd))
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
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
    visible        = toggle(mget(1,'c',fd)) // visible
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
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd)) // clip_box
    end
    set(h,"clip_state",clip_state);
    load_user_data(fd) // user_data
    
    // remove case because of bugs  
  case "Legend"
    visible        = toggle(mget(1,'c',fd)) // visible
    line_mode       = toggle(mget(1,'c',fd)) // line_mode
    mark_mode       = toggle(mget(1,'c',fd)) // mark_mode
    mark_foreground = mget(1,'il',fd) ; // mark_foreground
    mark_background = mget(1,'il',fd) ; // mark_background
    //text=ascii(mget(mget(1,'c',fd),'c',fd)) // text
    
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
    
    set(h,"font_style", mget(1,'c',fd)); // font_style
    set(h,"font_size" , mget(1,'c',fd)); // font_size
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
    if clip_state=='on' then
      set(h,"clip_box",mget(4,'dl',fd)); // clip_box
    end
    set(h,"clip_state",clip_state);
    
    
  case "Text"
    visible         = toggle(mget(1,'c',fd)) // visible
    text            = load_text_vector(fd) // text
    sz              = mget(2,'c',fd)
    data            = matrix(mget(prod(sz),'dl',fd),sz(1),-1) // data
    text_box        = mget(2,'dl',fd) // text_box
    text_box_mode   = ascii(mget(mget(1,'c',fd),'c',fd)); // text_box_mode
    
    // draw the text
    if text_box_mode == 'off' then
      xstring(data(1),data(2),text)
    else
      xstringb(data(1),data(2),text,text_box(1),text_box(2))
    end
    
    h=get('hdl');
    set(h,"visible",visible) ;
    set(h,"text_box_mode",text_box_mode)
    set(h,"foreground"           , mget(1,'il',fd)); // foreground
    set(h,"font_style"           , mget(1,'c',fd)); // font_style
    
    if text_box_mode == 'filled' then // font_size
      mget(1,'c',fd) ;
    else
      set(h,"font_size", mget(1,'c',fd));
    end
    
    set(h,"font_angle"           , mget(1,'dl',fd)); // font_angle
    
    //adding JB Silvy 28/11/05
    // box drawing
    if is_higher_than([3 1 0 1]) then
      set( h, "box"      , toggle( mget( 1, 'c', fd ) ) ) ; // box
      set( h, "line_mode", toggle( mget( 1, 'c', fd ) ) ) ; // line_mode
      set( h, "fill_mode", toggle( mget( 1, 'c', fd ) ) ) ; // fill_mode
      
      set( h, "font_foreground", mget( 1, 'il', fd ) ) ; // font_foreground
      set( h, "background"     , mget( 1, 'il', fd ) ) ; // background
    end
    
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
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
     
      visible          = toggle(mget(1,'c',fd)) // visible
      n                = mget(1,'il',fd) // tics_direction
      tics_direction   = ascii(mget(n,'c',fd));
      nx               = mget(1,'il',fd) // xtics_coord
      xtics_coord      = mget(nx,'dl',fd)
      ny               = mget(1,'il',fd) // ytics_coord
      ytics_coord      = mget(ny,'dl',fd)
      if nx>1 then dir='u',else dir='l',end
      drawaxis(x=xtics_coord,y=ytics_coord,dir=dir);
      h=gce()

      h.tics_color       = mget(1,'il',fd) // tics_color
      h.tics_segment     = toggle(mget(1,'c',fd)) // tics_segment
      h.tics_style       = ascii(mget(1,'c',fd)) // tics_style
      h.sub_tics         = mget(1,'il',fd) // sub_tics
      h.tics_labels     = load_text_vector(fd) // tics_label
      h.labels_font_size = mget(1,'il',fd); // label_font_size
      h.labels_font_color= mget(1,'il',fd); // labels_font_color
      // h.tics_style=tics_style // jb Silvy apparently strange

      clip_state       = ascii(mget(mget(1,'c',fd),'c',fd)) // clip_state
      if clip_state == 'on' then
	set(h,"clip_box",clip_box)
      end
      set(h,"clip_state",clip_state);
      load_user_data(fd) // user_data
    end
  else
      warning("type " +typ+" unhandled");
  end

endfunction

function r=toggle(k)
//Author S. Steer Sept 2004, Copyright INRIA
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
  T=mget(mget(1,'il',fd),'c',fd)
  newline=[find(T==10) size(T,'*')+1]
  text=[]
  p=1
  for k=1:size(newline,'*')
    text=[text;ascii(T(p:newline(k)-1))]
    p=newline(k)+1
  end
endfunction
