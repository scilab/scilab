function [equations,impl_type,Nfictif]=gen_code_FDM(a1,b1,a2,b2,a3,b3,a4,b4,..
          a5,b5,a6,b6,a7,b7,a,b,N,oper,vbc,kbc,DF_type,h)
  // Copyright INRIA
  // développé par EADS-CCR
  // Cette fonction est pour la génération des équations DAE du bloc        //
  // sorties :                                                              //
  //    - equations (String) : vecteur qui contient le code C des équations //
  //      d'etat (DAE)                                                      //
  //    - impl_type (Entier) : indique si le type des états                 //
  //      (différentiels 1 ou algébrique -1)                                //       
  //    - Nfictif (Entier) : le nombre de noeuds total avec les noeuds      //
  //      fictifs correspodent à l'implémentation de la condition Neumann   //
  // entrées :                                                              //
  //    - ai, bi (String) : les differents coeficients des opérateurs       //
  //      (ai(x) et bi(t))                                                  //
  //    - a, b (Double) : limites du domaine [a b]                          //
  //    - N (Entier) : est le nombre de noeuds                              //
  //    - oper (Entier) : vecteur des opérateurs selectionnes de 1 à 7      //   
  //    - vbc (String) : vecteur des conditions aux limites en a et b       //
  //    - kbc (Entier) : vecteur types des conditions au limites            //
  //    - DF_type (Entier) : 0 pour les differences finies centrees,        //
  //      1 pour les decentrees a gauche et 2 pour les decentrees à droite  // 
  //    - h (Double) : le pas de discretisation h=(b-a)/N (i.e x(i)= i* h)  //
  //------------------------------------------------------------------------//  

  coef4=[];coef2=[];coef5=[];
  impl_type=1; // 1 pour l'état différentiel, -1 pour l'état algébrique 
  sep=[',','*','/']; 
  
  // calcul de nombre de noeuds (noeuds + noeuds fictifs)
  [Nfictif,a1_ev,a2_ev,a3_ev,a4_ev,a5_ev,a6_ev,a7_ev]=nombre_etats(N,...
   DF_type,kbc,a,b,h,a1,a2,a3,a4,a5,a6,a7);
    
  if (find(oper == 2) ~= []) then
    C1=Disc_diff_oper2(Nfictif,h);
    for i=1:Nfictif
       coef2=[coef2;a2_ev(i)*C1(i,:)];
    end
  end
  if (find(oper == 5) ~= [] | find(oper == 4) ~= []) then
    C2=Disc_diff_oper45(Nfictif,h,DF_type);
    for i=1:Nfictif
      coef5=[coef5;a5_ev(i)*C2(i,:)];
      coef4=[coef4;a4_ev(i)*C2(i,:)];
    end
  end  
  
  //******************************************************************
  
 if ( find(oper == 1) ~= []) then
  // cas d2u/dt2
    equations=emptystr(2*Nfictif,1);
    vec2=equations;
    dvec2=vec2;
    for i=1:2*Nfictif
      vec2(i)='x['+string(i-1)+']';
      dvec2(i)='xd['+string(i-1)+']';
    end
      
    for i=1:Nfictif
      equations(i)='   res['+string(i-1)+']='+subf(vec2(i+Nfictif),dvec2(i))+';';
      F=mulfv(msprintfv(a7_ev(i)),b7);
      if (i == 1) then
        // prise en compte des conditions aux limites en a 
        if (kbc(1) == 0) then 
          equations(Nfictif+1)='   res['+string(Nfictif)+']='+subfv(vbc(1),'x['+string(0)+']')+';';
        else
          F1=mulfv(msprintfv(h),vbc(1));
          if (DF_type == 0) then
            equations(Nfictif+1)='   res['+string(Nfictif)+']='+subfv('2*'+F1,'x['+string(2)+']-x['+string(0)+']')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations(Nfictif+1)='   res['+string(Nfictif)+']='+subfv(F1,'x['+string(1)+']-x['+string(0)+']')+';';
          end
        end
      elseif (i == Nfictif) then
        // prise en compte des conditions aux limites en b 
        if (kbc(2) == 0) then 
          equations($)='   res['+string(2*Nfictif-1)+']='+subfv(vbc(2),'x['+string(Nfictif-1)+']')+';';
        else
          F1=mulfv(msprintfv(h),vbc(2));
          if (DF_type == 0) then
            equations($)='   res['+string(2*Nfictif-1)+']='+subfv('2*'+F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-3)+']')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations($)='   res['+string(2*Nfictif-1)+']='+subfv(F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-2)+']')+';';
          end
        end
      else        
        B=addfv(mulfv(mulfv(msprintfv(a3_ev(i)),b3),vec2(i+Nfictif)),mulfv(multMatVect(coef4(i,:),vec2(Nfictif+1:$)),b4));
        C=addfv(mulfv(multMatVect(coef2(i,:),vec2(1:Nfictif)),b2),addfv(mulfv(multMatVect(coef5(i,:),..
          vec2(1:Nfictif)),b5),mulf3v(msprintfv(a6_ev(i)),b6,vec2(i))));
        A=mulf3v(msprintfv(a1_ev(i)),b1,dvec2(i+Nfictif));
        equations(i+Nfictif)='   res['+string(i+Nfictif-1)+']='+subfv(subfv(subfv(F,A),B),C)+';';
      end
    end
  elseif ( find(oper == 4) ~= []) then
    // cas d2u/dtdx (oper 4) et/ou  3 ==> implicite 
    equations=emptystr(Nfictif,1);
    vec2=equations;
    dvec2=vec2;
    for i=1:Nfictif
      vec2(i)='x['+string(i-1)+']';
      dvec2(i)='xd['+string(i-1)+']';
    end
    
    for i=1:Nfictif
      F=mulfv(msprintfv(a7_ev(i)),b7);
      if (i == 1) then
        // prise en compte des conditions aux limites en a 
        if (kbc(1) == 0) then 
          equations(1)='   res['+string(i-1)+']='+subfv(vbc(1),'x[0]')+';';
        else
          F1=mulfv(msprintfv(h),vbc(1));
          if (DF_type == 0) then
            equations(1)='   res['+string(i-1)+']='+subfv('2*'+F1,'x[2]-x[0]')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations(1)='   res['+string(i-1)+']='+subfv(F1,'x[1]-x[0]')+';';
          end
        end
      elseif (i == Nfictif) then
        // prise en compte des conditions aux limites en b 
        if (kbc(2) == 0) then 
          equations($)='   res['+string(i-1)+']='+subfv(vbc(2),'x['+string(Nfictif-1)+']')+';';
        else
          F1=mulfv(msprintfv(h),vbc(2));
          if (DF_type == 0) then
            equations($)='   res['+string(i-1)+']='+subfv('2*'+F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-3)+']')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations($)='   res['+string(i-1)+']='+subfv(F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-2)+']')+';';
          end
        end
      else
        B=addfv(mulfv(mulfv(msprintfv(a3_ev(i)),b3),dvec2(i)),mulfv(multMatVect(coef4(i,:),dvec2(:)),b4));
        C=addfv(mulfv(multMatVect(coef2(i,:),vec2(:)),b2),addfv(mulfv(multMatVect(coef5(i,:),..
          vec2(:)),b5),mulf3v(msprintfv(a6_ev(i)),b6,vec2(i))));
        equations(i)='   res['+string(i-1)+']='+subfv(subfv(F,B),C)+';';
      end
    end 
  elseif ( find(oper == 3) ~= [] & find(oper == 4) == []) then
    // cas du/dt (oper 3)
    equations=emptystr(Nfictif,1);
    vec2=equations;
    dvec2=vec2;
    for i=1:Nfictif
      vec2(i)='x['+string(i-1)+']';
      dvec2(i)='xd['+string(i-1)+']';
    end

    for i=1:Nfictif
      F=mulfv(msprintfv(a7_ev(i)),b7);
      if (i == 1) then
        // prise en compte des conditions aux limites en a 
        if (kbc(1) == 0) then 
          equations(1)='   res['+string(i-1)+']='+subfv(vbc(1),'x[0]')+';';
        else
          F1=mulfv(msprintfv(h),vbc(1));
          if (DF_type == 0) then
            equations(1)='   res['+string(i-1)+']='+subfv('2*'+F1,'x[2]-x[0]')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations(1)='   res['+string(i-1)+']='+subfv(F1,'x[1]-x[0]')+';';
          end
        end
      elseif (i == Nfictif) then
        // prise en compte des conditions aux limites en b 
        if (kbc(2) == 0) then 
          equations($)='   res['+string(i-1)+']='+subfv(vbc(2),'x['+string(Nfictif-1)+']')+';';
        else
          F1=mulfv(msprintfv(h),vbc(2));
          if (DF_type == 0) then
            equations($)='   res['+string(i-1)+']='+subfv('2*'+F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-3)+']')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations($)='   res['+string(i-1)+']='+subfv(F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-2)+']')+';';
          end
        end
      else
        B=mulf3v(msprintfv(a3_ev(i)),b3,dvec2(i));
        C=addfv(mulfv(multMatVect(coef2(i,:),vec2(:)),b2),addfv(mulfv(multMatVect(coef5(i,:),..
          vec2(:)),b5),mulf3v(msprintfv(a6_ev(i)),b6,vec2(i))));
        equations(i)='   res['+string(i-1)+']='+subfv(subfv(F,B),C)+';';
      end
    end
  else
    // cas algébrique ==> implicite 
    impl_type=-1;
    equations=emptystr(Nfictif,1);
    vec2=equations;
    for i=1:Nfictif
      vec2(i)='x['+string(i-1)+']';
    end

    for i=1:Nfictif
      F=mulfv(msprintfv(a7_ev(i)),b7);
      if (i == 1) then
        // prise en compte des conditions aux limites en a 
        if (kbc(1) == 0) then 
          equations(1)='   res['+string(i-1)+']='+subfv(vbc(1),'x[0]')+';';
        else
          F1=mulfv(msprintfv(h),vbc(1));
          if (DF_type == 0) then
            equations(1)='   res['+string(i-1)+']='+subfv('2*'+F1,'x[2]-x[0]')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations(1)='   res['+string(i-1)+']='+subfv(F1,'x[1]-x[0]')+';';
          end
        end
      elseif (i == Nfictif) 
      // prise en compte des conditions aux limites en b 
        if (kbc(2) == 0) then 
          equations($)='   res['+string(i-1)+']='+subfv(vbc(2),'x['+string(Nfictif-1)+']')+';';
        else
          F1=mulfv(msprintfv(h),vbc(2));
          if (DF_type == 0) then
            equations($)='   res['+string(i-1)+']='+subfv('2*'+F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-3)+']')+';';
          elseif (DF_type == 1) | (DF_type == 2) then
            equations($)='   res['+string(i-1)+']='+subfv(F1,'x['+string(Nfictif-1)+']-x['+string(Nfictif-2)+']')+';';
          end
        end
      else     
        C=addfv(mulfv(multMatVect(coef2(i,:),vec2(:)),b2),addfv(mulfv(multMatVect(coef5(i,:),..
          vec2(:)),b5),mulf3v(msprintfv(a6_ev(i)),b6,vec2(i))));
        equations(i)='   res['+string(i-1)+']='+subfv(F,C)+';';
      end 
    end
  end
    
endfunction

