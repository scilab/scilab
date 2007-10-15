function %model_p(model)
// Copyright INRIA

  sim=model.sim
  if type(sim)==15 then
    if type(sim(1))==13 then
	txt='scifunc' 
    else
      txt=sim(1)+' type: '+string(sim(2))
    end
  else 
    txt=sim+' type: 0'
  end
  fn=getfield(1,model)
  for k=3:size(fn,'*')
    txt=[txt
	 sci2exp(model(fn(k)),fn(k))]
  end
  write(%io(2),txt,'(a)')
endfunction
