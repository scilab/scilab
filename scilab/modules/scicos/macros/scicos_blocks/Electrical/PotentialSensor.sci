function [x,y,typ]= PotentialSensor(job,arg1,arg2)
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
case 'define' then
  model=scicos_model()
  model.in=[1];
  model.out=[1];
  model.rpar=[]
  model.sim='PotentialSensor'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='PotentialSensor'
  mo.inputs='p';
  mo.outputs=['v']
  model.equations=mo
  gr_i=['xarc(orig(1)+sz(1)*1/8,orig(2)+sz(2)*4.3/5,sz(1)*3/4,sz(2)*3/4,0,360*64);';
	'xsegs(orig(1)+sz(1)*[0 1/8],orig(2)+sz(2)*[1/2 1/2],0)';
	' xsegs(orig(1)+sz(1)*[7/8 1],orig(2)+sz(2)*[1/2 1/2],0)';
	'xsegs(orig(1)+sz(1)*[1.5/8 2.5/8],orig(2)+sz(2)*[1.3/2 1.2/2],0)';
	'xsegs(orig(1)+sz(1)*[2.5/8 3.2/8],orig(2)+sz(2)*[1.62/2 1.3/2],0)';
	'xsegs(orig(1)+sz(1)*[1/2 1/2],orig(2)+sz(2)*[4.25/5 1.3/2],0)';
	'xsegs(orig(1)+sz(1)*[4.9/8 5.5/8],orig(2)+sz(2)*[1.3/2 1.65/2],0)';
	'xsegs(orig(1)+sz(1)*[5.5/8 6.5/8],orig(2)+sz(2)*[1.2/2 1.32/2],0)';
	'xsegs(orig(1)+sz(1)*[1/2 4.5/8],orig(2)+sz(2)*[1/2 1.32/2],0) ';
	'xfarc(orig(1)+sz(1)*0.93/2,orig(2)+sz(2)*1/2,sz(1)*0.2/4,sz(2)*0.2/4,0,360*64);';
	' xx=orig(1)+sz(1)*4.2/8+[.9 1 0 .9]*sz(1)/12;';
	'yy=orig(2)+sz(2)*1.27/2+[0.1 1 0.3 0.1]*sz(2)/7;';
	'xfpoly(xx,yy);']

  x=standard_define([2 2],model,'',list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['E']
end
endfunction
