function xs2gif(win_num,filen,color)
  xset('window',win_num)
  dr=driver()
  if dr<>"Rec" then 
    error("current driver is not ""Rec"" ")
  end
  driver('GIF')
  xinit(filen)
  if argn(2)==3 then xset('use color',color),end
  xtape('replay',win_num)
  xend()
  driver(dr)
endfunction
