// bug793

mode (-1);
clear all

affich_result(getdate(-1) == [0 0 0 0 0 0 0 0 0 0], 793)

//-->getdate(-1)
//dt=getdate(x) x must be >0.
// ans  =
//
//!   0.    0.    0.    0.    0.    0.    0.    0.    0. !  
clear all

