function h=%h_load(fd)
//Author S. Steer Sept 2004, Copyright INRIA
  xload_mode=%f
  version=mget(4,'c',fd)
  h=load_graphichandle(fd)
  f=gcf();f.visible='on'
  draw(f)
endfunction

function   h=load_graphichandle(fd)
//Author S. Steer Sept 2004, Copyright INRIA
  typ=ascii(mget(mget(1,'c',fd),'c',fd))
  if typ<>'Figure' then f=gcf();f.visible='off';end
  select typ
  case "Figure"

    if xload_mode then
      h=gcf()
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3)
      h.background=mget(1,'il',fd)
    else
      visible=toggle(mget(1,'c',fd));
      figure_position=mget(2,'sl',fd);
      figure_size=mget(2,'sl',fd);
      axes_size=mget(2,'sl',fd);
      auto_resize=toggle(mget(1,'c',fd));
      figure_name=ascii(mget(mget(1,'c',fd),'c',fd))
      figure_id=mget(1,'sl',fd);
      h=scf(figure_id)
      h.figure_position=figure_position
      h.figure_size=figure_size
      h.axes_size=axes_size
      h.auto_resize=auto_resize
      h.figure_name=figure_name
      h.color_map=matrix(mget(mget(1,'il',fd),"dl",fd),-1,3)
      h.pixmap=toggle(mget(1,'c',fd));
      h.pixel_drawing_mode=ascii(mget(mget(1,'c',fd),'c',fd))
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
    set(a,"tics_color"           , mget(1,'il',fd));
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
    set(a,"line_style"           , mget(1,'c',fd))
    set(a,"thickness"            , mget(1,'sl',fd)),
    set(a,"mark_mode"            , toggle(mget(1,'c',fd))),
    set(a,"mark_style"           , mget(1,'c',fd))
    set(a,"mark_size"            , mget(1,'c',fd))
    set(a,"background"           , mget(1,'il',fd)),
    set(a,"foreground"           , mget(1,'il',fd)),
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
    line_style     = mget(1,'c',fd);
    thickness      = mget(1,'sl',fd);
    polyline_style = mget(1,'c',fd);
    mark_mode      = toggle(mget(1,'c',fd))
    mark_style     = mget(1,'c',fd);
    mark_size      = mget(1,'c',fd);
    foreground     = mget(1,'il',fd);
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
    set(h,"line_style",line_style)
    set(h,"thickness",thickness)
    set(h,"polyline_style",max(1,polyline_style)),
    set(h,"mark_style",mark_style),
    set(h,"mark_size",mark_size),
    set(h,"mark_mode",mark_mode)
    set(h,"foreground",foreground),
    if clip_state =='' then clip_state='clipgrf',end
    if clip_state=='on' then set(h,"clip_box",clip_box),end
    set(h,"clip_state",clip_state);
  case "Plot3d" then
    visible=toggle(mget(1,'c',fd))
    foreground     = mget(1,'il',fd);
    thickness      = mget(1,'sl',fd);
    color_mode     = mget(1,'c',fd);
    color_flag     = mget(1,'c',fd);
    sz=mget(2,'il',fd);x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    if or(color_flag==[2,5]) then
         sz=mget(2,'il',fd);clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
    end
    hiddencolor    = mget(1,'il',fd);
    if or(color_flag==[2 5]) then
      plot3d1(x,y,list(z,clr))
    else
      plot3d(x,y,z)
    end
    h=gce();
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"foreground",foreground),
    set(h,"color_mode",color_mode),

    set(h,"color_flag",color_flag),
    set(h,"hiddencolor",hiddencolor),
  case "Fac3d" then
    visible=toggle(mget(1,'c',fd))
    foreground     = mget(1,'il',fd);
    thickness      = mget(1,'sl',fd);
    color_mode     = mget(1,'c',fd);
    color_flag     = mget(1,'c',fd);
    sz=mget(2,'il',fd);x=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);y=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    sz=mget(2,'il',fd);z=matrix(mget(prod(sz),'dl',fd),sz(1),-1);
    if or(color_flag==[2,5]) then
         sz=mget(2,'il',fd);clr=matrix(mget(prod(sz),'il',fd),sz(1),-1);
    end
    hiddencolor    = mget(1,'il',fd);
    if or(color_flag==[2 5]) then
      plot3d1(x,y,list(z,clr))
    else
      plot3d(x,y,z)
    end
    h=gce();
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"foreground",foreground),
    set(h,"color_mode",color_mode),

    set(h,"color_flag",color_flag),
    set(h,"hiddencolor",hiddencolor),
  case "Agregation"
    n=mget(1,'il',fd)
    H=[]
    for k=1:n
      H=[H load_graphichandle(fd)]
    end
    h=glue(H)
  case "Rectangle"
    visible        = toggle(mget(1,'c',fd))
    thickness      = mget(1,'sl',fd);
    mark_mode      = toggle(mget(1,'c',fd))
    mark_style     = mget(1,'c',fd);
    mark_size      = mget(1,'c',fd);      
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
    set(h,"mark_mode",mark_mode)
    set(h,"line_style",line_style)
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground),
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
    xarcs(data');
    h=get('hdl')
    set(h,"visible",visible)
    set(h,"thickness",thickness)
    set(h,"line_style",line_style)
    set(h,"fill_mode",fill_mode)
    set(h,"foreground",foreground),
    if clip_state=='on' then set(h,"clip_box",clip_box),end
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
    set(h,"line_style",mget(1,'c',fd));
    set(h,"thickness",mget(1,'sl',fd))
    set(h,"arrow_size",mget(1,'sl',fd))
    n=mget(1,'il',fd)
    set(h,"segs_color",mget(n,'il',fd))
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      set(h,"clip_box", mget(4,'dl',fd))
    end
    set(h,"clip_state",clip_state);
  case "Grayplot"
    visible        = toggle(mget(1,'c',fd))
    sz             = mget(2,'il',fd)
    data           = matrix(mget(prod(sz),'dl',fd),sz(1),-1)
    data_mapping   = ascii(mget(mget(1,'c',fd),'c',fd))
    clip_state     = ascii(mget(mget(1,'c',fd),'c',fd))
    if clip_state=='on' then
      clip_box     = mget(4,'dl',fd)
    else
      clip_box=[]
    end

    grayplot(data(2:$,1),data(1,2:$),data(2:$,2:$))
    h=get('hdl')
    set(h,"visible",visible)
    set(h,"data_mapping",data_mapping)
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
