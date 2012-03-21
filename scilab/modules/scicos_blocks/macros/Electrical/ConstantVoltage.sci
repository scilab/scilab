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

function [x,y,typ]=ConstantVoltage(job,arg1,arg2)
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
    [ok,V,exprs]=scicos_getvalue('Set ConstantVoltage block parameter',..
    			   'V (volt)',list('vec',1),exprs)
    if ~ok then break,end
    model.rpar=V
    model.equations.parameters(2)=list(V)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end

case 'define' then
  V=0.01

  model=scicos_model()
  model.rpar=V
  model.in=1;model.out=1;
  model.sim='ConstantVoltage';
  model.blocktype='c'
  model.dep_ut=[%f %f]
  mo=modelica()
  mo.model='ConstantVoltage';
  mo.inputs='p';
  mo.outputs='n';
  mo.parameters=list('V',list(V))
  model.equations=mo
  exprs=string(V)

  gr_i=['xset(''thickness'',2)'
	'xx=[0 5 5 5]/12;';
	'if orient then yy=[1 1 2 0]/2;else yy=[2,2,3,1]/4,end'
	'xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2));';
	'xx=[2 2 2 3]/3;';
	'if orient then yy=[1,3,2,2]/4;else yy=[0 2 1 1]/2;end'
	'xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2));']
  
  x=standard_define([1.5 1.1],model,exprs,list(gr_i,0))
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I']  
end
endfunction
