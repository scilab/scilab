//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"
[ma,na] = size(A);
//                  OPERATIONS
v=1:5;W=v'*v                 //constant matrix multiplication
W(1,:)                       //extracting first row
W(:,$)                       //extracting last column
Mp'*Mp+eye()                   //polynomial matrix
Mp1=Mp(1,1)+4.5*%i           //complex
Fi=C*(z*eye(ma,na)-A)^(-1)*B;       //transfer function evaluation
F(:,1)*Fi                    //operations with rationals
M=[Mp -Mp; Mp' Mp+eye()]       //concatenation of polynomial matrices
[Fi, Fi(:,1)]                // ... or rationals
F=syslin("c",F);
Num=F("num");Den=F("den");           //operation on transfer matrix

//                  SOME NUMERICAL PRIMITIVES
inv(A)                       //Inverse
inv(Mp)                      //Inverse
inv(Sl*Sl')                  //Product of two linear systems and inverse
w=ss2tf(ans)                 //Transfer function representation
w1=inv(ss2tf(Sl)*ss2tf(Sl'))    //Product of two transfer functions and inverse
clean(w-w1)
A=rand(3,3);;B=rand(3,1);n=contr(A,B)                 //Controllability
K=ppol(A,B,[-1-%i -1+%i -1])        //Pole placement
poly(A-B*K,"z")-poly([-1-%i -1+%i -1],"z")    //Check...

s=sin(0:0.1:5*%pi);
ss=fft(s(1:128),-1);        //FFT
my_handle = scf(100001);clf(my_handle,"reset");
plot2d3("enn",1,abs(ss)'); //simple plot
