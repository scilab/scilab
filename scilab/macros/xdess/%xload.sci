function %xload(fil,num)
  if argn(2)<2 then
    f=gcf();
  else
    f=scf(num)
  end
  xload_mode=%t
  load(fil)
  f.visible='on'
  draw(f)

endfunction
