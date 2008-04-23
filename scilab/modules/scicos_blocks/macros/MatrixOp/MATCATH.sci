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

function [x,y,typ]=MATCATH(job,arg1,arg2)
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
  model=arg1.model;graphics=arg1.graphics;label=graphics.exprs
  if size(label,'*')==14 then label(9)=[],end //compatiblity
  while %t do
    [ok,typ,l1,lab]=..
        getvalue('Set MATCATH block parameters',..
        ['Datatype(1=real double  2=Complex)';
	 'number of columns of each matrix'],..
         list('vec',1,'mat',[-1 1]),label)
    if ~ok then break,end
    l1=l1(:);
    if (typ==1) then
	junction_name='mat_cath';
      	ot=1;
	it=[ones(size(l1,1),1)'];
    elseif (typ==2) then
 	junction_name='matz_cath';
      	ot=2;
	it=[2*ones(size(l1,1),1)'];
    else message("Datatype is not supported");ok=%f;
    end
    if ok then
      label=lab
      in=[-ones(size(l1,1),1) l1]
      out=[-1,sum(l1)]
      [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
    end
    if ok then
      funtyp=4;
      model.sim=list(junction_name,funtyp)
      graphics.exprs=label
      arg1.graphics=graphics
      arg1.model=model
      x=arg1
      break
    end
  end
case 'define' then
  l1=[2;2]
  model=scicos_model()
  junction_name='mat_cath';
  funtyp=4;
  model.sim=list(junction_name,funtyp)
  model.in=[-1;-1]
  model.in2=[-2;-3]
  model.intyp=[1 1]
  model.out=1
  model.out2=1
  model.outtyp=1
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=[sci2exp(1);sci2exp(l1)];
  gr_i=['text=[''Horiz'';'' Cat''];';'xstringb(orig(1),orig(2),text,sz(1),sz(2),''fill'');']
  x=standard_define([2 3],model,label,gr_i)
end
endfunction

