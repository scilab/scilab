function [stk,top]=num2sci(val,stk)
// traduit la definition d'un nombre
//!
// Copyright INRIA
top=top+1
stk(top)=list(val,'0','1','1','1')

