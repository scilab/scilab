function [stk,txt,ilst,vnms,vtps,nwrk,lbl]=op2for(op,stk,vnms,vtps,nwrk,lbl)
//
//!
// Copyright INRIA
txt=[]
iop=evstr(op(2))
execstr('[stkr,nwrk,txt,top]=%'+ops(iop,2)+'2for(nwrk)')
stk(top)=stkr
s1=list()
for k=1:top,s1(k)=stk(k);end
stk=s1
endfunction
