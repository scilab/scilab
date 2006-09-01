function [s,nwrk,txt]=typconv(s,nwrk,typ)
//[s1,te1,t1,m1,n1]=s(1:5);
//!
// Copyright INRIA
txt=[]
if typ=='1'&s(3)=='0' then
  if s(4)=='1'&s(5)=='1' then
    if isnum(s(1)) then s(1)=s(1)+'D0',else s(1)='dble('+s(1)+')',end
  else
    if part(s(1),1:6)=='iwork(' then
      pti=part(s(1),7:length(s(1))-1)
      in='iwork'
    else
      pti='0'
      in=s(1)
    end
    [outn,nwrk,txt]=getwrk(nwrk,'1',s(4),s(5))
    out='work'
    pto1=part(outn,6:length(outn)-1),
    [lbl,nwrk]=newlab(nwrk)
    tl1=string(10*lbl);
    var='ilb'+tl1;
    t1=' '+out+'('+addf(pto1,var)+')=dble('+in+'('+addf(pti,var)+'))'
    txt=[txt;' do '+tl1+' '+var+' = 0,'+subf(mulf(s(4),s(5)),'1');
           indentfor(t1);part(tl1+'    ',1:6)+' continue']

  end
elseif typ=='0'&s(3)=='1' then
  if s(4)=='1'&s(5)=='1' then
    s(1)='int('+s(1)+')'
  else
    if part(s(1),1:5)=='work(' then
      pti=part(s(1),6:length(s(1))-1)
      in='work'
    else
      pti='0'
      in=s(1)
    end
    [outn,nwrk,txt]=getwrk(nwrk,'0',s(4),s(5))
    out='iwork'
    pto1=part(outn,7:length(outn)-1),
    [lbl,nwrk]=newlab(nwrk)
    tl1=string(10*lbl);
    var='ilb'+tl1;
    t1=' '+out+'('+addf(pto1,var)+')=int('+in+'('+addf(pti,var)+'))'
    txt=[txt;' do '+tl1+' '+var+' = 0,'+subf(mulf(s(4),s(5)),'1');
           indentfor(t1);part(tl1+'    ',1:6)+' continue']

  end
end
endfunction
