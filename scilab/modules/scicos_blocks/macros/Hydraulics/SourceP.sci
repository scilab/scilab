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

function [x,y,typ]=SourceP(job,arg1,arg2)
// exemple d'un bloc implicit, 
//   -  sans entree ni sortie de conditionnement
//   -  avec une entree et une sortie de type implicit et de dimension 1
//   - avec un dialogue de saisie de parametre
x=[];y=[];typ=[];

select job
case 'plot' then
 if arg1.graphics.flip then
    x1=0
    x2=4*arg1.graphics.sz(1)/7
  else
    x1=3*arg1.graphics.sz(1)/7
    x2=3*arg1.graphics.sz(1)/7
  end

    standard_draw(arg1,%f)
    
case 'getinputs' then
  //[x,y,typ]=source_inputs(arg1)
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
    [ok,P0,T0,H0,option_temperature,exprs]=scicos_getvalue('Paramètres du puit',..
       ['Pression de la source : P0 (Pa)';..
       'Temperature de la source : T0 (K)';..
       'Enthalpie spécifique de la source : H0 (J/kg)';..
       '1:température fixée - 2:enthalpie fixée : option_temperature'],..
       list('vec',-1,'vec',-1,'vec',-1,'vec',-1),exprs)
    if ~ok then break,end
    model.rpar=[P0;T0;H0;option_temperature]
    model.equations.parameters(2)=list(P0,T0,H0,option_temperature)
//    model.equations.parameters=list([P0;T0;H0;option_temperature])
    graphics.exprs=exprs
    x.graphics=graphics;x.model=model
    break
  end
case 'define' then
  model=scicos_model()
  P0=300000
  T0=290
  H0=100000
  option_temperature=1
  model.rpar=[P0;T0;H0;option_temperature]
  model.sim='Source'
  model.blocktype='c'
  model.dep_ut=[%t %f]

  mo=modelica()
    mo.model='Source'
    mo.inputs=[];
    mo.outputs=['C'];
    mo.parameters=list(['P0';'T0';'H0';'option_temperature'],[P0;T0;H0;option_temperature])
  model.equations=mo
  model.in=ones(size(mo.inputs,'*'),1)
  model.out=ones(size(mo.outputs,'*'),1)
  exprs=[string(P0);string(T0);string(H0);string(option_temperature)]
  gr_i=['xfpolys(orig(1)+x1+[0;4;4;0]*sz(1)/7,orig(2)+[0;0;4;4]*sz(2)/4,scs_color(15))'
  				'xsegs(orig(1)+x2+(2*orient-1)*[0;3;2;3;2;3]*sz(1)/7,orig(2)+[2;2;1.5;2;2.5;2]*sz(2)/4)'
  				'xstringb(orig(1)+x1,orig(2),''S'',4*sz(1)/7,sz(2),''fill'')'];
  x=standard_define([2.5 2],model,exprs,list(gr_i,0))
  x.graphics.out_implicit=['I']
end
endfunction
