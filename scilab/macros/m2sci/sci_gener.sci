function [stk,txt,top]=sci_gener(nam)
// stk : liste dont les elements sont des listes et qui joue  plus ou
//       moins un role similaire a celui de la partie haute  la pile scilab
//       (contient la description) des variables sur lesquelles on travaille
//       comme dans la pile scilab stk(top) est la derniere variable definie
//
//       chaque element de stk a la structure suivante:
//       stk(k)=list(definition,type_expr,nb_ligne,nb_col,typevar)
//
//      *definition peut etre soit:
//         - une expression fortran a+2*b-3*c(1) si sa valeur est scalaire
//         - une reference a la premiere adresse d'un tableau fortran:
//                 a   si a est une matrice qui est definie
//                 work(iwn) si la variable est stockee dans un tableau de
//                           travail double precision
//                 iwork(iiwn) si la variable est stockee dans un tableau de
//                           travail entier
//      *type_expr code le type de l'expression et sert essentiellement a
//          determiner comment parentheser
//          '2' : somme de termes
//          '1' : produits de facteurs
//          '0' : atome
//      *type_var unused
//      *nb_ligne , nb_col : nombre de ligne et de colonne, ce sont aussi
//          des chaines de caracteres
// ATTENTION: stk entre par le contexte et l'on ne ressort que la valeur
//            courante
//
//  txt  : est la portion de texte fortran genere pour realiser la fonction
//         si besoin est (calcul matriciel)
//!
// Copyright INRIA
txt=[] 

//
RHS=[]
if ~batch then
  set_infos('Unknown function '+nam+' ,the original calling sequence is used',2)
else
  set_infos('Unknown variable '+nam+' ,the original calling sequence is used',2)
end
if funptr(nam)<>0 then
  nam1='%'+nam
  set_infos('name conflict: function name changed from '+nam+' to '+nam1',0)
  nam=nam1
end
for k=1:rhs
  RHS=[stk(top)(1),RHS]
  top=top-1
end
top=top+1
if lhs==1 then
  stk=list(nam+rhsargs(RHS),'0','?','?','?')
else
  stk=list()
  for k=1:lhs
    stk(k)=list(nam+rhsargs(RHS),'-1','?','?','?')
  end
end



