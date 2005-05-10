function %xload(fil,num)
  if  argn(2)==2 then xset("window",num),end
  if get('figure_style')=='old' then set('figure_style','new'),end
  f=gcf()
  xload_mode=%t

  if execstr('load(fil)','errcatch')==0 then
    f.visible='on'
    f.immediate_drawing='on';
  else
    set('figure_style','old')
    xload(fil)
  end
//  draw(f)

endfunction
