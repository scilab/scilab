function [stk,txt,top]=sci_tf2ss()
// Copyright INRIA
txt=[]
H=gettempvar()
N=stk(top-1)(1)
D=stk(top)(1)
txt=H+' = tf2ss(rlist(poly('+N+'($:-1:1),''x''),poly('+D+'($:-1:1),''x''),[]))'
s=H+'(2:'+string(lhs+1)+')'
A=list(s,'-1','?','?','1')
B=list(s,'-1','?','1','1')
C=list(s,'-1',stk(top-1)(3),'?','1')
D=list(s,'-1',stk(top-1)(3),'?','1')
s=list(A,B,C,D)
stk=list()
for k=1:lhs
  stk(k)=s(lhs+1-k)
end
endfunction
