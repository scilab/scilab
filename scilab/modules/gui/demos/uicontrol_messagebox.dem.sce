//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// figure creation for a modal messagebox-like popup
// the variable btn = Nan while open ; -1 if closed ; 0 if cancel ; 1 if OK
// the variable data is used to store some output values
data = grand(4,3,"uin",0,1)
btn = %nan;

f = figure("figure_name", gettext("Please fill the table"), "closerequestfcn", "btn=-1; close();", "callback_type", 10, "menubar_visible", "off", "toolbar_visible", "off", "infobar_visible", "off", "dockable", "off", "default_axes", "off", "visible", "off", "layout", "border");

middle_frame = uicontrol(f, "style", "frame", "constraints", createConstraints("border", "center"));
bottom_frame = uicontrol(f, "style", "frame", "constraints", createConstraints("border", "bottom"), "layout" , "gridbag");

// the bottom buttons
uicontrol(bottom_frame, "style", "pushbutton", "string", gettext("Ok"), "callback", "btn = 1; data=save_data(); close();", "callback_type", 10, "margin", [10 10 10 10]);
uicontrol(bottom_frame, "style", "pushbutton", "string", gettext("Cancel"), "callback", "btn = 0; close();", "callback_type", 10, "margin", [10 10 10 10]);

// add a label and a grid of checkboxes
middle_frame.margins = [5 5 5 5];
middle_frame.layout = "border"
uicontrol(middle_frame, "style", "text", "string", "Please select the kind to use", "constraint", createConstraints("border", "top"));
inner_frame = uicontrol(middle_frame, "style", "frame", "constraint", createConstraints("border", "center"), "layout", "grid", "layout_options", createLayoutOptions("grid", size(data)));
d=data'
for i=size(d, "*"):-1:1
    uicontrol(inner_frame, "style", "checkbox", ...
    "value", d(i), ...
    "string", "validate "+string(i));
end

// callback used to store the updated values
function d = save_data()
    d = zeros(3,4);
    for i=size(d, "*"):-1:1
        d(i) = inner_frame.children(i).value;
    end
    d = d';
endfunction

// display the result before the edition
disp(data, "before")

// is modal but without burning the CPU
f.visible = "on";
while is_handle_valid(f),
    sleep(100);
end

// display the edited result
disp(data, "after edit")
