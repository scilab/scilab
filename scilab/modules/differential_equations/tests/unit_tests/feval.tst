// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// Test 1

function z=f(x,y,t)
    z=x^2+y^2+t;
endfunction

res  = feval(1:10,1:5,list(f,10));
res1 = [];
for j=1:5
  res1=[res1, ((1:10).*(1:10))'+(j**2+10)*ones(10,1)];
end
if res<>res1 then pause,end

// Test 2

function z=g(x,y)
    z=x+%i*y;
endfunction

res  =feval(1:10,1:5,g);
res1 =[];
for j=1:5
  res1=[res1, (1:10)'+%i*j*ones(10,1)];
end
if res<>res1 then pause,end


// An other example from :
// http://numerics.tinabargs.com/2008/07/scilab-functions-as-input-for-other-scilab-functions/
// Thanks to Ma. Cristina R. Bargo for the authorization to include this test 
// into Scilab
function y = plop(x)
  y = 2*x - 1;
endfunction

function y = plip(x)
  y = x^2 - 5*x + 2;
endfunction


function y = fcninput(fcnname, x)
// fcnname is the name of the function to be evaluated at x
  y = feval(x,fcnname)
endfunction

fcninput(plip,1:10)

fcninput(plop,1:10)
