function %params_p(par)
// Copyright INRIA
  write(%io(2),params2txt(par),'(a)')
endfunction
  
function txt=params2txt(par)
  fn=getfield(1,par)
  txt=[]
  for k=2:size(fn,'*')
    txt=[txt
	 sci2exp(par(fn(k)),fn(k))]
  end
endfunction

