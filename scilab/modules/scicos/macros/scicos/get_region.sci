function [reg,rect]=get_region(xc,yc,win)
// Copyright INRIA
alu=xget('alufunction')
wins=curwin
xset('window',win)
xset('alufunction',6)
reg=list();rect=[]
kc=find(win==windows(:,2))
if kc==[] then
  message('This window is not an active palette')
  xset('window',wins)
  xset('alufunction',alu)
  return
elseif windows(kc,1)<0 then //click dans une palette
  kpal=-windows(kc,1)
  scs_m=palettes(kpal)
elseif win==curwin then //click dans la fenetre courante
  scs_m=scs_m
elseif pal_mode&win==lastwin then 
  scs_m=scs_m_s
elseif slevel>1 then
  execstr('scs_m=scs_m_'+string(windows(kc,1)))
else
  message('This window is not an active palette')
  xset('window',wins)
  xset('alufunction',alu)
  return
end

[ox,oy,w,h,ok]=get_rectangle(xc,yc)
if ~ok then 
  xset('window',wins)
  xset('alufunction',alu)
  return;
end

[keep,del]=get_blocks_in_rect(scs_m,ox,oy,w,h)

[reg,DEL,DELL]=do_delete1(scs_m,del,%f)
reg=do_purge(reg)
rect=[ox,oy-h,w,h]

xset('window',wins)
xset('alufunction',alu)
endfunction
