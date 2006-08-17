function [ac,Mean]=acf(x,n,minim,maxim)
//function acp(x,n,[minim,maxim])
// Autocorrelation for one-deimensional process
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs <= 1 ; n=prod(size(x))/4;end
if rhs <= 2 ; minim=-1.0;end
if rhs <= 3 ; maxim= 1.0;end
[cov,Mean]=corr(x,n+1);
ac=cov'/cov(1);
plot2d3("onn",(0:n)',ac,[1],"011"," ",[0,minim,n,maxim]);
//stde=sqrt((1+2*ac(2:n+1)'*ac(2:n+1))*1/prod(size(x)))
stde=2*sqrt(1/prod(size(x)));
plot2d( [0,0,0;n,n,n],[0,stde,-stde;0,stde,-stde],[1,2,2],"000")
xtitle('Autocorrelation Function ');
endfunction
