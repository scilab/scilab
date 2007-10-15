function reg=get_inside(SelectRegion)
// Copyright INRIA
  [ox,oy,w,h,win]=SelectRegion(:)
  kc=find(win==windows(:,2))
  if kc==[] then
    message('This window is not an active palette')
    return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    scs_m=palettes(kpal)
  elseif win==curwin then //click dans la fenetre courante
    scs_m=scs_m
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  else
    message('This window is not an active palette')
    return
  end
  [keep,del]=get_blocks_in_rect(scs_m,ox,oy,w,h)
  
  [reg,DEL,DELL]=do_delete1(scs_m,del,%f)
  reg=do_purge(reg)
endfunction
