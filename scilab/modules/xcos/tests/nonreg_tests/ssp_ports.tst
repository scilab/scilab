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
// <-- Non-regression test for SSP export of ports -->
//
// <-- Short Description -->
// Saving to SSP failed when a superblock had more outer input ports than inner
// IN_f blocks, or when the IN_f port numbers had gaps.
// An SSPInputConnector inside a superblock was saved as a superblock input.
// At DEBUG log level, " ScicosID <n>" was appended to the port names written
// to SSP, so a labelled port reloaded with a different label.
//

loadXcosLibs();
loadScicos();

// Superblock with one IN_f per entry of ports (port numbers, in objs order)
// summed into an OUT_f, with nouter outer input ports
function sb = io_superblock(ports, nouter)
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
    sb.model.in = -ones(nouter, 1);
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
function [scs_m2, f] = ssp_roundtrip(scs_m, name)
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
        if typeof(o) == "Block" & o.gui == "SUPER_f" then
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

// reference: as many outer ports as IN_f, numbered out of objs order
scs_m = parent_diagram(io_superblock([2 1], 2));
scs_m2 = ssp_roundtrip(scs_m, "ssp_ports_ref");
assert_checkequal(size(scs_m2.objs), size(scs_m.objs));
assert_checkequal(linked_inputs(scs_m2), [1; 2]);

// one more outer port than IN_f, linked in the parent
scs_m = parent_diagram(io_superblock(1, 2));
scs_m2 = ssp_roundtrip(scs_m, "ssp_ports_extra");
assert_checkequal(size(scs_m2.objs), size(scs_m.objs));
assert_checkequal(linked_inputs(scs_m2), [1; 2]);

// IN_f numbered 1 and 3, outer port 2 has no IN_f
scs_m = parent_diagram(io_superblock([1 3], 3));
scs_m2 = ssp_roundtrip(scs_m, "ssp_ports_gap");
assert_checkequal(size(scs_m2.objs), size(scs_m.objs));
assert_checkequal(linked_inputs(scs_m2), [1; 2; 3]);

// SSPInputConnector inside a superblock is a block, not a superblock port
sb = SUPER_f("define");
sb.model.in = [];
sb.model.rpar.objs(1) = SSPInputConnector("define");
sb.model.rpar.objs(3) = scicos_link(from=[1, 1, 0], to=[2, 1, 1]);
scs_m = parent_diagram(sb);
scs_m2 = ssp_roundtrip(scs_m, "ssp_ports_connector");
assert_checkequal(size(scs_m2.objs(1).model.in, "*"), 0);
guis = [];
for o = scs_m2.objs(1).model.rpar.objs
    if typeof(o) == "Block" then
        guis = [guis; o.gui];
    end
end
assert_checkequal(gsort(guis, "g", "i"), ["OUT_f"; "SSPInputConnector"]);

// saved at DEBUG log level, with a labelled port
scs_m = parent_diagram(SUPER_f("define"));
scs_m.objs(2).graphics.out_label = "y";
previous = scicos_log("DEBUG");
[scs_m2, f] = ssp_roundtrip(scs_m, "ssp_ports_debug");
scicos_log(previous);

// connector names are written without spaces
files = decompress(f, fullfile(TMPDIR, "ssp_ports_debug"));
doc = xmlRead(files(grep(files, ".ssd")));
xp = xmlXPath(doc, "//*[local-name()=''Connector'']/@name");
assert_checktrue(size(xp, "*") > 0);
for k = 1:size(xp, "*")
    assert_checkequal(strindex(xp(k).content, " "), []);
end
xmlDelete(doc);

// labels are preserved on reload
for k = 1:size(scs_m2.objs)
    o = scs_m2.objs(k);
    if typeof(o) == "Block" & o.gui == "CONST_m" then
        assert_checkequal(o.graphics.out_label, "y");
    end
end
