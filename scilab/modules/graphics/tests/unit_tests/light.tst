// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test Light object

// Check each default property
figure();
a=gca();
l = light();

assert_checkequal(l.type, "Light");
assert_checkequal(l.parent.type, "Axes");
assert_checkequal(l.children, []);
assert_checkequal(l.visible, "on");
assert_checkequal(l.light_type, "point");



clf();
subplot(2, 1, 1);
a1=gca();
subplot(2, 1, 2);
a2=gca();
l = light(a1);

assert_checkequal(l.type, "Light");
assert_checkequal(l.parent.type, "Axes");
assert_checkequal(l.parent.UID, a1.UID);
assert_checkequal(l.children, []);
assert_checkequal(l.visible, "on");
assert_checkequal(l.light_type, "point");

clf();
a=gca();
l = light(a, visible="off", type="directional", position=[5 4 3], direction=[2 1 0], ambient_color=[0.3 0.3 0.3], diffuse_color=[0.2 0.2 0.2], specular_color=[0.1 0.1 0.1]);

assert_checkequal(l.type, "Light");
assert_checkequal(l.parent.type, "Axes");
assert_checkequal(l.children, []);
assert_checkequal(l.visible, "off");
assert_checkequal(l.light_type, "directional");
assert_checktrue(l.position - [5 4 3] < 1d-7);
assert_checktrue(l.direction - [2 1 0] < 1d-7);
assert_checktrue(l.ambient_color - [0.3 0.3 0.3] < 1d-7);
assert_checktrue(l.diffuse_color - [0.2 0.2 0.2] < 1d-7);
assert_checktrue(l.specular_color - [0.1 0.1 0.1] < 1d-7);




// check valid parameter type
errmsg=msprintf(_("Wrong type for ''%s'' property: Real matrix expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = ''asd''", "errcatch");
assert_checkequal(lasterror(), errmsg);


errmsg=msprintf(_("Wrong type for ''%s'' property: Real matrix expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = ''asd''", "errcatch");
assert_checkequal(lasterror(), errmsg);


errmsg=msprintf(_("Wrong type for ''%s'' property: Real matrix expected.\n"), "specular_color");
ierr = execstr("l.specular_color = ''asd''", "errcatch");
assert_checkequal(lasterror(), errmsg);


errmsg=msprintf(_("Wrong type for ''%s'' property: Real matrix expected.\n"), "position");
ierr = execstr("l.position = ''asd''", "errcatch");
assert_checkequal(lasterror(), errmsg);


errmsg=msprintf(_("Wrong type for ''%s'' property: Real matrix expected.\n"), "direction");
ierr = execstr("l.direction = ''asd''", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong type for ''%s'' property: String expected.\n"),  "light_type");
ierr = execstr("l.light_type = 0", "errcatch");
assert_checkequal(lasterror(), errmsg);


// check parameter  vector size
errmsg=msprintf(_("Wrong size for ''%s'' property: %d elements expected.\n"), "ambient_color", 3);
ierr = execstr("l.ambient_color = [1 1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong size for ''%s'' property: %d elements expected.\n"), "diffuse_color", 3);
ierr = execstr("l.diffuse_color = [1 1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong size for ''%s'' property: %d elements expected.\n"), "specular_color", 3);
ierr = execstr("l.specular_color = [1 1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong size for ''%s'' property: %d elements expected.\n"), "position", 3);
ierr = execstr("l.position = [1 1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong size for ''%s'' property: %d elements expected.\n"), "direction", 3);
ierr = execstr("l.direction = [1 1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

// check parameter bounds

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = [-1 0 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = [0 -1 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = [0 0 -1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = [1.5 0 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = [0 1.5 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "ambient_color");
ierr = execstr("l.ambient_color = [0 0 1.5]", "errcatch");
assert_checkequal(lasterror(), errmsg);

////////
errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = [-1 0 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = [0 -1 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = [0 0 -1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = [1.5 0 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = [0 1.5 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "diffuse_color");
ierr = execstr("l.diffuse_color = [0 0 1.5]", "errcatch");
assert_checkequal(lasterror(), errmsg);

/////////////
errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "specular_color");
ierr = execstr("l.specular_color = [-1 0 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "specular_color");
ierr = execstr("l.specular_color = [0 -1 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "specular_color");
ierr = execstr("l.specular_color = [0 0 -1]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "specular_color");
ierr = execstr("l.specular_color = [1.5 0 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "specular_color");
ierr = execstr("l.specular_color = [0 1.5 0]", "errcatch");
assert_checkequal(lasterror(), errmsg);

errmsg=msprintf(_("Wrong values for ''%s'' property: values between [0, 1] expected.\n"), "specular_color");
ierr = execstr("l.specular_color = [0 0 1.5]", "errcatch");
assert_checkequal(lasterror(), errmsg);




// check light_type input
errmsg=msprintf(_("Wrong value for ''%s'' property: ''directional'' or ''point'' expected.\n"), "light_type");
ierr = execstr("l.light_type = ''pointt''", "errcatch");
assert_checkequal(lasterror(), errmsg);


errmsg=msprintf(_("Wrong value for ''%s'' property: ''directional'' or ''point'' expected.\n"), "light_type");
ierr = execstr("l.light_type = ''ddirectional''", "errcatch");
assert_checkequal(lasterror(), errmsg);



delete(l);
assert_checkequal(a.children, []);
close();
