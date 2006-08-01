function M=Disc_diff_oper2(N,h)
// Copyright INRIA
// développé par EADS-CCR
// Cette fonction construit l'ecriture de la discritisation de l'operateur d2u/dt2 //
// sous forme matricielle.                                                         //  
// sortie :                                                                        //
//    - M (Double) : est la matrice elementaire de discretisation de l'operateur   //
// entrees :                                                                       //
//    - N (Entier) : est le nombre de noeuds                                       //
//    - h (Double) : est le pas de discretisation h= (b-a)/N  (i.e x(i)= i* h)     //
//---------------------------------------------------------------------------------//
  // operateur d'ordre 2 d2u/dx2
  
  h1=h^2;
  
  // Matrice identique quel que soit le centrage
  Mij=[[(1:N) (2:N) (1:N-1)]',[(1:N) (1:N-1) (2:N)]'];
  Mmn=[N, N];
  Mval=[(-2/h1)*ones(N,1); (1/h1)*ones(N-1,1); (1/h1)*ones(N-1,1)];
  M=sparse(Mij, Mval, Mmn);
  
endfunction

