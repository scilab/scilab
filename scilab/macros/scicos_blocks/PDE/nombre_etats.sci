function [N,a1_ev,a2_ev,a3_ev,a4_ev,a5_ev,a6_ev,a7_ev]=nombre_etats(N,...
          DF_type,kbc,a,b,h,a1,a2,a3,a4,a5,a6,a7)
  // Copyright INRIA
  // développé par EADS-CCR
  // cette fonction teste selon le type de discretisation differences       //
  // finies et le type des conditions aux limites, elle renvoie la nouvelle //
  // taille du systèmes (nombre d'etats) avec les noeuds fictifs.           //
  // sortie :                                                               //
  //    - N (Entier) : le nombre de noeuds total avec les noeuds fictifs    //
  //      correspodent à l'implémentation de la condition Neumann           //
  //    - ai_ev (Entiers) i=1:7 : vecteurs des valeurs des coefficients ai  //
  //      a chaque noeud.                                                   //    
  // entrées :                                                              //
  //    - N (Entier) : est le nombre de noeuds                              //
  //    - DF_type (Entier) : 0 pour les differences finies centrees,        //
  //      1 pour les decentrees a gauche et 2 pour les decentrees à droite  // 
  //    - kbc (Entier) : vecteur types des conditions au limites            //
  //    - a, b (Double) : limites du domaine [a b]                          //
  //    - h (Double) : le pas de discretisation h=(b-a)/N (i.e x(i)= i* h)  //
  //    - ai (String) : les coeficient (ai(x)) des opérateurs               //
  //------------------------------------------------------------------------//  
  x=a;
  for i=1:N
    a1_ev(i)=evstr(a1); a2_ev(i)=evstr(a2); a3_ev(i)=evstr(a3); a4_ev(i)=evstr(a4); 
    a5_ev(i)=evstr(a5); a7_ev(i)=evstr(a7); a6_ev(i)=evstr(a6);
    x=x+h;
  end
  
 if (kbc(1) == 1) then
   // Neumann en a
   if (DF_type == 0 | DF_type == 1) then
     if a1_ev ~= [] then, x=a-h; a1_ev=[evstr(a1);a1_ev]; end 
     if a2_ev ~= [] then, x=a-h; a2_ev=[evstr(a2);a2_ev]; end 
     if a3_ev ~= [] then, x=a-h; a3_ev=[evstr(a3);a3_ev]; end 
     if a4_ev ~= [] then, x=a-h; a4_ev=[evstr(a4);a4_ev]; end 
     if a5_ev ~= [] then, x=a-h; a5_ev=[evstr(a5);a5_ev]; end 
     if a6_ev ~= [] then, x=a-h; a6_ev=[evstr(a6);a6_ev]; end 
     if a7_ev ~= [] then, x=a-h; a7_ev=[evstr(a7);a7_ev]; end 
     N=N+1;
   end   
 end
 if (kbc(2) == 1) then
   // Neumann en b
   if (DF_type == 0 | DF_type == 2) then
     if a1_ev ~= [] then, x=b+h; a1_ev=[a1_ev;evstr(a1)]; end 
     if a2_ev ~= [] then, x=b+h; a2_ev=[a2_ev;evstr(a2)]; end 
     if a3_ev ~= [] then, x=b+h; a3_ev=[a3_ev;evstr(a3)]; end 
     if a4_ev ~= [] then, x=b+h; a4_ev=[a4_ev;evstr(a4)]; end 
     if a5_ev ~= [] then, x=b+h; a5_ev=[a5_ev;evstr(a5)]; end 
     if a6_ev ~= [] then, x=b+h; a6_ev=[a6_ev;evstr(a6)]; end 
     if a7_ev ~= [] then, x=b+h; a7_ev=[a7_ev;evstr(a7)]; end 
     N=N+1;
   end   
 end

endfunction
