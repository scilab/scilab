// bug487

mode (-1)
clear

//correct=%F
lvl= -0.0003 + (1:10)*(0.0003-(-0.0003))/(10+1)
result=execstr("contour2d(1:10,1:10,rand(10,10),lvl,rect=[0,0,11,11])","errcatch","n")  
xdel()
affich_result(result == 0, 487)

clear

// The Error Messages are:
//   !--error   204 
//Argument   2, wrong type argument: expecting a scalar
//at line       3 of function contour2d                called by :  
//   contour2d(rhogit,zgit,Jv, lvl, strf="041");
//
// lvl in contour2d is a vector
