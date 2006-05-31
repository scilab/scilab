// bug475

mode (-1)
clear

//correct=%F
a=hypermat([3,3,1],uint16(1:9))
result=execstr("[m,k]=max(a)","errcatch","n")  
if result == 0 then
   result=execstr("[m,k]=min(a)","errcatch","n")  
end
affich_result(result==0, 475)

clear

//    The Error Messages are:
//                                             !--error    59 
// incorrect # of outputs in the function




