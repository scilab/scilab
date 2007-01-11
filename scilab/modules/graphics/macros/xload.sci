function xload(fil,num)
  if  argn(2)==2 then xset("window",num),end
  f=gcf()
  res = execstr('load(fil)','errcatch') ;
  if res==0 then
    f.visible='on'
    f.immediate_drawing='on';
  else
    error('Given file is not a graphic one') ;
  end

endfunction
