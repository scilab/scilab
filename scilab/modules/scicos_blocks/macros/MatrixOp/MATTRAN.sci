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

function [x,y,typ]=MATTRAN(job,arg1,arg2)
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
 // if size(label,'*')==14 then label(9)=[],end //compatiblity
  if size(label,'*')==1 then label(2)=sci2exp(1),end
  while %t do
    [ok,typ,rule,exprs]=scicos_getvalue('Set MATTRAN Block',..
	    ['Datatype(1=real double 2=Complex)';'rule (1=.'' 2='')'],list('vec',1,'vec',1),label)
    if ~ok then break,end
    if (typ==1) then
	junction_name='mattran_m';
      	ot=1;
	it=1;
    elseif (typ==2) then
	if rule==1 then
 	   junction_name='matztran_m';
        else
	   junction_name='mathermit_m';
	end 
      	ot=2;
	it=2;
    else message("Datatype is not supported");ok=%f;
    end
    in=[model.in model.in2];
    out=[model.out model.out2];
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
  model.sim=list('mattran_m',4)

  model.in=-1
  model.in2=-2
  model.out=-2
  model.out2=-1
  model.dep_ut=[%t %f]

  label=[sci2exp(1)]
  gr_i=['xstringb(orig(1),orig(2),[''MATTRAN''],sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

