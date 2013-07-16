//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

//               SCILAB OBJECTS
//               1. SCALARS
a=1               //real constant
1==1              //boolean
"string"          //character string
z=poly(0,"z")     // polynomial with variable 'z' and with one root at zero
p=1+3*z+4.5*z^2   //polynomial
r=z/p             //rational

//                2. MATRICES
A=[a+1 2 3
0 0 atan(1)
5 9 -1]      //3 x 3 constant matrix

b=[%t,%f]         //1 x 2 boolean matrix

Mc=["this","is";
"a" ,"matrix"]   //2 x 2 matrix of strings

Mp=[p,1-z;
1,z*p]        //2 x 2 polynomial matrix

F=Mp/poly([1+%i 1-%i 1],"z")   //rational matrix

Sp=sparse([1,2;4,5;3,10],[1,2,3])   //sparse matrix

Sp(1,10)==Sp(1,1)                   //boolean sparse matrix

//                 3. LISTS
L=list(a,-(1:5), Mp,["this","is";"a","list"])   //list
L(2)(3)     //sub-entry in list
Lt=tlist(["mylist","color","position","weight"],"blue",[0,1],10)  //typed-list
Lt("color")      //extracting
Lt("weight")     //extracting
A=diag([2,3,4]);B=[1 0;0 1;0 0];C=[1 -1 0];D=0*C*B;x0=[0;0;0];
Sl=syslin("c",A,B,C,D,x0)    //Standard state-space linear system
Sl("A"), Sl("C")             //Retrieving elements of a typed list
Slt=ss2tf(Sl)                // Transfer matrix
Slt("num"), Slt("den")
