function SelectRegion_()
  %xc=%pt(1),%yc=%pt(2)
  Cmenu=[];%ppt=[];Select=[]
  winback=xget('window')
  xset('window',%win)
  aluback=xget('alufunction')
  xset('alufunction',6)
  disablemenus()
  [ox,oy,w,h,ok]=get_rectangle(%xc,%yc)
  xset('alufunction',aluback)
  xset('window',winback)
  if ~ok then 
    SelectRegion=list()
    return;
  else
    SelectRegion=list(ox,oy,w,h,%win)
  end
  enablemenus()
endfunction
  
