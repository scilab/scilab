function [x,y,typ]=Ground(job,arg1,arg2)
// Copyright INRIA
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
x=[];y=[];typ=[];
select job
case 'plot' then
   standard_draw(arg1,%f,ground_draw_ports)
case 'getinputs' then
  xf=60
  yf=40
  orig=arg1.graphics.orig;sz=arg1.graphics.sz;
  x=orig(1)+(sz(1)/2)
  y=(orig(2)+yf/7+sz(2))
  typ=2
  case 'getoutputs' then
  x=[];y=[],typ=[]
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.in=[1];
  model.out=[];
  model.sim='Ground'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='Ground'
  mo.inputs='p';
  model.equations=mo
  exprs=''
  gr_i=['xset(''thickness'',3)';
      'xsegs(orig(1)+sz(1)*[1/2 1/2],orig(2)+sz(2)*[1 1/2],0);';
      'xsegs(orig(1)+sz(1)*[0 1],orig(2)+sz(2)*[1/2 1/2],0);';
      'xsegs(orig(1)+sz(1)*[2/8 6/8],orig(2)+sz(2)*[1/4 1/4],0);';
      'xsegs(orig(1)+sz(1)*[3/8 5/8],orig(2)+sz(2)*[0 0],0);']
  x=standard_define([1 1],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']
end
endfunction

function ground_draw_ports(o)
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
xset('pattern',default_color(0))
// draw input ports
//------------------------

//------------------------
// set port shape

in= 0.8*[ -1  -1
    1  -1
    1   1
    -1   1]*diag([xf/14,yf/7])  

 xfpoly(in(:,1)+ones(4,1)*(orig(1)+sz(1)/2),..
      in(:,2)+ones(4,1)*(orig(2)+sz(2)+yf/14),1)
endfunction 
