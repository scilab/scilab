function [stk,nwrk,txt,top]=%r2for(nwrk)
// genere le code Fortran relatif a la division a droite
//!
// Copyright INRIA
txt=[]
s2=stk(top);s1=stk(top-1);top=top-1;
if s1(4)=='1'&s1(5)=='1'&s2(4)=='1'&s2(5)=='1' then
   if s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
   if s1(2)=='2' then s1(1)='('+s1(1)+')',end
   stk=list(s1(1)+'/'+s2(1),'1','1',s1(4),s1(5))
elseif s1(4)=='1'&s1(5)=='1' then
 
  [out,nwrk,txt]=outname(nwrk,s2(3),s2(5),s2(4))
  if out<>s2(1) then
    txt=gencall(['dcopy',mulf(s2(4),s2(5)),s2(1),'1',out,'1'])
  end
  [wrk,nwrk,t1]=getwrk(nwrk,'1',s2(4),'1')
  [ipvt,nwrk,t2]=getwrk(nwrk,'0',s2(4),'1')
  [errn,nwrk]=adderr(nwrk,'singular '+s2(1)+' matrix')
  txt=[txt;t1;t2;
       gencall(['dgefa',out,s2(4),s2(5),ipvt,'ierr']);
       genif('ierr.ne.0',[' ierr='+string(errn);' return']);
       gencall(['dgedi',out,s2(4),s2(5),ipvt,'w',wrk,'1'])]
  if op(2)==s1(1) then
    txt=[txt;gencall([' dcopy',mulf(s2(4),s2(5)),out,'1',s1(1),'1'])]
    out=s1(1)
  end
  stk=list(out,'-1','1',s2(4),s2(5))
elseif s2(4)=='1'&s2(5)=='1' then
  [out,nwrk,txt]=outname(nwrk,s2(3),s1(4),s1(5))
  if out<>s1(1) then
    txt=gencall(['dcopy',mulf(s1(4),s1(5)),s1(1),'1',out,'1'])
  end
  if s2(2)=='2'|s2(2)=='1' then s2(1)='('+s2(1)+')',end
  txt=[txt;gencall(['dscal',mulf(s1(4),s1(5)),'1.0d0/'+s2(1),out,'1'])]
 
  if op(2)==s2(1) then
    txt=[txt;gencall(['dcopy',mulf(s1(4),s1(5)),out,'1',s2(1),'1'])]
    out=s2(1)
  end
  stk=list(out,'-1','1',s1(4),s1(5))
else
  [res,nwrk,t1]=outname(nwrk,s2(3),s2(4),s2(5),[s1(1),s2(1)])
  [mat,nwrk,t2]=getwrk(nwrk,'1',s2(4),s2(4))
  [wrk,nwrk,t3]=getwrk(nwrk,'1',s2(4),'1')
  [ipvt,nwrk,t4]=getwrk(nwrk,'0',s2(4),'1')
  [errn,nwrk]=adderr(nwrk,'singular '+s2(1)+' matrix')
  txt=[t1;t2;t3;
       gencall(['dcopy',mulf(s2(4),s2(5)),s2(1),'1',mat,'1']);
       gencall(['dgefa',mat,s2(4),s2(5),ipvt,'ierr']);
       genif('ierr.ne.0',[' ierr='+string(errn);' return']);
       gencall(['dgesl',mat,s2(4),s2(5),ipvt,res,'0'])]
   stk=list(res,'-1',s1(3),s1(4),s1(5))
end
endfunction
