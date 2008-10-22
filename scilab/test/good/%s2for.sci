function [stk,nwrk,txt,top]=%s2for(nwrk)
// genere le code frotran relatif a la soustraction et au changement de signe
//!
// Copyright INRIA
s2=stk(top)
if s2(2)=='2' then s2(1)='('+s2(1)+')',end

if op(3)=='2' then
  s1=stk(top-1)
  if s1(3)<>s2(3) then
    if s1(3)=='0' then 
      s1(3)='1',
    elseif s2(3)=='0' then 
      s2(3)='1'
    end
  end

  if s1(4)=='1'&s1(5)=='1'&s2(4)=='1'&s2(5)=='1' then
    if s1(3)=='1' then
      if isnum(s1(1)) then s1(1)=s1(1)+'D0', end
      if isnum(s2(1)) then s2(1)=s2(1)+'D0', end
    end
    stk=list(s1(1)+'-'+s2(1),'2',s1(3),s1(4),s1(5))
  else
    [out,nwrk,txt]=outname(nwrk,'1',s1(4),s1(5))
    txt=[txt;gencall(['ddif',mulf(s1(4),s1(5)),s1(1),'1',s2(1),'1',out,'1'])]
    stk=list(out,'-1',s1(3),s1(4),s1(5))
  end
  top=top-1
else
  if s2(4)=='1'&s2(5)=='1' then
    stk=list('-'+s2(1),s2(2),s2(3),s2(4),s2(5))
  else
    if part(s2(1),1:5)<>'work('&part(s2(1),1:6)<>'iwork(' then
      [out,nwrk,txt]=outname(nwrk,'1',s2(4),s2(5))
      txt=[txt;gencall(['dcopy',mulf(s2(4),s2(5)),s2(1),'1',out,'1'])]
    else
      out=s2(1)
    end
    txt=[txt;gencall(['dscal',mulf(s2(4),s2(5)),'-1.0d0',out,'1'])]
    stk=list(out,'-1',s2(3),s2(4),s2(5))
  end
end
endfunction
