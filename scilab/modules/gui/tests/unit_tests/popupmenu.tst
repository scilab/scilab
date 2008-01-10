// Unitary tests for Scilab Java popupmenu
// Vincent COUVERT - INRIA 2008

// Default popupmenu
h = uicontrol("style", "popupmenu");
set(h, "string", "Default popupmenu");

// Creation without parent
h = uicontrol("style", "popupmenu");
set(h, "string", "Black popupmenu");
set(h, "backgroundcolor", [0 0 0]);
set(h, "foregroundcolor", [1 0 0]);
set(h, "position", [50 100 200 30]);
set(h, "fontweight", "normal");

// Creation with parent given first
h = uicontrol("parent", gcf(), "style", "popupmenu");
set(h, "string", "White popupmenu");
set(h, "backgroundcolor", [1 1 1]);
set(h, "foregroundcolor", [0 1 0]);
set(h, "position", [50 130 200 30]);
set(h, "fontweight", "light");

// Creation with parent given after creation
h = uicontrol("style", "popupmenu", "parent", gcf());
set(h, "string", "Light grey popupmenu");
set(h, "backgroundcolor", [0.7 0.7 0.7]);
set(h, "foregroundcolor", [0 0 1]);
set(h, "position", [50 160 200 30]);
set(h, "fontweight", "demi");

// Creation with parent given without "parent" property
h = uicontrol(gcf(), "style", "popupmenu");
set(h, "string", "Dark grey button");
set(h, "backgroundcolor", "0.3|0.3|0.3");
set(h, "foregroundcolor", "0|0.5|0");
set(h, "position", [250 160 200 30]);
set(h, "fontweight", "bold");

// Current figure size
f=gcf();
figsize = f.figure_size;

h = uicontrol("parent", gcf(), "style", "popupmenu", "position", [0 0 200 20], "string", "I''m a bottom left popupmenu");
set(h, "backgroundcolor", [1 0 0]);
set(h, "foregroundcolor", [0 0 1]);

h = uicontrol("parent", gcf(), "style", "popupmenu", "position", [figsize(1)-200 0 200 20], "string", "I''m a bottom right popupmenu");
set(h, "backgroundcolor", [0 1 0]);
set(h, "foregroundcolor", [1 0 0]);

h = uicontrol("parent", gcf(), "style", "popupmenu", "position", [0 figsize(2)-20 200 20], "string", "I''m a top left popupmenu");
set(h, "backgroundcolor", [0 0 1]);
set(h, "foregroundcolor", [1 0.5 0]);

h = uicontrol("parent", gcf(), "style", "popupmenu", "position", [figsize(1)-200 figsize(2)-20 200 20], "string", "I''m a top right popupmenu", "fontweight", "bold");
set(h, "backgroundcolor", [1 0 1]);
set(h, "foregroundcolor", [0 0 0]);
