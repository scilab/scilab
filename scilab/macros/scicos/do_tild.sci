function scs_m=do_tild(scs_m)
// Copyright INRIA
while %t
  if %pt==[] then
    [btn,xc,yc,win,Cmenu]=cosclick()
    if Cmenu<>[] then
      %pt=[]
      [Cmenu]=resume(Cmenu)
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];
      if Cmenu==emptystr() then Cmenu=[];%pt=[];end
      [%win,Cmenu]=resume(win,Cmenu)
    end
  else
    xc=%pt(1);yc=%pt(2);win=%win;%pt=[]
  end    
  k=getblock(scs_m,[xc;yc])
  if k<>[] then break,end
end
if get_connected(scs_m,k)<>[] then
  message('Connected block can''t be tilded')
  return
end
o=scs_m.objs(k)
drawobj(o)
if pixmap then xset('wshow'),end
//geom=o(2);geom(3)=~geom(3);o(2)=geom;
geom=o.graphics;geom.flip=~geom.flip;o.graphics=geom;
drawobj(o)
scs_m_save=scs_m
scs_m.objs(k)=o
[scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)
endfunction
