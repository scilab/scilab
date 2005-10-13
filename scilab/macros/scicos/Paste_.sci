function Paste_()
  if %win==curwin & ~isequal(Clipboard,[]) & %ppt<>[] then
    scs_m_save=scs_m,nc_save=needcompile
    blk=Clipboard
    blk.graphics.orig=%ppt
    scs_m.objs($+1)=blk
    drawobj(blk)
    edited=%t
    enable_undo=%t
    Select=[size(scs_m.objs),%win]
  end
  Cmenu=[];%pt=[];%ppt=[]
endfunction
