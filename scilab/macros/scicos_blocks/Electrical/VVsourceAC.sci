function [x,y,typ]=VVsourceAC(job,arg1,arg2)
// Copyright INRIA
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1,%f,VVac_draw_ports)
case 'getinputs' then
  [x,y,typ]=VVac_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,FR,exprs]=getvalue('Set voltage source parameter',..
    			   ['Frequency (Hz)'],..
			      list('vec',-1),exprs)
    if ~ok then break,end
    model.rpar=[FR]
    model.equations.parameters(2)=list(FR)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  model.in=[1;1];
  model.out=[1];
  VA=220
  FR=50
  model.rpar=[FR]
  model.sim='VVsourceAC'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='VVsourceAC'
  mo.inputs=['p','VA'];
  mo.outputs='n';
  mo.parameters=list(['f'],list(FR))
  model.equations=mo
  
  exprs=[string(FR)]
  gr_i=['xarc(orig(1)+sz(1)*1/8,orig(2)+sz(2)*4.3/5,sz(1)*3/4,sz(2)*3/4,0,360*64);';
	'xsegs(orig(1)+sz(1)*[0 1/8],orig(2)+sz(2)*[1/2 1/2],0)';
	'xsegs(orig(1)+sz(1)*[7/8 1],orig(2)+sz(2)*[1/2 1/2],0)';
	'V=string(model.rpar(1));'
	'xstringb(orig(1),orig(2)+sz(2)*0.2,''~'',sz(1),sz(2)*0.3,''fill'')'
	'xstringb(orig(1),orig(2)+sz(2)*0.5,V,sz(1),sz(2)*0.3,''fill'')']
 
  x=standard_define([2 2],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I','E']
  x.graphics.out_implicit=['I']
end
endfunction
function VVac_draw_ports(o)
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

function [x,y,typ]=VVac_inputs(o)
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
