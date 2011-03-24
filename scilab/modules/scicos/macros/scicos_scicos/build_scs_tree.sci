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

function [xx,yy,lp,larg]=build_scs_tree(scs_m,flag)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  [lhs,rhs]=argn(0) 
  if rhs<2 then
    flag='super'
  end
  xx=[];yy=[];
  lp=list()
  n=lstsize(scs_m.objs)
  blks=[]
  if flag=='super' then
    for k=1:n
      if typeof(scs_m.objs(k))=='Block' then 
	if scs_m.objs(k).model.sim=='super' then blks=[blks,k],end
      end
    end
  else
    for k=1:n
      if typeof(scs_m.objs(k))=='Block' then blks=[blks,k],end
    end
  end

  //
  nl=size(blks,2)
  xlk=x0
  for k=blks
    path=[path k];
    xx=[xx,[x0;xlk]];yy=[yy,[y0;y0-1]]
    larg=max(xlk,larg)
    lp($+1)=path
    if scs_m.objs(k).model.sim=='super' then
      y0s=y0;x0s=x0;x0=xlk;y0=y0-1
      largs=larg;
      [xxl,yyl,lpl,larg]=build_scs_tree(scs_m.objs(k).model.rpar)
      xx=[xx,xxl];yy=[yy,yyl]
      lp=lstcat(lp,lpl)

      xlk=xlk+(larg-largs)+1/(nl*(1+abs(y0)))
      y0=y0s;x0=x0s
    end
    path($)=[]
  end
endfunction
