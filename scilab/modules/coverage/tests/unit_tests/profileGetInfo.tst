// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

//
// nominal check using a user-defined function
//

function foo()
    2
endfunction
profileDisable();
profileEnable(foo);

prof = profileGetInfo();
expectedProf = tlist(["ProfilerStatistics" ; "FunctionTable" ; "FunctionCoverage" ; "LineCoverage"], struct(), struct(), list());
expectedProf.FunctionTable.FunctionName = "foo";
expectedProf.FunctionTable.FileName = get_absolute_file_path("profileGetInfo.tst")+"profileGetInfo.tst";
expectedProf.FunctionTable.FirstLine = 34; // FirstLine with test_run headers
expectedProf.FunctionTable.LibraryName = "script";
expectedProf.FunctionTable.ParentIndex = [0];

expectedProf.FunctionCoverage.NumCalls = uint64(0);
expectedProf.FunctionCoverage.TotalTime = 0;
expectedProf.FunctionCoverage.InstructionsCount = uint64([1 1]);
expectedProf.FunctionCoverage.BranchesCount = uint64([0 0]);
expectedProf.FunctionCoverage.PathsCount = uint64(0);

expectedProf.LineCoverage = list([-1 0 ; 0 0 ; -1 0]);

assert_checkequal(prof.FunctionTable, expectedProf.FunctionTable);
assert_checkequal(prof.FunctionCoverage, expectedProf.FunctionCoverage);
assert_checkequal(prof.LineCoverage, expectedProf.LineCoverage);


//
// check with inner functions
//

function with_inner()
    2
    function inner()
        4
    endfunction
    6
endfunction
profileEnable(with_inner);

prof = profileGetInfo();
expectedProf = tlist(["ProfilerStatistics" ; "FunctionTable" ; "FunctionCoverage" ; "LineCoverage"], struct(), struct(), list());

expectedProf.FunctionTable.FunctionName = ["foo" ; "with_inner" ; "inner"];
expectedProf.FunctionTable.FileName = emptystr(3,1) + get_absolute_file_path("profileGetInfo.tst")+"profileGetInfo.tst";
expectedProf.FunctionTable.FirstLine = [34 ; 65 ; 65];
expectedProf.FunctionTable.LibraryName = ["script" ; "script" ; "script"];
expectedProf.FunctionTable.ParentIndex = [0 ; 0 ; 2];

expectedProf.FunctionCoverage.NumCalls = uint64([0 ; 0 ; 0]);
expectedProf.FunctionCoverage.TotalTime = [0 ; 0 ; 0];
expectedProf.FunctionCoverage.InstructionsCount = uint64([1 1 ; 3 3 ; 1 1]);
expectedProf.FunctionCoverage.BranchesCount = uint64([0 0 ; 0 0 ; 0 0]);
expectedProf.FunctionCoverage.PathsCount = uint64([0 ; 0 ; 0]);

expectedProf.LineCoverage = list();
expectedProf.LineCoverage(1) = [-1 0 ; 0 0 ; -1 0];
expectedProf.LineCoverage(2) = [-1 0 ; 0 0 ; -1 0 ; -1 0 ; -1 0 ; 0 0 ; -1 0];
expectedProf.LineCoverage(3) = [-1 0 ; -1 0 ; -1 0 ; 0 0 ; -1 0 ; -1 0 ; -1 0];

assert_checkequal(prof.FunctionTable, expectedProf.FunctionTable);
assert_checkequal(prof.FunctionCoverage, expectedProf.FunctionCoverage);
assert_checkequal(prof.LineCoverage, expectedProf.LineCoverage);
profileDisable();

//
// Regular function profiling
//

profileEnable(iscolumn)
iscolumn(1);
prof = profileGetInfo();
profileDisable();

// assert property list
assert_checkequal(fieldnames(prof.FunctionTable), ["FunctionName" ; "FileName" ; "FirstLine" ; "LibraryName" ; "ParentIndex"]);
assert_checkequal(fieldnames(prof.FunctionCoverage), ["NumCalls" ; "TotalTime" ; "InstructionsCount" ; "BranchesCount" ; "PathsCount"]);

// assert basic properties values, properties value that are tightly related to
// the implementation are not listed on purpose.
assert_checkequal(prof.FunctionTable.FunctionName, "iscolumn");
assert_checkequal(part(prof.FunctionTable.FileName, ($-47):$), fullfile("modules", "elementary_functions", "macros", "iscolumn.bin"));
assert_checkequal(prof.FunctionTable.LibraryName, "elementary_functionslib");
assert_checkequal(prof.FunctionCoverage.NumCalls, uint64(1));

// Display the executed "real" lines using the source file
txt = mgetl(part(prof.FunctionTable.FileName, 1:($-3)) + "sci");
assert_checkequal(size(txt(prof.FunctionTable.FirstLine:$), "r"), size(prof.LineCoverage(1), "r"));

txt = txt(prof.FunctionTable.FirstLine:$);
assert_checkequal(grep(txt(1), "/^function /", "r"), 1);
assert_checkequal(txt($), "endfunction");

// check the statistics
assert_checktrue(sum(prof.LineCoverage(1)(:,1) > 0) <= double(prof.FunctionCoverage.InstructionsCount(1)));
assert_checktrue(sum(prof.LineCoverage(1)(:,1) == 0) <= double(prof.FunctionCoverage.InstructionsCount(2)));

//
// Macro with sub-function defined
//

profileEnable(assert_checkalmostequal)
assert_checkalmostequal(1, 1);
prof = profileGetInfo();
profileDisable();

// there is one bin file containing 1 main function and 2 sub-functions
assert_checkequal(prof.FunctionTable.FileName(1), prof.FunctionTable.FileName(2));
assert_checkequal(prof.FunctionTable.FileName(1), prof.FunctionTable.FileName(3));
assert_checkequal(prof.FunctionTable.FirstLine(1), prof.FunctionTable.FirstLine(2));
assert_checkequal(prof.FunctionTable.FirstLine(1), prof.FunctionTable.FirstLine(3));
assert_checkequal(prof.FunctionTable.ParentIndex, [0 ; 1 ; 1; 1]);
assert_checkequal(prof.FunctionTable.LibraryName, ["assertlib" ; "script" ; "script" ; "script"]);
