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

function [x,y,typ]=CONST_m(job,arg1,arg2)
x=[];y=[];typ=[];
select job
 case 'plot' then
  C=arg1.graphics.exprs;
  standard_draw(arg1)
case 'getinputs' then
  x=[];y=[];typ=[];
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
while %t do
    [ok,C,exprs]=getvalue('Set Contant Block',..
	    ['Constant'],list('vec',-1),exprs)
    if ~ok then break,end
    nout=size(C)
    if find(nout==0)<>[] then
      message('C must have at least one element')
    else
	model.sim=list('cstblk4_m',4)
	model.opar=list(C)
	if (type(C)==1) then
		 if isreal(C) then
      		     ot=1
		 else
		     ot=2
		 end
	elseif (typeof(C)=="int32") then
	 ot=3
	elseif (typeof(C)=="int16") then
	ot=4
	elseif (typeof(C)=="int8") then
	ot=5
	elseif (typeof(C)=="uint32") then
	ot=6
	elseif (typeof(C)=="uint16") then
	ot=7
	elseif (typeof(C)=="uint8") then
	ot=8
	else message("type not recognized");ok=%f;
	end

	if ok then
	   model.rpar=[]
	   [model,graphics,ok]=set_io(model,graphics,list(),list(nout,ot),[],[])
      	    graphics.exprs=exprs;
            x.graphics=graphics;x.model=model
            break;
	end
    end
  end
case 'define' then
  C=[1]

  model=scicos_model()
  model.sim=list('cstblk4',4)
  model.in=[]
  model.out=size(C,1)
  model.in2=[]
  model.out2=size(C,2)
  model.rpar=C
  model.opar=list()
  model.blocktype='d'
  model.dep_ut=[%f %f]
  exprs=sci2exp(C)
  gr_i=['dx=sz(1)/5;dy=sz(2)/10;';
    'w=sz(1)-2*dx;h=sz(2)-2*dy;';
   'txt=C;'
    'xstringb(orig(1)+dx,orig(2)+dy,txt,w,h,''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
