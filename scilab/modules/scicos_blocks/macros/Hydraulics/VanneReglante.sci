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
    [ok,Cvmax,p_rho,exprs]=getvalue('Paramètres de la vanne reglante',..
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
