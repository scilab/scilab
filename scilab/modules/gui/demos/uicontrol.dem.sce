//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file is distributed under the same license as the Scilab package.
//

function add_color_in_list()
listbox_handle = findobj("Tag","colors_listbox");
new_color = get(findobj("Tag","colors_edit"), "String");

if new_color==gettext("<Enter color name here>") | isempty(new_color) then
  messagebox(gettext("Please enter a color name first."), gettext("Error"), "error", "modal");
  return
end

colors = get(listbox_handle, "String");
colors = [colors  new_color];
set(listbox_handle, "String", strcat(colors,"|"));
endfunction

function disp_selected_colors()
listbox_handle = findobj("Tag","colors_listbox");
colors = get(listbox_handle, "String");
indices = get(listbox_handle, "Value");

if isempty(indices) then
  msg = gettext("No color selected.");
else
  msg = msprintf(gettext("Selected colors are: %s."), strcat(colors(indices), ", "));
end
messagebox(msg, gettext("Selected colors"), "info", "modal");
endfunction

function menu_callback()
msg = msprintf(gettext("You clicked on menu ''%s''."), get(gcbo, "Label"));
messagebox(msg, gettext("Selected menu"), "info", "modal");
endfunction

function checked_menu_callback()
if get(gcbo, "Checked")=="on" then
  state = gettext("checked");
else
  state = gettext("unchecked");
end
msg = msprintf(gettext("Menu ''%s'' is %s."), get(gcbo, "Label"), state);
messagebox(msg, gettext("Menu status"), "info", "modal");
endfunction

function slider_update()
sl = findobj("Tag", "demo_slider");
txt = findobj("Tag", "slider_text");
set(txt, "String", gettext("Slider value: ") + string(get(sl, "Value")));
endfunction

function popupmenu_callback()
pop = findobj("Tag", "popupmenu_demo");
items = get(pop, "String");
selected = get(pop, "Value");

msg = msprintf(gettext("You selected ''%s''."), items(selected));
messagebox(msg, gettext("Popupmenu selection"), "info", "modal");
endfunction

function close_uicontrols_demo()
delete(findobj("Tag", "uicontrols_demo_figure"));
endfunction

function exit_scilab()
msg = gettext("Do you relly want to quit Scilab?");
answ = messagebox(msg, gettext("Quit Scilab"), "quextion", [gettext("Yes") gettext("No")], "modal");
if answ==1 then
  exit;
end
endfunction


// Create a figure
f = figure("figure_name", gettext("Uicontrols demo"),...
    "Position",[50 50 300 195],...
    "BackgroundColor",[0.9 0.9 0.9],...
    "Units", "pixels",..
    "Tag", "uicontrols_demo_figure");

// Remove Scilab 'standard' menus
delmenu(f.figure_id, gettext("&File"));
delmenu(f.figure_id, gettext("&Tools"));
delmenu(f.figure_id, gettext("&Edit"));
delmenu(f.figure_id, gettext("&?"));

// Disable graphics toolbar
toolbar(f.figure_id, "off");

// Adjust figure size after toolbar removing
f.position = [50 50 300 260];

// Add custom menus
appli_menu = uimenu("Parent", f, "Label", gettext("Application"));
close_menu = uimenu("Parent", appli_menu, "Label", gettext("Exit figure"), "callback", "close_uicontrols_demo();");
quit_menu = uimenu("Parent", appli_menu, "Label", gettext("Quit Scilab"), "callback", "exit_scilab();");

graphics_menu = uimenu("Parent", f, "Label", gettext("Scilab Graphics"));
plot3d_menu = uimenu("Parent", graphics_menu, "Label", gettext("Launch plot3d"), "callback", "scf();plot3d();");
plot2d_menu = uimenu("Parent", graphics_menu, "Label", gettext("Launch plot2d"), "callback", "scf();plot2d();");

