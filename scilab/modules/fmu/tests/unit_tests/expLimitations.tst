//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2014 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->

// check of limitations for FMU code generation

loadXcosLibs();
path = fmigetPath();
blk = [];
oldprot = funcprot();
funcprot(0);
oldlevel = ilib_verbose();
ilib_verbose(0);

// getvalue overload 
function str = x_mdialog(desc, labels, ini)
    ini(1) = "expLimit";
    ini(2) = TMPDIR+"/expLimit/";
    str = ini;
endfunction

// msg box overloaded by the test harness

function checkSuperBlock(bool)
    for i = 1:size(scs_m.objs)
        if typeof(scs_m.objs(i)) == "Block" then
            blk = scs_m.objs(i);
            // try to generate an FMU 
            assert_checkequal(do_compile_fmu(), bool);
            if bool then
                assert_checktrue(isfile(TMPDIR+"/expLimit/expLimit.fmu"));
            end
        end
    end
endfunction

function checkSuperBlockInside(bool)
    for i = 1:size(scs_m.objs)
        for j = 1:size(scs_m.objs(i).model.rpar.objs)
            if typeof(scs_m.objs(i).model.rpar.objs(j)) == "Block" & scs_m.objs(i).model.rpar.objs(j).gui == "SUPER_f" then
                blk = scs_m.objs(i).model.rpar.objs(j);
                // try to generate an FMU 
                assert_checkequal(do_compile_fmu(), bool);
            end
        end
    end
endfunction

// check not supported blocks
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsNotSupported.zcos")));
checkSuperBlock(%f)

// check not supported blocks inside of the superblock
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsNotSupportedSuper.zcos")));
checkSuperBlockInside(%f);

// NOK check the superblock inside of another supeblock
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsSuperBlockInside.zcos")));
checkSuperBlock(%f);

// NOK check if only one sampleCLK block is on the diagram
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsOneSampleCLK.zcos")));
checkSuperBlock(%f);
checkSuperBlockInside(%f);

// check any imlicit blocks
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsImplicitBlocks.zcos")));
checkSuperBlock(%f);
checkSuperBlockInside(%f);

// mprintf overload
function mprintf(str, val)
endfunction
// OK check if only one sampleCLK block is on the diagram and only one superblock is inside of another
// FMU for ME
assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsOneSampleCLKOneSuperOK.zcos")));
checkSuperBlock(%t);
// FMU for CS
// getvalue overload 
function str = x_mdialog(desc, labels, ini)
    ini(1) = "expLimit";
    ini(2) = TMPDIR+"/expLimit/";
    str(3) = "cs";
    str = ini;
endfunction

assert_checktrue(importXcosDiagram(fullfile(path, "tests", "unit_tests", "expLimitationsOneSampleCLKOneSuperOK.zcos")));
checkSuperBlock(%t);

mdelete(TMPDIR+"/expLimit/");
funcprot(oldprot);
ilib_verbose(oldlevel);
