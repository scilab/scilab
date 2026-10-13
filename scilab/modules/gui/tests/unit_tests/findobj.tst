// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// Unit tests for findobj

// Checks that found contains exactly the handles of expected, in any order
function checkHandles(found, expected)
    assert_checkequal(size(found, "*"), size(expected, "*"));
    for k = 1:size(expected, "*")
        assert_checktrue(or(found == expected(k)));
    end
endfunction

// Test scene
//   f1 (no axes)
//   |- fr (frame)
//   |  |- b1 (pushbutton "OK")
//   |  |- b2 (pushbutton "Cancel")
//   |- t1 (text "OK")
//   |- m (menu)
//   f2 (default axes with title and labels)
f1 = figure("visible", "off", "menubar", "none", "toolbar", "none", "default_axes", "off", ..
            "figure_name", "findobj_f1", "tag", "fo_fig1");
fr = uicontrol(f1, "style", "frame", "tag", "fo_frame");
b1 = uicontrol(fr, "style", "pushbutton", "string", "OK", "tag", "fo_b1");
b2 = uicontrol(fr, "style", "pushbutton", "string", "Cancel", "tag", "fo_b2");
t1 = uicontrol(f1, "style", "text", "string", "OK", "tag", "fo_t1");
m = uimenu(f1, "label", "fo_menu", "tag", "fo_menu");

f2 = figure("visible", "off", "menubar", "none", "toolbar", "none", "tag", "fo_fig2");
a2 = gca();

// --- findobj() returns all objects
hAll = findobj();
for hk = [f1 f2 fr b1 b2 t1 m a2]
    assert_checktrue(or(hAll == hk));
end

// --- "tag" shortcut, property name is case insensitive
assert_checkequal(findobj("tag", "fo_b1"), b1);
assert_checkequal(findobj("Tag", "fo_b2"), b2);
assert_checkequal(findobj("TAG", "fo_fig1"), f1);
assert_checkequal(findobj("tag", "fo_menu"), m);
assert_checkequal(findobj("tag", "fo_does_not_exist"), []);

// --- "figure_id" shortcut does not change the current figure
scf(f2);
assert_checkequal(findobj("figure_id", f1.figure_id), f1);
assert_checkequal(gcf(), f2);
assert_checkequal(findobj("figure_id", 2^30), []);

// --- property/value search from the root
checkHandles(findobj("figure_name", "findobj_f1"), f1);
assert_checkequal(findobj("figure_name", "fo_does_not_exist"), []);

// --- search restricted to handles
checkHandles(findobj(f1, "style", "pushbutton"), [b1 b2]);
checkHandles(findobj(f1, "string", "OK"), [b1 t1]);
checkHandles(findobj(fr, "string", "OK"), b1);
assert_checkequal(findobj(f2, "style", "pushbutton"), []);
// row or column vector of handles
checkHandles(findobj([f1 f2], "type", "Figure"), [f1 f2]);
checkHandles(findobj([f1; f2], "type", "Figure"), [f1 f2]);
checkHandles(findobj([b1 t1], "string", "OK"), [b1 t1]);

// --- Axes title and labels are searched
checkHandles(findobj(f2, "type", "Label"), [a2.title a2.x_label a2.y_label a2.z_label]);

// --- operators
// implicit -and between pairs
assert_checkequal(findobj(f1, "style", "pushbutton", "string", "OK"), b1);
assert_checkequal(findobj(f1, "style", "pushbutton", "-and", "string", "OK"), b1);
assert_checkequal(findobj(f1, "style", "text", "string", "Cancel"), []);
// -or
checkHandles(findobj(f1, "tag", "fo_b1", "-or", "tag", "fo_t1"), [b1 t1]);
checkHandles(findobj(f1, "string", "OK", "-or", "string", "Cancel"), [b1 b2 t1]);
// -not: only objects having the property are tested
assert_checkequal(findobj(f1, "style", "pushbutton", "-not", "string", "OK"), b2);
assert_checkequal(findobj(f1, "style", "pushbutton", "-and", "-not", "string", "OK"), b2);
checkHandles(findobj(fr, "-not", "style", "pushbutton"), fr);

