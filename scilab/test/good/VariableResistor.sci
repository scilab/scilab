function [x,y,typ]=VariableResistor(job,arg1,arg2)
// Copyright INRIA
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
x=[];y=[];typ=[];
select job
case 'plot' then
  R=arg1.graphics.exprs;
  standard_draw(arg1,%f,VR_draw_ports)
case 'getinputs' then
  [x,y,typ]=VR_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.sim='VariableResistor'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
    mo.model='VariableResistor'
    mo.inputs=['p','R'];
    mo.outputs='n';
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  exprs=[]
  gr_i=[ 'xx=[0,1,1,7,7,8,7,7,1,1]/8;';
	 'yy=[1,1,0,0,1,1,1,2,2,1]/2;';
	 'xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2)); ']  
  x=standard_define([2 0.18],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I','E']
  x.graphics.out_implicit=['I']
end
endfunction
function VR_draw_ports(o)
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    xset('pattern',default_color(0))
    dy=sz(2)/2
  // draw input/output ports
  //------------------------
  xset('pattern',default_color(1))
  if orient then  //standard orientation
    // set port shape
    out2=[ 0  -1
	   1  -1
	   1   1
	   0   1]*diag([xf/7,yf/14])
    
    in2= [-1  -1
	   0  -1
	   0   1
	  -1   1]*diag([xf/7,yf/14])


    xpoly(out2(:,1)+(orig(1)+sz(1)),..
	  out2(:,2)+(orig(2)+sz(2)-dy),"lines",1)

    xfpoly(in2(:,1)+orig(1),..
	   in2(:,2)+(orig(2)+sz(2)-dy),1)	
  else //tilded orientation
      out2=[0  -1
	   -1  -1
	   -1   1
	    0   1]*diag([xf/7,yf/14])
      
      in2= [1  -1
	    0  -1
	    0   1
	    1   1]*diag([xf/7,yf/14])

      
      xset('pattern',default_color(1))
      xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),"lines",1)  

      xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	     in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1) 
  end

  // set port shape

  in= [-1/14  1/7
       0      0
       1/14   1/7
       -1/14  1/7]*diag([xf,yf])


  dx=sz(1)/2
    xfpoly(in(:,1)+ones(4,1)*(orig(1)+dx),..
	   in(:,2)+ones(4,1)*(orig(2)+sz(2)),1)


endfunction 

function [x,y,typ]=VR_inputs(o)
  xf=60;yf=40
  graphics=o.graphics
  orig=graphics.orig;sz=graphics.sz;
  if graphics.flip then
    xo=orig(1);dx=-xf/7
  else
    xo=orig(1)+sz(1);dx=yf/7
  end
  y=orig(2)+sz(2)/2
  x=(xo+dx)*ones(y)
  
  x=[x,orig(1)+(sz(1)/2)]
  y=[y,orig(2)+yf/7+sz(2)]
  typ=[2 1]
endfunction
