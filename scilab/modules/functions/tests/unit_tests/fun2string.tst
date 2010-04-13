// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

funcprot(0);

function y=foo(a)
  y=a+1
endfunction
ref=["function y=ans(a)";
     "  y = a + 1";
     "endfunction"];
if or(fun2string(foo)<>ref) then pause,end

ref=["function y=foo(a)";
     "  y = a + 1";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end

function y=foo()
  y=1
endfunction
ref=["function y=foo()";
     "  y = 1";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end


function foo()
  y=1
endfunction
ref=["function []=foo()";
     "  y = 1";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end

function foo()
  y=1,
endfunction
ref=["function []=foo()";
     "  y = 1,";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end

function foo()
  y=1;
endfunction
ref=["function []=foo()";
     "  y = 1;";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end


function foo(),y=1
endfunction
ref=["function []=foo(),y = 1";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end


function foo(),y=1;endfunction
ref=["function []=foo(),y = 1;endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end

function foo(),endfunction
ref=["function []=foo(),endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end

function foo()
if %t then a=1,end
if b==3 then
  a=5
else
  c=8
end
endfunction
ref = ["function []=foo()";
       "  if %t then   a = 1,end,";
       "  if b == 3 then ";
       "    a = 5";
       "  else ";
       "    c = 8";
       "  end,";
       "endfunction"]
if or(fun2string(foo,'foo')<>ref) then pause,end



// with deff
// ---------
deff('foo','y=1')
ref=["function []=foo()";
     "  y = 1";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end
deff('foo',['y=1';'z=3;'])
ref=["function []=foo()";
     "  y = 1";
     "  z = 3;";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end

// with getf
// ------------
ref=["function []=foo()";
     "  y = 1";
     "  z = 3;";
     "endfunction"];
mputl(ref,TMPDIR+'/foo.sci');
warning('off');
getf(TMPDIR+'/foo.sci');
warning('on');
if or(fun2string(foo,'foo')<>ref) then pause,end

// with nested functions
// ---------------------
function y=foo(x)
  function y=foo1(x),
  y=x+1;
  endfunction
endfunction
ref= ["function y=foo(x)";
      "  function y=foo1(x)";
      "    y=x+1;";
      "  endfunction";
      "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end



function foo()
  function foo3,a=1,
  endfunction,
endfunction	
ref=["function []=foo()";
     "  function foo3,a=1,";
     "  endfunction,";
     "endfunction"]
if or(fun2string(foo,'foo')<>ref) then pause,end


function foo()
  function foo3,a=1,endfunction
endfunction	
ref=["function []=foo()";
     "  function foo3,a=1,endfunction";
     "endfunction"]; 
if or(fun2string(foo,'foo')<>ref) then pause,end


// with comments
// -------------
function a=foo//
    if a> 10 then //
    end 
endfunction
ref= ["function a=foo(),//";
      "  if a > 10 then   //";
      "  end,";
      "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end


function foo()//
  function foo3()//
  endfunction
endfunction	
ref=["function []=foo(),//";
     "  function foo3(),//";
     "  endfunction";
     "endfunction"];
if or(fun2string(foo,'foo')<>ref) then pause,end
