function connected=connected_links(scs_m,k)
//given a link number returns all links connected with him through split
// Copyright INRIA
TYPE_OBJET=1;GRAPHIQUE=2;
OBJET_ORIGINE=8;OBJET_DESTINATION=9
$ENTREES=5;$SORTIES=6;$ENTREES_EVT=7;$SORTIES_EVT=8

if scs_m(k)(TYPE_OBJET)<>'Link' then
  error('Object is not a link')
end
connected = []
liaisons_a_traiter = k
    
//- Look for connected links
while liaisons_a_traiter <> []
  numero_liaison = liaisons_a_traiter(1)
  connected = [connected, numero_liaison]
  bloc_amont = scs_m(scs_m(numero_liaison)(OBJET_ORIGINE)(1))
  bloc_aval = scs_m(scs_m(numero_liaison)(OBJET_DESTINATION)(1))
  for bloc = list(bloc_amont, bloc_aval)
    if is_split(bloc) then
      for port = [$ENTREES, $SORTIES, $ENTREES_EVT, $SORTIES_EVT]
	liaisons_a_traiter = [liaisons_a_traiter; bloc(GRAPHIQUE)(port)]
      end
    end
  end
  for i = connected
    liaisons_a_traiter = liaisons_a_traiter(find(liaisons_a_traiter <> i))
  end
end
