// bug474

mode (-1)
clear

//correct=%F
a=matrix(1:9,3,3)
[mia,kia]=min(a) 
[maa,kaa]=max(a)

b=int16(matrix(1:9,3,3))
[mib,kib]=min(b)
[mab,kab]=max(b)

affich_result(mia==mib, 474.1)
affich_result(and(kia==kib), 474.2)
affich_result(maa==mab, 474.3)
affich_result(and(kaa==kab), 474.4)

clear

//
//The Error Messages are:
//   kib and kab should be a 2 element vector!!    




