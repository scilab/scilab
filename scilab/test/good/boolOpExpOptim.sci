function r=plop()
    disp("plop")
    r = %f
endfunction

function r=plip()
    disp("plip")
    r = %t
endfunction

plop() & plip()
plip() | plop()

if(plop() & plip())
    disp("then...")
else
    disp("else...")
end

if(plip() | plop())
    disp("then...")
else
    disp("else...")
end

while(plip() | plop())
    disp("while |...")
    break;
end

while(plop() & plip())
    disp("while &...")
    break;
end