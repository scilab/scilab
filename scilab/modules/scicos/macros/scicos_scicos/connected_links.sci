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

function connected=connected_links(scs_m,k)
//given a link number returns all links connected with him through split

  if typeof(scs_m.objs(k))<>'Link' then
    error('Object is not a link')
  end
  connected = []
  liaisons_a_traiter = k
  
  //- Look for connected links
  while liaisons_a_traiter <> []
    numero_liaison = liaisons_a_traiter(1)
    connected = [connected, numero_liaison]
    bloc_amont = scs_m.objs(scs_m.objs(numero_liaison).from(1))
    bloc_aval = scs_m.objs(scs_m.objs(numero_liaison).to(1))
    for bloc = list(bloc_amont, bloc_aval)
      if is_split(bloc) then
	liaisons_a_traiter = [liaisons_a_traiter;
		    bloc.graphics.pin
		    bloc.graphics.pout
		    bloc.graphics.pein
		    bloc.graphics.peout]
      end
    end
    for i = connected
      liaisons_a_traiter = liaisons_a_traiter(find(liaisons_a_traiter <> i))
    end
  end
endfunction
