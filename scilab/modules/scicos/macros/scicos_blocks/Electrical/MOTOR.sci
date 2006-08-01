function [x,y,typ]=MOTOR(job,arg1,arg2)
// Copyright INRIA
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec deux entrees et une sortie de type implicit et de dimension 1
//   -  avec des ports disposes en des positions non standard

x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1,%f,motor_draw_ports)
case 'getinputs' then
  [x,y,typ]=motor_inputs(arg1)
//[x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=motor_outputs(arg1)
// [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.out=[1;1]
  model.in=[1];
  model.sim='motor'
  model.blocktype='c'
  model.dep_ut=[%t %f]  
  gr_i=['xrects([orig(1)+sz(1)/3;orig(2)+sz(2);sz(1)/3;sz(2)],scs_color(32))';
'xrects([orig(1)+2*sz(1)/3;orig(2)+0.87*sz(2);sz(1)/6;0.74*sz(2)],scs_color(33))';
'xrects([orig(1)+sz(1)/3-sz(1)/6;orig(2)+0.87*sz(2);sz(1)/6;0.74*sz(2)],scs_color(33))';
'xrects([orig(1)+5*sz(1)/6;orig(2)+(0.5+1/20)*sz(2);sz(1)/6;sz(2)/10],scs_color(33))';
'xrects([orig(1);orig(2)+(0.5+1/20)*sz(2);sz(1)/6;sz(2)/10],scs_color(33))'];


  exprs=''
  x=standard_define([2 2],model,exprs,gr_i)
  x.graphics.out_implicit=['I';'I']
  x.graphics.in_implicit=['I']
end
endfunction
function motor_draw_ports(o)
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)


    xset('pattern',default_color(0));xset('thickness',1)
  // draw input/output ports
  //------------------------

  if orient then  //standard orientation
    // set port shape
    out2=[ 0  -1
	   1  -1
	   1   1
	   0   1]*diag([xf/7,yf/14])
    
 
    dy=sz(2)/2
    xset('pattern',default_color(1))
    xpoly(out2(:,1)+(orig(1)+sz(1)),..
	  out2(:,2)+(orig(2)+sz(2)-dy),"lines",1)

  else //tilded orientation
      out2=[0  -1
	   -1  -1
	   -1   1
	    0   1]*diag([xf/7,yf/14])
      
      dy=sz(2)/2
      xset('pattern',default_color(1))
      xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),"lines",1)  

  end
  // motor command  ports
  //------------------------
  // set port shape



  up= [-1  1
       -1  0
        1  0
        1  1]*diag([xf/14,yf/7])

  down= [-1  0
	 -1 -1
	  1 -1
	  1  0]*diag([xf/14,yf/7])


  dx=sz(1)/2

  xpoly(up(:,1)+ones(4,1)*(orig(1)+dx),..
	 up(:,2)+ones(4,1)*(orig(2)+sz(2)),"lines",1)

  xfpoly(down(:,1)+ones(4,1)*(orig(1)+dx),..
	   down(:,2)+ones(4,1)*orig(2),1)


endfunction 

function [x,y,typ]=motor_inputs(o)
// Copyright INRIA
xf=60
yf=40
orig=o.graphics.orig
sz=o.graphics.sz

x=orig(1)+sz(1)/2
y=orig(2)-yf/7
typ=2
endfunction
function [x,y,typ]=motor_outputs(o)
// Copyright INRIA
xf=60
yf=40

[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
if orient then
  xo=orig(1)+sz(1)
  dx=xf/7
else
  xo=orig(1)
  dx=-xf/7
end
y=[orig(2)+sz(2)-sz(2)/2 orig(2)+yf/7+sz(2)]
x=[xo+dx orig(1)+sz(1)/2]
typ=[2 2]


endfunction
