function [stk,txt,top]=sci_struct()
// Copyright INRIA
txt=[]
names=['struct'];vals=[]
for k=1:2:rhs-1
  names=[names;stk(top-rhs+k)(1)]
end
names='['+strcat(names,';')+']'
for k=2:2:rhs
  vals=[vals;stk(top-rhs+k)(1)]
end
stk=list('tlist('+makeargs([names;vals])+')','0',string(rhs+1),'?','16','?')
endfunction
