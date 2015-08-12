// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

%txt=["a=1";
"function r=foo(v)";
"    r=v;";
"endfunction";
"b=2";
"c=foo(3)";
"d=ones(1,1);";
""];

%ll=struct();
[%ll,%ierr] = script2var(%txt, %ll)

[%ll,%ierr] = script2var(%txt, %ll)

