// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH XCOS -->
//
// <-- Unitary test for the xcosBlockEval macro -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7129
//
// <-- Short Description -->
// The xcosBlockEval macro is used on Xcos to update SuperBlocks I/O blocks 
// from the context data.

loadXcosLibs();
funcprot(0); // avoid function assignation message

ioBlocks = ["IN_f", "OUT_f", "INIMPL_f", "OUTIMPL_f", "CLKIN_f", "CLKOUT_f", ..
            "CLKINV_f", "CLKOUTV_f"];

for index = 1:size(ioBlocks, '*')
    cmd = "scs_m = " + ioBlocks(index) + "(""define"");";
    if (execstr(cmd, "errcatch") <> 0) then pause, end;
    
    fileToLoad = TMPDIR + "/in.h5";
    fileToSave = TMPDIR + "/out.h5";
    fileContext = TMPDIR + "/context.h5";
    
    // context and block settings
    context = ["myVariable = 42;"];
    scs_m.graphics.exprs = "myVariable";
    
    // call xcosBlockEval
    export_to_hdf5(fileToLoad, "scs_m");
    export_to_hdf5(fileContext, "context");
    xcosBlockEval(fileToLoad, fileToSave, evstr(ioBlocks(index)), fileContext);
    import_from_hdf5(fileToSave);
    
    // check settings
    if (new_scs_m.graphics.exprs <> "myVariable") then pause, end;
    if (new_scs_m.model.ipar <> 42) then pause, end;
end

