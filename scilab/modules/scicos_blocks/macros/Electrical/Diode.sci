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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=Diode(job,arg1,arg2)
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
    [ok,Ids,Vt,Maxexp,R,exprs]=scicos_getvalue('Set Diode block parameter',..
					['Saturation cuurent (A)',..
		    'Voltage equivalent to temperature (Volt)',..
		    'Max exponent for linear continuation',..
		    'R (ohm)'],	list('vec',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    model.rpar=[Ids;Vt;Maxexp;R]
    model.equations.parameters=list(['Ids','Vt','Maxexp','R'],list(Ids,Vt,Maxexp,R))
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  Ids=1.e-6; Vt=0.04; Maxexp=15; R=1.e8;
  model=scicos_model()
  model.rpar=[Ids;Vt;Maxexp;R]

  model.in=1;model.out=1;
  model.sim='Diode';
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='Diode';
  mo.inputs='p';
  mo.outputs='n';
  mo.parameters=list(['Ids','Vt','Maxexp','R'],list(Ids,Vt,Maxexp,R))

  model.equations=mo
  exprs=string([Ids;Vt;Maxexp;R])
  gr_i=['xset(''thickness'',2)'
	'if orient then'
	'  xx=orig(1)+[0,1,1,2,2,2,2,3,2,1,1,2]*(sz(1)/3);';
	'  yy=orig(2)+[2,2,3,2,3,1,2,2,2,1,2,2]*(sz(2)/4);';
	'else'     
	'  xx=orig(1)+[0,1,1,1,1,2,2,3.06,2,2,1,2]*(sz(1)/3);';
	'  yy=orig(2)+[2,2,3,1,2,3,2,2,2,1,2,2]*(sz(2)/4);';
	'end'
	'xpoly(xx,yy);'] 
  x=standard_define([2 1],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']  
end
endfunction
