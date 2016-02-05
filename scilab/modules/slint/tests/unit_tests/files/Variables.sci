function y = Variables()
    sin(a); // a is not in this scope
    x = 123; // x is unused
    y = 0;
    for i=1:10 // i is unused but usefull for the loop
        y = y + 1;
    end
    [l, r] = argn(); // l, r unused
    [l1, r1] = argn();
    sin(r1); // r1 is used so l1 too
endfunction