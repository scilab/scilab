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

function prt=splitted_links(scs_m,keep,del)
//
//
// given a vectors of indexes of "kept" blocks and "deleted" blocks 
// forms at table prt. Each line of prt is relative to a link beetween 
// a kept and a deleted block as follow:
// [io,keep_block_#, port_#, port_typ, link_type, link_color, del_block_#, port_#, port_typ]
// io = xy : 
//    x=0 kept block  port is an "output"
//    x=1 kept block  port is an "input"
//    y=0 del block   port is an "output"
//    y=1 del block   port is an "input"
//
//
//** 11 Jan 2007 : 'Block' / 'Text' bug validation: this function is OK.
//** 


prt=[]

//keep :objs into the future super block
//del :objs ouside the future super block
for kkeep=1:size(keep,'*')
  kk=keep(kkeep)
  o=scs_m.objs(kk) //one block into the future super block
  
  if typeof(o)=='Block' then
    
    //regular and implicit links connected to this "internal"  block
    connected=unique([get_connected(scs_m,kk,'in'),get_connected(scs_m,kk,'out')])

    for kc=1:size(connected,'*') //loop on these links
      //lk: one link connected to this "internal"  block
      lk=scs_m.objs(connected(kc)); 

      //find those connected to a del block
      if or(lk.from(1)==del) then // link from del to keep
	prt=[prt;
	     lk.to(1:3),lk.ct(1:2),lk.from(1:3)]
      elseif or(lk.to(1)==del) then //link from keep to del
	prt=[prt;
	     lk.from(1:3),lk.ct(1:2),lk.to(1:3)]
      end //
    end  
      
    //event links going to this "internal"  block
    connected=get_connected(scs_m,kk,'clkin') 
    for kc=1:size(connected,'*') //loop on event input links 
      lk=scs_m.objs(connected(kc))
      if or(lk.from(1)==del) then // link between keep and del
	prt=[prt;
	     [lk.to(1:2),1,lk.ct(1:2),lk.from(1:2),0]]
      end
    end    
    
    //event linkS coming from this "internal"  block
    connected=get_connected(scs_m,kk,'clkout')
    for kc=1:size(connected,'*') //loop on event output links 
      lk=scs_m.objs(connected(kc))
      if or(lk.to(1)==del) then // link between keep and del
	prt=[prt;
	     [lk.from(1:2),0,lk.ct(1:2),lk.to(1:2),1]]
      end
    end  
    
  end   //if typeof(o)=='Block' then
end    //for kkeep=1:size(keep,'*')  
endfunction
