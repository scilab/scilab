function make_plot2d()
f = uiget("/mywin/tab1/p1/p3/sciplot1");
scf(double(f.figureId));
clf();
plot2d();
tf = uiget("/mywin/tab1/p1/p2/field1");
xtitle(tf.text)
endfunction

function make_plot3d()
f = uiget("/mywin/tab1/p1/p3/sciplot1");
scf(double(f.figureId));
clf();
plot3d();
tf = uiget("/mywin/tab1/p1/p2/field1");
xtitle(tf.text)
endfunction

function update_title()
f = uiget("/mywin/tab1/p1/p3/sciplot1");
scf(double(f.figureId));
tf = uiget("/mywin/tab1/p1/p2/field1");
xtitle(tf.text)
endfunction

function slider_cb(id, x)
f = uiget("/mywin/tab1/p1/p3/sciplot1");
scf(double(f.figureId));
a=gca();
ra = a.rotation_angles;
a.rotation_angles = [x, ra(2)];
endfunction

function rotate_cb(id, angles)
sl = uiget("/mywin/tab1/p1/p2/sl1");
uiset(sl, "onchangeEnable", "off", "value", pmodulo(angles(1), 360), "onchangeEnable", "on");
endfunction

win = uiwidget("style", "UIScilabWindow", "id", "mywin", "location", [200, 200], "size", [1500, 500]);
tab = uicontrol(win, "style", "UIScilabTab", "title", "My Tab", "id", "tab1");
p1 = uicontrol(tab, "style", "UIPanel", "layout", "name:grid; rows:1; cols:2", "id", "p1", "background", [1 1 1]);
p2 = uicontrol(p1, "style", "UIPanel", "layout", "name:gridbag", "border", "name:titled; title:Plot creation", "id", "p2");
uicontrol(p2, "style", "UILabel", "text", "Make the basic plot2d", "id", "label1", "constraint", "gridx:1; gridy:1; weightx:1; weighty:0; fill:horizontal");
b1 = uicontrol(p2, "style", "UIButton", "text", "plot2d", "onclick", "make_plot2d", "id", "b1", "constraint", "gridx:2; gridy:1; weightx:0; weighty:0");
uicontrol(p2, "style", "UILabel", "text", "Make the basic plot3d", "id", "label2", "constraint", "gridx:1; gridy:2; weightx:1; weighty:0; fill:horizontal");
b2 = uicontrol(p2, "style", "UIButton", "text", "plot3d", "onclick", "make_plot3d", "id", "b2", "constraint", "gridx:2; gridy:2; weightx:0; weighty:0");
tf = uicontrol(p2, "style", "UITextfield", "text", "My beautiful plot", "onenter", "update_title", "id", "field1", "constraint", "gridx:1; gridy:3; weightx:1; weighty:0; fill:horizontal");
b3 = uicontrol(p2, "style", "UIButton", "text", "Make title", "onclick", "update_title", "id", "b3", "constraint", "gridx:2; gridy:3; weightx:0; weighty:0");
uicontrol(p2, "style", "UILabel", "text", "You can use the slider to rotate the figure", "id", "label3", "constraint", "gridx:1; gridy:4; weightx:1; weighty:0; fill:horizontal");
sl = uicontrol(p2, "style", "UISlider", "id", "sl1", "orientation", "horizontal", "min", 0, "max", 360, "value", 0, "paint-ticks", %t, "labels-increment", 20, "size", 8, "onchange", "slider_cb", "constraint", "gridx:1; gridy:5; weightx:1; weighty:0; gridwidth:2; fill:horizontal");
//img = uicontrol(p2, "style", "UILabel", "icon", "http://www.scilab.org/var/ezwebin_site/storage/images/design/scilab-website/172-2-eng-GB/Scilab-WebSite.png", "id", "logo", "constraint", "gridx:1; gridy:3; weightx:1; weighty:1; gridwidth:2; fill:both");

p3 = uicontrol(p1, "style", "UIPanel", "layout", "name:border", "border", "name:titled; title:Plot representation", "id", "p3", "background", [1 1 1]);
pl = uicontrol(p3, "style", "UIScilabPlot", "constraint", "center", "id", "sciplot1", "onrotate", "rotate_cb");

win.visible = %t;