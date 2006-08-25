function [stk,nwrk,txt,top]=%i2for(nwrk,rhs,name)
//
//!
// Copyright INRIA

//rhs=abs(evstr(op(3)))-2
  
vn=find(name==vnms(:,2))
if vn==[] then  
  sto=[name '','0','0','0']
else
  typ=vtps(vn)
  sto=[vnms(vn,2), '',typ(1),typ(2),typ(3)]
end
sfrom=stk(top);top=top-1
if sto(3)=='1'|sfrom(3)=='1' then
  typ='1'
else
  typ='0'
end
top=top-rhs+1
arg=[]
 
s2=stk(top)
if rhs==1 then
  if s2(4)=='1'&s2(5)=='1' then
    txt=' '+sto(1)+'('+s2(1)+')='+sfrom(1)
    out=sto(1)
    if sto(4)=='0' then
      sto(4)=s2(1);sto(5)='1'
    elseif sto(4)=='1'&sto(5)=='1' then
      sto(4)=s2(1)
    elseif sto(4)=='1' then
      if isnum(sto(5))&isnum(s2(1)) then
        sto(5)=string(maxi(evstr(sto(5)),evstr(s2(1))))
      else
        sto(5)=callfun(['max',sto(5),s2(1)])
      end
    elseif sto(5)=='1' then
      if isnum(sto(4))&isnum(s2(1)) then
        sto(4)=string(maxi(evstr(sto(4)),evstr(s2(1))))
      else
        sto(4)=callfun(['max',sto(4),s2(1)])
      end
    else
      error(15)
    end
  else
    out=op(2);
    txt=[gencall(['dmins',sto(1),mulf(sto(4),sto(5)),'1',..
       s2(1),mulf(s2(4),s2(5)),'0','-1',sfrom(1),'1'])]
  end
else
  s1=stk(top+1)
  if s2(4)=='1'&s2(5)=='1'&s1(4)=='1'&s1(5)=='1' then
    txt=' '+sto(1)+'('+makeargs([s2(1),s1(1)])+')='+sfrom(1)
    out=sto(1)
    if isnum(sto(4))&isnum(s2(1)) then
      sto(4)=string(maxi(evstr(sto(4)),evstr(s2(1))))
    else
      sto(4)=callfun(['max',sto(4),s2(1)])
    end
    if isnum(sto(5))&isnum(s1(1)) then
      sto(5)=string(maxi(evstr(sto(5)),evstr(s1(1))))
    else
      sto(5)=callfun(['max',sto(5),s1(1)])
    end
    
  else
    out=op(2);
    txt=[gencall(['dmins',sto(1),sto(4),sto(5),..
       s2(1),mulf(s2(4),s2(5)),s1(1),mulf(s1(4),s1(5)),sfrom(1),sfrom(4)])]
  end
end

if sto(4)=='0'|sto(5)=='0' then
  WARNING('insertion : les dimensions de la matrice '+sto(1)+' sont inconnues')
  write(%io(2),'entrez le nombre de ligne:')
  sto(4)=read(%io(1),1,1,'(a)')
  write(%io(2),'entrez le nombre de colonnes:')
  sto(5)=read(%io(1),1,1,'(a)')
 
end
stk;
stk(top)=list(out,'-1',typ,sto(4),sto(5))
endfunction
