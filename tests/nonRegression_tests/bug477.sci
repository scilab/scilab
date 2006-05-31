// bug477

mode (-1)
clear

correct=%F
a=hypermat([3,3,1],1:9)
[m]=max(a)                    
result2=execstr("[n,i]=max(a)","errcatch","n")
if result2==0 then
   result3=execstr("[m,i,j,k]=max(a)","errcatch","n")
   correct = ((m==n)&(result2==0)&(and(i==[3,3,1]))&(result3==78))
end
affich_result(correct, 477)

clear

//    The Error Messages are:
//    -->[m,i]=max(a)
//             !--error    59 
// incorrect # of outputs in the function
// arguments are :
//  x         



