// exec("e:\testNonReg\bug573.sce")

disp("windows only, selection a directory and click OK");

tk_getdir("c:/has space")  

tk_getfile('*','C:/has space');

affich_result(%T,573);