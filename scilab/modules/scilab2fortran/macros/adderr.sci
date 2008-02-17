// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


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
nwrk(7)=nwrk(7)+1
  errn=nwrk(7)
  msgs=nwrk(8);msgs(nwrk(7))=txt
  nwrk(8)=msgs
endfunction
