// Unitary tests for Scilab Java pushbuttons
// Vincent COUVERT - INRIA 2007

h = uicontrol();
set(h, "label", "Black button");
set(h, "backgroundcolor", [0 0 0]);
set(h, "foregroundcolor", [1 0 0]);
set(h, "position", [50 100 200 30]);
set(h, "fontweight", "normal");

h = uicontrol(gcf());
set(h, "label", "White button");
set(h, "backgroundcolor", [1 1 1]);
set(h, "foregroundcolor", [0 1 0]);
set(h, "position", [50 125 200 30]);
set(h, "fontweight", "light");

h = uicontrol("parent", gcf());
set(h, "label", "Light grey button");
set(h, "backgroundcolor", [0.7 0.7 0.7]);
set(h, "foregroundcolor", [0 0 1]);
set(h, "position", [50 150 200 30]);
set(h, "fontweight", "demi");

h = uicontrol("style", "pushbutton");
set(h, "label", "Yellow button");
set(h, "backgroundcolor", "1|1|0");
set(h, "foregroundcolor", "1|0|0");
set(h, "position", [250 100 200 30]);
set(h, "fontweight", "bold");

h = uicontrol(gcf(), "style", "pushbutton");
set(h, "label", "Cyan button");
set(h, "backgroundcolor", "0|1|1");
set(h, "foregroundcolor", "0|1|0");
set(h, "position", [250 125 200 30]);

h = uicontrol("parent", gcf(), "style", "pushbutton");
set(h, "label", "Dark grey button");
set(h, "backgroundcolor", "0.3|0.3|0.3");
set(h, "foregroundcolor", "0|0.5|0");
set(h, "position", [250 150 200 30]);

// Current figure size
f=gcf();
figsize = f.figure_size;

h = uicontrol("parent", gcf(), "style", "pushbutton", "position", [0 0 200 20], "label", "I''m a bottom left button");
set(h, "backgroundcolor", [1 0 0]);
set(h, "foregroundcolor", [0 0 1]);

h = uicontrol("parent", gcf(), "style", "pushbutton", "position", [figsize(1)-200 0 200 20], "label", "I''m a bottom right button");
set(h, "backgroundcolor", [0 1 0]);
set(h, "foregroundcolor", [1 0 0]);

h = uicontrol("parent", gcf(), "style", "pushbutton", "position", [0 figsize(2)-20 200 20], "label", "I''m a top left button");
set(h, "backgroundcolor", [0 0 1]);
set(h, "foregroundcolor", [1 0.5 0]);

h = uicontrol("parent", gcf(), "style", "pushbutton", "position", [figsize(1)-200 figsize(2)-20 200 20], "label", "I''m a top right button", "fontweight", "bold");
set(h, "backgroundcolor", [1 0 1]);
set(h, "foregroundcolor", [0 0 0]);
