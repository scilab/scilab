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

function [x,y,typ]=Modulo_Count(job,arg1,arg2)
x=[];y=[];typ=[];
select job
 case 'plot' then
  graphics=arg1.graphics;base=evstr(graphics.exprs(2))
  standard_draw(arg1)
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
    [ok,ini_c,base,exprs]=scicos_getvalue('Set Modulo_Count  block parameters',..
	['initial state (>=0)';'Modulo what number (>0)'],list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    ini_c=int(ini_c);base=int(base);
    if ini_c<0|base<=0 then
      message('values  must be positive')
    else
      graphics.exprs=exprs
      model.ipar=base;
      model.dstate=ini_c;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  ini_c=0
  base=3
  model=scicos_model()
  model.sim=list('modulo_count',4)
  model.evtin=1
  model.out=1
  model.dstate=ini_c
  model.ipar=base
  model.blocktype='c'
  model.dep_ut=[%f %f]

  exprs=[string(ini_c);string(base)]
  gr_i=['xstringb(orig(1),orig(2),[''  Counter'';''Modulo ''+string(base)],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
