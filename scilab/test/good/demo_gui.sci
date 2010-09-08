//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function demo_gui()
	
	global demolist // Demos list is defined in scilab.start
	
	global margin_x;
	global margin_y;
	
	global frame_w;
	global frame_h;
	
	global button_w;
	global button_h;
	
	// Parameters
	// =========================================================================
	
	frame_w      = 200;     // Frame width
	frame_h      = 450;     // Frame height
	
	margin_x     = 15;      // Horizontal margin between each elements
	margin_y     = 15;      // Vertical margin between each elements
	
	button_w     = 0;       // Button width
	button_h     = 0;       // Button height
	
	defaultfont  = "arial"; // Default Font
	
	frame_number = 1;       // Frame number
	
	// Figure creation
	// ========================================================================= 
	
	axes_w       = (frame_number+1)*margin_x + frame_number*frame_w; // axes width
	axes_h       = 3*margin_y + frame_h + button_h;                  // axes height
	
	demo_fig = figure(100000);
	
	// Remove Scilab graphics menus & toolbar
	
	delmenu(demo_fig.figure_id, gettext("&File"));
	delmenu(demo_fig.figure_id, gettext("&Tools"));
	delmenu(demo_fig.figure_id, gettext("&Edit"));
	delmenu(demo_fig.figure_id, gettext("&?"));
	toolbar(demo_fig.figure_id, "off");
	demo_fig.visible = "off"; // to be sure that no plot can appear in the plot
	
	// Add the new one
	
	h = uimenu( "parent", demo_fig,          ..
	            "label" , gettext("File"));
	
	uimenu( "parent"    , h,                 ..
	        "label"     , gettext("Close"),  ..
	        "callback"  , "demo_fig=get_figure_handle(100000);delete(demo_fig);", ..
	        "tag"       , "close_menu");
	
	// Background, Resize, title ...
	
	demo_fig.background      = -2;
	demo_fig.axes_size       = [axes_w axes_h];
	demo_fig.figure_name     = gettext("Demos");
	
	// Frame creation
	
	create_frame(demo_fig,1,"Demos",demolist);
	
	demo_fig.axes_size       = [axes_w axes_h];
	
	// pushbutton creation
	
// 	my_button = uicontrol( ...
// 		"parent"              , demo_fig,...
// 		"relief"              , "groove",...
// 		"style"               , "pushbutton",...
// 		"string"              , gettext("View Code"),...
// 		"units"               , "pixels",...
//  	"position"            , [ floor(axes_w/2 - button_w/2) margin_y button_w button_h ], ...
// 		"fontunits"           , "points",...
// 		"fontsize"            , 12,...
// 		"fontweight"          , "bold", ...
// 		"horizontalalignment" , "center", ...
// 		"visible"             , "on", ...
// 		"tag"                 , "button_view_code" );
	
endfunction

function create_frame(my_fig_handle,fr_position,fr_title,fr_items)
	
	// my_fig_handle : Handle de la figure englobante
	// fr_position   : position de la frame à créer
	// fr_position   : titre de la frame à créer
	// fr_items      : liste des items de la listbox

	// Parameters
	// =========================================================================
	
	frame_w      = 200;     // Frame width
	frame_h      = 450;     // Frame height
	
	margin_x     = 15;      // Horizontal margin between each elements
	margin_y     = 15;      // Vertical margin between each elements
	
	button_w     = 0;       // Button width
	button_h     = 0;       // Button height
	
	defaultfont  = "arial"; // Default Font
	
	// =========================================================================
	
	
	// if no item, no display
	if fr_items == []  then
		my_visible = "off";
	else
		my_visible = "on";
	end
	
	axes_h = my_fig_handle.axes_size(2);
	
	
	if fr_position <> 1 then
		this_frame_w     = frame_w;
		this_frame_h     = frame_h;
		this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
		this_frame_pos_y = axes_h-(margin_y+frame_h);
	else
		this_frame_w     = frame_w;
		this_frame_h     = frame_h - 150 ;
		this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
		this_frame_pos_y = axes_h-(margin_y+frame_h)+150;
	end
	
	// frame
	uicontrol( ...
		"parent"              , my_fig_handle,...
		"relief"              , "groove",...
		"style"               , "frame",...
		"units"               , "pixels",...
		"position"            , [ this_frame_pos_x this_frame_pos_y this_frame_w this_frame_h ],...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"visible"             , my_visible, ...
		"tag"                 , "frame_"+string(fr_position));
	
	// Frame title
	uicontrol( ...
		"parent"              , my_fig_handle,...
		"style"               , "text",...
		"string"              , " "+gettext(fr_title),...
		"units"               , "pixels",...
		"position"            , [ this_frame_pos_x+10 axes_h-(margin_y)-9 this_frame_w-20 18 ],...
		"fontunits"           , "points",...
		"fontsize"            , 12,...
		"fontWeight"          , "bold",...
		"horizontalalignment" , "center", ...
		"background"          , [1 1 1], ...
		"visible"             , my_visible, ...
		"tag"                 , "title_frame_"+string(fr_position));
		
	// List Box
	uicontrol( ...
		"parent"              , my_fig_handle,...
		"style"               , "listbox",...
		"string"              , " "+strcat(fr_items(:,1),"| "),...
		"units"               , "pixels",...
		"position"            , [ this_frame_pos_x+5 this_frame_pos_y+5 this_frame_w-12 this_frame_h-14-10 ],...
		"fontunits"           , "points",...
		"fontsize"            , 12,...
		"horizontalalignment" , "left", ...
		"BackgroundColor"     , [255/255 , 255/255 , 255/255 ], ...
		"callback"            , "script_path = demo_gui_update();exec(script_path,-1);",...
		"visible"             , my_visible, ...
		"user_data"           , fr_items, ...
		"tag"                 , "listbox_"+string(fr_position));
	
	// Logo Scilab
	// =========================================================================
	
	if fr_position == 1 then
	
		str = "";
		str = str + "<html>";
		str = str + "<img src=""file:///"+SCI+"/modules/demo_tools/images/logo_scilab.png"" />";
		str = str + "</html>";
		
		my_text = uicontrol( ...
			"parent"              , my_fig_handle,...
			"style"               , "text",...
			"string"              , str,...
			"units"               , "pixels",...
			"position"            , [ (margin_x+(fr_position-1)*(margin_x+frame_w)) axes_h-(margin_y+frame_h) frame_w 150],...
			"background"          , [1 1 1], ...
			"tag"                 , "my_text", ...
			"horizontalalignment" , "center", ...
			"verticalalignment"   , "middle" ...
		);
		
	end
	
	
