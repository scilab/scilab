function [stk,txt,top]=sci_cell()
// Copyright INRIA
txt=[]
args=[]
for k=1:rhs
  args=[args,stk(top-rhs+k)(1)]
end
stk=list('mtlb_cell('+makeargs(args)+')','0','?','?','16','?')
endfunction
