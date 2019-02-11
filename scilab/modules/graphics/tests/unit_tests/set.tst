// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// Unitary tests for set(h), h.prop = and h("prop") =
//

clf;
plot();
f = gcf();

// SETS MULTIPLE PROPERTIES
// ========================
// .. of a scalar handle
set(f, "figure_name", "set() test", "axes_size", [500 400]);
assert_checkequal(f.figure_name, "set() test");
assert_checkequal(f.axes_size, [500 400]);

// .. of a vector of handles
set(f.children, "foreground", 2, "rotation_angles", [0 90], "view","2d");
assert_checkequal(f.children.foreground, [2 2]');
assert_checkequal(f.children.view, ["2d" "2d"]');
assert_checkequal(f.children.rotation_angles, [0 270 ; 0 270]);

// SCALAR HANDLE
// =============
// Scalar property value
set(f, "rotation_style", "multiple");
assert_checkequal(f.rotation_style, "multiple");
f.rotation_style = "unary";
assert_checkequal(f.rotation_style, "unary");
f("rotation_style") = "multiple";
assert_checkequal(f.rotation_style, "multiple");

// Row property value
set(f, "axes_size", [600, 500]); sleep(200)
assert_checkequal(f.axes_size, [600 500]);
f.axes_size = [700 600]; sleep(200)
assert_checkequal(f.axes_size, [700 600]);
f.axes_size = [650 550]; sleep(200)
assert_checkequal(f.axes_size, [650 550]);

// Matrix property value
cm = jetcolormap(50);
set(f, "color_map", cm);
assert_checkequal(f.color_map, cm);
cm = jetcolormap(51);
f.color_map = cm;
assert_checkequal(f.color_map, cm);
cm = jetcolormap(52);
f.color_map = cm;
assert_checkequal(f.color_map, cm);


// ARRAY OF HANDLES
// ================
e = gce().children;  // Set of curves
// Scalar property value (curve.thickness): repeated assignment
L = list(e(1:3) , 2, [2 2 2]', ..
         e(1:3)', 3, [3 3 3]', ..
         matrix(e(1:6),2,3), 2, [2 2 2 2 2 2]');
for i = 0:(size(L)/3-1)
    h = L(1+3*i);
    v = L(2+3*i);
    vref = L(3+3*i);
    set(h, "thickness",v);
    assert_checkequal(h.thickness, vref);
    h.thickness = 2*v;
    assert_checkequal(h.thickness, 2*vref);
    h("thickness") = v;
    assert_checkequal(h.thickness, vref);
end
// Scalar property value (curve.thickness): distributive assignment
L = list(e(1:3) , [2 3 4]', [2 3 4]', ..
         e(1:3)', [1 2 3],  [1 2 3]', ..
         matrix(e(1:6),2,3), [2 4 6; 3 5 7], (2:7)', ..
         matrix(e(1:6),2,3), 1:6, (1:6)', ..
         matrix(e(1:6),2,3), (1:6)', (1:6)', ..
         );
for i = 0:(size(L)/3-1)
    h = L(1+3*i);
    v = L(2+3*i);
    vref = L(3+3*i);
    set(h, "thickness",v);
    assert_checkequal(h.thickness, vref);
    h.thickness = 2*v;
    assert_checkequal(h.thickness, 2*vref);
    h("thickness") = v;
    assert_checkequal(h.thickness, vref);
end

// Row property value (axes.grid): repeated assignments
a = gcf().children;
L = list(a , [1 2], [1 2 ; 1 2], ..
         a', [2 3], [2 3 ; 2 3], ..
        );
for i = 0:(size(L)/3-1)
    h = L(1+3*i);
    v = L(2+3*i);
    vref = L(3+3*i);
    set(h, "grid",v);
    assert_checkequal(h.grid, vref);
    h.grid = v;
    assert_checkequal(h.grid, vref);
    h("grid") = v;
    assert_checkequal(h.grid, vref);
end

// Row property value (axes.grid): row-wise assignments
L = list(a , [1 5 ; 3 5], [1 5 ; 3 5], ..
         a' , [1 5 ; 3 5], [1 5 ; 3 5], ..
        );
for i = 0:(size(L)/3-1)
    h = L(1+3*i);
    v = L(2+3*i);
    vref = L(3+3*i);
    set(h, "grid",v);
    assert_checkequal(h.grid, vref);
    h.grid = v;
    assert_checkequal(h.grid, vref);
    h("grid") = v;
    assert_checkequal(h.grid, vref);
end

// Matrix property value (axes.data_bounds): repeated rowed assignments
L = list(a , [1 1.5 0 0.5], [1 0 ; 1.5 0.5 ; 1 0 ; 1.5 0.5], ..
         a', [1 1.5 0 0.5], [1 0 ; 1.5 0.5 ; 1 0 ; 1.5 0.5], ..
        );
for i = 0:(size(L)/3-1)
    h = L(1+3*i);
    v = L(2+3*i);
    vref = L(3+3*i);
    set(h, "data_bounds",v);
    assert_checkequal(h.data_bounds, vref);
    h.data_bounds = v;
    assert_checkequal(h.data_bounds, vref);
    h("data_bounds") = v;
    assert_checkequal(h.data_bounds, vref);
end

// Matrix property value (axes.data_bounds): rowed-wise assignments
L = list(a , [1 1.5 0 0.5 ; 1.5 2.2 -0.5 1], [1 0 ; 1.5 0.5 ; 1.5 -0.5 ; 2.2 1], ..
         a', [1 1.5 0 0.5 ; 1.5 2.2 -0.5 1], [1 0 ; 1.5 0.5 ; 1.5 -0.5 ; 2.2 1], ..
        );  // beware about http://bugzilla.scilab.org/15079 for vref
for i = 0:(size(L)/3-1)
    h = L(1+3*i);
    v = L(2+3*i);
    vref = L(3+3*i);
    set(h, "data_bounds",v);
    assert_checkequal(h.data_bounds, vref);
    h.data_bounds = v;
    assert_checkequal(h.data_bounds, vref);
    h("data_bounds") = v;
    assert_checkequal(h.data_bounds, vref);
end
