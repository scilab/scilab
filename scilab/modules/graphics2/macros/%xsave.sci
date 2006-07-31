function %xsave(fil,num)
  if argn(2)<2 then
    f=gcf()
    preservecur=%f
  else
    preservecur=winsid()<>[]
    if preservecur then old=gcf();end
    f=scf(num);
  end
  save(fil,f)
  if preservecur then scf(old),end
endfunction
