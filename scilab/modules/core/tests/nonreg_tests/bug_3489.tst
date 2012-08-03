// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2009 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3489 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3489
//
// <-- Short Description -->
//catching protected variable redefinition

prot=funcprot();funcprot(0);
if execstr('%pi=3','errcatch')==0 then pause,end

r=%f;
try,execstr('%pi=3'),catch,r=%t,end
if ~r then pause,end


r=%f;
try,%pi=3,catch,r=%t,end
if ~r then pause,end



r=%f;
function setpi(pi), %pi = resume(pi),endfunction
try,setpi(3),catch,r=%t,end
if ~r then pause,end

r=%f;clear a
function a=setpi(pi), a=228;%pi = resume(pi),endfunction
try,setpi(3),catch,r=%t,end
if ~r then pause,end
if exists('a')<>0 then pause,end



function setpi(pi), execstr('%pi = resume(pi)'),endfunction
r=%f;
try,setpi(3),catch,r=%t,end
if ~r then pause,end

clear a
function a=setpi(pi),a=123; execstr('%pi = resume(pi)'),endfunction
r=%f;
try,setpi(3),catch,r=%t,end
if ~r then pause,end
if exists('a')<>0 then pause,end


funcprot(prot);
