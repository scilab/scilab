function [stk,txt,top]=sci_load()
// Copyright INRIA
txt=[]
if rhs<=0 then
  stk=list('load(''scilab.save'')','0','0','0','0')
  return
end
args=[]
for k=1:rhs
  args=[args,stk(top-rhs+k)(1)]
end
stk=list('mtlb_load'+rhsargs(args),'0','0','0','0')
endfunction
