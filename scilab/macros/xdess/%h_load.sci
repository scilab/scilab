function h=%h_load(fd)
//Author S. Steer Sept 2004, Copyright INRIA
  if exists('xload_mode')==0 then xload_mode=%f,end
  version=mget(4,'c',fd)
  immediate_drawing="";
  [h,immediate_drawing] = load_graphichandle(fd)
//  f=gcf();f.visible='on'
//  draw(f)
  f=gcf(); 
  f.immediate_drawing = immediate_drawing;
endfunction

function [h,immediate_drawing] = load_graphichandle(fd)
//Author S. Steer Sept 2004, Copyright INRIA
  typ=ascii(mget(mget(1,'c',fd),'c',fd))
  if typ<>'Figure'
    f=gcf();
    immediate_drawing = f.immediate_drawing;
    f.immediate_drawing ='off';
  end
  select typ
  case "Figure"
    
    if xload_mode then
      h=gcf()
      visible=toggle(mget(1,'c',fd));
      figure_position=mget(2,'sl',fd);
      figure_size=mget(2,'sl',fd);
      axes_size=mget(2,'sl',fd);
      auto_resize=toggle(mget(1,'c',fd));
      figure_name=ascii(mget(mget(1,'c',fd),'c',fd))
      figure_id=mget(1,'sl',fd);
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3)
      pixmap=toggle(mget(1,'c',fd));
      pixel_drawing_mode=ascii(mget(mget(1,'c',fd),'c',fd))
      immediate_drawing=toggle(mget(1,'c',fd)); // init. global variable immediate_drawing
      h.immediate_drawing = 'off'; // set it to 'off' to pass useless redraw due to several 'set' calls
      h.background=mget(1,'il',fd)
      rotation_style=ascii(mget(mget(1,'c',fd),'c',fd))
    else
      visible=toggle(mget(1,'c',fd));
      figure_position=mget(2,'sl',fd);
      figure_size=mget(2,'sl',fd);
      axes_size=mget(2,'sl',fd);
      auto_resize=toggle(mget(1,'c',fd));
      figure_name=ascii(mget(mget(1,'c',fd),'c',fd))
      figure_id=mget(1,'sl',fd);
      h=scf(figure_id)
      h.visible=visible; // can be set now as we act on immediate_drawing everywhere else F.Leray 18.02.05
      h.figure_position=figure_position
      h.figure_size=figure_size
      h.axes_size=axes_size
      h.auto_resize=auto_resize
      h.figure_name=figure_name
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3)
      h.pixmap=toggle(mget(1,'c',fd));
      h.pixel_drawing_mode=ascii(mget(mget(1,'c',fd),'c',fd))
      immediate_drawing=toggle(mget(1,'c',fd)); // init. global variable immediate_drawing
      h.immediate_drawing = 'off'; // set it to 'off' to pass useless redraw due to several 'set' calls
      h.background=mget(1,'il',fd);
      h.rotation_style=ascii(mget(mget(1,'c',fd),'c',fd))
    end
    n_axes=mget(1,'il',fd);
    if n_axes==1 then
      load_graphichandle(fd)
    else
      for k=1:n_axes
	xsetech(wrect=[0 0 .1 .1])
	load_graphichandle(fd)
      end
    end
  case "Axes" 
    a=gca();title=a.title;x_label=a.x_label;y_label=a.y_label
    set(a,"visible"              , toggle(mget(1,'c',fd)))
    if and(version==[3 0 0 0]) then
      axes_visible= toggle(mget(1,'c',fd));
      axes_visible=emptystr(1,3)+axes_visible
    else
      axes_visible= toggle(mget(mget(1,'c',fd),'c',fd));
    end
    set(a,"axes_visible"         ,axes_visible)
    set(a,"grid"                 , mget(mget(1,'c',fd),'il',fd))
    set(a,"x_location"           , ascii(mget(mget(1,'c',fd),'c',fd)))
    set(a,"y_location"           , ascii(mget(mget(1,'c',fd),'c',fd)))
    view            =  ascii(mget(2,'c',fd));

    set(title,"visible"          , toggle(mget(1,'c',fd)))
    set(title,"text"             , ascii(mget(mget(1,'c',fd),'c',fd)))
    set(title,"foreground"       , mget(1,'il',fd));
    set(title,"font_style"       , mget(1,'c',fd));
    set(title,"font_size"        , mget(1,'c',fd));
    set(x_label,"visible"        , toggle(mget(1,'c',fd)))
    set(x_label,"text"           , ascii(mget(mget(1,'c',fd),'c',fd)))
    set(x_label,"foreground"     , mget(1,'il',fd));
    set(x_label,"font_style"     , mget(1,'c',fd));
    set(x_label,"font_size"      , mget(1,'c',fd));
    set(y_label,"visible"        , toggle(mget(1,'c',fd)))
    set(y_label,"text"           , ascii(mget(mget(1,'c',fd),'c',fd)))
    set(y_label,"foreground"     , mget(1,'il',fd));
    set(y_label,"font_style"     , mget(1,'c',fd));
    set(y_label,"font_size"      , mget(1,'c',fd));

    if view=='3d' then
      z_label=a.z_label
      set(z_label,"visible"        , toggle(mget(1,'c',fd)))
      set(z_label,"text"           , ascii(mget(mget(1,'c',fd),'c',fd)))
      set(z_label,"foreground"     , mget(1,'il',fd));
      set(z_label,"font_style"     , mget(1,'c',fd));
      set(z_label,"font_size"      , mget(1,'c',fd));
    end
    if ~and(version==[3 0 0 0]) then
      auto_ticks=toggle(mget(mget(1,'c',fd),'c',fd));

      ticks=['ticks','locations','labels']
      sz=mget(1,'sl',fd)
      x_ticks_locations=mget(sz,'dl',fd)'
      lz=mget(sz,'c',fd)
      x_ticks_labels=[];for ks=1:sz,x_ticks_labels(ks)=ascii(mget(lz(ks),'c',fd));end
      set(a,'x_ticks',tlist(ticks,x_ticks_locations,x_ticks_labels))
      
      sz=mget(1,'sl',fd)
      y_ticks_locations=mget(sz,'dl',fd)'
      lz=mget(sz,'c',fd)
      y_ticks_labels=[];for ks=1:sz,y_ticks_labels(ks)=ascii(mget(lz(ks),'c',fd));end
      set(a,'y_ticks',tlist(ticks,y_ticks_locations,y_ticks_labels))

      sz=mget(1,'sl',fd)
      z_ticks_locations=mget(sz,'dl',fd)'
      lz=mget(sz,'c',fd)
      z_ticks_labels=[];for ks=1:sz,z_ticks_labels(ks)=ascii(mget(lz(ks),'c',fd));end
      set(a,'z_ticks',tlist(ticks,z_ticks_locations,z_ticks_labels))

      set(a,"auto_ticks"           , auto_ticks)
    end
    set(a,"box"                  , toggle(mget(1,'c',fd)))
    set(a,"sub_tics"             , mget(mget(1,'c',fd),'c',fd))
    mget(1,'il',fd); // tics_color is removed F.Leray 15.03.05
    set(a,"font_style"           , mget(1,'c',fd));
    set(a,"font_size"            , mget(1,'c',fd));
    set(a,"font_color"           , mget(1,'il',fd));
    set(a,"isoview"              , toggle(mget(1,'c',fd)))
 
    cube_scaling    = toggle(mget(1,'c',fd))
    rotation_angles = mget(2,'dl',fd);
    
    if a.view=='2d' then
      set(a,"view"               , view);
    end
    if a.view=='3d' then
      set(a,"cube_scaling"      , cube_scaling)
      if view=='3d' then
	set(a,"rotation_angles"   , rotation_angles)
      end
    end
    if and(version==[3 0 0 0]) then
      set(a,"log_flags"            , ascii(mget(2,'c',fd)));
    else
      set(a,"log_flags"            , ascii(mget(3,'c',fd)));
    end
    set(a,"tight_limits"         , toggle(mget(1,'c',fd)))

    data_bounds = matrix(mget(mget(1,'c',fd),'dl',fd),2,-1)
    if view=='2d'& a.view=='3d' then
      data_bounds(2,3)=0;
    end
    if xload_mode 
      old_bounds=a.data_bounds;
      for k=1:size(old_bounds,2)
	data_bounds(1,k)=min(data_bounds(1,k),old_bounds(1,k));
	data_bounds(2,k)=max(data_bounds(2,k),old_bounds(2,k));
      end
    end
    if ~and(version==[3 0 0 0]) then
      if mget(1,'c',fd)<>0 then
	set(a,"zoom_box"          , mget(4,'dl',fd))  
      end
    end
    set(a,"axes_bounds"          , mget(4,'dl',fd))  
    set(a,"auto_clear"           , toggle(mget(1,'c',fd)))
    set(a,"auto_scale"           , toggle(mget(1,'c',fd)))
    set(a,"hiddencolor"          , mget(1,'il',fd)),
    set(a,"line_mode"            , toggle(mget(1,'c',fd))),
    set(a,"line_style"           , mget(1,'c',fd))
    set(a,"thickness"            , mget(1,'sl',fd)),
    set(a,"mark_mode"            , toggle(mget(1,'c',fd))),
    set(a,"mark_style"           , mget(1,'c',fd))
    set(a,"mark_size"            , mget(1,'c',fd))
    if version(1)>=3&version(2)>=1 then
      if ascii(mget(1,'c',fd))=='t' then msu='tabulated',else msu='point';end
      set(a,"mark_size_unit"   , msu)
      set(a,"mark_foreground"      , mget(1,'il',fd)),
      set(a,"mark_background"      , mget(1,'il',fd)),
    else
      set(a,"mark_size_unit"     , 'tabulated')
      set(a,"mark_foreground"      , -1),
      set(a,"mark_background"      , -2),
    end
    set(a,"foreground"           , mget(1,'il',fd)),
    set(a,"background"           , mget(1,'il',fd)),
    clip_state                   = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then set(a,"clip_box",mget(4,'dl',fd)),end
    set(a,"clip_state"           ,clip_state);
 
    for k=1: mget(1,'il',fd);
       load_graphichandle(fd)
    end
    //next lines because tools used to rebuild children change the
    //data_bounds an axes_visible properties
    set(a,"data_bounds"          , data_bounds)  
    set(a,"axes_visible"          , axes_visible)  
    
    h=a;
  case 'Polyline'
    visible=toggle(mget(1,'c',fd))
    sz=mget(2,'il',fd); data=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    line_mode      = toggle(mget(1,'c',fd))
    line_style     = mget(1,'c',fd);
    thickness      = mget(1,'sl',fd);
    polyline_style = mget(1,'c',fd);
    mark_mode      = toggle(mget(1,'c',fd))
    mark_style     = mget(1,'c',fd);
    mark_size      = mget(1,'c',fd);
    msu='tabulated'
    if version(1)>=3&version(2)>=1 then
      if ascii(mget(1,'c',fd))=='t' then msu='tabulated',else msu='point';end
      foreground     = mget(1,'il',fd);
      mark_foreground=mget(1,'il',fd)
      mark_background=mget(1,'il',fd)
    else
      foreground     = mget(1,'il',fd);
      mark_foreground=-1
      mark_background=-2
    end
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd)
    else
      clip_box=[]
    end
    xpoly(data(:,1),data(:,2))
    h=get('hdl')
    set(h,"data",data)
    set(h,"visible",visible)
    set(h,"line_mode",line_mode),
    set(h,"line_style",line_style)
    set(h,"thickness",thickness)
    set(h,"polyline_style",max(1,polyline_style)),
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_size_unit",msu),
    set(h,"mark_foreground",mark_foreground),
    set(h,"mark_background",mark_background)
    set(h,"mark_mode",mark_mode)
    set(h,"foreground",foreground),

    if clip_state =='' then clip_state='clipgrf',end
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
  case "Plot3d" then
    visible=toggle(mget(1,'c',fd))
    surface_mode   = toggle(mget(1,'c',fd))
    foreground     = mget(1,'il',fd);
    thickness      = mget(1,'sl',fd);
    mark_mode      = toggle(mget(1,'c',fd))
    mark_style     = mget(1,'c',fd);
    mark_size      = mget(1,'c',fd);
    if version(1)>=3&version(2)>=1 then
      if ascii(mget(1,'c',fd))=='t' then msu='tabulated',else msu='point';end
      mark_foreground=mget(1,'il',fd)
      mark_background=mget(1,'il',fd)
    else
      msu='tabulated'
      mark_foreground=-1
      mark_background=-2
    end
    color_mode     = mget(1,'c',fd);
    color_flag     = mget(1,'c',fd);

    sz=mget(2,'il',fd);x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    if or(color_flag==[2,5]) then
         sz=mget(2,'il',fd);clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
    end
    hiddencolor    = mget(1,'il',fd);
    
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
    set(h,"mark_size_unit",msu),
    set(h,"mark_foreground",mark_foreground),
    set(h,"mark_background",mark_background)
    set(h,"mark_mode",mark_mode)
    set(h,"color_flag",color_flag),
    set(h,"hiddencolor",hiddencolor),
  case "Fac3d" then
    visible=toggle(mget(1,'c',fd))
    surface_mode   = toggle(mget(1,'c',fd))
    foreground     = mget(1,'il',fd);
    thickness      = mget(1,'sl',fd);
    mark_mode      = toggle(mget(1,'c',fd))
    mark_style     = mget(1,'c',fd);
    mark_size      = mget(1,'c',fd);
    if version(1)>=3&version(2)>=1 then
      if ascii(mget(1,'c',fd))=='t' then msu='tabulated',else msu='point';end
      mark_foreground=mget(1,'il',fd)
      mark_background=mget(1,'il',fd)
    else
      msu='tabulated'
      mark_foreground=-1
      mark_background=-2
    end
    color_mode     = mget(1,'c',fd);
    color_flag     = mget(1,'c',fd);
 
    sz=mget(2,'il',fd);x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    if or(color_flag==[2,5]) then
         sz=mget(2,'il',fd);clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
    end
    hiddencolor    = mget(1,'il',fd);
    
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
    set(h,"color_flag",color_flag),
    set(h,"hiddencolor",hiddencolor),
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_size_unit",msu),
    set(h,"mark_mode",mark_mode)
    set(h,"mark_foreground",mark_foreground),
    set(h,"mark_background",mark_background)
    
  case "Agregation"
    n=mget(1,'il',fd)
    H=[]
    for k=1:n
      htmp = load_graphichandle(fd)
      H=[H htmp]
    end
    h=glue(H)
  case "Rectangle"
    visible        = toggle(mget(1,'c',fd))
    thickness      = mget(1,'sl',fd);
    mark_mode      = toggle(mget(1,'c',fd))
    mark_style     = mget(1,'c',fd);
    mark_size      = mget(1,'c',fd);   
    if version(1)>=3&version(2)>=1 then
      if ascii(mget(1,'c',fd))=='t' then msu='tabulated',else msu='point';end
      mark_foreground=mget(1,'il',fd)
      mark_background=mget(1,'il',fd)
    else
      msu='tabulated'
      mark_foreground=-1
      mark_background=-2
    end
    line_mode   = toggle(mget(1,'c',fd))
    line_style     = mget(1,'c',fd);      
    fill_mode      = toggle(mget(1,'c',fd))
    foreground     = mget(1,'il',fd);
    data           = mget(4,'dl',fd);
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd)
    else
      clip_box=[]
    end
    xrect(data);
    h=get('hdl')
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_size_unit",msu),
    set(h,"mark_foreground",mark_foreground),
    set(h,"mark_background",mark_background)
    set(h,"mark_mode",mark_mode)
    set(h,"line_style",line_style)
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground),
    set(h,"line_mode",line_mode)
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
  case "Arc"
    visible        = toggle(mget(1,'c',fd))
    thickness      = mget(1,'sl',fd);
    line_style     = mget(1,'c',fd);      
    fill_mode      = toggle(mget(1,'c',fd))
    foreground     = mget(1,'il',fd);
    data           = mget(6,'dl',fd);
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd)
    else
      clip_box=[]
    end
    xarc(data(1),data(2),data(3),data(4),data(5),data(6));
    h=get('hdl')
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"line_style",line_style)
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground),
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
  case "Champ"
    visible        = toggle(mget(1,'c',fd))
    sz=mget(2,'il',fd);x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);fx=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);fy=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    champ(x,y,fx,fy);
    h=gce();
        
    set(h,"line_style",mget(1,'c',fd));
    set(h,"thickness",mget(1,'sl',fd))
    set(h,"colored",toggle(mget(1,'c',fd)))
    set(h,"arrow_size",mget(1,'dl',fd))
    
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd))
    end
    set(h,"clip_state",clip_state);  
  case "Segs"
    visible        = toggle(mget(1,'c',fd))
    sz             = mget(2,'il',fd)
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    xsegs(data(:,1),data(:,2))
    h=gce()
    if size(data,2)==3 then
      h.data=data
    end
    set(h,"line_mode" ,toggle(mget(1,'c',fd)))
    set(h,"line_style",mget(1,'c',fd));
    set(h,"thickness",mget(1,'sl',fd))
    set(h,"arrow_size",mget(1,'dl',fd))
    
    n=mget(1,'il',fd)
    set(h,"segs_color",mget(n,'il',fd))
    set(h,"mark_mode" ,toggle(mget(1,'c',fd)))
    set(h,"mark_style"           , mget(1,'c',fd))
    set(h,"mark_size"            , mget(1,'c',fd))
    if version(1)>=3&version(2)>=1 then
      if ascii(mget(1,'c',fd))=='t' then msu='tabulated',else msu='point';end
      set(h,"mark_size_unit"     , msu)
      set(h,"mark_foreground"    , mget(1,'il',fd)),
      set(h,"mark_background"    , mget(1,'il',fd)),
    else
      set(h,"mark_size_unit"     , 'tabulated')
      set(h,"mark_foreground"    , -1),
      set(h,"mark_background"    , -2),
    end
    
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd))
    end
    set(h,"clip_state",clip_state);
  case "Grayplot"
    visible        = toggle(mget(1,'c',fd))
    if version(1)>=3&version(2)>=1 then
      sz=mget(2,'il',fd);x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
      sz=mget(2,'il',fd);y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
      sz=mget(2,'il',fd);z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    else
      sz = mget(2,'il',fd)
      data = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    end
    
    data_mapping   = ascii(mget(mget(1,'c',fd),'c',fd))
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd)
    else
      clip_box=[]
    end
    
    if version(1)>=3&version(2)>=1 then
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
  case "Matplot"
    visible        = toggle(mget(1,'c',fd))
    sz=mget(2,'il',fd); data=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
