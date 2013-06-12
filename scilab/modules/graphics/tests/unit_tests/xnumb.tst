// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Unit test for xnumb

nb_grad = 2;
[valeurs] = linspace (0.935000 ,1.000000 ,nb_grad);
[x] = linspace (0.95,0.95,nb_grad);
[y] = linspace (-0.02,0.02,nb_grad);
plot2d ([0.935000 ,1.000000] ,[-0.03,0.03]);
xnumb(x,y,valeurs);
a1=gca();
t1=a1.children(1);
assert_checkequal(t1.type,"Text");
assert_checkequal(t1.visible,"on");
assert_checkalmostequal(strtod(t1.text),valeurs(2));
assert_checkequal(t1.alignment,"left");
assert_checkequal(t1.data,[x(2),y(2)]);
assert_checkequal(t1.box,"off");
assert_checkequal(t1.line_mode,"on");
assert_checkequal(t1.fill_mode,"off");
assert_checkequal(t1.text_box,[0,0]);
assert_checkequal(t1.text_box_mode,"off");
assert_checkequal(t1.font_foreground,-1);
assert_checkequal(t1.foreground,-1);
assert_checkequal(t1.background,-2);
assert_checkequal(t1.font_style,6);
assert_checkequal(t1.fractional_font,"off");
assert_checkequal(t1.auto_dimensionning,"on");
assert_checkequal(t1.font_angle,0);
assert_checkequal(t1.clip_state,"clipgrf");
assert_checkequal(t1.clip_box,[]);
assert_checkequal(t1.userdata,[]);
t2=a1.children(2);
assert_checkequal(t2.type,"Text");
assert_checkequal(t2.visible,"on");
assert_checkalmostequal(strtod(t2.text),valeurs(1));
assert_checkequal(t2.alignment,"left");
assert_checkequal(t2.data,[x(1),y(1)]);
assert_checkequal(t2.box,"off");
assert_checkequal(t2.line_mode,"on");
assert_checkequal(t2.fill_mode,"off");
assert_checkequal(t2.text_box,[0,0]);
assert_checkequal(t2.text_box_mode,"off");
assert_checkequal(t2.font_foreground,-1);
assert_checkequal(t2.foreground,-1);
assert_checkequal(t2.background,-2);
assert_checkequal(t2.font_style,6);
assert_checkequal(t2.fractional_font,"off");
assert_checkequal(t2.auto_dimensionning,"on");
assert_checkequal(t2.font_angle,0);
assert_checkequal(t2.clip_state,"clipgrf");
assert_checkequal(t2.clip_box,[]);
assert_checkequal(t2.userdata,[]);

// With box =1
scf();
nb_grad = 2;
[valeurs] = linspace (0.935000 ,1.000000 ,nb_grad);
[x] = linspace (0.95,0.95,nb_grad);
[y] = linspace (-0.02,0.02,nb_grad);
plot2d ([0.935000 ,1.000000] ,[-0.03,0.03]);
xnumb(x,y,valeurs,1);
abox=gca();
t1box=abox.children(1);
assert_checkequal(t1box.type,t1.type);
assert_checkequal(t1box.visible,t1.visible);
assert_checkequal(t1box.text,t1.text);
assert_checkequal(t1box.alignment,t1.alignment);
assert_checkequal(t1box.data,t1.data);
assert_checkequal(t1box.box,"on");
assert_checkequal(t1box.line_mode,t1.line_mode);
assert_checkequal(t1box.fill_mode,t1.fill_mode);
assert_checkequal(t1box.text_box,t1.text_box);
assert_checkequal(t1box.text_box_mode,t1.text_box_mode);
assert_checkequal(t1box.font_foreground,t1.font_foreground);
assert_checkequal(t1box.foreground,t1.foreground);
assert_checkequal(t1box.background,t1.background);
assert_checkequal(t1box.font_style,t1.font_style);
assert_checkequal(t1box.fractional_font,t1.fractional_font);
assert_checkequal(t1box.auto_dimensionning,t1.auto_dimensionning);
assert_checkequal(t1box.font_angle,t1.font_angle);
assert_checkequal(t1box.clip_state,t1.clip_state);
assert_checkequal(t1box.clip_box,t1.clip_box);
assert_checkequal(t1box.userdata,t1.userdata);
t2box=abox.children(2);
assert_checkequal(t2box.type,t2.type);
assert_checkequal(t2box.visible,t2.visible);
assert_checkequal(t2box.text,t2.text);
assert_checkequal(t2box.alignment,t2.alignment);
assert_checkequal(t2box.data,t2.data);
assert_checkequal(t2box.box,"on");
assert_checkequal(t2box.line_mode,t2.line_mode);
assert_checkequal(t2box.fill_mode,t2.fill_mode);
assert_checkequal(t2box.text_box,t2.text_box);
assert_checkequal(t2box.text_box_mode,t2.text_box_mode);
assert_checkequal(t2box.font_foreground,t2.font_foreground);
assert_checkequal(t2box.foreground,t2.foreground);
assert_checkequal(t2box.background,t2.background);
assert_checkequal(t2box.font_style,t2.font_style);
assert_checkequal(t2box.fractional_font,t2.fractional_font);
assert_checkequal(t2box.auto_dimensionning,t2.auto_dimensionning);
assert_checkequal(t2box.font_angle,t2.font_angle);
assert_checkequal(t2box.clip_state,t2.clip_state);
assert_checkequal(t2box.clip_box,t2.clip_box);
assert_checkequal(t2box.userdata,t2.userdata);
close

