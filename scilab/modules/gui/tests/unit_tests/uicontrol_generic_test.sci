// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function ierr = uicontrol_generic_test(uicontrol_style)

// Default error status
ierr = 0;

// Default uicontrol of style 'uicontrol_style'
h = uicontrol("style", uicontrol_style);
set(h, "string", "Default "+uicontrol_style);

// Creation without parent
h = uicontrol("style", uicontrol_style);
set(h, "string", "Black background "+uicontrol_style);
set(h, "backgroundcolor", [0 0 0]);
set(h, "foregroundcolor", [1 0 0]);
set(h, "position", [50 100 200 30]);
set(h, "fontweight", "normal");

// Creation with parent given first
h = uicontrol("parent", gcf(), "style", uicontrol_style);
set(h, "string", "White background "+uicontrol_style);
set(h, "backgroundcolor", [1 1 1]);
set(h, "foregroundcolor", [0 1 0]);
set(h, "position", [50 130 200 30]);
set(h, "fontweight", "light");

// Creation with parent given after creation
h = uicontrol("style", uicontrol_style, "parent", gcf());
set(h, "string", "Light grey background "+uicontrol_style);
set(h, "backgroundcolor", [0.7 0.7 0.7]);
set(h, "foregroundcolor", [0 0 1]);
set(h, "position", [50 160 200 30]);
set(h, "fontweight", "demi");

// Creation with parent given without "parent" property
h = uicontrol(gcf(), "style", uicontrol_style);
set(h, "string", "Dark grey background "+uicontrol_style);
set(h, "backgroundcolor", "0.3|0.3|0.3");
set(h, "foregroundcolor", "0|0.5|0");
set(h, "position", [250 160 200 30]);
set(h, "fontweight", "bold");

// Current figure size
f=gcf();
figsize = f.axes_size;

h = uicontrol("parent", gcf(), "style", uicontrol_style, "position", [0 0 200 20], "string", "Bottom left "+uicontrol_style);
set(h, "backgroundcolor", [1 0 0]);
set(h, "foregroundcolor", [0 0 1]);

h = uicontrol("parent", gcf(), "style", uicontrol_style, "position", [figsize(1)-200 0 200 20], "string", "Bottom right "+uicontrol_style);
set(h, "backgroundcolor", [0 1 0]);
set(h, "foregroundcolor", [1 0 0]);

h = uicontrol("parent", gcf(), "style", uicontrol_style, "position", [0 figsize(2)-20 200 20], "string", "Top left "+uicontrol_style);
set(h, "backgroundcolor", [0 0 1]);
set(h, "foregroundcolor", [1 0.5 0]);

h = uicontrol("parent", gcf(), "style", uicontrol_style, "position", [figsize(1)-200 figsize(2)-20 200 20], "string", "Top right "+uicontrol_style, "fontweight", "bold");
set(h, "backgroundcolor", [1 0 1]);
set(h, "foregroundcolor", [0 0 0]);

// Test all properties with all possible values
// - First: set the property
// - Then: read the property and copare to the value set

// Create a new figure
f = scf();

// Create a new uicontrol
h = uicontrol("parent", f, "style", uicontrol_style);

// --- BackgroundColor ---
// TODO test default value
// Vector of 'integer' values
set(h, "backgroundcolor", [1 0 0]);
if ~and(get(h, "backgroundcolor") == [1 0 0]) then
  ierr = 1;
end
// Vector of 'real' values
set(h, "backgroundcolor", [0.3 0.4 0.5]);
if ~and(get(h, "backgroundcolor") == [0.3 0.4 0.5]) then
  ierr = 1;
end
// String of 'integer' values
set(h, "backgroundcolor", "1|1|0");
if ~and(get(h, "backgroundcolor") == [1 1 0]) then
  ierr = 1;
end
// String of 'real' values
set(h, "backgroundcolor", "0.2|0.3|0.4");
if ~and(get(h, "backgroundcolor") == [0.2 0.3 0.4]) then
  ierr = 1;
end
// TODO Test with wrong format values: "aze", "", [], ...

// --- Callback tests ---
// Default values
if get(h, "callback") <> "" then
  ierr = 1;
end
if get(h, "callback_type") <> -1 then
  ierr = 1;
end
// Set a callback and do not precise type
set(h, "callback", "disp(0);");
if get(h, "callback") <> "disp(0);" then
  ierr = 1;
end
if get(h, "callback_type") <> 0 then
  ierr = 1;
end
set(h, "callback", "disp(1);");
set(h, "callback_type", 0);
if get(h, "callback") <> "disp(1);" then
  ierr = 1;
