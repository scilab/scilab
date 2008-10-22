function [errn,nwrk]=adderr(nwrk,txt)
//Cette macro ajoute une erreur dont le texte du message est donne dans txt
//    dans la table des erreurs possibles et retourne
// errn : le numero associe a cette erreur  dans le sous programme fortran
// il faut alors inserer dans le code fortran les lignes suivantes
//         if(.......) then
//             ierr=errn
//             return
//         endif
//!
// Copyright INRIA
nwrk(7)=nwrk(7)+1
  errn=nwrk(7)
  msgs=nwrk(8);msgs(nwrk(7))=txt
  nwrk(8)=msgs
endfunction