// With angle=[20,20]
scf();
nb_grad = 2;
[valeurs] = linspace (0.935000 ,1.000000 ,nb_grad);
[x] = linspace (0.95,0.95,nb_grad);
[y] = linspace (-0.02,0.02,nb_grad);
plot2d ([0.935000 ,1.000000] ,[-0.03,0.03]);
xnumb(x,y,valeurs,0,[20,20]);
aangle=gca();
t1angle=aangle.children(1);
assert_checkequal(t1angle.type,t1.type);
assert_checkequal(t1angle.visible,t1.visible);
assert_checkequal(t1angle.text,t1.text);
assert_checkequal(t1angle.alignment,t1.alignment);
assert_checkequal(t1angle.data,t1.data);
assert_checkequal(t1angle.box,t1.box);
assert_checkequal(t1angle.line_mode,t1.line_mode);
assert_checkequal(t1angle.fill_mode,t1.fill_mode);
assert_checkequal(t1angle.text_box,t1.text_box);
assert_checkequal(t1angle.text_box_mode,t1.text_box_mode);
assert_checkequal(t1angle.font_foreground,t1.font_foreground);
assert_checkequal(t1angle.foreground,t1.foreground);
assert_checkequal(t1angle.background,t1.background);
assert_checkequal(t1angle.font_style,t1.font_style);
assert_checkequal(t1angle.fractional_font,t1.fractional_font);
assert_checkequal(t1angle.auto_dimensionning,t1.auto_dimensionning);
assert_checkequal(t1angle.font_angle,20);
assert_checkequal(t1angle.clip_state,t1.clip_state);
assert_checkequal(t1angle.clip_box,t1.clip_box);
assert_checkequal(t1angle.userdata,t1.userdata);
t2angle=aangle.children(2);
assert_checkequal(t2angle.type,t2.type);
assert_checkequal(t2angle.visible,t2.visible);
assert_checkequal(t2angle.text,t2.text);
assert_checkequal(t2angle.alignment,t2.alignment);
assert_checkequal(t2angle.data,t2.data);
assert_checkequal(t2angle.box,"off");
assert_checkequal(t2angle.line_mode,t2.line_mode);
assert_checkequal(t2angle.fill_mode,t2.fill_mode);
assert_checkequal(t2angle.text_box,t2.text_box);
assert_checkequal(t2angle.text_box_mode,t2.text_box_mode);
assert_checkequal(t2angle.font_foreground,t2.font_foreground);
assert_checkequal(t2angle.foreground,t2.foreground);
assert_checkequal(t2angle.background,t2.background);
assert_checkequal(t2angle.font_style,t2.font_style);
assert_checkequal(t2angle.fractional_font,t2.fractional_font);
assert_checkequal(t2angle.auto_dimensionning,t2.auto_dimensionning);
assert_checkequal(t2angle.font_angle,20);
assert_checkequal(t2angle.clip_state,t2.clip_state);
assert_checkequal(t2angle.clip_box,t2.clip_box);
assert_checkequal(t2angle.userdata,t2.userdata);
close
close

// Error messages
[valeurs] = linspace (0.935000 ,1.000000 ,nb_grad);
[x] = linspace (0.95,0.95,nb_grad);
[y] = linspace (-0.02,0.02,nb_grad);
plot2d ([0.935000 ,1.000000] ,[-0.03,0.03]);

errmsg1=msprintf(_("%s: Wrong size for input arguments: Same sizes expected.\n"),"xnumb");
assert_checkerror("xnumb(x,y,valeurs,1,20);",errmsg1);

errmsg2=msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"), "xnumb", 3, 5);
assert_checkerror("xnumb(x,y);",errmsg2);

close
