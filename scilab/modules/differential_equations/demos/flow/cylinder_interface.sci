//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Cylinder interface functions
// =============================================================================

// =============================================================================
// Step 1, set up the gradient
// =============================================================================
function execylinder()
	gravity        = 9.81
	g_pente  = 14;
	
	g_x      = 0.6;
	g_y      = 0.7;
	
	g_V      = 1;
	g_Vdir   = 180;
	
	g_alpha  = 35;
	g_theta  = 45;
	
	// Figure parameters
	// =========================================================================
	
	surface_frame_w = 600;     // surface frame width
	surface_frame_h = 600;     // surface frame height
	
	control_frame_w = 600;
	control_frame_h = 200;
	
	margin_x     = 15;      // Horizontal margin between each elements
	margin_y     = 15;      // Vertical margin between each elements
	
	axes_figure_w = 2*margin_x + max(surface_frame_w,control_frame_w);
	axes_figure_h = 3*margin_y + surface_frame_h + control_frame_h;
	
	// Figure creation
	// =========================================================================
	
	my_figure_handle = scf(100001);
	clf(my_figure_handle,"reset");
	my_figure_handle.immediate_drawing = "off";
	my_figure_handle.background      = -2;
	my_figure_handle.figure_position = [0 0];
	my_figure_handle.axes_size       = [axes_figure_w axes_figure_h];
	my_figure_handle.figure_name     = gettext("Cylindre");

	my_axe                           = gca()
	my_axe.axes_bounds               = [0,0,1,3/4];
	my_axe.tight_limits              = "on";
	
	draw_cylinder(g_pente,%T);
	cylinder_create_gui()
	change_x(g_x)
	change_y(g_y)
	change_speed(g_V)
	change_dir(g_Vdir)
	
	draw_initial_point(g_x,g_y,g_V,g_Vdir,g_pente,%T)

	my_figure_handle.immediate_drawing = "on";
endfunction

// draw_cylinder
// =============================================================================

