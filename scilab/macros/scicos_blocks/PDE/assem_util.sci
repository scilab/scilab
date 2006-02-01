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

