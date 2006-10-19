function [x,y,typ]=NPN(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
standard_draw(arg1,%f) 

case 'getinputs' then
   [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
   [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,Bf,Br,Is,Vak,Tauf,Taur,Ccs,Cje,Cjc,Phie,..
     Me,Phic,Mc,Gbc,Gbe,Vt,EMinMax,exprs]=getvalue('Set NPN Transistor block parameters',..
	['Forward beta';..
	 'Reverse beta';..
	 'Transport saturation current';..
	 'Early voltage (inverse), 1/Volt';..
	 'Ideal forward transit time';..
	 'Ideal reverse transit time';..
	 'Collector-substrat(ground) cap.';..
	 'Base-emitter zero bias depletion cap.';..
	 'Base-coll. zero bias depletion cap.';..
	 'Base-emitter diffusion voltage';..
	 'Base-emitter gradation exponent';..
	 'Base-collector diffusion voltage';..
	 'Base-collector gradation exponent';..
	 'Base-collector conductance';..
	 'Base-emitter conductance';..
	 'Voltage equivalent of temperature';..
	 'if x> EMinMax, the exp(x) function is linearized'],..
        list('vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,..
	     'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1),exprs)
	 
    if ~ok then break,end
    model.equations.parameters(2)=list(Bf,Br,Is,Vak,Tauf,Taur,Ccs,Cje,Cjc,Phie,..
				       Me,Phic,Mc,Gbc,Gbe,Vt,EMinMax)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  Bf=50 
  Br=0.1 
  Is=1.e-16 
  Vak=0.02 
  Tauf=0.12e-9 
  Taur=5e-9 
  Ccs=1e-12 
  Cje=0.4e-12 
  Cjc=0.5e-12 
  Phie=0.8 
  Me=0.4 
  Phic=0.8 
  Mc=0.333 
  Gbc=1e-15 
  Gbe=1e-15 
  Vt=0.02585 
  EMinMax=40 

  model.sim='NPN'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='NPN';
  mo.outputs=['C';'E']
  mo.inputs='B';
  mo.parameters=list(['Bf';'Br';'Is';'Vak';'Tauf';'Taur';'Ccs';'Cje';..
		      'Cjc';'Phie';'Me';'Phic';'Mc';'Gbc';'Gbe';'Vt';..
		      'EMinMax'],[Bf;Br;Is;Vak;Tauf;Taur;Ccs;Cje;Cjc;Phie;..
				       Me;Phic;Mc;Gbc;Gbe;Vt;EMinMax])
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  exprs=[string(Bf);string(Br);string(Is);string(Vak);string(Tauf);string(Taur);string(Ccs);string(Cje);string(Cjc);string(Phie);string(Me);string(Phic);string(Mc);string(Gbc);string(Gbe);string(Vt);string(EMinMax)]
  gr_i=['if orient then';
      '  xx=orig(1)+[0,8,8,10,10,21,20,24,32,24,22,21,10,20,32,20,10,10,8,8]*(sz(1)/32);';
      '  yy=orig(2)+5+[30,30,15,15,30,17,15,15,15,15,20,17,30,43,43,43,30,43,43,30]*(sz(2)/70);';
    'else';
      '  xx=orig(1)+[32,24,24,22,22,11,12,8,0,8,10,11,22,12,0,12,22,22,24,24]*(sz(1)/32);';
      '  yy=orig(2)+5+[30,30,15,15,30,17,15,15,15,15,20,17,30,43,43,43,30,43,43,30]*(sz(2)/70);';
    'end';
    'xpoly(xx,yy);'
    'xfpoly(xx,yy)']
  x=standard_define([2 3],model,exprs,gr_i)
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I';'I']
end
endfunction
