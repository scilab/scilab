// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2011 - Clément DAVID
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check scicos_diagram


function assert_check_diagram(scs_m)

    // check type
    assert_checktrue(or(type(scs_m) == [17 128]));
    assert_checkequal(fieldnames(scs_m), ["props"; "objs"; "version"; "contrib"]);

    // check props
    assert_check_props(scs_m.props);

    // check objs
    assert_checkequal(scs_m.objs, list());

    // check version
    assert_checkequal(scs_m.version, "");

    // check contrib
    assert_checkequal(scs_m.contrib, list());
endfunction

function assert_check_props(props)
    // check type
    assert_checkequal(type(props), 16);
    expected = ["wpar";"title";"tol";"tf";"context";"void1";"options";"void2";"void3";"doc"];
    assert_checkequal(fieldnames(props), expected);

    // check wpar
    assert_checkequal(props.wpar, [600,450,0,0,600,450]);

    // do not check title equal, check non emptiness only
    // assert_checkequal(props.title, "Untitled");
    assert_checkfalse(isempty(props.title))

    // check tol
    assert_checkequal(props.tol(:)', [0.000001,0.000001,1.000D-10,100001,0,1,0]);

    // check tf
    assert_checkequal(props.tf, 100000);

    // check context
    assert_checkequal(props.context, []);

    // check void1
    assert_checkequal(props.void1, []);

    // check options
    assert_check_options(props.options);

    // check void2
    assert_checkequal(props.void2, []);

    // check void3
    assert_checkequal(props.void3, []);

    // check doc
    assert_checkequal(props.doc, list());

endfunction

function assert_check_options(options)
    // check type
    assert_checkequal(type(options), 16);
    expected = ["3D";"Background";"Link";"ID";"Cmap"];
    assert_checkequal(fieldnames(options), expected);

    // check 3D
    assert_checkequal(options("3D"), list(%t,33));

    // check Background
    assert_checkequal(options("Background"), [8,1]);

    // check Link
    assert_checkequal(options("Link"), [1,5]);

    // do not check ID
    // assert_checkequal(options("ID"), list([4,1,10,1],[4,1,2,1]));

    // check Cmap
    assert_checkequal(options("Cmap"), [0.8,0.8,0.8]);
endfunction

// assert on a new scicos_diagram
assert_check_diagram(scicos_diagram());

// assert on an empty xcos diagram
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/empty.zcos"));
assert_check_diagram(scs_m);
