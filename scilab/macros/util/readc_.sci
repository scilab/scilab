function [c]=readc_(unit)
//Syntaxes : c=readc_(unit)
//           c=readc_()
//
//readc_ reads a character string 
//This macro allows one to interrupt an exec file without pause;
//the exec file stops until carrige return is made.
//!
//
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs<=0 then unit=%io(1); end;
c=read(unit,1,1,'(a)');
endfunction
