function [stk,nwrk,txt,top]=%e2for(nwrk)
// genere le code fortran relatif a l'extraction d'une sous matrice
//!
// Copyright INRIA
rhs=maxi(0,abs(evstr(op(3)))-1)
sn=stk(top);top=top-1
arg=[]
s2=stk(top)
mn2=mulf(s2(4),s2(5))
if rhs==1 then
  if s2(4)=='1'&s2(5)=='1' then
    stk=list(sn(1)+'('+s2(1)+')','0',sn(3),'1','1')
  else
    [out,nwrk,txt]=outname(nwrk,sn(3),s2(4),s2(5))
    txt=[txt;
         gencall(['dmext',sn(1),mulf(sn(4),sn(5)),'1',s2(1),mn2,'0','-1',out])]
    stk=list(out,'-1',sn(3),mn2,'1')
  end
else
  s1=stk(top-1);top=top-1
  if s2(4)=='1'&s2(5)=='1'&s1(4)=='1'&s1(5)=='1' then
    stk=list(sn(1)+'('+makeargs([s1(1),s2(1)])+')','0',sn(3),'1','1')
  else
    if s1(4)=='-1' then
      nrow='-1'
      nr=sn(4)
      s1(1)='0'
    else
      nrow=mulf(s1(4),s1(5))
      nr=nrow
    end
    if s2(4)=='-1' then
      ncol='-1'
      nc=sn(5)
      s2(1)='0'
    else
      ncol=mn2
      nc=ncol
    end
 
    [out,nwrk,txt]=outname(nwrk,sn(3),mulf(s1(4),s1(5)),mn2)
    txt=[txt;gencall(['dmext',sn(1),sn(4),sn(5),s1(1),nrow,s2(1),ncol,out])]
    stk=list(out,'-1',sn(3),nr,nc)
  end
end
endfunction
