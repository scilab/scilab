function [x,y,typ]=Capacitor(job,arg1,arg2)
// Copyright INRIA
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
x=[];y=[];typ=[];
select job
case 'plot' then
  C=arg1.graphics.exprs(1);
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
    [ok,C,v,exprs]=getvalue('Set Capacitor block parameter',..
    			   ['C (F)','Initial Voltage'],list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    model.rpar=C
    model.equations.parameters(2)=list(C,v)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  C=0.01,v=0
  model.rpar=[C;v]
  model.sim='Capacitor'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='Capacitor'
  mo.inputs='p';
  mo.outputs='n';
  mo.parameters=list(['C','v'],list(C,v),[0,1])
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)

  exprs=string([C;v])
  gr_i=['xset(''thickness'',2)'
	'xx=orig(1)+[0 1/3 1/3 1/3 ]*sz(1);';
	'yy=orig(2)+[1/2 1/2 1 0]*sz(2);';
	'xpoly(xx,yy);'
	'xsegs(orig(1)+ sz(1)*2/3*[1 1 1 3/2],orig(2)+(sz(2)*1/2)*[2 0 1 1],0);';
        'if orient then'
	'  xrects([orig(1)+sz(1)*1/2;orig(2)+sz(2);sz(1)*1/6;sz(2)],scs_color(33));'
	'  xstring(orig(1)+sz(1)*1/12,orig(2)+sz(2)*3/4,''+'');';
	'  xstring(orig(1)+sz(1)*7/8,orig(2)+sz(2)*3/4,''-'');';
        'else'
	'  xrects([orig(1)+sz(1)*1/3;orig(2)+sz(2);sz(1)*1/6;sz(2)],scs_color(33));'
	'  xstring(orig(1)+sz(1)*1/12,orig(2)+sz(2)*3/4,''-'');';
	'  xstring(orig(1)+sz(1)*7/8,orig(2)+sz(2)*3/4,''+'');';
	'end'
	'rect=xstringl(0,0,''C=''+C)'
        'xstring(orig(1)+(sz(1)-rect(3))/2,orig(2)-rect(4)*1.2,''C= ''+C);'
	];
  x=standard_define([2 1.1],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']
end
endfunction
