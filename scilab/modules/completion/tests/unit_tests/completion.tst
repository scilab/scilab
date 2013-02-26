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

// File paths

if (getos() == "Windows") then;
    r = completion('C:\Documents and');
    assert_checkequal(r, 'Documents and Settings\');
else
    r = completion('/tm');
    assert_checkequal(r, 'tmp/');
end

r = completion(SCI+'/to');
if (getos() == "Windows") then;
   assert_checkequal(r, ['tools\']);
else;
   assert_checkequal(r, ['tools/']);
end

// Predefined variables

r = completion('ho', 'variables');
assert_checkequal(r, 'home');

r = completion('%na');
assert_checkequal(r, '%nan');

r = completion('TMP');
assert_checkequal(r, 'TMPDIR');

r = completion('%', 'variables');
rexpected = ['%F'; '%T'; '%e'; '%eps'; '%f'; '%fftw'; '%gui'; '%i';'%inf'; '%io';
  '%modalWarning'; '%nan'; '%pi'; '%s'; '%t'; '%tk'; '%toolboxes'; '%toolboxes_dir'; '%z'];
assert_checkequal(r, rexpected);


// User variables

variable_completion1 = 1;
variable_completion2 = 2;
r = completion('variable_comp');
assert_checkequal(r, ['variable_completion1'; 'variable_completion2']);


// Predefined functions, macros and commands

r = completion('floo');
assert_checkequal(r, 'floor');

r = completion('abort');
assert_checkequal(r, 'abort');


// User functions, macros, commands
function [] = function_completion(x);
endfunction;
r = completion('function_comp');
assert_checkequal(r, 'function_completion');


// Filter argument

r = completion('ho');
assert_checkequal(r, ['home'; 'horizontalalignment'; 'horner'; 'host'; 'hotcolormap'; 'householder']);

r = completion('ho', 'functions');
assert_checkequal(r, 'host');

r = completion('ho', 'commands');
assert_checkequal(r, []);

r = completion('ho', 'macros');
assert_checkequal(r, ['horner'; 'hotcolormap'; 'householder']);

r = completion('ho', 'variables');
assert_checkequal(r, 'home');

r = completion('ho', 'graphic_properties');
assert_checkequal(r, 'horizontalalignment');

r = completion('ho', 'files');
assert_checkequal(r, []);

r = completion('abo');
assert_checkequal(r, ['abort'; 'about']);

r = completion('abo', 'functions');
assert_checkequal(r, 'about');

r = completion('abo', 'commands');
assert_checkequal(r, 'abort');

r = completion('abo', 'variables');
assert_checkequal(r, []);

r = completion('abo', 'macros');
assert_checkequal(r, []);

r = completion('abo', 'graphic_properties');
assert_checkequal(r, []);

r = completion('abo', 'files');
assert_checkequal(r, []);


// Output arguments

[functions,commands,variables,macros,graphic_properties,files] = completion('ho');
assert_checkequal(functions, ['host']);
assert_checkequal(commands, []);
assert_checkequal(variables, ['home']);
assert_checkequal(macros, ['horner'; 'hotcolormap'; 'householder']);
assert_checkequal(graphic_properties, ['horizontalalignment']);
assert_checkequal(files, []);

[functions,commands,variables,macros,graphic_properties,files] = completion('abo');
assert_checkequal(functions, ['about']);
assert_checkequal(commands, ['abort']);
assert_checkequal(variables, []);
assert_checkequal(macros, []);
assert_checkequal(graphic_properties, []);
assert_checkequal(files, []);

if (getos() == "Windows") then;
    [functions,commands,variables,macros,graphic_properties,files] = completion('C:\Documents and');
    assert_checkequal(files, ['Documents and Settings\']);
else
    [functions,commands,variables,macros,graphic_properties,files] = completion('/tm');
    assert_checkequal(files, ['tmp/']);
end
