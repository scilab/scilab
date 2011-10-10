// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Blocks must have valid dimensions for their settings.
// Some dimensions were not coherents between theirs "set" and "define" method.

loadXcosLibs();

defaultlibs = ["Branching",
               "Electrical",
               "Hydraulics",
               "Linear",
               "Sources",
               "Misc",
               "Events",
               "Integerop",
               "Matrixop",
               "Nonlinear",
               "Sinks",
               "Threshold"];

defaultlibs  = defaultlibs + "lib";
notTested = ["DSUPER" "SUPER_f" "TEXT_f" "PAL_f", ..    // Specific blocks
            "CBLOCK" "CBLOCK4" "c_block" ];             // native blocks

funcprot(0);
ilib_verbose(0);
// Stubbing global scicos flags
needcompile = 0;
alreadyran = %f;
%scicos_context = struct();

// If the following block display something, the test is failed.
for i = 1:size(defaultlibs,"*")
    [macros, path] = libraryinfo(defaultlibs(i));
    macros = gsort(macros);
    for j = 1:size(macros,"*")
        interfunction = macros(j);

        // Not tested blocks (Xcos customs)
        if or(interfunction == notTested) then
          continue;
        end

        // Test scs_m = BLOCK("define") then scs_m = BLOCK("set", scs_m)
        [status, message] = xcosValidateBlockSet(interfunction);
        if status == %f
            disp(message)
        end
        assert_checktrue(status);

        // Exported hdf5 instance
        filePath = SCI + "/modules/scicos_blocks/blocks/" + interfunction + ".h5";
        if isfile(filePath) <> %t then
            mprintf("%s not found.\n", filePath);
            continue;
        end

        if import_from_hdf5(filePath) <> %t then
            mprintf("%s not loaded\n", filePath); pause, end
        // the saved data is stored as the out variable
        scs_m = out;
        cmd = "scs_m=" + interfunction + "(""set"", scs_m, []); // loaded block";
    end
end

