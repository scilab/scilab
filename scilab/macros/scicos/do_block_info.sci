function %pt=do_block_info(%pt,scs_m)
// Copyright INRIA
while %t
  if %pt==[] then
    [btn,xc,yc,win,Cmenu]=cosclick()
    if Cmenu<>[] then
      %pt=[];
      [Cmenu]=resume(Cmenu)
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];
      if Cmenu==emptystr() then Cmenu=[];%pt=[];end
      
      [%win,Cmenu]=resume(win,Cmenu)
    end
  else
    xc=%pt(1);yc=%pt(2);win=%win;%pt=[]
  end
  kc=find(win==windows(:,2))
  if kc==[] then
    txt='This window is not an active palette'
    k=[];break
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getobj(palette,[xc;yc])
    if k<>[] then txt=get_block_info(palette,k),break,end
  elseif win==curwin then //click dans la fenetre courante
    k=getobj(scs_m,[xc;yc])
    if k<>[] then txt=get_block_info(scs_m,k),break,end
  end
end
x_message_modeless(txt)
endfunction
