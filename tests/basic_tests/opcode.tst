//a primitive as an argument of a primitive
//-----------------------------------------
function y=foo(),y=sin(cos),endfunction
//is it properly compiled
L=macr2lst(foo);
if or(L(5)<>["2","cos","-4","0"]) then pause,end // get variable named cos
if or(L(6)<>["2","sin","-2","1"]) then pause,end // get variable named sin (extraction)
if or(L(7)<>["5","25","2","1"]) then pause,end //realize sin(cos)
if or(L(8)<>["29","52","y","0"]) then pause,end //affectation

//check if it executes properly
function z=%fptr_sin(f), z=sin(f(%pi/6)),endfunction
if foo()<>sin(cos(%pi/6)) then pause,end

//check modified pseudocode
L1=macr2lst(foo);
if or(L1(5)<>["27","6","24","cos"]) then pause,end // create varfunptr
if or(L1(6)<>["20","sin","1","1"]) then pause,end // primitive all
if or(L1(7)<>["29","52","y","0"]) then pause,end //affectation
if foo()<>sin(cos(%pi/6)) then pause,end


//a primitive as an argument of a macro
//-----------------------------------------
function y=foo(),y=toto(cos),endfunction
L=macr2lst(foo);
if or(L(5)<>["2","cos","-4","0"]) then pause,end //get variable named cos
if or(L(6)<>["2","toto","-2","1"]) then pause,end // get variable named toto (extraction)
if or(L(7)<>["5","25","2","1"]) then pause,end //realize toto(cos)

//check if it executes properly
function y=toto(x),y=x(1),endfunction
if foo()<>cos(1) then pause,end

//check modified pseudocode
L1=macr2lst(foo);
if or(L1(5)<>["27","6","24","cos"]) then pause,end // create varfunptr
if or(L1(6)<>["2","toto","-2","1"]) then pause,end // primitive all
if or(L1(7)<>["5","25","2","1"]) then pause,end //affectation
if foo()<>cos(1) then pause,end

// unknown variable as an argument of a macro
//-----------------------------------------
clear toto
function y=foo(),y=toto(xxx),endfunction
L=macr2lst(foo);
if or(L(5)<>["2","xxx","-4","0"]) then pause,end //get variable named xxx
if or(L(6)<>["2","toto","-2","1"]) then pause,end // get variable named toto (extraction)
if or(L(7)<>["5","25","2","1"]) then pause,end //realize toto(xxx)

function y=toto(x),y=x(1),endfunction
xxx=cos;
if foo()<>cos(1) then pause,end
L1=macr2lst(foo);
if or(L1(5)<>["2","xxx","-4","0"]) then pause,end  //get variable named xxx
if or(L1(6)<>["2","toto","-2","1"]) then pause,end // get variable named toto (extraction)
if or(L1(7)<>["5","25","2","1"]) then pause,end //realize toto(xxx)
if foo()<>cos(1) then pause,end

//xxxx
//-----------------------------------------
function y=foo(),y=sin(cos(%pi/6)),endfunction
L=macr2lst(foo);
if foo()<>sin(cos(%pi/6)) then pause,end
if foo()<>sin(cos(%pi/6)) then pause,end

//affectation of a primitive
//-----------------------------------------

function y=foo(),y=sin,endfunction
L=macr2lst(foo);
if or(L(5)<>["2","sin","0","0"]) then pause,end // create varfunptr
if type(foo())<>130 then pause,end
L=macr2lst(foo);
if or(L(5)<>["27","6","23","sin"]) then pause,end // create varfunptr
if type(foo())<>130 then pause,end

return
sin=1
sin //devrait repondre 1 or il essaye d'executer sin
a=sin //est ok 

