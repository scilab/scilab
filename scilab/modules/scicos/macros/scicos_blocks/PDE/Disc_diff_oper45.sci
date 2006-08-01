function M=Disc_diff_oper45(N,h,DF_type)
// Copyright INRIA
// développé par EADS-CCR
// Cette fonction construit l'ecriture de la discritisation de l'operateur d2u/dtdx //
// ou de l'operateur du/dx sous forme matricielle.                                  //  
// sortie :                                                                         //
//    - M (Double) : est la matrice elementaire de discretisation de l'operateur    //
// entrees :                                                                        //
//    - N (Entier) : est le nombre de noeuds                                        //
//    - h (Double) : est le pas de discretisation h= (b-a)/N  (i.e x(i)= i* h)      //
//    - DF_type (Entier) : 0 pour les differences finies centrees, 1 pour les       //  
//      decentrees a gauche et 2 pour les decentrees à droite                       //
//----------------------------------------------------------------------------------//
 
  if ( DF_type == 0) then
    // cas centrée : 
    Mij=[[(1:N-1) (2:N)]',[(2:N) (1:N-1)]'];
    Mmn=[N, N];
    Mval=[(1/(2*h))*ones(N-1,1); (-1/(2*h))*ones(N-1,1)];
    M=sparse(Mij, Mval, Mmn);
  
  elseif ( DF_type == 1 | DF_type == 2) then
    // les cas décentrée
    Mij=[[(1:N) (2:N)]',[(1:N) (1:N-1)]'];
    Mmn=[N, N];
    Mval=[(1/h)*ones(N,1); (-1/h)*ones(N-1,1)];
    M=sparse(Mij, Mval, Mmn);
   
    if ( DF_type == 2) then
      M=-M';
    end
  end

endfunction
 
