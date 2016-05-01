// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
// Copyright (C) 2015 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7537 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7537
//
// <-- Short Description -->
// block instance may contains superblocks with wrong versions. Check that
// no errors are thrown when adding them to a diagram.


scs_m = scicos_diagram();

doc = xmlRead(SCI + "/modules/xcos/etc/palettes.xml");
blocks = xmlXPath(doc, "//block/@name");
blocks = blocks.content;
xmlDelete(doc);
for blockIndex=1:size(blocks, "*")
    interfunction = blocks(blockIndex);

    execstr("out = " + interfunction + "(''define'');");

    xx = (modulo(blockIndex, 10) + 1) * 100;
    yy = int(blockIndex / 10) * 100;
    out.graphics.orig =[xx yy];

    scs_m.objs(blockIndex) = out;
end


// export the diagram to h5
h5name = TMPDIR + "/diagram.sod";
err = execstr("save(h5name, ""scs_m"")", "errcatch");
if err <> 0 then pause, end

// import to xcos (decode/encode synchronous version)
status = importXcosDiagram(h5name);
if status <> 0 then pause, end

