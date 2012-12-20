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

function [x,y,typ]=Switch(job,arg1,arg2)
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,Ron,Roff,exprs]=scicos_getvalue('Set non-ideal electrical switch parameters',..
	['Resistance in On state (Ohm)';'Resistance in Off state (Ohm)'],list('vec',1,'vec',1),exprs)
    if ~ok then break,end  
    model.equations.parameters(2)=list(Ron,Roff)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  Ron=0.01;
  Roff=1e5;
  S=['Ron';'Roff'];
  Z=eval(S);
  model.sim='Switch'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model=model.sim
  mo.inputs=['p';'inp'];
  mo.outputs='n';
  mo.parameters=list(S,Z);
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  model.rpar=Z;
  exprs=string(Z);
    gr_i=[' Thick=xget(''thickness'');xset(''thickness'',2);';
    'x=orig(1)+[0,3,5.0]*sz(1)/8;';
	'y=orig(2)+[1,1,2.5]*sz(2)/2;';
	'xpoly(x,y,''lines'',0);';
	'xpoly(orig(1)+3*sz(1)/8,orig(2)+1*sz(2)/2,''marks'',1);'; 
	'x=orig(1)+[5,8]*sz(1)/8;';
	'y=orig(2)+[1,1]*sz(2)/2;';
	'xpoly(x,y,''lines'',0);';
	'xstring(orig(1)+3,orig(2)+1.8,''sw'');';
	'xset(''thickness'',Thick)'];
  x=standard_define([2 2],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I';'E']
  x.graphics.out_implicit=['I']
end
endfunction
// Switch

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

