function %graphics_p(graphics)
// Copyright INRIA
  fn=getfield(1,graphics)
  txt=[]
  for k=2:size(fn,'*')
    txt=[txt
	 sci2exp(graphics(fn(k)),fn(k))]
  end
  write(%io(2),txt,'(a)')
endfunction
