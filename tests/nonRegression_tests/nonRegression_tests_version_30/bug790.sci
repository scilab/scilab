// bug790
mode (-1);
clear;
disp('findobj no parameter');
affich_result(findobj() == [], 790)
disp('findobj 1 parameter');
affich_result(findobj('test') == [], 790)
disp('findobj 2 parameters');
affich_result(findobj('Param1','Param2') == [], 790)

clear;

