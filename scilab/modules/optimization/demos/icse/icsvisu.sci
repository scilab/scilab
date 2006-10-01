function [yobt,yob]=icsvisu(vue)
//Visualisation de l'observation de l'etat
//Syntaxe
//[yobt,yob]=icsvisu(vue)
//variables d'entree :
//vue  : vecteur des indices des composantes a visualiser
//variables de sortie :
//yobt : observation de l'etat a tous les instants
//yob  : idem, aux instants d'observation
//!
//
// Copyright INRIA
tob1=[t0,tob];
tobt=t0*ones(1,nti+ntf)+[dti*[1:nti],dtf*[(nti*dti/dtf)+1:..
(nti*dti/dtf)+ntf]];
tobt=[t0,tobt];
[ytob]=icob(dtv);
[ytot]=icot(dtv);
yobt=obs*[y0',ytot];
yob=obs*[y0',ytob];
for i=vue plot(tobt,yobt(i,:)); plot(tob1,yob(i,:));end;



