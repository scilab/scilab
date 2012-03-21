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

function [x,y,typ]=PMOS(job,arg1,arg2)
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
    [ok,W,L,Beta,Vt,K2,K5,dW,dL,RDS,exprs]=scicos_getvalue('Set PMOS Transistor parameters',..
	['Width [m]';..
	 'Length [m]';..
	 'Transconductance parameter [A/(V*V)]';..
	 'Zero bias threshold voltage [V]';..
	 'Bulk threshold parameter';..
	 'Reduction of pinch-off region';..
	 'Narrowing of channel [m]';..
	 'Shortening of channel [m]';..
	 'Drain-Source-Resistance [Ohm]'],..
        list('vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec',-1),exprs)
	 
    if ~ok then break,end
    model.equations.parameters(2)=list(W,L,Beta,Vt,K2,K5,dW,dL,RDS)
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
   W=50.0e-6;
   L=6.0e-6;
   Beta=0.0105e-3;
   Vt=-1;
   K2=0.41;
   K5=0.839;
   dW=-2.5e-6;
   dL=-2.1e-6;
   RDS=1.e+7;
  model.sim='PMOS'
  model.blocktype='c'
  model.dep_ut=[%t %f]
  mo=modelica()
  mo.model='PMOS';
  mo.outputs=['D';'B';'S']
  mo.inputs='G';
  mo.parameters=list(['W';'L';'Beta';'Vt';'K2';'K5';'dW';'dL';'RDS'],[W;L;Beta;Vt;K2;K5;dW;dL;RDS])
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  exprs=[string(W);string(L);string(Beta);string(Vt);string(K2);string(K5);string(dW);string(dL);string(RDS)]
  gr_i=[' Thick=xget(''thickness'');xset(''thickness'',2);';
    'if orient then,'; 
        'x1=orig(1)+  [0, 15,15,15]*(sz(1)/32);';
		'x2=orig(1)+[17,17,19,19,32,19,19,17]*(sz(1)/32);';
		'x3=orig(1)+  [17,17,19,19,22, 22,27,32,27,22,22,19,19,17]*(sz(1)/32);';
		'x4=orig(1)+[17,17,19,19,32,19,19,17]*(sz(1)/32);';
    'else,';
        'x1=orig(1)+  [32, 17,17,17]*(sz(1)/32);';
		'x2=orig(1)+[15,15,13,13,0,13,13,15]*(sz(1)/32);';
		'x3=orig(1)+  [15,15,13,13,10, 10, 5, 0,05,10,10,13,13,15]*(sz(1)/32);';
		'x4=orig(1)+[15,15,13,13,0,13,13,15]*(sz(1)/32);';
    'end,'
	'y1=orig(2)+5+[27,27,48,27]*(sz(2)/70);';
	'y2=orig(2)+5+[48,40,40,44,44,44,48,48]*(sz(2)/70);';
	'y3=orig(2)+5+[30,23,23,27,27, 31,27,27,27,23,27,27,30,30]*(sz(2)/70);';
	'y4=orig(2)+5+[16,7,7, 11,11, 11, 16,16]*(sz(2)/70);';
	'xpoly(x1,y1);';
	'xpoly(x2,y2);';
	'xpoly(x3,y3);';
	'xpoly(x4,y4);';
 	'xset(''thickness'',Thick);'
	]
    
  x=standard_define([2 2],model,exprs,gr_i)
  x.graphics.in_implicit=['I']
  x.graphics.out_implicit=['I';'I';'I']
end
endfunction


