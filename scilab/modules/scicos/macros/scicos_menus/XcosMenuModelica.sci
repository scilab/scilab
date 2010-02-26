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

function XcosMenuModelica()
  Cmenu=[]
  xinfo('Click on a Implcit Superblock '+..
	' to obtain a modelica block ! ')
  k=[]
  
  while %t 
    [btn,xc,yc,win,Cmenu]=cosclick()
    
    if Cmenu<>[] then
      %pt=[];break
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];%win=win
      if Cmenu==emptystr() then 
	Cmenu=[];%pt=[];
      end 
      break
    end
    k=getobj(scs_m,[xc;yc])
    if k<>[] then
      if scs_m.objs(k).model.sim(1)=='super' then
	all_scs_m=scs_m;
	XX=scs_m.objs(k).model.rpar;
	name=XX.props.title(1)
	txt=gen_modelica(XX,name)
	mputl(txt,scs_m.props.title(2)+name+'.mo');
	compile_modelica(scs_m.props.title(2)+name+'.mo')
	Cmenu=[] 
      else
	messagebox('Generation Code only work for a Superblock ! ','modal')
	break
      end
    end
  end    
  
endfunction 
 
