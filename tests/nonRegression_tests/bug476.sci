// bug476

mode (-1)
clear

//correct=%F
a=hypermat([3,3,1],int8(1:9))
[i,j]=find(a==5) 
[k,l,m]=find(a==5)

affich_result((m==1)&((i==k)==(j==l)), 476)

clear

//    No Error Messages 
//    wrong result with " %hm_find " function



