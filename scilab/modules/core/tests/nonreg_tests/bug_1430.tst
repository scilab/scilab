// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1430 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1430
//
// <-- Short Description -->
//in errcatch(n,'continue') if an error arises in a for loop variable
//evaluation, or during the evaluation of a decision expression in an  if
//a while or select called in a compiled function, Scilab loops indefinetely.


// <-- NO TRY CATCH -->

errcatch(-1,'continue','nomessage');

function r=testselect1()
  r=0
  select zzzz
   case 1 then
    r=-5
  end;
  r=r+2
  select 1
   case 1 then
    r=r-5
  end;
endfunction

if testselect1()<>-3 then pause,end
[msg,n]=lasterror();
if n<>4 then pause,end

function r=testselect2()
  r=0
  select 1
   case zzzz then
    r=-5
  end;
  r=r+2
  select 1
   case 1 then
    r=r-5
  end;
endfunction

if testselect2()<>-3 then pause,end
[msg,n]=lasterror();
if n<>4 then pause,end

function r=testif1()
  r=0
  if zzzz then
    r=r+1
  elseif 1==3 then
    r=-5
  elseif 1==4 then
    r=-5
  else
    r=0.03
  end
  if %t then
    r=r+2
  end
endfunction

if testif1()<>2 then pause,end
[msg,n]=lasterror();
if n<>4 then pause,end


function r=testif2()
  r=0
  if %f then
    r=r+1
  elseif zzz then
    r=-5
  elseif 1==4 then
    r=-5
  else
    r=0.03
  end
  if %t then
    r=r+2
  end
endfunction

if testif1()<>2 then pause,end
[msg,n]=lasterror();
if n<>4 then pause,end

function r=testfor()
  r=0
  for k=xxxx
  r=r+1
  end	
  if %t then
    r=r+2
  end
endfunction

if testfor()<>2 then pause,end
[msg,n]=lasterror();
if n<>4 then pause,end
