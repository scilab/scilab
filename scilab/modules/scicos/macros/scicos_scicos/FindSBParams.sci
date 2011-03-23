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

function [params,param_types]=FindSBParams(scs_m,params)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  prot = funcprot();
  funcprot(0);
  deff('varargout=scicos_getvalue(a,b,c,d)',..
    ['global par_types'
     'par_types=c'
     'x=1;y=x(2)'])
  funcprot(prot);

  global par_types

  Fun=scs_m.props.context;
  for i=1:size(scs_m.objs)
    o=scs_m.objs(i);
    if typeof(o)=='Block' then
      if o.gui<>'PAL_f' then
        model=o.model;
        if model.sim=='super'|model.sim=='csuper'|model.sim(1)=='asuper' then
          Funi='['+FindSBParams(model.rpar,params)+']'
        else
          if typeof(o.graphics.exprs)=="MBLOCK" then //modelica block
            Funi=[];
            for j=1:lstsize(o.graphics.exprs.paramv)
               Funi=[Funi;
                     '['+o.graphics.exprs.paramv(j)+']'];
            end
          else
            if type(o.graphics.exprs)==15 then
              Funi='['+o.graphics.exprs(1)(:)+']';
            else
              Funi='['+o.graphics.exprs(:)+']';
            end
            par_types=[];
            execstr('blk='+o.gui+'(''define'')')
            execstr(o.gui+'(''set'',blk)','errcatch')
            Del=[];kk=1;
            for jj=1:2:length(par_types)
              if par_types(jj)=='str' then Del=[Del,kk],end
              kk=kk+1
            end
            Funi(Del)=[]
          end
        end
        Fun=[Fun;Funi]
      end
    end
  end
  deff('%Font3()',Fun)
  xx=macrovar(%Font3);
  params=xx(3)
  %vaar=["%s" "%z" "%e" "%i" "%pi"]
  [%junk,%ind]=intersect(params,%vaar)  
  params(%ind)=[]

  Del=[]
  for ii=1:size(params,1)
   if ~exists(params(ii)) then Del=[Del,ii],end
  end
  params(Del)=[]
  param_types=list()
  for X=params'
    select evstr('type('+X+')')
    case 1
      param_types($+1)='pol'
      param_types($+1)=-1
    case 2
      param_types($+1)='pol'
      param_types($+1)=-1
    case 8
      param_types($+1)='mat'
      param_types($+1)=[-1,-1]
    case 15
      param_types($+1)='lis'
      param_types($+1)=-1
    case 16
      param_types($+1)='lis'
      param_types($+1)=-1
    case 17
      param_types($+1)='lis'
      param_types($+1)=-1
    else
      param_types($+1)='gen'
      param_types($+1)=-1
    end
  end
//  clearglobal('par_types')  //recursive call, so it cannot be cleared here
endfunction

