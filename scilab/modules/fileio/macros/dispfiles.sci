function dispfiles(unit)
[lhs,rhs]=argn()
if rhs==0 then
  [units,typ,nams,mod,swap]=file() 
else
  units=[],typ=[],nams=[],mod=[],swap=[]
  for k=1:size(unit,'*')
    [uk,tk,nk,mk,sk]=file(unit(k)) 
    units=[units;uk],
    typ=[typ;tk],
    nams=[nams;nk],
    mod=[mod;mk],
    swap=[swap;sk]  
  end    
end
    
n=size(units,'*')
info=emptystr(n,1)
for k=1:n
  if units(k)==5 then nams(k)='Input',end
  if units(k)==6 then nams(k)='Output',end
  a=int(mod(k)/100)
  b=int((mod(k)-100*a)/10)
  c=int((mod(k)-100*a-10*b))
  if typ(k)=='F' then //fortran file
    typ(k)='F77'
    select c
    case 0 then
      info(k)=info(k)+'new '
    case 1 then
      info(k)=info(k)+'old '
    case 2 then
      info(k)=info(k)+'scratch '
    case 3 then
      info(k)=info(k)+'unknown '
    end
    if a==0 then 
      info(k)=info(k)+'formatted '
    else
      info(k)=info(k)+'unformatted '
    end
    if b==0 then
      //info(k)=info(k)+' '
    else
      info(k)=info(k)+'direct '
    end
  else //C file
    select a
    case 1 then
      info(k)=info(k)+'r '
    case 2 then
      info(k)=info(k)+'w '
    case 3 then
      info(k)=info(k)+'a '
    end
    if b==1 then info(k)=info(k)+'+ ',end
    if c==1 then info(k)=info(k)+'b ';end
    if swap(k)<>0 then info(k)=info(k)+'swap=on';end
  end
end 
tab=['File name' 'Unit' 'Type' 'Options';
     nams(:) string(units(:)) typ(:) info]
t=emptystr(n+1,1)+'|'
for k=1:size(tab,2)
  t=t+part(tab(:,k),1:max(length(tab(:,k))))+'|'
end
t=[t(1);'|'+part('-',ones(1,max(length(t))-2))+'|';t(2:$)]
write(%io(2),t,'(a)')
endfunction
