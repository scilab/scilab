//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function ytob=icob(dtv)
//<ytob>=icob(dtv)
//extraction de l'etat aux instants de mesure
//variables d'entree:
//dtv(ndtv) : tableau de travail double precision obtenu par
//la mise en oeuvre de icse,icsu,icsua ou icsuq
//variables de sortie :
//ytob(ny,ntob) : valeur de l'etat aux instants de mesure
//!
ytob(ny,ntob)=0.d0;
lob=ndtu+ny+ntob+nob*ny+nex*ntob*nob+2*nu+ntob*nob+ny*(nti+ntf+..
1+ny+nuc+nuv+1);
ytob=matrix(dtv(lob+1:lob+ny*ntob),ny,ntob)



