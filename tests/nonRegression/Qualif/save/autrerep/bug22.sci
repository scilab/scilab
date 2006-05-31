// bug22

mode (-1)
clear all
//getf ('../fonct_qualif.sci')

[Q,R]=qr([1,2,3]');
pascorrect=R(2)==0 & R(3)==0;

affich_result(pascorrect, 22)

clear all

// La matrice triangulaire supérieure renvoyée par cette fonction contient 
// des valeurs non nulles en dessous de la diagonale :
// 
// -->[Q,R]=qr([1,2,3]')
//  R  =
// ! - 3.7416574 !
// !   4.881-313 ! // !!!!!
// !   6.154-313 ! // !!!!!
// 
// 
//  Q  =
// ! - 0.2672612  - 0.5345225  - 0.8017837 !
// ! - 0.5345225    0.7745419  - 0.3381871 !
// ! - 0.8017837  - 0.3381871    0.4927193 !
// 
