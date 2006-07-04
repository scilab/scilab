function %pt=do_block_info(%pt,scs_m)
// Copyright INRIA
  if Select==[] then
    win=%win;
    xc=%pt(1);yc=%pt(2);%pt=[]
    kc=find(win==windows(:,2))
    if kc==[] then
      txt='This window is not an active palette'
      k=[];
      return
    elseif windows(kc,1)<0 then //click dans une palette
      kpal=-windows(kc,1)
      palette=palettes(kpal)
      k=getobj(palette,[xc;yc])
      if k<>[] then txt=get_block_info(palette,k),end
    elseif win==curwin then //click dans la fenetre courante
      k=getobj(scs_m,[xc;yc])
      if k<>[] then txt=get_block_info(scs_m,k),end
    end
    x_message_modeless(txt)
  else
    if size(Select,'1')>1 then
      message("Only one block can be selected for this operation.")
      Cmenu=[];%pt=[];return
    end
    win=Select(1,2);
    kc=find(win==windows(:,2))
    k=Select(1,1)
    if kc==[] then
      txt='This window is not an active palette'
      k=[];
      return
    elseif windows(kc,1)<0 then //click dans une palette
      kpal=-windows(kc,1)
      palette=palettes(kpal)
      txt=get_block_info(palette,k)
    elseif win==curwin then //click dans la fenetre courante
      txt=get_block_info(scs_m,k),
    end
    x_message_modeless(txt)
    
    
    
    
  end
  
endfunction
