function [stk,txt,top]=sci_menu()
// Copyright INRIA
txt=[]
RHS=[]
for k=2:rhs
  RHS=[RHS stk(top-rhs+k)(1)]
end
dlg=stk(top-rhs+1)(1)
c=lhsargs(RHS)
stk=list('max(x_choose('+c+','+dlg+'),1)','0','1','1','1')




