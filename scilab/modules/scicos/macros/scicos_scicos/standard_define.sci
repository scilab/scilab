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

function o=standard_define(sz,model,label,gr_i)
    //initialize graphic part of the block data structure

    if argn(2)<4 then gr_i=[],end

    nin=size(model.in,1);
    if nin>0 then pin(nin,1)=0,else pin=[],end

    nout=size(model.out,1);
    if nout>0 then pout(nout,1)=0,else pout=[],end

    ncin=size(model.evtin,1);
    if ncin>0 then pein(ncin,1)=0,else pein=[],end

    ncout=size(model.evtout,1);
    if ncout>0 then peout(ncout,1)=0,else peout=[],end

    if type(gr_i)<>15 then gr_i=list(gr_i,8),end
    if gr_i(2)==[] then gr_i(2)=8,end
    if gr_i(2)==0 then gr_i(2)=[],end

    graphics=scicos_graphics(sz=sz,pin=pin,pout=pout,pein=pein,peout=peout, ...
    gr_i=gr_i,exprs=label)

    //  if model.sim(1)=='super' then
    //    o=scicos_block(graphics=graphics,model=model,gui='SUPER_f')
    //  else
    [ln,mc]=where()
    o=scicos_block(graphics=graphics,model=model,gui=mc(2))
    //  end
endfunction
