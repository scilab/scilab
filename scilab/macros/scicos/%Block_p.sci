function %Block_p(block)
// Copyright INRIA
  txt=['GUI     : '+block.gui 
       'Graphics: '
       '          '+graphics2txt(block.graphics)
       'Model   : '
       '          '+model2txt(block.model)]
  write(%io(2),txt,'(a)')
endfunction

function txt=graphics2txt(graphics)
  fn=getfield(1,graphics)
  txt=[]
  for k=2:size(fn,'*')
    txt=[txt
	 sci2exp(graphics(fn(k)),fn(k))]
  end
endfunction

function txt=model2txt(model)
  sim=model.sim
  if type(sim)==15 then
    txt=sim(1)+' type: '+string(sim(2))
  else
    txt=sim+' type: 0'
  end
  fn=getfield(1,model)
  for k=3:size(fn,'*')
    if fn(k)=='rpar' & type(model(fn(k)))==15 then
      txt=[txt;fn(k)+' : SuperBlock'];
    else
      txt=[txt
	   sci2exp(model(fn(k)),fn(k))];
    end
  end
endfunction