menu = uimenu("Parent", f, "Label", gettext("Menu"));
submenu1 = uimenu("Parent", menu, "Label", gettext("Sub-menu 1"), "Callback", "menu_callback()");
submenu2 = uimenu("Parent", menu, "Label", gettext("Sub-menu 2"), "Callback", "menu_callback()");
submenu21 = uimenu("Parent", submenu2, "Label", gettext("Sub-menu 2-1"), "Callback", "menu_callback()");
submenu22 = uimenu("Parent", submenu2, "Label", gettext("Sub-menu 2-2"), "Callback", "menu_callback()");
submenu3 = uimenu("Parent", menu, "Label", gettext("Sub-menu 2"), "Checked", "on", "Callback", "checked_menu_callback()");

// Frame with uicontrols used to edit the colors list
edit_frame = uicontrol(f, "Position", [10 10 170 75], ...
    "Style", "frame", ...
    "BackgroundColor", [0.9 0.9 0.9]);
edit_frame_title = uicontrol(f, "Position", [20 75 90 15], ...
    "Style", "text",...
    "String", gettext("Colors list edition"),...
    "BackgroundColor", [0.9 0.9 0.9]);

edit_edit = uicontrol(f, "Position", [20 50 150 20], ...
    "Style", "edit", ...
    "FontSize", 11, ...
    "String", gettext("<Enter color name here>"), ...
    "BackgroundColor", [1 1 1], ...
    "Tag", "colors_edit");

edit_button = uicontrol(f, "Position", [30 20 130 20], ...
    "Style", "pushbutton", ...
    "FontSize", 11, ...
    "String", gettext("Add color in listbox"), ...
    "callback", "add_color_in_list()");  

// Listbox used to display color list
initial_colors = strcat([gettext("red") gettext("green") gettext("blue") gettext("yellow")], "|");

thelistbox = uicontrol(f, "Position", [190 10 100 160], ...
    "Style", "listbox", ...
    "FontSize", 11, ...
    "String",  initial_colors, ...
    "BackgroundColor", [1 1 1], ...
    "Tag", "colors_listbox");       


// Button used to display selected colors in a message box
disp_button = uicontrol(f, "Position", [190 180 100 20], ...
    "Style", "pushbutton", ...
    "FontSize", 11, ...
    "String", gettext("Display selection"), ...
    "callback", "disp_selected_colors()");

// Frame containing a slider
slider_frame = uicontrol(f, "Position", [10 95 170 75], ...
    "Style", "frame", ...
    "BackgroundColor", [0.9 0.9 0.9]);
slider_frame_title = uicontrol(f, "Position", [20 160 60 15], ...
    "Style", "text",...
    "String", gettext("Slider demo"),...
    "BackgroundColor", [0.9 0.9 0.9]);

slider_text = uicontrol(f, "Position", [20 105 150 20],...
    "Style", "text",...
    "FontSize", 11, ...
    "FontWeight", "bold", ...
    "BackgroundColor",[1 1 1], ...
    "HorizontalAlignment", "center", ...
    "Tag", "slider_text");

theslider = uicontrol(f, "Position", [20 135 150 15], ...
    "Style", "slider", ...
    "Min", 0, ...
    "Max", 100, ...
    "Value", 50, ...
    "SliderStep", [2 10], ...
    "Tag", "demo_slider", ...
    "Callback", "slider_update();");
// Update the text displayed
slider_update();

// A popup menu
popup_demo = uicontrol(f, "Position", [10 180 170 20], ...
    "Style", "popupmenu", ...
    "String", gettext("item1|item2|item3|item4"), ...
    "Callback", "popupmenu_callback();", ...
    "Tag", "popupmenu_demo");

// Exit application
quit_button = uicontrol(f, "Position", [10 210 280 20], ...
    "Style", "pushbutton", ...
    "String", gettext("Quit demonstration"), ...
    "FontWeight", "bold", ...
    "FontSize", 11, ...
    "Callback", "close_uicontrols_demo();");

