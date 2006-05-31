// bug480

mode (-1)
clear

// correct=%F
a=hypermat([3,3,1],1:9)
result = execstr("int8(a)","errcatch","n")  
if result == 0 then 
   toto    = (hypermat(a.dims,int8(a.entries)) == int8(a))
end
affich_result(and(toto(:,:,1)), 480)

clear

//    No Error Messages 
//    wrong result with " %hm_find " function



