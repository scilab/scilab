// bug71

mode (-1)
clear all
//getf ('../fonct_qualif.sci')

function pascorrect = bug()
// This part replay bug
deff("foo","exit")
pascorrect=execstr("macr2lst(foo)","errcatch","n")
clear foo;
endfunction

affich_result(bug()==0, 71)

disp("Error n° "+string(bug()));
disp(lasterror());

clear all

// Error message :
//macr2lst(foo)
//              !--error    37 
//incorrect function at line ******
