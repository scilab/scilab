function do_help()
// Copyright INRIA

while %t do
  [btn,xc,yc,cwin,Cmenu]=cosclick(0)
  if Cmenu<>[] then
    name=Cmenu
    nm=1
    break
  elseif cwin==curwin then 
    k=getobj(scs_m,[xc;yc])
    if k<>[] then
      o=scs_m(k)
      name=o(5)
      nm=0
      break
    end
  elseif or(windows(find(windows(:,1)<0),2)==cwin) then
    kwin=find(windows(:,2)==cwin)
    pal=palettes(-windows(kwin,1))
    k=getobj(pal,[xc;yc])
    if k<>[] then
      o=pal(k)
      name=o(5)
      nm=0
      break
    end
  end
end

if nm==0 then
  help(name)
//  unix_s('$SCI/bin/scilab -help ""'+name+'"" | $SCI/bin/xless &')
  return
end

errcatch(-1,'continue','nomessage')
mess=%scicos_help(name)
errcatch(-1)
if iserror(-1) then
  errclear(-1)
  mess='No help available on this topic. Sorry.';
end
message(mess)  
endfunction
