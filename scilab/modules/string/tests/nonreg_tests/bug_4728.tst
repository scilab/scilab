// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 4728 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4728
//
// <-- Short Description -->
// Different result between Scilab 4.1.2 and Scilab master for strcat function
//
tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

if (strcat([ascii(0),"t"]) == "t" + ascii(0)) <> %f then pause,end

if (strcat([ascii(0),"t",ascii(0)]) <> ascii(0) + "t" + ascii(0)) <> %f then pause,end

A = strcat([ascii(0),"t"]);
B = "t"+ascii(0);
C = ascii(0) + "t";

if A <> C then pause,end

sA = length(A);
if sA <> 2 then pause,end

sB = length(B);
if sB <> 2 then pause,end

sC = length(C);
if sC <> 2 then pause,end

for i = 1 : size(tab_ref,'*')
  r1 = strcat([tab_ref(i),ascii(0),tab_ref(i)]);
  r2 = tab_ref(i) + ascii(0) + tab_ref(i);
  if length(r1) <> ( 2 * length(tab_ref(i)) + 1) then pause,end
  if r1 <> r2 then pause, end 
end  
