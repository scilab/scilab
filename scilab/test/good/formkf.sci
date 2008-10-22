function [gk,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a6,operi,kbc,vbc)
// Copyright INRIA
// développé par EADS-CCR
//   la fonction formkf, construit le système discret de l'element        //
//   finis en appelant la fonction elem pour avoir la matrices locales    //
//   ek, ef et la fonction assemb pour les  ajoutées aux matrices          //
//   globales gk et le second membre gf.                                  //
//   Sorties :                                                            //
//      - gk (Double) : matrice globale                                   //
//      - gf (Double) : vecteur qui correspond au scond membre            //
//   Entrées :                                                            //
//      - nelem (Entier) : est le nombre d'éléments                       //
//      - kind(i) (Entier) : ordre des fonctions de test                  //
//      - ninit(i) (Entier) :ordre d'integration Gaussian                 //
//      - x (Double):  vecteur des cordonnées des points nodales          //
//      - xi, w (Doubles) : les points Gausse et leurs poids obtenu       //
//        de setint()                                                     //
//      - a6 (String) : coefficient a(x) de l'opérateur pour lequel nous  //
//        calculons ca forme variationelle.                               //
//      - operi (Entier) : l'opérateur concerné                           //
//      - kbc (Entier) : vecteur types des conditions au limites          //
//      - vbc (String) : vecteur des conditions aux limites en a et b     //
//------------------------------------------------------------------------//

// système discrétisé
  
  gk = spzeros(nnode,nnode);
  gf = zeros(nnode,1);
    
  for nel = 1:nelem
    n = kind(nel) + 1;
    i1 = nodes(1,nel);
    i2 = nodes(n,nel);
    i3 = nint(nel);

//  Prendre le i3-éme ordre de la quadrature Gaussienne: 1, ordre 1; 2, ordre 2, ...

       xic = xi(:,i3);      wc = w(:,i3);
       [ek,ef] = elemoper(x(i1),x(i2),n,i3,xic,wc,operi,a6);
       // assemblage
       [gk,gf]=assemb(gk,gf,ek,ef,nel,n,nodes);
    end
    
    // ajustement par apport aux conditions aux limites.
      if (kbc(1) == 0 ) then
        gk(1,:)=0;
        gf(1)=0;
      end
      if (kbc(2) == 0 ) then		                    
        gk(nnode,:)=0;
        gf(nnode)=0;
      end
endfunction
