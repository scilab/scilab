// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4727 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4727
//
// <-- Short Description -->
// The function length does not return the same value in master and Scilab 4.1.2
//
tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

if length(ascii(0)) <> 1 then pause,end

for i = 1 : size(tab_ref,'*')
  str = ascii(0) + tab_ref(i) + ascii(0);
  if length(str) <> ( 1 + length(tab_ref(i)) + 1) then pause,end
end  

strs = [ascii(0) ; ascii(0)];
if length(strs) <> [1;1] then pause,end

strs = ['' ; ''];
if length(strs) <> [0;0] then pause,end
