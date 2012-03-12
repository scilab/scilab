//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=VanneReglante(job,arg1,arg2)
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
x=[];y=[];typ=[];

select job
case 'plot' then
  standard_draw(arg1,%f,vanne_draw_ports)
case 'getinputs' then
  [x,y,typ]=vanne_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=vanne_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,Cvmax,p_rho,exprs]=scicos_getvalue('Paramètres de la vanne reglante',..
       ['Cvmax';'p_rho'],..
       list('vec',-1,'vec',-1),exprs)
    if ~ok then break,end
    model.rpar=[Cvmax;p_rho]
    model.equations.parameters(2)=list(Cvmax,p_rho)
//    model.equations.parameters=list([Cvmax;p_rho])
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  model.in=[1;1];
  model.out=[1];
  Cvmax=8005.42
  p_rho=0
  model.rpar=[Cvmax;p_rho]
  model.sim='VanneReglante'
  model.blocktype='c'
  model.dep_ut=[%t %f]

  mo=modelica()
    mo.model='VanneReglante'
    mo.inputs=['C1' 'Ouv'];
    mo.outputs='C2';
    mo.parameters=list(['Cvmax';'p_rho'],[Cvmax;p_rho])
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  exprs=[string(Cvmax);string(p_rho)]
  gr_i=['xfpolys(orig(1)+[0;5;7;3;5;10;10;0;0]*sz(1)/10,orig(2)+[4;2;7;7;2;0;4;0;4]*sz(2)/10,scs_color(15))'
  				'xfarcs([orig(1)+3*sz(1)/10;orig(2)+sz(2);4*sz(1)/10;6*sz(2)/10;0;180*64],scs_color(15))'
  				'xarcs([orig(1)+3*sz(1)/10;orig(2)+sz(2);4*sz(1)/10;6*sz(2)/10;0;180*64],scs_color(1))'];
  x=standard_define([2 2],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I';'E']
  x.graphics.out_implicit=['I']
end
endfunction

function [x,y,typ]=vanne_inputs(o)

xf=60
yf=40
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
//[orig,sz,orient]=o(2)(1:3);
  inp=size(o.model.in,1);clkinp=size(o.model.evtin,1);

if orient then
  x1=orig(1)
  dx1=-xf/7
  x2=orig(1)+sz(1)
  dx2=xf/7
else
  x1=orig(1)+sz(1)
  dx1=yf/7
  x2=orig(1)
  dx2=-xf/7
end

//y=[orig(2)+sz(2)-(sz(2)/2) ,orig(2)+yf/7+sz(2)]
y=[orig(2)+2*sz(2)/10 ,orig(2)+yf/7+sz(2)]
x=[(x1+dx1)  orig(1)+sz(1)/2]
typ=[2 1]

endfunction

function [x,y,typ]=vanne_outputs(o)
// Copyright INRIA
xf=60
yf=40
[orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
//[orig,sz,orient]=o(2)(1:3);
 out=size(o.model.out,1);clkout=size(o.model.evtout,1);
if orient then
  x1=orig(1)
  dx1=-xf/7
  x2=orig(1)+sz(1)
  dx2=xf/7
else
  x1=orig(1)+sz(1)
  dx1=yf/7
  x2=orig(1)
  dx2=-xf/7
end

y=[orig(2)+2*sz(2)/10]
x=[(x2+dx2) ]
typ=[2]

endfunction
function vanne_draw_ports(o)
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
    //dy=sz(2)/2
    xset('pattern',default_color(1))
    //xpoly(out2(:,1)+(orig(1)+sz(1)),..
	//  out2(:,2)+(orig(2)+sz(2)-dy),"lines",1)
    xpoly(out2(:,1)+(orig(1)+sz(1)),..
	  out2(:,2)+(orig(2)+2*sz(2)/10),"lines",1)

    //dy=sz(2)/2
    //xfpoly(in2(:,1)+orig(1),..
	//   in2(:,2)+(orig(2)+sz(2)-dy),1)
   
    xfpoly(in2(:,1)+orig(1),..
	   in2(:,2)+(orig(2)+2*sz(2)/10),1)	
  else //tilded orientation
      out2=[0  -1
	   -1  -1
	   -1   1
	    0   1]*diag([xf/7,yf/14])
      
      in2= [1  -1
	    0  -1
	    0   1
	    1   1]*diag([xf/7,yf/14])

      
      //dy=sz(2)/2
      xset('pattern',default_color(1))
      //xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	//    out2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),"lines",1)  
      //dy=sz(2)/2
      //xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	//     in2(:,2)+ones(4,1)*(orig(2)+sz(2)-dy),1) 
      xpoly(out2(:,1)+ones(4,1)*orig(1)-1,..
	    out2(:,2)+ones(4,1)*(orig(2)+2*sz(2)/10),"lines",1)  
      dy=sz(2)/2
      xfpoly(in2(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	     in2(:,2)+ones(4,1)*(orig(2)+2*sz(2)/10),1) 
  end
  // valve command port port
  //------------------------
  // set port shape



  in= [-1  1
        0  0
        1  1
       -1  1]*diag([xf/14,yf/7])



  dx=sz(1)/2

  xfpoly(in(:,1)+ones(4,1)*(orig(1)+dx),..
	 in(:,2)+ones(4,1)*(orig(2)+sz(2)),1)

endfunction 