//    data_mapping   = ascii(mget(mget(1,'c',fd),'c',fd))
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd)
    else
      clip_box=[]
    end
    
    Matplot(data);

    h=get('hdl')
    set(h,"visible",visible)
//    set(h,"data_mapping",data_mapping)
    if clip_state=='on' then
      set(h,"clip_box",  clip_box)
    end
    set(h,"clip_state",clip_state);
  case "Fec"
    visible        = toggle(mget(1,'c',fd))
    sz             = mget(2,'il',fd)
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    sz             = mget(2,'il',fd)
    triangles      = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    z_bounds       = mget(2,'dl',fd)
    fec(data(:,1),data(:,2),triangles,data(:,3))
    h=unglue(get('hdl'))
    set(h,"visible",visible)
    set(h,"z_bounds",z_bounds)
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd))
    end
    set(h,"clip_state",clip_state);
  case "Legend"
    visible        = toggle(mget(1,'c',fd))
    text=ascii(mget(mget(1,'c',fd),'c',fd))
    plot2d(0,0,leg=text)
    H=unglue(get('hdl'));h=H(1);delete(H(2));
    set(h,"visible",visible)
    set(h,"foreground"           , mget(1,'il',fd));

    set(h,"font_style"           , mget(1,'c',fd));
    set(h,"font_size"            , mget(1,'c',fd));
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
  case "Text"
    visible        = toggle(mget(1,'c',fd))
    T=mget(mget(1,'il',fd),'c',fd)
    newline=[find(T==10) size(T,'*')+1]
    text=[]
    p=1
    for k=1:size(newline,'*')
      text=[text;ascii(T(p:newline(k)-1))]
      p=newline(k)+1
    end
    sz             = mget(2,'c',fd)
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    text_box       = mget(2,'dl',fd)
    text_box_mode  = ascii(mget(mget(1,'c',fd),'c',fd));
    if text_box_mode == 'off' then
      xstring(data(1),data(2),text)
    else
      xstringb(data(1),data(2),text,text_box(1),text_box(2))
    end
    h=get('hdl');
    set(h,"visible",visible);
    set(h,"text_box_mode",text_box_mode)
    set(h,"foreground"           , mget(1,'il',fd));
    set(h,"font_style"           , mget(1,'c',fd));
    if text_box_mode == 'filled' then
      mget(1,'c',fd)
    else
      set(h,"font_size"            , mget(1,'c',fd));
    end
    set(h,"font_angle"           , mget(1,'dl',fd));
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
   
  else
      disp('type " +typ+" unhandled");pause
  end

endfunction

function r=toggle(k)
//Author S. Steer Sept 2004, Copyright INRIA
  r=emptystr(k)+'on'
  r(k==0)='off'
endfunction
