// bug1110
mode (-1)
clear 
L=list(4,5,[1 2]);
ok=execstr('z=L(3)(:)(1)','errcatch')==21
L=list(4,5,[1 1/%s]);
ok=ok&execstr('z=L(3)(1,:)(2)','errcatch')==0&and(z==[1 1])

affich_result(ok, 1110)
clear 
