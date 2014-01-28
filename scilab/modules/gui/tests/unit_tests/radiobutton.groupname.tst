// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

function check(rb)
    rb.value = rb.max;
endfunction

function uncheck(rb)
    rb.value = rb.min;
endfunction

function a = checked(rb)
    a = rb.value == rb.max;
endfunction

function a = getStatus()
    a = [checked(rb1), checked(rb2), checked(rb3), checked(rb4)];
endfunction


f1 = figure("toolbar_visible", "off", "menubar_visible", "off", "infobar_visible", "off");
f1.figure_position = [100, 100];
f1.figure_size = [200, 200];
f2 = figure("toolbar_visible", "off", "menubar_visible", "off", "infobar_visible", "off");
f2.figure_position = [300, 100];
f2.figure_size = [200, 200];


//all in same group
rb1 = uicontrol(f1, "style", "radiobutton", "string", "radiobutton 1", "position", [30 35 100 25], "groupname", "group1");
rb2 = uicontrol(f1, "style", "checkbox", "string", "checkbox 2", "position", [30 85 100 25], "groupname", "group1");
rb3 = uicontrol(f2, "style", "radiobutton", "string", "radiobutton 3", "position", [30 35 100 25], "groupname", "group1");
rb4 = uicontrol(f2, "style", "checkbox", "string", "checkbox 4", "position", [30 85 100 25], "groupname", "group1");

check(rb1);
assert_checkequal(getStatus(), [%t %f %f %f]);
uncheck(rb1);
assert_checkequal(getStatus(), [%t %f %f %f]);

check(rb2);
assert_checkequal(getStatus(), [%f %t %f %f]);
uncheck(rb2);
assert_checkequal(getStatus(), [%f %t %f %f]);

check(rb3);
assert_checkequal(getStatus(), [%f %f %t %f]);
uncheck(rb3);
assert_checkequal(getStatus(), [%f %f %t %f]);

check(rb4);
assert_checkequal(getStatus(), [%f %f %f %t]);
uncheck(rb4);
assert_checkequal(getStatus(), [%f %f %f %t]);


//remove rb1 and rb3 from group 1
rb1.groupname = "";
rb3.groupname = "";

check(rb1);
assert_checkequal(getStatus(), [%t %f %f %t]);
uncheck(rb1);
assert_checkequal(getStatus(), [%f %f %f %t]);

check(rb2);
assert_checkequal(getStatus(), [%f %t %f %f]);
uncheck(rb2);
assert_checkequal(getStatus(), [%f %t %f %f]);

check(rb3);
assert_checkequal(getStatus(), [%f %t %t %f]);
uncheck(rb3);
assert_checkequal(getStatus(), [%f %t %f %f]);

check(rb4);
assert_checkequal(getStatus(), [%f %f %f %t]);
uncheck(rb4);
assert_checkequal(getStatus(), [%f %f %f %t]);

//add rb1 and rb2 in a second group
rb1.groupname = "group2";
rb3.groupname = "group2";

check(rb1);
assert_checkequal(getStatus(), [%t %f %f %t]);
uncheck(rb1);
assert_checkequal(getStatus(), [%t %f %f %t]);

check(rb2);
assert_checkequal(getStatus(), [%t %t %f %f]);
uncheck(rb2);
assert_checkequal(getStatus(), [%t %t %f %f]);

check(rb3);
assert_checkequal(getStatus(), [%f %t %t %f]);
uncheck(rb3);
assert_checkequal(getStatus(), [%f %t %t %f]);

check(rb4);
assert_checkequal(getStatus(), [%f %f %t %t]);
uncheck(rb4);
assert_checkequal(getStatus(), [%f %f %t %t]);

//move rb1 and rb3 from group2 to group1
rb1.groupname = "group1";
rb3.groupname = "group1";

check(rb1);
assert_checkequal(getStatus(), [%t %f %f %f]);
uncheck(rb1);
assert_checkequal(getStatus(), [%t %f %f %f]);

check(rb2);
assert_checkequal(getStatus(), [%f %t %f %f]);
uncheck(rb2);
assert_checkequal(getStatus(), [%f %t %f %f]);

check(rb3);
assert_checkequal(getStatus(), [%f %f %t %f]);
uncheck(rb3);
assert_checkequal(getStatus(), [%f %f %t %f]);

check(rb4);
assert_checkequal(getStatus(), [%f %f %f %t]);
uncheck(rb4);
assert_checkequal(getStatus(), [%f %f %f %t]);

delete(f1);
delete(f2);