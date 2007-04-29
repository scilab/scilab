// Non-regression test file for bug 2330
// Serge Steer - Scilab Project
// Copyright INRIA
// 30 apr 2007

mode(-1);
clear;

//build the data to fit
//---------------------
function Xcalc=biexp(p,t)
  Xcalc=p(1).*exp(-p(2).*t)+p(3).*exp(-p(4).*t)+p(5);
endfunction;
t=(0:100:36000)'; 
p=[0.1;0.0001;0.2;0.0002;0.3];
X=biexp(p,t);

//try to fit the data
//-------------------
//the error function
function e=myerf(p,X,t),e=X-biexp(p,t),endfunction
// the initial point
p0=[0.01;0.001;0.01;0.001;0.1];
//call datafit
[pr,err]=datafit(list(myerf,t),X,p0);
affich_result(err<5d-6,2330);
clear

