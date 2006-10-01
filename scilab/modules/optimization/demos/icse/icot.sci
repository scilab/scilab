function [ytot]=icot(dtv)
//<ytot>=icot(dtv)
//extraction de l'etat total
//variables d'entree :
//dtv(ndtv) : tableau de travail double precision obtenu par
//la mise en oeuvre de icse,icsu,icsua ou icsuq
//variables de sortie:
//ytot(ny,nt) : etat du systeme
//!
// Copyright INRIA
ytot(ny,nti+ntf)=0.d0;
lot=ndtu+ny+ntob+nob*ny+nex*ntob*nob+2*nu+ntob*nob+ny*(1+ny+nuc+nuv);
ytot=matrix(dtv(lot+1:lot+ny*(nti+ntf)),ny,nti+ntf)



