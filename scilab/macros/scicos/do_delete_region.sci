function [scs_m,needcompile]=do_delete_region(scs_m,needcompile)
// Copyright INRIA
xinfo('Click, drag to select region and click to fix the selection')

[btn,xc,yc,win,Cmenu]=cosclick()
if Cmenu<>[] then
  Cmenu=resume(Cmenu)
end
disablemenus()
[ox,oy,w,h,ok]=get_rectangle(xc,yc)
if ~ok then  enablemenus();return;end
[del,keep]=get_blocks_in_rect(scs_m,ox,oy,w,h)

modified=del<>[]
if modified then
  needreplay=replayifnecessary()
  scs_m_save=scs_m,nc_save=needcompile
  [scs_m,DEL,DELL]=do_delete1(scs_m,del,%t)
  needcompile=4
  enablemenus()
  [scs_m_save,nc_save,enable_undo,edited,needreplay]=resume(scs_m_save,nc_save,%t,%t,needreplay)
end
enablemenus()
endfunction