function draw_cylinder(g_pente,orig)
  a  = -tan(g_pente/180*%pi);
  xx = -1:0.1:1;
  yy = (0:1:14)';
  
  if orig then
    Z                       = -sqrt(1-ones(yy)*(xx^2))+a*yy*ones(xx);
    plot3d(xx,yy,Z');
  else
    my_figure_handle        = scf(100001);
    my_axe                  = gca()
    Z                       = -sqrt(1-ones(yy)*(xx^2))+a*yy*ones(xx);
    e                       = my_axe.children(size(my_axe.children,"*"));
    e.data.z                = Z';
    my_axe.data_bounds(1,3) = min(Z);
  end
endfunction

// draw_initial_point
// =============================================================================
function draw_initial_point(x,y,g_V,g_Vdir,slope,create)
  a = -tan(slope*%pi/180);
  r = 0.1;
  z = -sqrt(1-x^2)+a*y+r;
  dx1 = g_V*cos(g_Vdir*%pi/180);
  dy1 = g_V*sin(g_Vdir*%pi/180);
  my_figure_handle = scf(100001);
  my_figure_handle.immediate_drawing = "off";
	
  if create then
    //create the graphical object used to visualize the trajectory
    xpoly(0,0) 
    traj_handle                        =gce();
    traj_handle.thickness              = 2; 
    traj_handle.foreground             = 5;
    //create the graphical object used to visualize the initial speed
    xpoly(0,0) 
    speed_handle                       =  gce();
    speed_handle.thickness             =  1;
    speed_handle.foreground            =  5;
    speed_handle.polyline_style        =  4;
    speed_handle.arrow_size_factor     =  2;
    //create the graphical objects used to visualize the initial position
    param3d([x-r,x+r],[y,y],[z,z]);  
    e1                                 = gce(); 
    e1.thickness                       = 1; 
    e1.foreground                      = 5;
    param3d([x,x],[y-r,y+r],[z,z]);  
    e2                                 = gce(); 
    e2.thickness                       = 1; 
    e2.foreground                      = 5;
    param3d([x,x],[y,y],[z-r,z+r]);  
    e3                                 = gce(); 
    e3.thickness                       = 1; 
    e3.foreground                      = 5;
    glue([traj_handle speed_handle e1 e2 e3])
  else
    curAxe = gca();
    initial=curAxe.children(1).children
    initial(1).data = [x,y,z-r;x,y,z+r];
    initial(2).data = [x,y-r,z;x,y+r,z];
    initial(3).data = [x-r,y,z;x+r,y,z];
  end
  z1=-sqrt(1-(x+dx1)^2)+a*(y+dy1)+r;
  initial(4).data = [x,y,z;x+dx1,y+dy1,z1];

  my_figure_handle.immediate_drawing = "on";	
endfunction

// =============================================================================
// The gui controls builder
// =============================================================================

function cylinder_create_gui()
  //initial values
    gravity = 9.81
    g_pente = 14
    g_x     = 0;
    g_y     = 0;
    g_speed = 0;
    g_Vdir  = 0;
	
    my_figure_handle = scf(100001);
	
    axes_size_figure = my_figure_handle.axes_size;
    axes_figure_w    = axes_size_figure(1);
    axes_figure_h    = axes_size_figure(2);
	
    frame_x      = 25;
    frame_y      = 200;
	
    slider_height   = 15;
    slider_width    = 140;
    value_width     = 30;
    unit_width      = 30;
    text_width      = 50;
    y_margin        = 5;
    x_margin        = 10;
    title_width     =  (text_width + slider_width + value_width + unit_width +3*x_margin)
	
    x_text         = frame_x
    x_slider        = x_text + text_width + x_margin;
    x_value         = x_slider + slider_width+x_margin;
    x_unit          = x_value  + value_width+x_margin;
	
    y               = frame_y-40;
	
    // Title position
    // =========================================================================
    
    title = uicontrol(my_figure_handle                             , ...
		      "position"            , [x_text y 2*title_width slider_height+y_margin]   , ...
		      "Style"               , "text"                                   , ...
		      "String"              , "Set parameters"                         , ...
		      "BackgroundColor"     , [1 1 1]                                  , ...
		      "Fontsize"            , 16                                       , ...
		      "horizontalalignment" , "center"                                 , ...
		      "tag"                 , "physics" );
    
    // Slider "gravity"
    // =========================================================================
    y  = y - (slider_height+y_margin)
    y0 = y
    text_g = uicontrol(my_figure_handle                              , ...
		       "position"          , [x_text y text_width slider_height]    , ...
		       "Style"             , "text"                                 , ...
		       "String"            , "gravity"                                    , ...
		       "BackgroundColor"   , [1 1 1]                                , ...
		       "tag"               , "text_g"                           );
    
    slider_g = uicontrol(my_figure_handle                            , ...
			 "position"          , [x_slider y slider_width slider_height], ...
			 "Style"             , "slider"                               , ...
			 "Value"             , gravity                                 , ...
			 "Min"               , 0                                      , ...
			 "Max"               , 100                                      , ...
			 "callback"          , "change_gravity()"                           , ...
			 "tag"               , "slider_gravity"                         );
    
    value_g = uicontrol(my_figure_handle                             , ...
			"position"          , [x_value  y value_width slider_height] , ...
			"Style"             , "text"                                 , ...
			"String"            , string(gravity)                            , ...
			"BackgroundColor"   , [1 1 1]                                , ...
			"tag"               , "value_g"                          );
    
    unite_g = uicontrol(my_figure_handle                             , ...
			"position"          , [x_unit y unit_width slider_height]    , ...
			"Style"             , "text"                                 , ...
			"String"            , "(m/s^-2)"                                  , ...
			"BackgroundColor"   , [1 1 1]                                , ...
			"tag"               , "unite_g"                          );
    
    // Slider x
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_x = uicontrol(my_figure_handle                          , ...
		       "position"       , [x_text y text_width slider_height]       , ...
		       "Style"          , "text"                                    , ...
		       "String"         , "x"                                   , ...
		       "BackgroundColor", [1 1 1]                                   , ...
		       "tag"            , "text_x"                              );
    slider_x = uicontrol(my_figure_handle                        , ...
			 "position"  , [x_slider y slider_width slider_height]        , ...
			 "Style"     , "slider"                                       , ...
			 "Min"       , 0                                              , ...
			 "Max"       , 100                                            , ...
			 "Value"     , g_x                                            , ...
			 "callback"  , "change_x()"                                   , ...
			 "tag"       , "slider_x"                                 );
    
    value_x = uicontrol(my_figure_handle                         , ...
			"position"        , [x_value  y value_width  slider_height]  , ...
			"Style"           , "text"                                   , ...
			"String"          , string(g_x)                              , ...
			"BackgroundColor" , [1 1 1]                                  , ...
			"tag"             , "value_x"                            );
    
    unite_x = uicontrol(my_figure_handle                         , ...
			"position"        , [x_unit y unit_width slider_height]      , ...
			"Style"           , "text"                                   , ...
			"String"          , "(m)"                                  , ...
			"BackgroundColor" , [1 1 1]                                  , ...
			"tag"             , "unite_x" ...
			);
    //Slider speed
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_speed = uicontrol(my_figure_handle                          , ...
			   "position"          , [x_text y text_width slider_height]    , ...
			   "Style"             , "text"                                 , ...
			   "String"            , "speed"                                , ...
			   "BackgroundColor"   , [1 1 1]                                , ...
			   "tag"               , "text_speed"                           );
    
    slider_speed = uicontrol(my_figure_handle                        , ...
			     "position"          , [x_slider y slider_width slider_height], ...
			     "Style"             , "slider"                               , ...
			     "Value"             , g_speed                                , ...
			     "Min"               , 0                                      , ...
			     "Max"               , 100                                      , ...
			     "callback"          , "change_speed()"                       , ...
			     "tag"               , "slider_speed"                         );
    
    value_speed = uicontrol(my_figure_handle                         , ...
			    "position"          , [x_value  y value_width slider_height] , ...
			    "Style"             , "text"                                 , ...
			    "String"            , string(g_speed)                        , ...
			    "BackgroundColor"   , [1 1 1]                                , ...
			    "tag"               , "value_speed"                          );
    
    unite_speed = uicontrol(my_figure_handle                         , ...
			    "position"          , [x_unit y unit_width slider_height]    , ...
			    "Style"             , "text"                                 , ...
			    "String"            , "(m/s)"                                  , ...
			    "BackgroundColor"   , [1 1 1]                                , ...
			    "tag"               , "unite_speed"                          );
    
    
    
    //---------------------------------------------------------------------------
    //second column 
    x_text          = frame_x + title_width + 3*x_margin
    x_slider        = x_text + text_width + x_margin;
    x_value         = x_slider + slider_width+x_margin;
    x_unit          = x_value  + value_width+x_margin;
    
    y               = y0

    // Slider slope
    // =========================================================================
    
    text_slope = uicontrol(my_figure_handle                          , ...
			   "position"       , [x_text y text_width slider_height]       , ...
			   "Style"          , "text"                                    , ...
			   "String"         , "slope"                                   , ...
			   "BackgroundColor", [1 1 1]                                   , ...
			   "tag"            , "text_slope"                              );
    
    slider_slope = uicontrol(my_figure_handle                        , ...
			     "position"  , [x_slider y slider_width slider_height]        , ...
			     "Style"     , "slider"                                       , ...
			     "Min"       , 0                                              , ...
			     "Max"       , 100                                            , ...
			     "Value"     , g_pente                                            , ...
			     "callback"  , "change_slope()"                                   , ...
			     "tag"       , "slider_slope"                                 );
    
    value_slope = uicontrol(my_figure_handle                         , ...
			    "position"        , [x_value  y value_width  slider_height]  , ...
			    "Style"           , "text"                                   , ...
			    "String"          , string(g_pente)                              , ...
			    "BackgroundColor" , [1 1 1]                                  , ...
			    "tag"             , "value_slope"                            );
    
    unite_slope = uicontrol(my_figure_handle                         , ...
			    "position"        , [x_unit y unit_width slider_height]      , ...
			    "Style"           , "text"                                   , ...
			    "String"          , "(deg)"                                  , ...
			    "BackgroundColor" , [1 1 1]                                  , ...
			    "tag"             , "unite_slope" ...
			    );
    // Slider y
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_y = uicontrol(my_figure_handle                          , ...
		       "position"       , [x_text y text_width slider_height]       , ...
		       "Style"          , "text"                                    , ...
		       "String"         , "y"                                   , ...
		       "BackgroundColor", [1 1 1]                                   , ...
		       "tag"            , "text_y"                              );
    slider_y = uicontrol(my_figure_handle                        , ...
			 "position"  , [x_slider y slider_width slider_height]        , ...
			 "Style"     , "slider"                                       , ...
			 "Min"       , 0                                              , ...
			 "Max"       , 100                                            , ...
			 "Value"     , g_y                                            , ...
			 "callback"  , "change_y()"                                   , ...
			 "tag"       , "slider_y"                                 );
    
    value_y = uicontrol(my_figure_handle                         , ...
			"position"        , [x_value  y value_width  slider_height]  , ...
			"Style"           , "text"                                   , ...
			"String"          , string(g_y)                              , ...
			"BackgroundColor" , [1 1 1]                                  , ...
			"tag"             , "value_y"                            );
    
    unite_y = uicontrol(my_figure_handle                         , ...
			"position"        , [x_unit y unit_width slider_height]      , ...
			"Style"           , "text"                                   , ...
			"String"          , "(m)"                                  , ...
			"BackgroundColor" , [1 1 1]                                  , ...
			"tag"             , "unite_y" ...
			);
    // Slider direction
    // =========================================================================
    y = y - (slider_height+y_margin)
    text_dir = uicontrol(my_figure_handle                            , ...
			 "position"       , [x_text y text_width slider_height]       , ...
			 "Style"          , "text"                                    , ...
			 "String"         , "direction"                                   , ...
			 "BackgroundColor", [1 1 1]                                   , ...
			 "tag"            , "text_dir"                              );
    
    slider_dir = uicontrol(my_figure_handle                          , ...
			   "position"  , [x_slider y slider_width slider_height]        , ...
			   "Style"     , "slider"                                       , ...
			   "Min"       , 0                                              , ...
			   "Max"       , 100                                            , ...
			   "Value"     , g_Vdir                                           , ...
			   "callback"  , "change_dir()"                                   , ...
			   "tag"       , "slider_dir"                                 );
    
    value_dir = uicontrol(my_figure_handle                           , ...
			  "position"        , [x_value  y value_width  slider_height]  , ...
			  "Style"           , "text"                                   , ...
			  "String"          , string(g_Vdir)                              , ...
			  "BackgroundColor" , [1 1 1]                                  , ...
			  "tag"             , "value_dir"                            );
    
    unite_dir = uicontrol(my_figure_handle                           , ...
			  "position"        , [x_unit y unit_width slider_height]      , ...
			  "Style"           , "text"                                   , ...
			  "String"          , "(deg)"                                  , ...
			  "BackgroundColor" , [1 1 1]                                  , ...
			  "tag"             , "unite_dir"                            );
    // Buttons
    // =========================================================================
    y = y - (slider_height+5*y_margin)
    btn_width=100;
    btn_margin=50;
    btn_height=20;
    x=frame_x+80
    start_button = uicontrol(my_figure_handle                                   , ...
			     "Position"             , [x y btn_width btn_height]                 , ...
			     "Style"                , "pushbutton"                               , ...
			     "String"               , "Start"                                    , ...
			     "Fontsize"             , 14                                         , ...
			     "BackgroundColor"      , [1 1 1]                                    , ...
			     "callback"             , "start_simu()"                             , ...
			     "tag"                  , "start_button"                             );
    
    x=x+btn_width+btn_margin
    stop_button = uicontrol(my_figure_handle                                    , ...
			    "Position"               , [x y btn_width btn_height]               , ...
			    "Style"                  , "pushbutton"                             , ...
			    "String"                 , "Stop"                                   , ...
			    "Fontsize"               , 14                                       , ...
			    "BackgroundColor"        , [1 1 1]                                  , ...
			    "callback"               , "stop_simu"                              , ...
			    "tag"                    , "stop_button"                            );

    x=x+btn_width+btn_margin
    clear_button = uicontrol(my_figure_handle                                   , ...
			     "Position"               , [x y btn_width btn_height]               , ...
			     "Style"                  , "pushbutton"                             , ...
			     "String"                 , "Clear"                                  , ...
			     "Fontsize"               , 14                                       , ...
			     "BackgroundColor"        , [1 1 1]                                  , ...
			     "callback"               , "clear_simu"                             , ...
			     "tag"                    , "clear_button"                           );
    
    return
endfunction


// =============================================================================
// The callbacks
// =============================================================================

function change_x(x)
//x slider callback 
// x is in [-0.9 0.9]
  slider_x     = findobj("tag", "slider_x");
  slider_y     = findobj("tag", "slider_y");
  slider_slope = findobj("tag", "slider_slope");
  slider_speed = findobj("tag", "slider_speed");
  slider_dir   = findobj("tag", "slider_dir");
 
  value_x       = findobj("tag", "value_x");
  if argn(2)==1 then
    slider_x.Value=(x+0.9)*100/1.8;
  else
    x=slider_x.Value
    x=-0.9+x*1.8/100
  end
  value_x.String=msprintf("%.3f",x)
  draw_initial_point(-0.9+slider_x.Value*1.8/100,slider_y.value*3/100,..
		     slider_speed.Value*2/100,slider_dir.value*360/100,slider_slope.Value*70/100,%F);
endfunction

function change_y(y)
//y slider callback 
//y is in [0 3]
  slider_x     = findobj("tag", "slider_x");
  slider_y     = findobj("tag", "slider_y");
  slider_slope = findobj("tag", "slider_slope");
  slider_speed = findobj("tag", "slider_speed");
  slider_dir   = findobj("tag", "slider_dir");
 
  value_y       = findobj("tag", "value_y");
  if argn(2)==1 then
    slider_y.Value=(y)*100/3;
  else
    y=slider_y.Value
    y=-y*3/100
  end
  value_y.String=msprintf("%.3f",y)
  draw_initial_point(-0.9+slider_x.Value*1.8/100,slider_y.value*3/100,..
		     slider_speed.Value*2/100,slider_dir.value*360/100,slider_slope.Value*70/100,%F);
endfunction

function change_speed(speed)
//speed slider callback  
//speed is in [0 2]
  slider_x     = findobj("tag", "slider_x");
  slider_y     = findobj("tag", "slider_y");
  slider_slope = findobj("tag", "slider_slope");
  slider_speed = findobj("tag", "slider_speed");
  slider_dir   = findobj("tag", "slider_dir");

  value_speed  = findobj("tag", "value_speed");
  if argn(2)==1 then
    slider_speed.Value=(speed)*100/2;
  else
    speed=slider_speed.Value
    speed=speed*2/100
  end
  value_speed.String=msprintf("%.2f",speed)
  draw_initial_point(-0.9+slider_x.Value*1.8/100,slider_y.value*3/100,..
		     slider_speed.Value*2/100,slider_dir.value*360/100,slider_slope.Value*70/100,%F);
endfunction

function change_slope(slope)
//slope slider callback  
//slope is in [0 70]
  slider_x     = findobj("tag", "slider_x");
  slider_y     = findobj("tag", "slider_y");
  slider_slope = findobj("tag", "slider_slope");
  slider_speed = findobj("tag", "slider_speed");
  slider_dir   = findobj("tag", "slider_dir");

  value_slope  = findobj("tag", "value_slope");
  if argn(2)==1 then
    slider_slope.Value=(slope)*100/70;
  else
    slope=slider_slope.Value
    slope=slope*70/100
  end
  value_slope.String=msprintf("%.2f",slope)
  g_pente=slope
  draw_cylinder(g_pente,%f);
  draw_initial_point(-0.9+slider_x.Value*1.8/100,slider_y.value*3/100,..
		     slider_speed.Value*2/100,slider_dir.value*360/100,slider_slope.Value*70/100,%F);
endfunction

function change_dir(dir)
//direction slider callback  
//dir is in [0 360]
  slider_x     = findobj("tag", "slider_x");
  slider_y     = findobj("tag", "slider_y");
  slider_slope = findobj("tag", "slider_slope");
  slider_speed = findobj("tag", "slider_speed");
  slider_dir   = findobj("tag", "slider_dir");

  value_dir  = findobj("tag", "value_dir");
  if argn(2)==1 then
    slider_dir.Value=(dir)*100/360;
  else
    dir=slider_dir.Value
    dir=dir*360/100
  end
  value_dir.String=msprintf("%.0f",dir)
  draw_initial_point(-0.9+slider_x.Value*1.8/100,slider_y.value*3/100,..
		     slider_speed.Value*2/100,slider_dir.value*360/100,slider_slope.Value*70/100,%F);
endfunction

function change_gravity(gravity)
//gravity slider callback  
//gravity is in [0 10]
  slider_x       = findobj("tag", "slider_x");
  slider_y       = findobj("tag", "slider_y");
  slider_slope   = findobj("tag", "slider_slope");
  slider_speed   = findobj("tag", "slider_speed");
  slider_dir     = findobj("tag", "slider_dir");
  slider_g       = findobj("tag", "slider_gravity");

  value_g  = findobj("tag", "value_g");
  if argn(2)==1 then
    slider_g.Value=(gravity)*100/10;
  else
    gravity=slider_g.Value
    gravity=gravity*360/10
  end
  value_g.String=msprintf("%.2f",gravity)
  //  draw_initial_point(-0.9+slider_x.Value*1.8/100,slider_y.value*3/100,..
  //		     slider_speed.Value*2/100,slider_dir.value*360/100,slider_slope.Value*70/100,%F);

endfunction


function start_simu()
//start button callback
  my_figure_handle           = scf(100001);
  fin                        = my_figure_handle.user_data
  my_figure_handle.user_data = %f

  slider_x       = findobj("tag", "slider_x");
  slider_y       = findobj("tag", "slider_y");
  slider_slope   = findobj("tag", "slider_slope");
  slider_speed   = findobj("tag", "slider_speed");
  slider_dir     = findobj("tag", "slider_dir");
  slider_g       = findobj("tag", "slider_gravity");

  
  t              = 0:0.01:10; //1001 track points

  x       = -0.9+slider_x.Value*1.8/100
  y       = slider_y.value*3/100
  V       = slider_speed.Value*2/100
  Vdir    = slider_dir.value*360/100
  slope   = slider_slope.Value*70/100
  gravity = slider_g.Value*360/10
  Y = calculate_traj(x,y,V,Vdir,t,gravity,slope)
  x = Y(1,:)
  y = Y(3,:)
  r = 0.1; //bias to have the curve above the surface
  a  = -tan(slope/180*%pi);
  z = -sqrt(1-x^2)+a*y+r;
  my_figure_handle = scf(100001);
  curAxe = gca();
  traj_handle=curAxe.children(1).children(5);
  traj_handle.data=[x(1),y(1),z(1)];
  for k=2:size(x,'*')
    if execstr('fin=my_figure_handle.user_data','errcatch')<>0|fin then break,end
    traj_handle.data=[traj_handle.data;[x(k),y(k),z(k)]];
  end
  my_figure_handle.user_data=%t
endfunction

function stop_simu()
//stop button callback
  my_figure_handle = scf(100001);
  fin   = %T;
  my_figure_handle.user_data=fin
endfunction

function clear_simu()
//clear button callback
  my_figure_handle = scf(100001);
  fin=my_figure_handle.user_data
  if fin then
    my_figure_handle = scf(100001);
    curAxe = gca();
    traj_handle=curAxe.children(1).children(5);
    traj_handle.data=[0,0,0];
  end
endfunction
