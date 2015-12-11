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
    consoleHandle = get(0);
    set(consoleHandle, "UseDeprecatedSkin", "on");
    h = uicontrol("parent", f, "style", uicontrol_style);

    // --- BackgroundColor ---
    // Default value is tested in separate files because is different according the style
    // Vector of 'integer' values
    set(h, "backgroundcolor", [1 0 0]);
    assert_checkequal(get(h, "backgroundcolor"), [1 0 0]);
    // Vector of 'real' values
    set(h, "backgroundcolor", [0.3 0.4 0.5]);
    assert_checkequal(get(h, "backgroundcolor"), [0.3 0.4 0.5]);
    // String of 'integer' values
    set(h, "backgroundcolor", "1|1|0");
    assert_checkequal(get(h, "backgroundcolor"), [1 1 0]);
    // String of 'real' values
    set(h, "backgroundcolor", "0.2|0.3|0.4");
    assert_checkequal(get(h, "backgroundcolor"), [0.2 0.3 0.4]);
    // TODO Test with wrong format values: 'aze', '', [], ...

    // --- Callback tests ---
    // Default values
    assert_checkequal(get(h, "callback"), "");
    assert_checkequal(get(h, "callback_type"), -1);
    // Set a callback and do not set type
    set(h, "callback", "disp(0);");
    assert_checkequal(get(h, "callback"), "disp(0);");
    assert_checkequal(get(h, "callback_type"), 0);
    // Set a callback and set type
    set(h, "callback", "disp(1);");
    set(h, "callback_type", 0);
    assert_checkequal(get(h, "callback"), "disp(1);");
    assert_checkequal(get(h, "callback_type"), 0);
    // Remove callback --> callbackType must switch automatically to -1
    set(h, "callback", "");
    assert_checkequal(get(h, "callback"), "");
    assert_checkequal(get(h, "callback_type"), -1);
    // TODO tests with wrong callbackType ??

    // --- Enable ---
    assert_checkequal(get(h, "enable"), "on"); // Default value
    set(h, "enable", "off");
    assert_checkequal(get(h, "enable"), "off");
    set(h, "enable", "on");
    assert_checkequal(get(h, "enable"), "on");
    // TODO test with wrong values

    // --- FontAngle ---
    assert_checkequal(get(h, "fontangle"), "normal"); // Default value
    set(h, "fontangle", "italic");
    assert_checkequal(get(h, "fontangle"), "italic");
    set(h, "fontangle", "oblique");
    assert_checkequal(get(h, "fontangle"), "oblique");
    set(h, "fontangle", "normal");
    assert_checkequal(get(h, "fontangle"), "normal");
    // TODO test with wrong values

    // --- FontSize ---
    assert_checkequal(get(h, "fontsize"), 10); // Default value
    set(h, "fontsize", 12);
    assert_checkequal(get(h, "fontsize"), 12);
    // TODO test with wrong values

    // --- FontUnits ---
    // Default value
    assert_checkequal(get(h, "fontunits"), "points"); // Default value
    set(h, "fontunits", "pixels");
    assert_checkequal(get(h, "fontunits"), "pixels");
    set(h, "fontunits", "normalized");
    assert_checkequal(get(h, "fontunits"), "normalized");
    set(h, "fontunits", "points");
    assert_checkequal(get(h, "fontunits"), "points");
    // TODO test with wrong values

    // --- FontWeight ---
    assert_checkequal(get(h, "fontweight"), "normal"); // Default value
    set(h, "fontweight", "light");
    assert_checkequal(get(h, "fontweight"), "light");
    set(h, "fontweight", "demi");
    assert_checkequal(get(h, "fontweight"), "demi");
    set(h, "fontweight", "bold");
    assert_checkequal(get(h, "fontweight"), "bold");
    set(h, "fontweight", "normal");
    assert_checkequal(get(h, "fontweight"), "normal");
    // TODO test with wrong values

    // --- Fontname tests ---
    assert_checkequal(get(h, "fontname"), "helvetica"); // Default value
    // Try to set an existing font
    set(h, "fontname", "courier new");
    assert_checkequal(get(h, "fontname"), "courier new");
    // Try to set a not-existing font
    set(h, "fontname", "a not-existing font");
    assert_checkequal(get(h, "fontname"), "a not-existing font");

    // --- ForegroundColor ---
    // Default value
    assert_checkequal(get(h, "foregroundcolor"), [-1 -1 -1]); // Set by Look&Feel
    // Vector of 'integer' values
    set(h, "foregroundcolor", [1 0 1]);
    assert_checkequal(get(h, "foregroundcolor"), [1 0 1]);
    // Vector of 'real' values
    set(h, "foregroundcolor", [0.6 0.5 0.4]);
    assert_checkequal(get(h, "foregroundcolor"), [0.6 0.5 0.4]);
    // String of 'integer' values
    set(h, "foregroundcolor", "1|1|1");
    assert_checkequal(get(h, "foregroundcolor"), [1 1 1]);
    // String of 'real' values
    set(h, "foregroundcolor", "0.5|0.6|0.7");
    assert_checkequal(get(h, "foregroundcolor"), [0.5 0.6 0.7]);
    // TODO Test with wrong format values: 'aze', '', [], ...

    // --- HorizontalAlignment ---
    assert_checkequal(get(h, "horizontalalignment"), "center"); // Default value
    set(h, "horizontalalignment", "left");
    assert_checkequal(get(h, "horizontalalignment"), "left");
    set(h, "horizontalalignment", "right");
    assert_checkequal(get(h, "horizontalalignment"), "right");
    set(h, "horizontalalignment", "center");
    assert_checkequal(get(h, "horizontalalignment"), "center");
    // TODO test with wrong values

    // TODO ListboxTop tests

    // --- Max ---
    assert_checkequal(get(h, "max"), 1); // Default value
    set(h, "max", 10);
    assert_checkequal(get(h, "max"), 10);
    set(h, "max", 1); // Back to default value needed by SliderStep test
    assert_checkequal(get(h, "max"), 1);
    // TODO test with min > max
    // TODO test with wrong values

    // --- Min ---
    assert_checkequal(get(h, "min"), 0); // Default value
    set(h, "min", 1);
    assert_checkequal(get(h, "min"), 1);
    set(h, "min", 0); // Back to default value needed by SliderStep test
    assert_checkequal(get(h, "min"), 0);
    // TODO test with min > max
    // TODO test with wrong values

    // TODO Parent tests

    // --- Position ---
    assert_checkequal(get(h, "position"), [20 40 40 20]); // Default value
    // Vector of 'integer' values
    set(h, "position", [0 10 100 200])
    assert_checkequal(get(h, "position"), [0 10 100 200]);
    // String of 'integer' values
    set(h, "position", "10|20|50|100");
    assert_checkequal(get(h, "position"), [10 20 50 100]);
    // TODO Test with wrong format values: 'aze', '', [], ...

    // --- Relief ---
    // Default value is tested in separate files because is different according the style
    set(h, "relief", "groove");
    assert_checkequal(get(h, "relief"), "groove");
    set(h, "relief", "raised");
    assert_checkequal(get(h, "relief"), "raised");
    set(h, "relief", "ridge");
    assert_checkequal(get(h, "relief"), "ridge");
    set(h, "relief", "solid");
    assert_checkequal(get(h, "relief"), "solid");
    set(h, "relief", "sunken");
    assert_checkequal(get(h, "relief"), "sunken");
    set(h, "relief", "flat");
    assert_checkequal(get(h, "relief"), "flat");
    // TODO test with wrong values

    // --- SliderStep ---
    assert_checkequal(get(h, "sliderstep"), [0.01 0.1]); // Default value
    set(h, "sliderstep", [0.2 0.5]);
    assert_checkequal(get(h, "sliderstep"), [0.2 0.5]);
    set(h, "sliderstep", [0.01 0.1]);
    assert_checkequal(get(h, "sliderstep"), [0.01 0.1]);
    // TODO test for value1 > value2
    // TODO test with wrong values

    // --- String ---
    assert_checktrue(isempty(get(h, "string")));  // Default value
    set(h, "string", ["test for string"]);
    assert_checkequal(get(h, "string"), "test for string");
    set(h, "string", "");
    assert_checkequal(get(h, "string"), "");
    // TODO test with wrong values

    // --- TooltipString ---
    assert_checkequal(get(h, "tooltipstring"), "");  // Default value
    set(h, "tooltipstring", ["test for tooltipstring"]);
    assert_checkequal(get(h, "tooltipstring"), "test for tooltipstring");
    set(h, "tooltipstring", ["test;for;tooltipstring"]);
    assert_checkequal(get(h, "tooltipstring"), ["test;for;tooltipstring"]);
    set(h, "tooltipstring", ["test,for,tooltipstring"]);
    assert_checkequal(get(h, "tooltipstring"), ["test,for,tooltipstring"]);
    set(h, "tooltipstring", "");
    assert_checkequal(get(h, "tooltipstring"), "");
    // TODO test with wrong values

    // --- Style ---
    assert_checkequal(get(h, "style"), uicontrol_style);
    // TODO style changes tests (when implemented)

    // --- Tag ---
    assert_checkequal(get(h, "tag"), ""); // Default value
    set(h, "tag", "test for tag");
    assert_checkequal(get(h, "tag"), "test for tag");
    set(h, "tag", "");
    assert_checkequal(get(h, "tag"), "");
    // TODO test with wrong values

    // --- Units ---
    assert_checkequal(get(h, "units"), "pixels"); // Default value
    set(h, "units", "points");
    assert_checkequal(get(h, "units"), "points");
    set(h, "units", "normalized");
    assert_checkequal(get(h, "units"), "normalized");
    set(h, "units", "pixels");
    assert_checkequal(get(h, "units"), "pixels");
    // TODO test with wrong values

    if 0 // TODO Activate this test
        // --- UserData ---
        assert_checkequal(get(h, "user_data"), []); // Default value
        set(h, "user_data", [1 2 3]);
        assert_checkequal(get(h, "user_data"), [1 2 3]);
        set(h, "user_data", []);
        assert_checkequal(get(h, "user_data"),[]);
    end

    // --- Value tests ---
    // Tested in separate files beause is different according the style
    // TODO tests for checkbox and radiobuttons should fail !

    // --- VerticalalAlignment ---
    assert_checkequal(get(h, "verticalalignment"), "middle"); // Default value
    set(h, "verticalalignment", "top");
    assert_checkequal(get(h, "verticalalignment"), "top");
    set(h, "verticalalignment", "bottom");
    assert_checkequal(get(h, "verticalalignment"), "bottom");
    set(h, "verticalalignment", "middle");
    assert_checkequal(get(h, "verticalalignment"), "middle");
    // TODO test with wrong values

    // Layout
    assert_checkequal(get(h, "layout"), "none"); // Default value
    set(h, "layout", "border");
    assert_checkequal(get(h, "layout"), "border");
    assert_checktrue(execstr("set(h, ""layout"", ""grid"");", "errcatch")<>0)

    // Margins
    assert_checkequal(get(h, "margins"), [0 0 0 0]); // Default value
    set(h, "margins", [1 2 3 4]);
    assert_checkequal(get(h, "margins"), [1 2 3 4]);
    // TODO test with wrong values

    // Constraints
    h.parent.layout = "border";
    assert_checkequal(get(h, "constraints"), createConstraints("border", "center")); // Default value
    set(h, "constraints", createConstraints("border", "left"));
    assert_checkequal(get(h, "constraints"), createConstraints("border", "left"));
    // TODO test with wrong values & different layouts

endfunction
