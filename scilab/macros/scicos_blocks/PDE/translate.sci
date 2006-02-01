function [flag_type,rdnom,DF_type,Code]=translate(CI,CI1,CLa_type,CLa_exp,CLb_type,CLb_exp,oper,..
          type_meth,degre,a,b,N,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,nom,mesures)
// Copyright INRIA
// développé par EADS-CCR
// Cette fonction contient les différents algrithme de discritisation spaciale, ainnsi que la        //
// génération du code du bloc EDP. Elle est appelée par la fonction graphic du bloc EDP.Sci          // 
// Sorties:                                                                                          //
//    - flag_type (Entier) : renvoi le type des équations générées, ( 1 pour l'explicite,            //
//      2 pour l'implicite)                                                                          //
//    - rdnom (String) : renvoie le nom du bloc plus "_explicite" si le bloc est explicite,          //
//      "_implicite" si le bloc est implicite                                                        //
//    - DF_type (Entier) : 0 pour les differences finies centrees, 1 pour les decentrees a gauche    //
//      et 2 pour les decentrees à droite                                                            //
//    - Code (String) : vecteur qui contient le code C du bloc                                       //
// Entrées:                                                                                          //
//    - CI, CI1(String) : expressions des conditions initiales resp u(t0,x) et du/dt|t=0             // 
//    - CLa_type, CLb_type(entiers) : types des conditions aux limites (0 : Dirichlet, 1 : Neumann)  //
//    - CLb_exp, CLa_exp (String) :  expressions des conditions aux limites resp en a et en b        //   
//    - oper (vecteur des entiers) : code les opérateurs selectionnes de 1 à 7                       //      
//    - type_meth (entier) : type de la methode de discretisation (type_meth=1 : DF, 2 : EF, 3 : VF) // 
//    - degre (entier) : le degre de la methode de discretisation)                                   //
//    - a, b (doubles) : correspondant resp aux valeurs des bords du domaine a et b                  //
//    - N (entier) : nombre de noeuds ave les noeuds aux limmites                                    //
//    - ai, bi (String) : avec i=1:7 : expressions des coefficients des differents operateurs        // 
//    - nom (String) : correspond au nom du bloc a generer choisis par l'utilisateur dans la fenêtre //
//      SCILAB "GIVE US BLOCK's NAME"                                                                // 
//    - mesures (vecteur des doubles) : renvoi la liste des points de mesures                        //  
//---------------------------------------------------------------------------------------------------//
  DF_type=[];
  // fonction principale
  
  // Conditions aux limites
  // kbc est le vecteur type
  kbc(1)=CLa_type;
  kbc(2)=CLb_type;
  // vbc est le vecteur des valeurs
  vbc(1)=CLa_exp;
  vbc(2)=CLb_exp;
  
  //h=(b_domaine-a_domaine)/Nbr_maillage; 
  vh=linspace(a,b,N);
  h=vh(2)-vh(1);
  eq_pts_mes=[];
  Nfictif=N;
  // FEM 
  timer();
  if (type_meth == 2) then
    //dans les éléments finis on génère de l'implicite
    flag_type=2; // 1 pour les systèmes explicites, 2 pour l'implicite
    
    nnode=N;
    [xi,w] = setint(); // Get Gaussian points and weights.
  
    [x,nelem,nodes,kind,nint]=maillage_FE1D(a,b,degre,nnode,..
    CLa_type,CLa_exp,CLb_type,CLb_exp); //maillage

    // calcul de A,B,C et F
    [A,B1,B2,C1,C2,C3,F3]=coef_FEM1d(oper,nelem,kind,nint,nodes,x,xi,w,..
    nnode,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,kbc,vbc);  
   
    // intepolation de la solution aux points de mesures par le polynomme de Lagrange
    [eq_pts_mes]=eval_pts_EF(a,b,nelem,kind,nint,nodes,x,xi,w,nnode,mesures);
    
    // génération des équations

    [equations,impl_type]=gen_code_FEM(A,B1,B2,C1,C2,C3,F3,oper,N,a,b,..
     b1,b2,b3,b4,b5,b6,b7,vbc,kbc);
    disp('Le temps de discrétisation par éléments finis est '+string(timer())+'s');
  // FDM
  elseif (type_meth == 1) then
    //dans les différences finies on génère de l'implicite
     flag_type=2; // 1 : explicie, 2 : implicite
     
    //centré ou decentré qui viendera du système expert apres
    if (degre == 2) then
      DF_type=0; //centré 
    elseif (degre == 1)
      DF_type=1; //decentré 
    end

    // intercalage de la solution aux points de mesures aux points les plus proches
    [eq_pts_mes]=eval_pts_df(a,h,N,mesures);
    
    // génération des équations
    [equations,impl_type,Nfictif]=gen_code_FDM(a1,b1,a2,b2,a3,b3,a4,b4,..
     a5,b5,a6,b6,a7,b7,a,b,N,oper,vbc,kbc,DF_type,h)
    disp('Le temps de discrétisation par différences finies est '+string(timer())+'s');
  // FVM
  else 
    // maillage
    [xc,xn]=unimesh1D(N,a,b);
    
     // intercalage de la solution aux points de mesures aux points les plus proches
    [eq_pts_mes]=eval_pts_vf(a,h,N,mesures);
    
    // génération des équations

    [equations,flag_type,impl_type]=gen_code_FVM(a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,..
     a7,b7,N,oper,vbc,xn,xc);
     disp('Le temps de discrétisation par volumes finis est '+string(timer())+'s');
  end
  // Assemblage de code
  if (flag_type == 1) then
    rdnom = nom+'_explicite';
  else
    rdnom = nom+'_implicite';
  end
 
  Code=code_generation(rdnom,equations,eq_pts_mes,flag_type,h,CI,CI1,a,Nfictif,N,impl_type,type_meth,oper);
  disp('Le temps de la génération, compilation et linkage du code est '+string(timer())+'s');