// --- cells control the priority of operators
checkHandles(findobj(f1, "style", "pushbutton", {"tag", "fo_b1", "-or", "tag", "fo_b2"}), [b1 b2]);
checkHandles(findobj(f1, {"string", "OK", "-or", "string", "Cancel"}, "style", "pushbutton"), [b1 b2]);
checkHandles(findobj(f1, {"style", "pushbutton", "string", "OK"}, "-or", "tag", "fo_t1"), [b1 t1]);
checkHandles(findobj(f1, "-not", {"style", "pushbutton"}, "-flat"), []);
// cell as first argument (no search handles)
checkHandles(findobj({"tag", "fo_b1", "-or", "tag", "fo_b2"}), [b1 b2]);
checkHandles(findobj({"style", "pushbutton"}, "string", "OK"), b1);
checkHandles(findobj({"tag", "fo_fig1"}, "-flat"), []);

// --- -property
assert_checkequal(findobj(f1, "-property", "figure_name"), f1);
checkHandles(findobj([f1 f2], "-property", "figure_name"), [f1 f2]);
assert_checkequal(findobj(f1, "-property", "fo_not_a_property"), []);
checkHandles(findobj(fr, "-property", "style", "string", "OK"), b1);

// --- -depth and -flat
assert_checkequal(findobj(f1, "-depth", 0, "tag", "fo_fig1"), f1);
assert_checkequal(findobj(f1, "-depth", 0, "style", "pushbutton"), []);
assert_checkequal(findobj(f1, "-depth", 1, "style", "pushbutton"), []);
checkHandles(findobj(f1, "-depth", 1, "string", "OK"), t1);
checkHandles(findobj(f1, "-depth", 2, "style", "pushbutton"), [b1 b2]);
checkHandles(findobj(f1, "style", "pushbutton", "-depth", 2), [b1 b2]);
assert_checkequal(findobj(f1, "-flat", "tag", "fo_fig1"), f1);
assert_checkequal(findobj(f1, "-flat", "style", "pushbutton"), []);
checkHandles(findobj([b1 b2 t1], "-flat", "string", "OK"), [b1 t1]);

// --- Figures are visited in creation order, not from the most recent one
o1 = figure("visible", "off", "tag", "fo_order");
o2 = figure("visible", "off", "tag", "fo_order");
hOrder = findobj("tag", "fo_order", "-depth", 1);
assert_checkequal(size(hOrder, "*"), 2);
assert_checktrue(hOrder(1) == o1);
assert_checktrue(hOrder(2) == o2);
delete(o1);
delete(o2);

// --- a property name without value matches nothing
assert_checkequal(findobj("toto"), []);
assert_checkequal(findobj("tag"), []);
assert_checkequal(findobj(f1, "toto"), []);

// --- errors
f3 = figure("visible", "off");
delete(f3);
msg = msprintf(_("%s: Input argument #%d must be a valid handle.\n"), "findobj", 1);
assert_checkerror("findobj(f3, ""tag"", ""fo_b1"")", msg);
assert_checkerror("findobj([f1 f3], ""tag"", ""fo_b1"")", msg);

msg = msprintf(_("%s: Wrong type for argument #%d: A string expected.\n"), "findobj", 1);
assert_checkerror("findobj(1, ""tag"", ""fo_b1"")", msg);
assert_checkerror("findobj(%t)", msg);

msg = msprintf(_("%s: Wrong type for argument #%d: A string expected.\n"), "findobj", 2);
assert_checkerror("findobj(""-property"", 1)", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: A positive integer value expected.\n"), "findobj", 2);
assert_checkerror("findobj(""-depth"", -1)", msg);
assert_checkerror("findobj(""-depth"", 1.5)", msg);
assert_checkerror("findobj(""-depth"", ""1"")", msg);
assert_checkerror("findobj(""-depth"", [1 2])", msg);

msg = msprintf(_("%s: Argument #%d cannot be a binary operator."), "findobj", 1);
assert_checkerror("findobj(""-and"", ""tag"", ""fo_b1"")", msg);
assert_checkerror("findobj(""-or"", ""tag"", ""fo_b1"")", msg);
msg = msprintf(_("%s: Argument #%d cannot be a binary operator."), "findobj", 4);
assert_checkerror("findobj(""tag"", ""fo_b1"", ""-and"", ""-or"", ""tag"", ""fo_b2"")", msg);

msg = msprintf(_("%s: Wrong number of input argument(s): At least %d expected."), "findobj", 2);
assert_checkerror("findobj(""-depth"")", msg);
assert_checkerror("findobj(""-property"")", msg);
assert_checkerror("findobj(""-not"")", msg);
msg = msprintf(_("%s: Wrong number of input argument(s): At least %d expected."), "findobj", 4);
assert_checkerror("findobj(""tag"", ""fo_b1"", ""-or"")", msg);
assert_checkerror("findobj(""tag"", ""fo_b1"", ""style"")", msg);

delete(f1);
delete(f2);
