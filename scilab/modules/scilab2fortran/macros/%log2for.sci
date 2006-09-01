function [stk,nwrk,txt,top]=%log2for(nwrk)
// Copyright INRIA
txt=[]
iop=evstr(op(2))
s2=stk(top);s1=stk(top-1);top=top-1
if s1(4)=='1'&s1(5)=='1'&s2(4)=='1'&s2(5)=='1' then
  if s2(2)=='2' then s2(1)='('+s2(1)+')',end
  if s1(2)=='2' then s1(1)='('+s1(1)+')',end
  stk=list(s1(1)+ops(iop,1)+s2(1),'1','0','1','1')
else
  nwrk=dclfun(nwrk,'mcompar','0')
  if s1(1)=='[]' then s1(1)='0.0d0',end //void reference
  if s2(1)=='[]' then s2(1)='0.0d0',end //void reference
  out=callfun(['mcompar',s1(1),s1(4),s1(5),''''+ops(iop,1)+'''',s2(1),s2(4),s2(5)],'0')
  stk=list(out+'.eq.1','-1','0','1','1')
end
endfunction