endfunction

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

function vect=subf_mat(M,N)
// Copyright INRIA
// développé par EADS-CCR
// fonction pour la soustraction ele/ele matrice //
// (string) M .- N matrice(string)               //
// sortie :                                      //
//    - vect (String) : Matrice de taille de M   //
// entrées :                                     //
//    - M (String) : Matrice                     //
//    - N (String) : Matrice                     //
//-----------------------------------------------//
  vect=[];
  if (N ==[]) then
    vect=M;
    return;
  elseif (M==[]) then
    vect='-1'.*N;
    return;
  end
  [n,m]=size(M);
  for i=1:n
    for j=1:m
      vect(i,j)=subf(M(i,j),N(i,j));
    end
  end
endfunction

function vect=mulf_string(M,vec)
// Copyright INRIA
// développé par EADS-CCR
// fonction pour la multiplication matrice     //
// (string) * vecteur(string)                  //
// sortie :                                    //
//    - vect (String) : Vecteur de taille de M //
// entrées :                                   //
//    - M (String) : Matrice                   //
//    - N (String) : Vecteur                   //
//---------------------------------------------//
  vect=[];
  N=size(vec,'*');
  for i=1:N
    line=[];
    for j=1:N
      line=[line,mulf(M(i,j),vec(j))];
    end
    som='';
    for i=1:N
      som=addf(som,line(i));
    end
    vect=[vect;som];
  end
endfunction

function x=mulf3(x1,x2,x3)
// Copyright INRIA
// développé par EADS-CCR
// mulf avec 3 entrée : x=mulf(x1,x2,x3)
  xx=mulf(x1,x2);
  x=mulf(xx,x3);
endfunction

