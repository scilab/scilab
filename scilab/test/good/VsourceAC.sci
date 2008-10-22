function [x,y,typ]=VsourceAC(job,arg1,arg2)
// Copyright INRIA
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
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
    [ok,VA,FR,exprs]=getvalue('Set voltage source parameter',..
    			   ['Amplitude (Volt)';'Frequency (Hz)'],..
			      list('vec',-1,'vec',-1),exprs)
    if ~ok then break,end
    model.rpar=[VA;FR]
    model.equations.parameters(2)=list(VA,FR)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  model.in=[1];
  model.out=[1];
  VA=220
  FR=50
  model.rpar=[VA;FR]
  model.sim='VsourceAC'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='VsourceAC'
  mo.inputs='p';
  mo.outputs='n';
  mo.parameters=list(['VA';'f'],list(VA,FR))
  model.equations=mo
  
  exprs=[string(VA);string(FR)]
  gr_i=['xarc(orig(1)+sz(1)*1/8,orig(2)+sz(2)*4.3/5,sz(1)*3/4,sz(2)*3/4,0,360*64);';
	'xsegs(orig(1)+sz(1)*[0 1/8],orig(2)+sz(2)*[1/2 1/2],0)';
	'xsegs(orig(1)+sz(1)*[7/8 1],orig(2)+sz(2)*[1/2 1/2],0)';
	'V=string(model.rpar(1));'
	'xstringb(orig(1),orig(2)+sz(2)*0.2,''~'',sz(1),sz(2)*0.3,''fill'')'
	'xstringb(orig(1),orig(2)+sz(2)*0.5,V,sz(1),sz(2)*0.3,''fill'')']
 
  x=standard_define([2 2],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']
end
endfunction
