function [stk,nwrk,txt,top]=%imp2for(nwrk)
//fortran code for 1:n
//
// Voir s'il est possible d'ameliorer dans le cas ou ces vecteurs implicite
// servent dans la designation d'une sous matrice
//!
// Copyright INRIA
if forexp==1 then
  s1=stk(top-1);s2=stk(top)
  if op(3)=='2' then
    stk=list([s1(1),s2(1)],'2')
    top=top-1
  else
    s0=stk(top-2)
    stk=list([s0(1),s2(1),s1(1)],'2')
    top=top-2
  end
  return
end
rhs=abs(evstr(op(3)))
if rhs==2 then
  fin=stk(top);
  deb=stk(top-1);
  pas=['1','0','0']
  top=top-1
else
  fin=stk(top);
  deb=stk(top-2);
  pas=stk(top-1);
  top=top-2
end
 
if pas(1)=='1' then
  ln=addf(subf(fin(1),deb(1)),'1')
else
  ln=addf('('+subf(fin(1),deb(1))+')/'+pas(1),'1')
end
//
if deb(3)=='1'|fin(3)=='1'|pas(3)=='1' then
  typ='1'
else
  typ='0'
end
//
  [out,nwrk,txt]=outname(nwrk,typ,'1',ln)
  if typ=='1' then
    txt=[txt;gencall(['dvimp',deb(1),fin(1),pas(1),out])]
  else
    txt=[txt;gencall(['ivimp',deb(1),fin(1),pas(1),out])]
  end
 
  stk=list(out,'-1',typ,'1',ln)
//end
endfunction
