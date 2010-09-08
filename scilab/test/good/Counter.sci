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

function [x,y,typ] = Counter(job,arg1,arg2)
x=[];y=[];typ=[];
select job
 case 'plot' then
  graphics=arg1.graphics;
  ierr=execstr('(evstr(graphics.exprs(3))==1)','errcatch')
  if ierr<>0 then graphics.exprs(3)='1';end
  if (evstr(graphics.exprs(3))==1) then
  from=graphics.exprs(1)
  to=graphics.exprs(2)
  else
  from=graphics.exprs(2)
  to=graphics.exprs(1)
  end
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
    [ok,minim,maxim,rule,exprs]=scicos_getvalue('Set Counter  block parameters',..
	['Minimum';'Maximum';'Rule (1=Increment 2=Decrement)'],..
          list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    maxim=int(maxim);minim=int(minim);
    if maxim<minim then
      message('Maximum value must be greater than the minimum value')
    elseif (rule<>1 & rule <> 2) then 
      message ('Rule can only be 1 or 2') 
    else
      graphics.exprs=exprs 
      model.dstate=0
      model.ipar=[rule;maxim;minim]
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  minim=0
  maxim=2
  rule=1
  model=scicos_model()
  model.sim=list('counter',4)
  model.evtin=1
  model.out=1
  model.out2=1
  model.dstate=0
  model.ipar=[rule;maxim;minim]
  model.blocktype='c'
  model.dep_ut=[%f %f]

  exprs=[string(minim);string(maxim);string(rule)]
  gr_i=['xstringb(orig(1),orig(2),[''Counter'';+from+'' --> ''+to],sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,exprs,gr_i)
end
endfunction
