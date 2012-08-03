// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

X=[0;1;0;0];
Y=[0;0;1;0];
Z=[0;0;0;1];
[RESULT,ERROR]=int3d(X,Y,Z,'int3dex');
// computes the integrand exp(x*x+y*y+z*z) over the 
//tetrahedron (0.,0.,0.),(1.,0.,0.),(0.,1.,0.),(0.,0.,1.)


//integration over a cube  -1<=x<=1;-1<=y<=1;-1<=z<=1

//  bottom  -top-     right    -left-   front   -rear- 
X=[ 0, 0,    0, 0,    0, 0,    0, 0,    0, 0,    0, 0;          
   -1,-1,   -1,-1,    1, 1,   -1,-1,   -1,-1,   -1,-1; 
    1,-1,    1,-1,    1, 1,   -1,-1,    1,-1,    1,-1;     
    1, 1,    1, 1,    1, 1,   -1,-1,    1, 1,    1, 1];         
Y=[ 0, 0,    0, 0,    0, 0,    0, 0,    0, 0,    0, 0; 
   -1,-1,   -1,-1,   -1, 1,   -1, 1,   -1,-1,    1, 1;
   -1, 1,   -1, 1,    1, 1,    1, 1,   -1,-1,    1, 1;   
    1, 1,    1, 1,   -1,-1,   -1,-1,   -1,-1,    1, 1]; 
Z=[ 0, 0,    0, 0,    0, 0,    0, 0,    0, 0,    0, 0;
   -1,-1,    1, 1,   -1, 1,   -1, 1,   -1,-1,   -1,-1; 
   -1,-1,    1, 1,   -1,-1,   -1,-1,   -1, 1,   -1, 1;  
   -1,-1,    1, 1,    1, 1,    1, 1,    1, 1,    1, 1];      

function v=f1(xyz,numfun),v=exp(xyz'*xyz),endfunction
[result,err]=int3d(X,Y,Z,f1,1,[0,100000,1.d-5,1.d-7]);

if result > 26 then pause,end
if err <%eps then pause,end

function v=f2(xyz,numfun),v=1,endfunction
[result,err]=int3d(X,Y,Z,f2,1,[0,100000,1.d-5,1.d-7]);

if result <> 8 then pause,end
if err <%eps then pause,end
