function vect=addf_mat(M,N)
// Copyright INRIA
// développé par EADS-CCR
// fonction pour l'addition ele/ele matrice    //
// (string) .+ matrice(string)                 //
// sortie :                                    //
//    - vect (String) : Matrice de taille de M //
// entrées :                                   //
//    - M (String) : Matrice                   //
//    - N (String) : Matrice                   //
//---------------------------------------------//
  vect=[];
  if (N ==[]) then
    vect=M;
    return;
  elseif (M==[]) then
    vect=N;
    return;
  end
  [n,m]=size(M);
  for i=1:n
    for j=1:m
      vect(i,j)=addf(M(i,j),N(i,j));
    end
  end
endfunction
