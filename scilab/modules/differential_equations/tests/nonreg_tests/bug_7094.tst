// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7094 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7094
//
// <-- Short Description -->
// If badly called, ode may makes Scilab crash



function ydot = flin(t,y,a),ydot=a*y,endfunction
function j = jlin(t,y,a),j=a,endfunction
if execstr("x=ode(1,0,0:0.1:0.3,list(flin,0.9))","errcatch")<>0 then pause,end
if execstr("x=ode(1,0,0:0.1:0.3,list(1,0.9))","errcatch")<>999 then pause,end



if execstr("x=ode(1,0,0:0.1:0.3,list(flin,0.9),list(jlin,0.9))","errcatch")<>0 then pause,end

if execstr("x=ode(1,0,0:0.1:0.3,list(flin,0.9),list(1,0.9))","errcatch")<>999 then pause,end
if execstr("x=ode(1,0,0:0.1:0.3,list(1,0.9),list(jlin,0.9))","errcatch")<>999 then pause,end


function s = slin(t,y,a),s=y-3,endfunction
if execstr("x=ode(""root"",1,0,0:0.1:0.3,list(flin,0.9),1,slin)","errcatch")<>0 then pause,end
if execstr("x=ode(""root"",1,0,0:0.1:0.3,list(flin,0.9),1,list(slin,0))","errcatch")<>0 then pause,end
if execstr("x=ode(""root"",1,0,0:0.1:0.3,list(flin,0.9),1,list(1,0))","errcatch")<>999 then pause,end

