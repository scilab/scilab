function [x,y,typ]=NMOS(job,arg1,arg2)
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
    [ok,W,L,Beta,Vt,K2,K5,dW,dL,RDS,exprs]=getvalue('Set NMOS Transistor block parameters',..
	['Width [m]';..
	 'Length [m]';..
	 'Transconductance parameter [A/(V*V)]';..
	 'Zero bias threshold voltage [V]';..
	 'Bulk threshold parameter';..
	 'Reduction of pinch-off region';..
	 'Narrowing of channel [m]';..
	 'Shortening of channel [m]';..
	 'Drain-Source-Resistance [Ohm]'],..
        list('vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1),exprs)
	 
    if ~ok then break,end
    model.equations.parameters(2)=list(W,L,Beta,Vt,K2,K5,dW,dL,RDS)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  W=20.e-6;
  L=6.e-6;
  Beta=0.041e-3;
  Vt=0.8;
  K2=1.144;
  K5= 0.7311;
  dW=-2.5e-6;
  dL= -1.5e-6;
  RDS=1.e+7;
    
  model.sim='NMOS'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='NMOS';
  mo.outputs=['D';'B';'S']
  mo.inputs='G';
  mo.parameters=list(['W';'L';'Beta';'Vt';'K2';'K5';'dW';'dL';'RDS'],[W;L;Beta;Vt;K2;K5;dW;dL;RDS])
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  exprs=[string(W);string(L);string(Beta);string(Vt);string(K2);string(K5);string(dW);string(dL);string(RDS)]
  gr_i=[' Thick=xget(''thickness'');xset(''thickness'',2);';
  'if orient then,';
        'x1=orig(1)+  [0, 15,15,15]*(sz(1)/32);';
		'x2=orig(1)+[17,17,19,19,32,19,19,17]*(sz(1)/32);';
		'x3=orig(1)+  [17,17,19,19,21,26,26,32,26,26,21,19,19,17]*(sz(1)/32);';
		'x4=orig(1)+[17,17,19,19,32,19,19,17]*(sz(1)/32);';
    'else,';
        'x1=orig(1)+  [32, 17,17,17]*(sz(1)/32);';
		'x2=orig(1)+[15,15,13,13,0,13,13,15]*(sz(1)/32);';
		'x3=orig(1)+  [15,15,13,13,11, 6, 6, 0, 6, 6,11,13,13,15]*(sz(1)/32);';
		'x4=orig(1)+[15,15,13,13,0,13,13,15]*(sz(1)/32);';
    'end;'
	'y1=orig(2)+5+[27,27,48,27]*(sz(2)/70);';
	'y2=orig(2)+5+[48,40,40,44,44,44,48,48]*(sz(2)/70);';
	'y3=orig(2)+5+[30,23,23,27,27, 31,27,27,27,23,27,27,30,30]*(sz(2)/70);';
	'y4=orig(2)+5+[16,7,7, 11,11, 11, 16,16]*(sz(2)/70);';
	'xpoly(x1,y1);';
	'xpoly(x2,y2);';
	'xpoly(x3,y3);';
	'xpoly(x4,y4);';
	'xset(''thickness'',Thick);'
	]
    
  x=standard_define([2 2],model,exprs,gr_i)
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I';'I';'I']
end
endfunction


