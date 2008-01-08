// Unitary tests for Scilab Java edits
// Vincent COUVERT - INRIA 2008

// Default edit
h = uicontrol("style", "edit");
set(h, "string", "Default edit");

// Creation without parent
h = uicontrol("style", "edit");
set(h, "string", "Black edit");
set(h, "backgroundcolor", [0 0 0]);
set(h, "foregroundcolor", [1 0 0]);
set(h, "position", [50 100 200 30]);
set(h, "fontweight", "normal");

// Creation with parent given first
h = uicontrol("parent", gcf(), "style", "edit");
set(h, "string", "White edit");
set(h, "backgroundcolor", [1 1 1]);
set(h, "foregroundcolor", [0 1 0]);
set(h, "position", [50 130 200 30]);
set(h, "fontweight", "light");

// Creation with parent given after creation
h = uicontrol("style", "edit", "parent", gcf());
set(h, "string", "Light grey edit");
set(h, "backgroundcolor", [0.7 0.7 0.7]);
set(h, "foregroundcolor", [0 0 1]);
set(h, "position", [50 160 200 30]);
set(h, "fontweight", "demi");

// Creation with parent given without "parent" property
h = uicontrol(gcf(), "style", "edit");
set(h, "string", "Dark grey button");
set(h, "backgroundcolor", "0.3|0.3|0.3");
set(h, "foregroundcolor", "0|0.5|0");
set(h, "position", [250 160 200 30]);
set(h, "fontweight", "bold");

// Current figure size
f=gcf();
figsize = f.figure_size;

h = uicontrol("parent", gcf(), "style", "edit", "position", [0 0 200 20], "string", "I''m a bottom left edit");
set(h, "backgroundcolor", [1 0 0]);
set(h, "foregroundcolor", [0 0 1]);

h = uicontrol("parent", gcf(), "style", "edit", "position", [figsize(1)-200 0 200 20], "string", "I''m a bottom right edit");
set(h, "backgroundcolor", [0 1 0]);
set(h, "foregroundcolor", [1 0 0]);

h = uicontrol("parent", gcf(), "style", "edit", "position", [0 figsize(2)-20 200 20], "string", "I''m a top left edit");
set(h, "backgroundcolor", [0 0 1]);
set(h, "foregroundcolor", [1 0.5 0]);

h = uicontrol("parent", gcf(), "style", "edit", "position", [figsize(1)-200 figsize(2)-20 200 20], "string", "I''m a top right edit", "fontweight", "bold");
set(h, "backgroundcolor", [1 0 1]);
set(h, "foregroundcolor", [0 0 0]);