endfunction

function script_path = demo_gui_update()
	
	my_counter = 0;
	
	global subdemolist;
	global frame_number;
	
	my_selframe     = get(gcbo,"tag");
	
	// Suppression d'une figure précédemment dessiné, si figure il y a ...
	fig_to_del = get_figure_handle(100001);
	if fig_to_del <> [] then
		delete(fig_to_del);
	end
	
	// Handle de la figure
	demo_fig        = gcbo.parent;

	// Frame sur laquelle on a cliqué
	my_selframe_num = msscanf(my_selframe,"listbox_%d");
	
	// Récupération de la liste des démos
	my_index = get(findobj("tag",my_selframe), "value"    );
	my_data  = get(findobj("tag",my_selframe), "user_data");
	
	script_path = my_data(my_index(1,1),2);
	
	if grep(script_path,"dem.gateway.sce") == 1 then
		
		// On est dans le cas ou une nouvelle frame va être affichée
		
		// Mise à jour du nombre de frame
		frame_number    = my_selframe_num+1;
		resize_gui(demo_fig,frame_number);
		previous_demolist      = subdemolist;
		
		mode(-1);
		exec(script_path,-1);
		create_frame(demo_fig,my_selframe_num+1,my_data(my_index(1,1),1),subdemolist);
		subdemolist = previous_demolist;
		
	else
		// Mise à jour du nombre de frame
		frame_number    = my_selframe_num;
		resize_gui(demo_fig,frame_number);
	end
	
endfunction

function resize_gui(my_fig_handle,frame_number)
	
	// Parameters
	// =========================================================================
	
	frame_w      = 200;     // Frame width
	frame_h      = 450;     // Frame height
	
	margin_x     = 15;      // Horizontal margin between each elements
	margin_y     = 15;      // Vertical margin between each elements
	
	button_w     = 0;       // Button width
	button_h     = 0;       // Button height
	
	defaultfont  = "arial"; // Default Font
	
	// =========================================================================
	
	axes_w                     = (frame_number+1)*margin_x + frame_number*frame_w; // axes width
	my_fig_handle.axes_size(1) = axes_w;
	
	for i=(frame_number+1):10
		
		my_frame               = findobj("tag", "frame_"      +string(i));
		my_frame_title         = findobj("tag", "title_frame_"+string(i));
		my_listbox             = findobj("tag", "listbox_"    +string(i));
		
		if my_frame <> [] then
			delete_frame(my_fig_handle,i);
		end
		
	end
	
	// update_button_position(my_fig_handle,axes_w);
	
endfunction

function update_button_position(my_fig_handle,axes_w)
	
	// Update button position
	
	global margin_y;
	global button_w;
	global button_h;
	
	my_button = findobj("tag", "button_view_code");
	my_button.position = [ floor((axes_w/2) - (button_w/2)) margin_y button_w button_h ];
	
endfunction

function delete_frame(my_fig_handle,fr_position)
	
	my_frame               = findobj("tag", "frame_"      +string(fr_position));
	my_frame_title         = findobj("tag", "title_frame_"+string(fr_position));
	my_listbox             = findobj("tag", "listbox_"    +string(fr_position));
	
	delete(my_frame);
	delete(my_frame_title);
	delete(my_listbox);
	
endfunction
