// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clement DAVID
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for SSP export of masked superblocks -->
//
// <-- Short Description -->
// Inside a masked superblock, links from IN_f did not refer to the outer port
// names, so the wiring was swapped or the file could not be loaded.
//

loadXcosLibs();
loadScicos();

// Masked superblock with one IN_f per entry of ports (port numbers, in objs
// order) summed into an OUT_f
function sb = io_masked_superblock(ports)
    n = size(ports, "*");
    scs = scicos_diagram();
    s = BIGSOM_f("define");
    s.model.in = -ones(n, 1);
    s.model.rpar = ones(n, 1);
    s.graphics.exprs = sci2exp(ones(n, 1));
    scs.objs(1) = s;
    for k = 1:n
        b = IN_f("define");
        b.graphics.exprs = string(ports(k));
        b.model.ipar = ports(k);
        scs.objs(k + 1) = b;
    end
    scs.objs(n + 2) = OUT_f("define");
    for k = 1:n
        scs.objs($ + 1) = scicos_link(from=[k + 1, 1, 0], to=[1, k, 1]);
    end
    scs.objs($ + 1) = scicos_link(from=[1, 1, 0], to=[n + 2, 1, 1]);

    sb = SUPER_f("define");
    sb.gui = "DSUPER";
    sb.model.sim = "csuper";
    sb.model.in = -ones(n, 1);
    sb.model.rpar = scs;
endfunction

// Diagram with a CONST_m linked to each outer input of sb
function scs_m = parent_diagram(sb)
    scs_m = scicos_diagram();
    scs_m.objs(1) = sb;
    nin = size(sb.model.in, "*");
    for k = 1:nin
        scs_m.objs(k + 1) = CONST_m("define");
    end
    for k = 1:nin
        scs_m.objs($ + 1) = scicos_link(from=[k + 1, 1, 0], to=[1, k, 1]);
    end
endfunction

// Save to SSP then reload; the output is deleted first so that a file left by a previous run is not taken as a success
function scs_m2 = ssp_roundtrip(scs_m, name)
    f = fullfile(TMPDIR, name + ".ssp");
    deletefile(f);
    scicosDiagramToScilab(f, scs_m);
    assert_checktrue(isfile(f));
    scs_m2 = scicosDiagramToScilab(f);
    assert_checkequal(typeof(scs_m2), "diagram");
endfunction

// Sorted input port numbers of the links ending on the superblock
function p = linked_inputs(scs_m)
    sb = 0;
    p = [];
    for k = 1:size(scs_m.objs)
        o = scs_m.objs(k);
        if typeof(o) == "Block" & o.gui == "DSUPER" then
            sb = k;
        end
    end
    for k = 1:size(scs_m.objs)
        o = scs_m.objs(k);
        if typeof(o) == "Link" & o.to(1) == sb then
            p = [p; o.to(2)];
        end
    end
    p = gsort(p, "g", "i");
endfunction

// [IN_f port number, BIGSOM_f input] of each link inside sb, sorted
function w = inner_wiring(sb)
    w = [];
    objs = sb.model.rpar.objs;
    for k = 1:size(objs)
        o = objs(k);
        if typeof(o) == "Link" then
            b = objs(o.from(1));
            if b.gui == "IN_f" then
                w = [w; b.model.ipar, o.to(2)];
            end
        end
    end
    w = gsort(w, "lr", "i");
endfunction

// inner links keep their outer port
scs_m = parent_diagram(io_masked_superblock([2 1]));
scs_m2 = ssp_roundtrip(scs_m, "ssp_masked_superblock");
assert_checkequal(linked_inputs(scs_m2), [1; 2]);
assert_checkequal(inner_wiring(scs_m2.objs(1)), [1 2; 2 1]);

// with labelled outer ports
scs_m = parent_diagram(io_masked_superblock([2 1]));
o = scs_m.objs(1);
o.graphics.in_label = ["u"; "v"];
scs_m.objs(1) = o;
scs_m2 = ssp_roundtrip(scs_m, "ssp_masked_superblock_labels");
assert_checkequal(linked_inputs(scs_m2), [1; 2]);
assert_checkequal(inner_wiring(scs_m2.objs(1)), [1 2; 2 1]);
assert_checkequal(scs_m2.objs(1).graphics.in_label, ["u"; "v"]);
