function newpixmap=do_Displaymode(pixmap)
// Copyright INRIA
  [ok,newpixmap]=getvalue('Set Display mode (Backing store)',[
      'mode (0 or 1)'],list('vec',1),string(%scicos_display_mode))
  if ~ok then 
    newpixmap=pixmap
  else
    if newpixmap<>0 then
      newpixmap=1
    end
  end
  xset('pixmap',newpixmap)
endfunction
