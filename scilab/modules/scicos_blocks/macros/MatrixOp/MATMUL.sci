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

function [x,y,typ]=MATMUL(job,arg1,arg2)
//
x=[];y=[];typ=[];
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
  graphics=arg1.graphics;label=graphics.exprs
  model=arg1.model;
  if size(label,'*')==1 then label(2)=sci2exp(1),end //compatiblity
  while %t do
    [ok,typ,rule,exprs]=scicos_getvalue('Set MATMUL Block',..
	    ['Datatype(1=real double 2=Complex)';
	     'Multiplication rule (1= * 2= .* )'],list('vec',1,'vec',1),label)
    if ~ok then break,end
    if (typ<>1&typ<>2) then message("type is not supported");ok=%f;end
    if rule<>2 then rule=1;end
    if ((typ==1)&(rule==1)) then
	junction_name='matmul_m';
    elseif ((typ==2)&(rule==1)) then
 	junction_name='matzmul_m';
    elseif((typ==1)&(rule==2)) then
 	junction_name='matmul2_m';
    elseif((typ==2)&(rule==2)) then
 	junction_name='matzmul2_m';
    end
    it=typ*ones(1,2);
    ot=typ
    if rule==1 then
    	in=[model.in model.in2];
    	out=[model.out model.out2];
    else
	in=[-1 -2;-1 -2]
	out=[-1 -2]
    end
    funtyp=4;
    if ok then
      label=exprs;
      [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      model.sim=list(junction_name,funtyp);
      graphics.exprs=label;
      arg1.graphics=graphics;arg1.model=model;
      x=arg1
      break
    end
  end
case 'define' then
  model=scicos_model()
  model.sim=list('matmul_m',4)

  model.in=[-1;-2]
  model.in2=[-2;-3]
  model.out=-1
  model.out2=-3
  model.dep_ut=[%t %f]

  label=[sci2exp(1)]
  gr_i=['xstringb(orig(1),orig(2),[''MATMUL''],sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