end
if get(h, "callback_type") <> 0 then
  ierr = 1;
end
// Remove callback --> callbackType must switch automatically to -1
set(h, "callback", "");
if get(h, "callback") <> "" then
  ierr = 1;
end
if get(h, "callback_type") <> -1 then
  ierr = 1;
end
// TODO tests with wrong callbackType ??

// --- Enable ---
// Default value
if get(h, "enable") <> "on" then
  ierr = 1;
end
set(h, "enable", "off");
if get(h, "enable") <> "off" then
  ierr = 1;
end
set(h, "enable", "on");
if get(h, "enable") <> "on" then
  ierr = 1;
end
// TODO test with wrong values

// --- FontAngle ---
// Default value
if get(h, "fontangle") <> "normal" then
  ierr = 1;
end
set(h, "fontangle", "italic");
if get(h, "fontangle") <> "italic" then
  ierr = 1;
end
set(h, "fontangle", "oblique");
if get(h, "fontangle") <> "oblique" then
  ierr = 1;
end
set(h, "fontangle", "normal");
if get(h, "fontangle") <> "normal" then
  ierr = 1;
end
// TODO test with wrong values

// --- FontSize ---
// Default value
if get(h, "fontsize") <> 10 then
  ierr = 1;
end
set(h, "fontsize", 12);
if get(h, "fontsize") <> 12 then
  ierr = 1;
end
// TODO test with wrong values

// --- FontUnits ---
// Default value
if get(h, "fontunits") <> "points" then
  ierr = 1;
end
set(h, "fontunits", "pixels");
if get(h, "fontunits") <> "pixels" then
  ierr = 1;
end
set(h, "fontunits", "normalized");
if get(h, "fontunits") <> "normalized" then
  ierr = 1;
end
set(h, "fontunits", "points");
if get(h, "fontunits") <> "points" then
  ierr = 1;
end
// TODO test with wrong values

// --- FontWeight ---
// Default value
if get(h, "fontweight") <> "normal" then
  ierr = 1;
end
set(h, "fontweight", "light");
if get(h, "fontweight") <> "light" then
  ierr = 1;
end
set(h, "fontweight", "demi");
if get(h, "fontweight") <> "demi" then
  ierr = 1;
end
set(h, "fontweight", "bold");
if get(h, "fontweight") <> "bold" then
  ierr = 1;
end
set(h, "fontweight", "normal");
if get(h, "fontweight") <> "normal" then
  ierr = 1;
end
// TODO test with wrong values

// --- Fontname tests ---
if get(h, "fontname") <> "helvetica" then
  ierr = 1;
end
// Try to set an existing font
set(h, "fontname", "courier new");
if get(h, "fontname") <> "courier new" then
  ierr = 1;
end
// Try to set a not-existing font
set(h, "fontname", "an invented font");
if get(h, "fontname") <> "an invented font" then
  ierr = 1;
end

// --- ForegroundColor --- 
// TODO test default value
// Vector of 'integer' values
set(h, "foregroundcolor", [1 0 1]);
if ~and(get(h, "foregroundcolor") == [1 0 1]) then
  ierr = 1;
end
// Vector of 'real' values
set(h, "foregroundcolor", [0.6 0.5 0.4]);
if ~and(get(h, "foregroundcolor") == [0.6 0.5 0.4]) then
  ierr = 1;
end
// String of 'integer' values
set(h, "foregroundcolor", "1|1|1");
if ~and(get(h, "foregroundcolor") == [1 1 1]) then
  ierr = 1;
end
// String of 'real' values
set(h, "foregroundcolor", "0.5|0.6|0.7");
if ~and(get(h, "foregroundcolor") == [0.5 0.6 0.7]) then
  ierr = 1;
end
// TODO Test with wrong format values: "aze", "", [], ...

// --- HorizontalAlignment --- 
if get(h, "horizontalalignment") <> "center" then
  ierr = 1;
end
set(h, "horizontalalignment", "left");
if get(h, "horizontalalignment") <> "left" then
  ierr = 1;
end
set(h, "horizontalalignment", "right");
if get(h, "horizontalalignment") <> "right" then
  ierr = 1;
end
set(h, "horizontalalignment", "center");
if get(h, "horizontalalignment") <> "center" then
  ierr = 1;
end
// TODO test with wrong values

// TODO ListboxTop tests

// --- Max ---
// Default value
if get(h, "max") <> 1 then
  ierr = 1;
end
set(h, "max", 10);
if get(h, "max") <> 10 then
  ierr = 1;
