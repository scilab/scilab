// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//===============================
// unit tests completion
//===============================

function [] = check_completion(actual, expected)
    for i = 1 : size(actual, "r")
        [start,end,match] = regexp(actual(i,1), "/(?i)^(" + expected + ")/");
        assert_checktrue(match <> "");
    end
endfunction

// File paths

if (getos() == "Windows") then
    r = completion("C:\Documents and");
    assert_checkequal(r, "Documents and Settings\");

    r = completion("c:\documents And");
    assert_checkequal(r, "Documents and Settings\");
else
    r = completion("/tm");
    assert_checkequal(r, "tmp/");

    r = completion("/TM");
    // We are still case sensitive
    assert_checkequal(r, []);
end

r = completion(SCI+"/to");
r2 = completion(SCI+"/TO");
if (getos() == "Windows") then
    assert_checkequal(r, ["tools\"]);
    assert_checkequal(r2, ["tools\"]);
else
    assert_checkequal(r, ["tools/"]);
    assert_checkequal(r2, []);
end


// Predefined variables

r = completion("ho", "variables");
check_completion(r, "home");

r = completion("HO", "variables");
check_completion(r, "home");

r = completion("%na");
check_completion(r, "%nan");

r = completion("%Na");
check_completion(r, "%nan");

r = completion("tmp");
check_completion(r, "TMPDIR");

r = completion("Tmp");
check_completion(r, "TMPDIR");

r = completion("%", "variables");
rexpected = ["%e"; "%eps"; "%fftw"; "%gui"; "%i";"%inf"; "%io"; "%nan"; "%pi"; "%s"; "%tk"; "%z"];
assert_checkequal(r, rexpected);


// User variables

variable1_completion1 = 1;
variable1_completion2 = 2;
r = completion("variable1_comp");
assert_checkequal(r, ["variable1_completion1"; "variable1_completion2"]);
r = completion("VARIABLE1_COMP");
assert_checkequal(r, ["variable1_completion1"; "variable1_completion2"]);

variable2_completion = 1;
VARIABLE2_COMPLETION = 2;
r = completion("variable2_comp");
assert_checkequal(r, ["VARIABLE2_COMPLETION"; "variable2_completion"]);
r = completion("VARIABLE2_COMP");
assert_checkequal(r, ["VARIABLE2_COMPLETION"; "variable2_completion"]);


// Predefined functions, macros and commands

r = completion("floo");
check_completion(r, "floor");

r = completion("FLOO");
check_completion(r, "floor");

r = completion("abort");
check_completion(r, "abort");

r = completion("Abort");
check_completion(r, "abort");


// User functions, macros, commands

function [] = function_completion(x);
endfunction;
r = completion("function_comp");
assert_checkequal(r, "function_completion");
r = completion("FUNCTION_COMP");
assert_checkequal(r, "function_completion");

function [] = FUNCTION_COMPLETION(x);
endfunction;
r = completion("function_comp");
assert_checkequal(r, ["FUNCTION_COMPLETION"; "function_completion"]);
r = completion("FUNCTION_COMP");
assert_checkequal(r, ["FUNCTION_COMPLETION"; "function_completion"]);


// Filter argument

r = completion("ho");
assert_checkequal(r, ["home"; "horizontalalignment"; "horner"; "host"; "hotcolormap"; "householder"]);

r = completion("ho", "functions");
assert_checkequal(r, "host");

r = completion("ho", "commands");
assert_checkequal(r, []);

r = completion("ho", "macros");
assert_checkequal(r, ["horner"; "hotcolormap"; "householder"]);

r = completion("ho", "variables");
assert_checkequal(r, "home");

r = completion("ho", "graphic_properties");
assert_checkequal(r, "horizontalalignment");

r = completion("ho", "files");
assert_checkequal(r, []);

r = completion("abo");
assert_checkequal(r, ["abort"; "about"]);

r = completion("abo", "functions");
assert_checkequal(r, ["abort"; "about"]);

r = completion("abo", "commands");
assert_checkequal(r, "abort");

r = completion("abo", "variables");
assert_checkequal(r, []);

r = completion("abo", "macros");
assert_checkequal(r, []);

r = completion("abo", "graphic_properties");
assert_checkequal(r, []);

r = completion("abo", "files");
assert_checkequal(r, []);


// Output arguments

[functions,commands,variables,macros,graphic_properties,files] = completion("ho");
assert_checkequal(functions, ["host"]);
assert_checkequal(commands, []);
assert_checkequal(variables, ["home"]);
assert_checkequal(macros, ["horner"; "hotcolormap"; "householder"]);
assert_checkequal(graphic_properties, ["horizontalalignment"]);
assert_checkequal(files, []);

[functions,commands,variables,macros,graphic_properties,files] = completion("abo");
assert_checkequal(functions, ["abort"; "about"]);
assert_checkequal(commands, ["abort"]);
assert_checkequal(variables, []);
assert_checkequal(macros, []);
assert_checkequal(graphic_properties, []);
assert_checkequal(files, []);

if (getos() == "Windows") then;
    [functions,commands,variables,macros,graphic_properties,files] = completion("C:\Documents and");
    assert_checkequal(files, ["Documents and Settings\"]);
else
    [functions,commands,variables,macros,graphic_properties,files] = completion("/tm");
    assert_checkequal(files, ["tmp/"]);
end

assert_checkerror("completion(2)",[], 999);

assert_checkerror("completion([''aze'',''bze''])",[], 999);
assert_checkerror("completion(''a'',''foo'')",[], 999);
assert_checkerror("completion(''a'',2)",[], 999);
assert_checkerror("[aazeaz,azeza,e,aze,azea,zazeaze,azeaze,azeze,aze,azeaz]=completion(""aaze"")",[],78);
