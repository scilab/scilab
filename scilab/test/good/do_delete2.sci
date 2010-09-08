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

function [scs_m,DEL] = do_delete2(scs_m,K,gr)
//perform deletion of scs_m object whose index are given in the vector 
//K and all connected links. splits which are not given in K are not deleted
//!
//
//** 11 Jan 2007 : 'Block' / 'Text' bug validation
//** BEWARE: functions 'do_region2block' and 'do_select2block' call this function 
//** that contains some completely un useful drawobj() operations inside 
//** For the moment the drawobj() are not executed because this function is called
//** with the options %f that disable the graphics update.
//** In all the cases the the situation is restored by the final "Replot" operation.
//


DEL=[] //table of deleted objects

K0 = K ; //** save the K 

while K<>[] do
  k=K(1); K(1)=[]
  o = scs_m.objs(k);
  //** filter 'deleted' object already present in scs_m
  if find(DEL==k)==[] then 
    typ = typeof(o)
  else
    typ = 'Deleted'
  end
   
  DEL = [DEL k]

  if typ=='Link' then
    //**--------------------------------- LINK ------------------------------------
    [ct,from,to]=(o.ct,o.from,o.to)
    tokill=[%t,%t]
    if ct(2)<>2 then
      //disconnect to block
      scs_m.objs(to(1))=mark_prt(scs_m.objs(to(1)),to(2),'in',ct(2),0)
      //disconnect from block
      scs_m.objs(from(1))=mark_prt(scs_m.objs(from(1)),from(2),'out',ct(2),0)
    else
      if or(scs_m.objs(to(1)).graphics.pin==k) then
	scs_m.objs(to(1))=mark_prt(scs_m.objs(to(1)),to(2),'in',ct(2),0)
      else
	scs_m.objs(to(1))=mark_prt(scs_m.objs(to(1)),to(2),'out',ct(2),0)
      end
       if or(scs_m.objs(from(1)).graphics.pin==k) then
	 scs_m.objs(from(1))=mark_prt(scs_m.objs(from(1)),from(2),'in',ct(2),0)
       else
	 scs_m.objs(from(1))=mark_prt(scs_m.objs(from(1)),from(2),'out',ct(2),0)
       end
    end
    
    // erase and delete link
    if gr==%t then drawobj(o),end
  
  //**-------------------------------- BLOCK ----------------------------------------  
  elseif typ=='Block' then
    // get connected links
    connected=get_connected(scs_m,k)
    //ask for connected links deletion
    K=[K connected]
    // erase and delete block
    if gr==%t then drawobj(scs_m.objs(k)),end
  
  //**-------------------------------- TEXT ------------------------------------------  
  elseif typ=='Text' then
    if gr==%t then drawobj(o),end
  //**-------------------------------- DELETED ---------------------------------------
  elseif typ=='Deleted' then
  //**-------------------------------- U.F.O. ----------------------------------------
  else
    messagebox('This object can''t be deleted','modal')
  end
end

for k=DEL
   scs_m.objs(k) = mlist('Deleted')
end

endfunction
