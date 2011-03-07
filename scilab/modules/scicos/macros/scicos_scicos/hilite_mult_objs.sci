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
function ok=hilite_mult_objs(path,objs,mess)
// this function hilight a block by two different way:
//   -First method: the block is given by its position in the diagram. In this case
//               - path: the super_path
//               - objs: the number of the block in the super_path.
//   -Second method: the block is given by its compiled number. In this case
//               - path is a list of vector (corinv)
//               - objs: the compiled number of the block.
ok=%t;
global Scicos_commands
Scicos_commands=[];
if type(path)==1 then
  for i=1:size(objs,'*')
    Scicos_commands=[Scicos_commands(1:$-1);
	Scicos_commands($)+'%diagram_path_objective='+sci2exp(path)+';%scicos_navig=1';
	'hilite_obj('+sci2exp(objs(i))+');'];
  end
elseif type(path)==15 then
  for i=1:size(objs,'*')
    temppath=path(objs(i))
    if type(temppath)==1 then
      temppath=check_csuper(temppath)
      nbr_obj=temppath($)
      temppath2=temppath(1:$-1);     
      Scicos_commands=[Scicos_commands(1:$-1);
	  Scicos_commands($)+'%diagram_path_objective='+sci2exp(temppath2)+';%scicos_navig=1';
	  'hilite_obj('+sci2exp(nbr_obj)+');'];
    elseif type(temppath)==15 then //modelica and sampleclk
      for j=1:lstsize(temppath)
	nbr_obj=temppath(j)($)
	temppath2=temppath(j)(1:$-1)
	Scicos_commands=[Scicos_commands(1:$-1);
	    Scicos_commands($)+'%diagram_path_objective='+sci2exp(temppath2)+';%scicos_navig=1';
	    'hilite_obj('+sci2exp(nbr_obj)+');'];
      end
    else
      messagebox('The path must be a vector or a list of vectors','modal');
      ok=%f;
      return;
    end
  end
else
  messagebox('The path must be a vector or a list of vectors','modal');
  ok=%f;
  return;
end
Scicos_commands=[Scicos_commands(1:$-1);
    Scicos_commands($)+'%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
    'Cmenu="""";']
if argn(2)==3 then
  Scicos_commands=[Scicos_commands(1:$-1);
      Scicos_commands($)+'messagebox('+onelinemess(mess)+',''modal'');']
end
endfunction

function tt=onelinemess(mess) 
tt='['; 
for i=1:size(mess,'*')
  tt=tt+'""'+mess(i)+'"";';
end
tt=tt+']';
endfunction

function temp2=check_csuper(temp,scs_m)
temp2=[temp(1)];
if scs_m.objs(temp(1)).model.sim=='super' then
  temp2b=check_csuper(temp(2:$),scs_m.objs(temp(1)).model.rpar);
  temp2=[temp2 temp2b];
end
endfunction
 
 