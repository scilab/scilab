function [gk,gf]=assemb(gk,gf,ek,ef,nel,n,nodes)
// Copyright INRIA
// développé par EADS-CCR
//  la fonction assemb assemble la matrice de regidité gk et le second   //
//  membre gf en bouclant sur les nel- elements.                         //                  
//   Sorties :                                                           //
//      - gk (Double) E/S : matrice globale                              //
//      - gf (Double) E/S : vecteur qui correspond au scond membre       //
//   Entrées :                                                           //     
//      - ek (Double) : matrice élémentaire                              //
//      - ef (Double) : vecteur élémentaire du scond membre              //
//      - nel (Entier) : le nombre d'element.                            //                                     
//      - n (Entier) :   nombre de points nodales dans un element,       //
//        e.g. linear n=2; quadrtic, n=3; cubic: n=4.                    //                                  
//      - nodes (Entier) : nombre des points nodales du noeud dans les   //
//        nel elements, nodes(1,nel), nodes(2,nel), ..., nodes(n,nel).   //            
//-----------------------------------------------------------------------//
 
  for i=1:n
    ig = nodes(i,nel);
    // Assemblage global du vecteur gf
    gf(ig) = gf(ig) + ef(i);

    for j=1:n
      jg = nodes(j,nel);
      // Assemblage global de la matrice de regidité gk
      gk(ig,jg) = gk(ig,jg) + ek(i,j);
    end
  end 
endfunction
//--------------------------- FIN DE ASSEMB -----------------------------//
