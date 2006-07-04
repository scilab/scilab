function connected=connected_links(scs_m,k)
//given a link number returns all links connected with him through split
// Copyright INRIA

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
