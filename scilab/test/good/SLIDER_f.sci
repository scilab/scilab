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

function [x,y,typ] = SLIDER_f(job,arg1,arg2)
//
x=[];y=[];typ=[]
select job
case 'plot' then
  ipar=arg1.model.ipar
  dpar=arg1.model.rpar
  standard_draw(arg1)
case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,min_r,max_r,sl_type,color,herit,exprs]=scicos_getvalue(..
	'Set  parameters',..
	['Min range';
	 'Max range';
	 'Type';
	 'Color';
	 'Block inherits (1) or not (0)'],..
	 list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end //user cancel modification
    mess=[]
    if max_r <= min_r then
      mess=[mess;'min range < max range ';' ']
      ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';
	' ';mess]);
    end
    if ~or(herit==[0 1]) then
      mess=[mess;'Accept inherited values are 0 and 1';' ']
      ok=%f
    end
     if ~ok then
      message(['Some specified values are inconsistent:';
	  ' ';mess])
    end
    if ok then
      [model,graphics,ok]=check_io(model,graphics,1,[],ones(1-herit,1),[])
    end
    if ok then
      ipar=[sl_type;color;xget('window')];
      dpar=[min_r;max_r];
      model.ipar=ipar;
      model.rpar=dpar;
      model.evtin=ones(1-herit,1)
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  min_r= 0
  max_r= 1
  color= 1
  sl_type = 1

  model=scicos_model()
  model.sim='slider'
  model.in=1
  model.evtin=1
  model.dstate=[0;-1;1;2;3;4]
  model.rpar=[min_r;max_r]
  model.ipar=[sl_type;color;0]
  model.blocktype='d'
  model.dep_ut=[%t %f]
 
  exprs=[string(min_r);
	 string(max_r);
	 string(sl_type);
	 string(color);
	 string(0)]

  gr_i=' '
  x=standard_define([3 1],model,exprs,gr_i)
end
endfunction
