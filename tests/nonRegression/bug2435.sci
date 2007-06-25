// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : June 2007

mode(-1) ;
clear ;
BugNumber=2435;

function [f,g,ind]=cost(x,ind)
  xref=[1;2;3];
  f=0.5*norm(x-xref)^2
  g=x-xref;
endfunction

x0=[1;-1;1];
T=execstr('[f,xopt]=optim(cost,,''b'',-2*ones(x0),2*ones(x0),x0);','errcatch')==54
    
affich_result(T,BugNumber) ;
clear ;
