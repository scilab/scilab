// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
//
// <-- Short Description -->
// White-box test for the xcosPalAdd macro.


prot = funcprot();
funcprot(0);

// stub methods (white box)
function [status, msg] = xcosPalExport(pal, path)
    status = %t;
    msg = "";
    disp("export to " + path);
endfunction

function xcosPalLoad(pal, category)
    if typeof(pal) <> "string" then pause, end
    if size(pal, "*") <> 1 then pause, end
    if typeof(category) <> "string" then pause, end
    if and([size(category, "r") > 1, size(category, "c") > 1]) then pause, end
endfunction

funcprot(prot);

// start of the test
pal = xcosPal();
pal = xcosPalAddBlock(pal, "SUM_f");
pal = xcosPalAddBlock(pal, "BIGSOM_f");

if ~xcosPalAdd(pal) then pause, end
if ~xcosPalAdd(pal, "my Summation blocks") then pause, end
if ~xcosPalAdd(pal, ["Customs" "my Summation blocks"]) then pause, end

save(TMPDIR + "/palette.sod", "pal");
if ~xcosPalAdd(TMPDIR + "/palette.sod") then pause, end

