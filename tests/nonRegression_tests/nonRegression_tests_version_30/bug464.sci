// bug464

mode (-1)
clear

correct=%F
result=execstr("inttype(1)","errcatch","n")  
if result == 0 then 
   correct=(inttype(1)==0)
end
affich_result(correct, 464)

clear

// "1" is a double, and inttype is not defined for a double argument 
// the error message is confusing
//Error message :
//  -->inttype(1)  
//           !--error    44 
//***th argument is incorrect
