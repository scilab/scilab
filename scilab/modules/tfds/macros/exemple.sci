function []=exemple()
// un essai de calcul de gains avec riccati
// et simulation.
//!
// Copyright INRIA
  systems();
ue=1;
xe=equilcom(ue);
[f,g,h,vv]=lincomp(ue);
obsric(3,1,1);
comric(3,1,1);
ue=1;
xe=equilcom(ue);
boucle('bcomp',0.1,[0,150,50,210],[100,10]);
endfunction
