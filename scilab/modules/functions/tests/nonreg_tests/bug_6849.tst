// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 6849 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6849
//
// <-- Short Description -->
// add_profiling badly handles else that may make plotprofile function
// produce an incorrect report

function t
  if ok then
    //i
  else
    //l
  end
endfunction
add_profiling('t')
ref=[0,0,0;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,0];
if or(profile(t)<>ref ) then pause,end



function t1
  if ok then
    //i
  elseif a then
    //e
  else
    //l
  end
endfunction
add_profiling('t1')
ref=[0,0,0;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,0];
if or(profile(t1)<>ref) then pause,end


function t2
  while ok 
    //w 
  else
    //e
  end      
endfunction

add_profiling('t2')
ref=[0,0,0;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,0]; 
if or(profile(t2)<>ref ) then pause,end

function t3
  select x
  case 1
    //1
  case 2
    //2
  end      
endfunction

add_profiling('t3')
ref=[0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,0];
if or(profile(t3)<>ref ) then pause,end

function t4
  select x
  case 1
    //1
  case 2
    //2
  else
    //e
  end      
endfunction

add_profiling('t4')
ref=[0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,1;0,0,0;0,0,0];
if or(profile(t4)<>ref ) then pause,end

