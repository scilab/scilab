function [A,B1,B2,C1,C2,C3,F3]=coef_FEM1d(oper,nelem,kind,nint,nodes,x,xi,w,..
          nnode,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,kbc,vbc)
// Copyright INRIA
// développé par EADS-CCR
// Cette fonction renvoie les matrices d'assemblage                       //
// sorties :                                                              //
//    - Ai, Bi (Doubles) : matrices d'assemblage A, B (B1 (oper 3) et     //
//      B2(oper 4), C (C1 pour oper 2, C2 pour oper 5 et C3 pour oper 6)  //
//      et F3 pour oper 7) pour le système: A*d2u/dt2 + B*du/dt + C*u = F // 
//      (différentiels 1 ou algébrique -1)                                //       
// entrées :                                                              //
//    - oper (Entier) : vecteur des opérateurs selectionnes de 1 à 7      //  
//    - nelem (Entier) = (nnode-1)/inf_ele; c'est le nombre d'élément.    //
//    - kind(i) (Entier), i=1, ..., nelem, = 1, ou 2, ou 3. les fonctions //
//      de base dans le i-eme element. Pour la triangulation uniforme,    //
//      kind(i) = inf_ele.                                                //
//    - nint(i) (Entier), i=1, ..., nelem = 1, ou 2, ou 3, ou 4. Pour     //
//      l'ordre du Gaussian quadratique dans le i-eme element.            //
//    - nodes(j,i) (Entier): Matrice de connection des element,           //
//    - x (Double):  vecteur des cordonnées des points nodales            //
//    - xi, w (Doubles) : les points Gausse et leurs poids obtenu         //
//      de setint()                                                       //
//    - N (Entier) : est le nombre de noeuds                              //
//    - nnode (Entier) : nombre de noeuds.                                //
//    - ai, bi (String) : les differents coeficients des opérateurs       //
//      (ai(x) et bi(t))                                                  //
//    - kbc (Entier) : vecteur types des conditions au limites            //
//    - vbc (String) : vecteur des conditions aux limites en a et b       //
//------------------------------------------------------------------------//  
 
  A=[];B1=[];B2=[];C1=[];C2=[];C3=[];F3=[];
    
  for i=1:size(oper,'*')
    select oper(i)
    case 1 then      
      [A,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a1,oper(i),kbc,vbc);
    case 3 then
      [B1,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a3,oper(i),kbc,vbc);
    case 4 then
      [B2,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a4,oper(i),kbc,vbc);
    case 2 then
      [C1,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a2,oper(i),kbc,vbc);
    case 5 then
      [C2,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a5,oper(i),kbc,vbc);
    case 6 then
      [C3,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a6,oper(i),kbc,vbc);
    case 7 then
      [gk,F3]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a7,oper(i),kbc,vbc);
    end  
  end
endfunction

