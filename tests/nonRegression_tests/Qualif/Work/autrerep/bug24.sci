// bug24

mode (-1)
clear all
//getf ('../fonct_qualif.sci')

function pascorrect = bug()
// This part replay bug
pascorrect=execstr("contour2d(1:10,1:10,rand(10,10),[0.1 0.5 0.9])","errcatch","n")
endfunction

affich_result(bug()==0, 24)
disp("Error n° "+string(bug()));
disp(lasterror());
clear all

//
// Une option de contour2d a ete oubliee dans la version 2.7
//