end
set(h, "max", 1); // Back to default value needed by SliderStep test
if get(h, "max") <> 1 then
  ierr = 1;
end

// TODO test with min > max
// TODO test with wrong values

// --- Min ---
// Default value
if get(h, "min") <> 0 then
  ierr = 1;
end
set(h, "min", 1);
if get(h, "min") <> 1 then
  ierr = 1;
end
set(h, "min", 0); // Back to default value needed by SliderStep test
if get(h, "min") <> 0 then
  ierr = 1;
end
// TODO test with min > max
// TODO test with wrong values

// TODO Parent tests

// --- Position ---
// Default value
if ~and(get(h, "position") == [20 40 40 20]) then
  ierr = 1;
end
// Vector of 'integer' values
set(h, "position", [0 10 100 200])
if ~and(get(h, "position") == [0 10 100 200]) then
  ierr = 1;
end
// String of 'integer' values
set(h, "position", "10|20|50|100");
if ~and(get(h, "position") == [10 20 50 100]) then
  ierr = 1;
end
// TODO Test with wrong format values: "aze", "", [], ...

// --- Relief ---
// Default value is tested in separate files because is different according the style
set(h, "relief", "groove");
if get(h, "relief") <> "groove" then
  ierr = 1;
end
set(h, "relief", "raised");
if get(h, "relief") <> "raised" then
  ierr = 1;
end
set(h, "relief", "ridge");
if get(h, "relief") <> "ridge" then
  ierr = 1;
end
set(h, "relief", "solid");
if get(h, "relief") <> "solid" then
  ierr = 1;
end
set(h, "relief", "sunken");
if get(h, "relief") <> "sunken" then
  ierr = 1;
end
set(h, "relief", "flat");
if get(h, "relief") <> "flat" then
  ierr = 1;
end
// TODO test with wrong values

// --- SliderStep ---
// Default value
if ~and(get(h, "sliderstep") == [0.01 0.1]) then
  ierr = 1;
end
set(h, "sliderstep", [0.2 0.5]);
if ~and(get(h, "sliderstep") == [0.2 0.5]) then
  ierr = 1;
end
set(h, "sliderstep", [0.01 0.1]);
if ~and(get(h, "sliderstep") == [0.01 0.1]) then
  ierr = 1;
end
// TODO test for value1 > value2
// TODO test with wrong values

// --- String ---
if get(h, "string") <> "" then
  ierr = 1;
end
set(h, "string", ["test for string"]);
if get(h, "string") <> "test for string" then
  ierr = 1;
end
set(h, "string", "");
if get(h, "string") <> "" then
  ierr = 1;
end

// TODO test with wrong values

// --- Style ---
// Default value
if get(h, "style") <>  uicontrol_style then
  ierr = 1;
end
// TODO style changes tests (when implemented)

// --- Tag ---
// Default value
if get(h, "tag") <> "" then
  ierr = 1;
end
set(h, "tag", "test for tag");
if get(h, "tag") <> "test for tag" then
  ierr = 1;
end
set(h, "tag", "");
if get(h, "tag") <> "" then
  ierr = 1;
end
// TODO test with wrong values

// --- Units ---
// Default value
if get(h, "units") <> "pixels" then
  ierr = 1;
end
set(h, "units", "points");
if get(h, "units") <> "points" then
  ierr = 1;
end
set(h, "units", "normalized");
if get(h, "units") <> "normalized" then
  ierr = 1;
end
set(h, "units", "pixels");
if get(h, "units") <> "pixels" then
  ierr = 1;
end
// TODO test with wrong values

// --- UserData ---
// Default value
if ~isempty(get(h, "user_data")) then
  ierr = 1;
end
set(h, "user_data", [1 2 3]);
if ~and(get(h, "user_data") == [1 2 3]) then
  ierr = 1;
end
set(h, "user_data", []);
if ~isempty(get(h, "user_data")) then
  ierr = 1;
end

// --- Value tests ---
// Tested in separate files beause is different according the style
// TODO tests for checkbox and radiobuttons should fail !

// --- VerticalalAlignment --- 
if get(h, "verticalalignment") <> "middle" then
  ierr = 1;
end
set(h, "verticalalignment", "top");
if get(h, "verticalalignment") <> "top" then
  ierr = 1;
end
set(h, "verticalalignment", "bottom");
if get(h, "verticalalignment") <> "bottom" then
  ierr = 1;
end
set(h, "verticalalignment", "middle");
if get(h, "verticalalignment") <> "middle" then
  ierr = 1;
end
// TODO test with wrong values


endfunction
