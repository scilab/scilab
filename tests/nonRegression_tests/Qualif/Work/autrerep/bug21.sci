// bug21

mode (-1)
clear all
//getf ('../fonct_qualif.sci')

// This part replay bug
bool_val =(cumprod([1+1*%i,2+2*%i,3+3*%i])==cumprod([1+1*%i,2+2*%i,3+3*%i],2));
//endfunction

affich_result(isequal(bool_val,[%t,%t,%t]), 21)

clear all

//La fonction cumprod() donne des résultats érronés pour les complexes.
//
// Exemple :
// -->res1 = cumprod([1+1*%i,2+2*%i,3+3*%i])
//  res1  =
//  
// !   1. + i      2. + 2.i    6. + 6.i !
//  
// Alors que l'on devrait avoir :
// 
//  res1  =
//  
// !   1. + i      4.i  - 12. + 12.i !
// 