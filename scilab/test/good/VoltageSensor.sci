function [x,y,typ]=VoltageSensor(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1,%f,sensor_draw_ports) 
case 'getinputs' then
 [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=sensor_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.in=1;model.out=[1; 1];
  model.sim='VoltageSensor'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='VoltageSensor'
  mo.inputs='p';
  mo.outputs=['n';'v']
  model.equations=mo
  exprs=[]
  gr_i=['xarc(orig(1)+sz(1)*1/8,orig(2)+sz(2)*4.3/5,sz(1)*3/4,sz(2)*3/4,0,360*64);';
	'xsegs(orig(1)+sz(1)*[0 1/8],orig(2)+sz(2)*[1/2 1/2],0)';
	'xsegs(orig(1)+sz(1)*[7/8 1],orig(2)+sz(2)*[1/2 1/2],0)';
	'xsegs(orig(1)+sz(1)*[1.5/8 2.5/8],orig(2)+sz(2)*[1.3/2 1.2/2],0)';
	'xsegs(orig(1)+sz(1)*[2.5/8 3.2/8],orig(2)+sz(2)*[1.62/2 1.3/2],0)';
	'xsegs(orig(1)+sz(1)*[1/2 1/2],orig(2)+sz(2)*[4.25/5 1.3/2],0)';
	'xsegs(orig(1)+sz(1)*[4.9/8 5.5/8],orig(2)+sz(2)*[1.3/2 1.65/2],0)';
	'xsegs(orig(1)+sz(1)*[5.5/8 6.5/8],orig(2)+sz(2)*[1.2/2 1.32/2],0)';
	'xsegs(orig(1)+sz(1)*[1/2 4.5/8],orig(2)+sz(2)*[1/2 1.32/2],0) ';
	'xsegs(orig(1)+sz(1)*[1/2 1/2],orig(2)+sz(2)*[0.9/8 -1/8],0)';
	'xfarc(orig(1)+sz(1)*0.93/2,orig(2)+sz(2)*1/2,sz(1)*0.2/4,sz(2)*0.2/4,0,360*64);';
	' xx=orig(1)+sz(1)*4.2/8+[.9 1 0 .9]*sz(1)/12;';
	'yy=orig(2)+sz(2)*1.27/2+[0.1 1 0.3 0.1]*sz(2)/7;';
	'xfpoly(xx,yy);'
	'xstring(orig(1)+sz(1)/2,orig(2)+sz(2)/5,''V'')']
  x=standard_define([2 2],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I';'E']
end
endfunction

function sensor_draw_ports(o)
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
    xset('pattern',default_color(0))
  // draw input/output ports
  //------------------------

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
    dy=sz(2)/2
    xset('pattern',default_color(1))
    xpoly(out2(:,1)+(orig(1)+sz(1)),..
	  out2(:,2)+(orig(2)+sz(2)-dy),"lines",1)


    dy=sz(2)/2
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

      
      dy=sz(2)/2
      xset('pattern',default_color(1))
      xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),"lines",1)  
      dy=sz(2)/2
      xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	     in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1) 
  end
  // valve command port port
  //------------------------
  // set port shape



  out= [-1  1
        0  0
        1  1
       -1  1]*diag([xf/14,yf/7])



  dx=sz(1)/2

  xpoly(out(:,1)+ones(4,1)*(orig(1)+dx),..
	 out(:,2)+ones(4,1)*(orig(2)-sz(2)*2/8),"lines",1)

endfunction 

function [x,y,typ]=sensor_outputs(o)
// Copyright INRIA
xf=60
yf=40
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
//[orig,sz,orient]=o(2)(1:3);
  inp=size(o.model.in,1);clkinp=size(o.model.evtin,1);

if orient then
  x1=orig(1)+sz(1)
  dx1=-xf/7
  x2=orig(1)
  dx2=xf/7
else
  x1=orig(1)
  dx1=yf/7
  x2=orig(1)+sz(1)
  dx2=-xf/7
end

y=[orig(2)+sz(2)-(sz(2)/2) ,orig(2)-sz(2)*2/8]
x=[(x1+dx2)  orig(1)+sz(1)/2]
typ=[2 1]
endfunction
