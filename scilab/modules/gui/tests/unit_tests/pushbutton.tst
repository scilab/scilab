// Unitary tests for Scilab Java pushbuttons
// Vincent COUVERT - INRIA 2007

h = uicontrol();
set(h, "label", "test1");
h = uicontrol(gcf());
set(h, "label", "test2");
h = uicontrol("parent", gcf());
set(h, "label", "test3");

h = uicontrol("style", "pushbutton");
set(h, "label", "test4");
h = uicontrol(gcf(), "style", "pushbutton");
set(h, "label", "test5");
h = uicontrol("parent", gcf(), "style", "pushbutton");
set(h, "label", "test6");
