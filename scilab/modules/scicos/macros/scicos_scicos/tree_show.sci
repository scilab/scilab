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

function tree_show(x,titletop)

  if type(x)<>16&type(x)<>17&type(x)<>15 then
    error("Wrong type; input must be a list.")
  end

  Path = 'root'
  tree = createNode("Root");
  tree = crlist3(x,Path, tree);
  displaytree(tree);
endfunction


function java = crlist3(x,Path, java)
//** 26/06/2009, Serge Steer: added function case
  if type(x)==15 then
    II=1:size(x);v=string(II);
  else
    v=getfield(1,x);
    if type(x)==17 & v(1)=='st' then
      II=3:size(v,'*');
    else
      II=2:size(v,'*');
    end
  end
  for i=II
    path=Path+','+string(i)
    titre=v(i);
    o=getfield(i,x);
    if type(o)==16 |type(o)==17 then
      w=getfield(1,o);
      titre2=titre+' ('+w(1)+')';
      currentNode = createNode(titre2);
      currentNode = crlist3(o,path,currentNode); //* recursive
      java = concatTree(java, currentNode);

    elseif type(o)==15 then
      titre2=titre;
      currentNode = createNode(titre2);
      currentNode = crlist3(o,path,currentNode); //* recursive
      java = concatTree(java, currentNode);
    elseif or(type(o)==[11 13]) then
      //to be revisited (SS)
      titre2=[titre+': function'
	      fun2string(o)]
      java = concatTree(java, createNode(titre2));
    else
      if size(o,'*')>40 then
	tts=typeof(o)+' of size '+sci2exp(size(o))
      else
	tts=sprintf('%s',sci2exp(o))
      end
      titre2=titre+': '+tts  ;
      java = concatTree(java, createNode(titre2));
    end
  end

endfunction