function x=%c_x_c(a,b)
// Copyright INRIA
// développé par EADS-CCR
  if ( a == [] | b == []) then
     x=[];
     return;
  end
  [l,m]=size(a);[c,n]=size(b);
  if size(b,'*')==1 then
    b=b(ones(l,m))
  elseif size(a,'*')==1 then
    a=a(ones(c,n))
  end
 
  [l,m]=size(a);[c,n]=size(b);x=[];
  for j=1:m,
    y=[];
    for i=1:l,
      t=' ';      
          t=mulf(a(i,j),b(i,j));      
      y=[y;t];
    end
    x=[x y];
  end
endfunction

function x=%c_m_c(a,b)
// Copyright INRIA
// développé par EADS-CCR
  if ( a == [] | b == []) then
     x=[];
     return;
  end
  [l,m]=size(a);[m,n]=size(b);x=[];
  for j=1:n,
    y=[];
    for i=1:l,
      t=' ';
      for k=1:m,
        if (k > 1) then
          //t=t+'+('+a(i,k)+')*'+'('+b(k,j)+')';
          t=addf(t,mulf(a(i,k),b(k,j)));
        else
          //t='(' + a(i,k) +')*' + '(' + b(k,j) + ')';
          t=mulf(a(i,k),b(k,j));
        end
      end
      y=[y;t];
    end
    x=[x y];
  end
endfunction

function x=mulfstring(a,b)
// Copyright INRIA
// développé par EADS-CCR
  if ( a == [] | b == []) then
     x=[];
     return;
  end
  [l,m]=size(a);[m,n]=size(b);x=[];
  for j=1:n,
    y=[];
    for i=1:l,
      t=' ';
      for k=1:m,
        if (k > 1) then
          //t=t+'+('+a(i,k)+')*'+'('+b(k,j)+')';
          t=addfv(t,mulfv(a(i,k),b(k,j)));
        else
          //t='(' + a(i,k) +')*' + '(' + b(k,j) + ')';
          t=mulfv(a(i,k),b(k,j));
        end
      end
      y=[y;t];
    end
    x=[x y];
  end
endfunction

function x=mulfstringee(a,b)
// Copyright INRIA
// développé par EADS-CCR
  if ( a == [] | b == []) then
     x=[];
     return;
  end
  [l,m]=size(a);[c,n]=size(b);
  if size(b,'*')==1 then
    b=b(ones(l,m))
  elseif size(a,'*')==1 then
    a=a(ones(c,n))
  end
 
  [l,m]=size(a);[c,n]=size(b);x=[];
  for j=1:m,
    y=[];
    for i=1:l,
      t=' ';      
          t=mulfv(b(i,j),a(i,j)); 
      y=[y;t];
    end
    x=[x y];
  end
endfunction
// les fonctions ci-dessous c'est emulations de leurs fonctions
// Scilab qui n'accepetent pas le cas des entrées nulles []

function x=msprintfv(x)
// Copyright INRIA
// développé par EADS-CCR
  if (x == []) then
    x=[];
  else
    x=msprintf('%.16g\n',x);
  end
endfunction

function x=addfv(x1,x2)
// Copyright INRIA
// développé par EADS-CCR
  if (x1 == [] ) then
    x=x2;
  elseif (x2 == []) then
    x=x1;
  else
    x=addf(x1,x2);
  end
endfunction

function x=subfv(x1,x2)
// Copyright INRIA
// développé par EADS-CCR
  if (x1 == [] ) then
    x='-'+x2;
  elseif (x2 == []) then
    x=x1;
  else
    x=subf(x1,x2);
  end
endfunction

function x=mulfv(x1,x2)
// Copyright INRIA
// développé par EADS-CCR
  if (x1 == [] | x2 == [] | x1 == '' | x2 == '') then
    x='0';
  else
    x=mulf(x1,x2);
  end
endfunction

function x=mulf3v(x1,x2,x3)
// Copyright INRIA
// développé par EADS-CCR
  if (x1 == [] | x2 == [] | x3 == [] ) then
    x='0';
  else
    x=mulf3(x1,x2,x3);
  end
endfunction

