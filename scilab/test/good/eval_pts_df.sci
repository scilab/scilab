function [eq_pts_mes]=eval_pts_df(a,h,N,mesures)     
  // Copyright INRIA
  // développé par EADS-CCR
  // Cette fonction renvoie les equations de sorties correspondent aux        //
  // points de mesures en approchant leus solution au noeud le plus poche     //
  // sortie :                                                                 //
  //    - eq_pts_mes (String) : vecteur des équations de sorties choisit      //
  // entrees :                                                                //
  //    - a (Double) : limite inférieur du domaine [a b]                      //
  //    - h (Double) : est le pas de discretisation h=(b-a)/N (i.e x(i)= i* h)//
  //    - N (Entier) : est le nombre de noeuds                                //
  //    - mesures (Double) : vecteur des abcaisses des points de mesures      //
  //--------------------------------------------------------------------------// 
  
  u=emptystr(N,1);;  
  for i=1:N
      u(i)='x['+string(i-1)+']';
  end
  nmes=size(mesures,'*');
  eq_pts_mes=emptystr(nmes,1);
  for npt=1:nmes
    // numéro d'element auquel npt appartient  
    nel=int((mesures(npt)-a)/h)+1;
    if (mesures(npt)-(a+h*(nel-1)) <= (a+h*nel)-mesures(npt)) then
      l=nel;
    else
      l=nel+1;
    end
    eq_pts_mes(npt) = u(l);
  end

endfunction

