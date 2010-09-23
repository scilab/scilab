// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
// <-- LONG TIME EXECUTION -->
//
// <-- Non-regression test for bug 7537 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7537
//
// <-- Short Description -->
// HDF5 block instance may contains superblocks with wrong versions. Check that
// no errors are thrown when adding them to a diagram.

loadScicosLibs();

scs_m = scicos_diagram();

blocks = ls(SCI + "/modules/scicos_blocks/blocks/*.h5");
for blockIndex=1:size(blocks, '*')
    blockFile = blocks(blockIndex);
    
    ok = import_from_hdf5(blockFile);
    if ~ok then pause, end
    
    xx = (modulo(blockIndex, 10) + 1) * 100;
    yy = int(blockIndex / 10) * 100;
    out.graphics.orig =[xx yy];
    
    scs_m.objs(blockIndex) = out;
end

// export the diagram to h5
h5name = TMPDIR + "/diagram.h5";
status = export_to_hdf5(h5name, "scs_m");
if ~status then pause, end

// import to xcos (synchronous version)
status = xcosDiagramToHDF5(h5name, TMPDIR + "/duplicate.h5", %t);
if status <> 0 then pause, end

