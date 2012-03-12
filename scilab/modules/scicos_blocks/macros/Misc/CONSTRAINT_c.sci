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

function [x,y,typ]=CONSTRAINT_c(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
    while %t do
    [ok,x0,exprs]=scicos_getvalue('Set solver block parameters',..
	'Initial guess values',list('vec',-1),exprs)
    if ~ok then break,end
    x0=x0(:);N=size(x0,'*');
    
    if N<=0 then
      message('number of states (constraints) must be > 0 ')
    else
      [model,graphics,ok]=check_io(model,graphics,N,N,[],[])
      if ok then
	graphics.exprs=exprs
        model.state=[x0;zeros(N,1)];
	model.out=N
	model.in=N
	model.ipar=-1*ones(N,1);
	x.graphics=graphics;x.model=model
	break
      end
    end
    end
      
  
 case 'define' then
  x0=[0;0]
  model=scicos_model()
  model.sim=list('constraint_c',10004)
  model.in=1
  model.out=1
  model.ipar=0
  model.state=x0
  model.blocktype='c'
  model.dep_ut=[%f %t]
  exprs='0'
  
gr_i=['txt=[''=f(x)''];xstringb(orig(1),orig(2)+sz(2)*0.20,txt,sz(1)/2,sz(2)/2,''fill'');';
'txt=[''x''];xstringb(orig(1)+sz(1)*0.75,orig(2)+sz(2)*0.35,txt,sz(1)/5,sz(2)/5,''fill'');';
'txt=''solve f(x)=0'';xstringb(orig(1)+.01*sz(1), orig(2)+.4*sz(1), txt, sz(1),sz(2)/3,''fill'');'];

x=standard_define([3 2],model,exprs,gr_i)
end
endfunction

 
 