// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
// This file is released into the public domain

funcprot(0);

function demo_custom()
	
	global margin_x;
	global margin_y;
	
	global padding_x;
	global padding_y;
	
	global frame_w;
	global frame_h;
	
	global plot_w;
	global plot_h;
	
	// Parameters
	// =========================================================================
	
	frame_w      = 300;     // Frame width
	frame_h      = 765;     // Frame height
	
	plot_w       = 600;     // Plot width
	plot_h       = frame_h; // Plot height
	
	margin_x     = 15;      // Horizontal margin between each elements
	margin_y     = 15;      // Vertical margin between each elements
	
	padding_x    = 10;      // Horizontal padding between each elements
	padding_y    = 10;      // Vertical padding between each elements
	
	button_w     = 100;     // Button width
	button_h     = 30;      // Button height
	
	defaultfont  = "arial"; // Default Font
	
	// Figure creation
	// ========================================================================= 
	
	axes_w       = 3*margin_x + frame_w + plot_w; // axes width
	axes_h       = 2*margin_y + frame_h;          // axes height (100 => toolbar height)
	
	demo_fig = figure(100001);
	clf(demo_fig,"reset");
	demo_viewCode("demo_custom.dem.sce");
	
	demo_fig.background      = -2;
	demo_fig.color_map       = jetcolormap(128);
	demo_fig.figure_position = [0 0];
	demo_fig.axes_size       = [axes_w axes_h];
	demo_fig.figure_name     = gettext("Customize your complex function");
	
	// Frames creation [Control Panel]
	// =========================================================================
	
	my_frame = uicontrol( ...
		"parent"              , demo_fig,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ margin_x margin_y frame_w frame_h],...
		"fontname"            , "arial",...
		"fontunits"           , "points",...
		"fontsize"            , 13,...
		"fontweight"          , "bold", ...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "frame_control" ...
	);
	
	// Frame title
	my_frame_title = uicontrol( ...
		"parent"              , demo_fig,...
		"style"               , "text",...
		"string"              , "Control Panel",...
		"units"               , "pixels",...
		"position"            , [ 30+margin_x margin_y+frame_h-10 frame_w-60 20 ],...
		"fontname"            , defaultfont,...
		"fontunits"           , "points",...
		"fontsize"            , 14,...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "title_frame_control" ...
	);
	
	// explanatory text
	// =========================================================================
	
	my_exptext_w     = frame_w-2*padding_x
	my_exptext_h     = 350;
	my_exptext_pos_x = padding_x+margin_x;
	my_exptext_pos_y = (margin_y+frame_h) - 4*padding_y - my_exptext_h;
	
	my_exptext_string = "";
	
	my_exptext_string = my_exptext_string + "<html>";
	
	my_exptext_string = my_exptext_string + "<center>";
	my_exptext_string = my_exptext_string + "<img src=""file:///"+SCI+"/modules/graphics/demos/cmplxfunc/warning.png"+""">";
	my_exptext_string = my_exptext_string + "BE CAREFUL NOTHING IS PROTECTED";
	my_exptext_string = my_exptext_string + "</center>";
	
	my_exptext_string = my_exptext_string + "<hr><br />";
	
	my_exptext_string = my_exptext_string + " To draw your own complex  function :";
	
	my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px''>";
	
	my_exptext_string = my_exptext_string + "1. define the  function by a correct ";
	my_exptext_string = my_exptext_string + "string where  the complex var must ";
	my_exptext_string = my_exptext_string + "be z ; also  as  the function will ";
	my_exptext_string = my_exptext_string + "be evaluated  on  a matrix  don''t ";
	my_exptext_string = my_exptext_string + "forget the . to operate in element ";
	my_exptext_string = my_exptext_string + "wize meaning ; examples :";
	
	my_exptext_string = my_exptext_string + "</div>";
	
	
	my_exptext_string = my_exptext_string + "<pre>z.^2     (z+1).*(z-1)   (1)./(z+2)</pre>";
	my_exptext_string = my_exptext_string + "<pre>sqrt(z)  (z+%i).*(z-%i) 1+2*z+z.^2</pre>";
	
	my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px;''>";
	my_exptext_string = my_exptext_string + "2. define the type of the domain string Square or Disk";
	my_exptext_string = my_exptext_string + "</div>";
	
	
	my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px;''>";
	my_exptext_string = my_exptext_string + "3. define the ""radius"" R of the domain"+ "<br />";
	my_exptext_string = my_exptext_string + "</div>";
	
	
	my_exptext_string = my_exptext_string + "<div style=''text-align:justify; margin-top:5px;''>";
	my_exptext_string = my_exptext_string + "4. may be your function has a kind of ";
	my_exptext_string = my_exptext_string + "discontinuity on Ox  or  Oy => put ";
	my_exptext_string = my_exptext_string + "the string Ox or Oy or No if not";
	my_exptext_string = my_exptext_string + "</div>";
	
	my_exptext_string = my_exptext_string + "</html>";
	
	
	// Function definition : frame
	
	my_exptext_box = uicontrol( ...
		"parent"              , demo_fig,...
		"relief"              , "groove",...
		"style"               , "text",...
		"units"               , "pixels",...
		"string"              , my_exptext_string,...
		"position"            , [ my_exptext_pos_x my_exptext_pos_y my_exptext_w my_exptext_h],...
		"fontname"            , "arial",...
		"fontunits"           , "points",...
		"fontsize"            , 9,...
		"horizontalalignment" , "left", ...
		"verticalalignment"   , "top", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_exptext_box" ...
	);
	
	
	// Function definition
	// =========================================================================
	
	my_fundef_frame_w     = frame_w-2*padding_x
	my_fundef_frame_h     = 80;
	my_fundef_frame_pos_x = padding_x+margin_x;
	my_fundef_frame_pos_y = my_exptext_pos_y - 2*padding_y - my_fundef_frame_h;
	
	// Function definition : frame
	
	my_fundef_frame = uicontrol( ...
		"parent"              , demo_fig,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ my_fundef_frame_pos_x my_fundef_frame_pos_y my_fundef_frame_w my_fundef_frame_h],...
		"fontname"            , "arial",...
		"fontunits"           , "points",...
		"fontsize"            , 13,...
		"fontweight"          , "bold", ...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_fundef_frame" ...
	);
	
	// Function definition : Frame title
	
	my_fundef_frame_title = uicontrol( ...
		"parent"              , demo_fig,...
		"style"               , "text",...
		"string"              , "Function definition",...
		"units"               , "pixels",...
		"position"            , [ 70+my_fundef_frame_pos_x my_fundef_frame_pos_y+my_fundef_frame_h-9 my_fundef_frame_w-140 18 ],...
		"fontname"            , defaultfont,...
		"fontunits"           , "points",...
		"fontsize"            , 11,...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_fundef_frame_title" ...
	);
	
	// Function definition : Edit box
	
	my_fundef_editbox = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "edit",...
		"position"           , [ my_fundef_frame_pos_x+10 my_fundef_frame_pos_y+my_fundef_frame_h-60 my_fundef_frame_w-25 30],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 11,...
		"string"             , "(1)./((z+2).*(2-z))", ...
		"callback"           , "update_fundef()",...
		"tag"                , "my_fundef_editbox");
	
	
	
	// Domain type
	// =========================================================================
	
	my_dt_frame_w     = frame_w-2*padding_x
	my_dt_frame_h     = 60;
	my_dt_frame_pos_x = padding_x+margin_x;
	my_dt_frame_pos_y = my_fundef_frame_pos_y - 2*padding_y - my_dt_frame_h;
	
	// Domain type : frame
	
	my_dt_frame = uicontrol( ...
		"parent"              , demo_fig,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ my_dt_frame_pos_x my_dt_frame_pos_y my_dt_frame_w my_dt_frame_h],...
		"fontname"            , "arial",...
		"fontunits"           , "points",...
		"fontsize"            , 13,...
		"fontweight"          , "bold", ...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_dt_frame" ...
	);
	
	// Domain type : Frame title
	
	my_dt_frame_title = uicontrol( ...
		"parent"              , demo_fig,...
		"style"               , "text",...
		"string"              , "Domain type",...
		"units"               , "pixels",...
		"position"            , [ 100+my_dt_frame_pos_x my_dt_frame_pos_y+my_dt_frame_h-9 my_dt_frame_w-200 18 ],...
		"fontname"            , defaultfont,...
		"fontunits"           , "points",...
		"fontsize"            , 9,...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_dt_frame_title" ...
	);
	
	// Domain type : square
	
	square_radio = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "radiobutton",...
		"string"             , "Square",...
		"position"           , [ my_dt_frame_pos_x+10 my_dt_frame_pos_y+my_dt_frame_h-42 ((my_dt_frame_w-25)/2) 20],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 10,...
		"value"              , 0, ...
		"background"          , [1 1 1], ...
		"callback"           , "update_domain_type();",...
		"tag"                , "square_radio");
	
	// Domain type : Disk
	
	disk_radio = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "radiobutton",...
		"string"             , "Disk",...
		"position"           , [ my_dt_frame_pos_x+(my_dt_frame_w/2) my_dt_frame_pos_y+my_dt_frame_h-42 ((my_dt_frame_w-25)/2) 20],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 10,...
		"value"              , 1, ...
		"background"          , [1 1 1], ...
		"callback"           , "update_domain_type();",...
		"tag"                , "disk_radio");
	
	// Radius of the domain
	// =========================================================================
	
	my_radius_frame_w     = frame_w-2*padding_x
	my_radius_frame_h     = 80;
	my_radius_frame_pos_x = padding_x+margin_x;
	my_radius_frame_pos_y = my_dt_frame_pos_y - 2*padding_y - my_radius_frame_h;
	
	// Function definition : frame
	
	my_radius_frame = uicontrol( ...
		"parent"              , demo_fig,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ my_radius_frame_pos_x my_radius_frame_pos_y my_radius_frame_w my_radius_frame_h],...
		"fontname"            , "arial",...
		"fontunits"           , "points",...
		"fontsize"            , 13,...
		"fontweight"          , "bold", ...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_radius_frame" ...
	);
	
	// Function definition : Frame title
	
	my_radius_frame_title = uicontrol( ...
		"parent"              , demo_fig,...
		"style"               , "text",...
		"string"              , "Radius of the domain",...
		"units"               , "pixels",...
		"position"            , [ 50+my_radius_frame_pos_x my_radius_frame_pos_y+my_radius_frame_h-9 my_radius_frame_w-100 18 ],...
		"fontname"            , defaultfont,...
		"fontunits"           , "points",...
		"fontsize"            , 11,...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_radius_frame_title" ...
	);
	
	// Function definition : Edit box
	
	my_radius_editbox = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "edit",...
		"position"           , [ my_radius_frame_pos_x+10 my_radius_frame_pos_y+my_radius_frame_h-60 my_radius_frame_w-25 30],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 11,...
		"string"             , "1.9", ...
		"callback"           , "update_radius()",...
		"tag"                , "my_radius_editbox");
	
	
	// Cut on axe
	// =========================================================================
	
	my_coa_frame_w     = frame_w-2*padding_x
	my_coa_frame_h     = 60;
	my_coa_frame_pos_x = padding_x+margin_x;
	my_coa_frame_pos_y = my_radius_frame_pos_y - 2*padding_y - my_coa_frame_h;
	
	// Cut on axe : frame
	
	my_coa_frame = uicontrol( ...
		"parent"              , demo_fig,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ my_coa_frame_pos_x my_coa_frame_pos_y my_coa_frame_w my_coa_frame_h],...
		"fontname"            , "arial",...
		"fontunits"           , "points",...
		"fontsize"            , 13,...
		"fontweight"          , "bold", ...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_coa_frame" ...
	);
	
	// Cut on axe : Frame title
	
	my_coa_frame_title = uicontrol( ...
		"parent"              , demo_fig,...
		"style"               , "text",...
		"string"              , "Cut on axes",...
		"units"               , "pixels",...
		"position"            , [ 100+my_coa_frame_pos_x my_coa_frame_pos_y+my_coa_frame_h-9 my_coa_frame_w-200 18 ],...
		"fontname"            , defaultfont,...
		"fontunits"           , "points",...
		"fontsize"            , 9,...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"tag"                 , "my_coa_frame_title" ...
	);
	
	// Cut on axe : Ox
	
	ox_radio = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "radiobutton",...
		"string"             , "Ox",...
		"position"           , [ my_coa_frame_pos_x+10 my_coa_frame_pos_y+my_coa_frame_h-42 (my_coa_frame_w-25)/3 20],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 10,...
		"value"              , 0, ...
		"background"         , [1 1 1], ...
		"callback"           , "update_cao();",...
		"tag"                , "ox_radio");
	
	// Cut on axe : Oy
	
	oy_radio = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "radiobutton",...
		"string"             , "Oy",...
		"position"           , [ my_coa_frame_pos_x+(my_coa_frame_w/3) my_coa_frame_pos_y+my_coa_frame_h-42 (my_coa_frame_w-25)/3 20],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 10,...
		"value"              , 0, ...
		"background"         , [1 1 1], ...
		"callback"           , "update_cao();",...
		"tag"                , "oy_radio");
	
	// Cut on axe : No
	
	no_radio = uicontrol( ...
		"parent"             , demo_fig,...
		"style"              , "radiobutton",...
		"string"             , "No",...
		"position"           , [ my_coa_frame_pos_x+(2*(my_coa_frame_w/3)) my_coa_frame_pos_y+my_coa_frame_h-42 (my_coa_frame_w-25)/3 20],...
		"horizontalalignment", "left",...
		"fontname"           , defaultfont,...
		"fontunits"          , "points",...
		"fontsize"           , 10,...
		"value"              , 1, ...
		"background"         , [1 1 1], ...
		"callback"           , "update_cao();",...
		"tag"                , "no_radio");
	
	// Default Values
	// =========================================================================
	
	global my_fundef_val;
	global my_dt_val;
	global my_radius;
	global my_typeCut;
	global my_e;
	
	
	my_fundef_val      = "(1)./((z+2).*(2-z))";
	my_dt_val          = "Disk";
	my_radius          = 1.9;
	my_typeCut         = "No";
	
	my_e               = 0.001;
	theta              = -110;
	alpha              = 75;
	
	deff("Z=f(z)","Z="+my_fundef_val);
	
	demo_fig.immediate_drawing = "off";
	PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
	demo_fig.immediate_drawing = "on";
	
endfunction

function update_fundef()

	global my_fundef_val;
	global my_dt_val;
	global my_radius;
	global my_typeCut;
	global my_e;
	
	my_fundef_val = get(gcbo,"string");
	
	// Delete the old plots
	
	demo_fig = gcf();
	demo_fig.immediate_drawing = "off";
	
	a = gca(); delete(a);
	a = gca(); delete(a);
	a = gca(); delete(a);
	
	theta              = -110;
	alpha              = 75;
	
	deff("Z=f(z)","Z="+my_fundef_val);
	
	if my_dt_val == "Square" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
	elseif my_dt_val == "Disk" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
	end
	
	demo_fig.immediate_drawing = "on";

endfunction

function update_domain_type()
	
	global my_fundef_val;
	global my_dt_val;
	global my_radius;
	global my_typeCut;
	global my_e;
	
	// Update the radio elements
	set(findobj("tag", "square_radio") , "value", 0);
	set(findobj("tag", "disk_radio")   , "value", 0);
	set(gcbo, "value", 1);
	
	// Delete the old plots
	
	demo_fig = gcf();
	demo_fig.immediate_drawing = "off";
	
	a = gca(); delete(a);
	a = gca(); delete(a);
	a = gca(); delete(a);
	
	theta              = -110;
	alpha              = 75;
	
	deff("Z=f(z)","Z="+my_fundef_val);
	
	if get(gcbo, "tag") == "square_radio" then
		my_dt_val = "Square";
	elseif get(gcbo, "tag") == "disk_radio" then
		my_dt_val = "Disk";
	end
	
	if my_dt_val == "Square" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
	elseif my_dt_val == "Disk" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
	end
	
	demo_fig.immediate_drawing = "on";
	
endfunction

function update_radius()
	
	global my_fundef_val;
	global my_dt_val;
	global my_radius;
	global my_typeCut;
	global my_e;
	
	my_radius = evstr(get(gcbo,"string"));
	
	// Delete the old plots
	
	demo_fig = gcf();
	demo_fig.immediate_drawing = "off";
	
	a = gca(); delete(a);
	a = gca(); delete(a);
	a = gca(); delete(a);
	
	theta              = -110;
	alpha              = 75;
	
	deff("Z=f(z)","Z="+my_fundef_val);
	
	if my_dt_val == "Square" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
	elseif my_dt_val == "Disk" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
	end
	
	demo_fig.immediate_drawing = "on";
	
endfunction

function update_cao()

	global my_fundef_val;
	global my_dt_val;
	global my_radius;
	global my_typeCut;
	global my_e;
	
	// Update the radio elements
	set(findobj("tag", "ox_radio") , "value", 0);
	set(findobj("tag", "oy_radio") , "value", 0);
	set(findobj("tag", "no_radio") , "value", 0);
	set(gcbo, "value", 1);
	
	// Delete the old plots
	
	demo_fig = gcf();
	demo_fig.immediate_drawing = "off";
	
	a = gca(); delete(a);
	a = gca(); delete(a);
	a = gca(); delete(a);
	
	theta              = -110;
	alpha              = 75;
	
	deff("Z=f(z)","Z="+my_fundef_val);
	
	if get(gcbo, "tag")     == "ox_radio" then
		my_typeCut = "Ox";
	elseif get(gcbo, "tag") == "oy_radio" then
		my_typeCut = "Oy";
	elseif get(gcbo, "tag") == "no_radio" then
		my_typeCut = "No";
	end
	
	if my_dt_val == "Square" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,41,"f",theta,alpha,[0;0]);
	elseif my_dt_val == "Disk" then
		PlotCmplxFunc(my_radius,my_e,my_dt_val,my_typeCut,[40 20],"f",theta,alpha,[0;0]);
	end
	
	demo_fig.immediate_drawing = "on";
	
endfunction

function []=PlotCmplxFunc(R,e,TypeDomain,TypeCut,n,StrFunc,theta,alpha,DomReal)

	//  A function to draw on a square or a disk a complex function
	//  with branch(es) cut(s) on Ox or Oy
	//
	//  TypeDomain : "Square" or "Disk"
	//     TypeCut : "Ox" or "Oy"
	//           R : length of half a side of the square or radius of the disk
	//           e : thin layer to avoid the branch(es) cut(s)
	//           n : a scalar (for Square) or a 2-vector = [ntheta, nr]
	//               (for Disk) for discretization
	//     StrFunc : the string which names the complex function (this is
	//               because primitive don't pass as function argument)
	// theta,alpha : usual parameters for plot3d
	//     DomReal : interval for which the real restriction is drawn
	
	// computes the facets
	
	[xr,yr,zr,xi,yi,zi] = CmplxFacets(R,e,TypeDomain,TypeCut,n,StrFunc)
	
	// draw
	// ============================================
	
	// plot Im(z)
	// ============================================
	
	my_IM_axes = newaxes();
	my_IM_axes.axes_bounds = [1/3+0.05,0,1/3,1];
	my_IM_axes.margins     = [ 0.2 0.2 0.2 0.2 ];
	
	plot3d(xi,yi,zi,theta,alpha,"Re(z)@Im(z)@",[2 6 4]);
	
	my_IM_plot             = my_IM_axes.children;
	my_IM_plot.color_flag  = 1;
	
	xtitle("Im("+StrFunc+"(z))");
	
	// plot Re(z) + the real restriction
	// ============================================
	
	my_RE_axes = newaxes();
	my_RE_axes.axes_bounds = [2/3+0.05,0,1/3,1];
	my_RE_axes.margins     = [ 0.2 0.2 0.2 0.2 ];
	
	plot3d(xr,yr,zr,theta,alpha,"Re(z)@Im(z)@",[ 2 6 4]);
	
	my_RE_plot             = my_RE_axes.children;
	my_RE_plot.color_flag  = 1;
	
	xtitle("Re("+StrFunc+"(z))");
	
	// real function in yellow
	// ============================================
	
	if DomReal(2) > DomReal(1) then
		xstring(0.1,-0.15," In yellow : the real "+StrFunc+" function")
	end
	
	if DomReal(2) > DomReal(1) then
		xx = linspace(DomReal(1),DomReal(2),40)';
		yy = zeros(xx);
		zz = evstr(StrFunc+"(xx)");
		param3d1(xx,yy,list(zz,32),theta,alpha,flag=[0,0]);
		yellow_line = get('hdl');
		yellow_line.thickness = 3;
	end
	
	// Title
	// ============================================
	
	my_title_axes             = newaxes();
	my_title_axes.axes_bounds = [1/3,0,2/3,1];
	my_title_axes.margins     = [ 0.08 0.08	0.08 0.08 ]

	// make axes transparent
	my_title_axes.filled = "off";
	
	Rs = string(R);
	
	if TypeDomain == "Square" then
		end_title = " Function on [-"+Rs+","+Rs+"]x[-"+Rs+","+Rs+"]"
	else
		end_title = " Function on D(0,R="+Rs+")"
	end
	
	if StrFunc == "f" then
		the_title = "Your Custom (named f) Complex" + end_title;
	else
		the_title = "The Complex " + StrFunc + end_title;
	end
	
	xtitle(the_title);
	
	my_title_axes.title.text       = the_title;
	my_title_axes.title.font_size  = 3;
	
endfunction

function [xr,yr,zr,xi,yi,zi] = CmplxFacets(R,e,TypeDomain,TypeCut,n,StrFunc)

	//  A function to compute the facets for drawing a complex function
	//  on a square or a disk with branch(es) cut(s) on Ox or Oy
	//
	//  TypeDomain : "Square" or "Disk"
	//     TypeCut : "Ox" or "Oy"
	//           R : length of half a side of the square or radius of the disk
	//           e : thin layer to avoid the branch(es) cut(s)
	//           n : a scalar (for Square) or a 2-vector = [ntheta, nr]
	//               (for Disk) for discretization
	//     StrFunc : the string which names the complex function (this is
	//               because primitive don't pass as function argument)
	
	if TypeDomain == "Square" then
		if TypeCut == "Ox" then
			x1 = linspace(-R, R, n);
			y1 = linspace( e, R, n/2);
		else  // for TypeCut = "Oy" ...
			x1 = linspace( e, R, n/2);
			y1 = linspace(-R, R, n);
		end
		X1 = ones(y1')*x1 ; Y1 = y1'*ones(x1);
	
	else // for TypeDomain = "Disk"
		r = linspace(0,R, n(2));
		if TypeCut == "Ox" then
			theta = linspace(0,%pi,n(1))';
			X1 = cos(theta)*r;
			Y1 = e + sin(theta)*r;
		else // for TypeCut = "Oy"
			theta = linspace(-%pi/2,%pi/2,n(1))';
			X1 = e + cos(theta)*r;
			Y1 = sin(theta)*r;
		end
	end
	
	X2 = -X1 ; Y2 = -Y1;
	Z1 = evstr(StrFunc+"(X1 + %i*Y1)");
	Z2 = evstr(StrFunc+"(X2 + %i*Y2)");
	[xr1,yr1,zr1] = nf3d(X1,Y1,real(Z1));
	[xr2,yr2,zr2] = nf3d(X2,Y2,real(Z2));
	xr = [xr1 xr2]; yr = [yr1 yr2]; zr = [zr1 zr2];
	[xi1,yi1,zi1] = nf3d(X1,Y1,imag(Z1));
	[xi2,yi2,zi2] = nf3d(X2,Y2,imag(Z2));
	xi = [xi1 xi2]; yi = [yi1 yi2]; zi = [zi1 zi2];
	
endfunction

funcprot(1);

demo_custom();

clear demo_custom();
