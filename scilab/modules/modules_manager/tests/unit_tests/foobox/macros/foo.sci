function res = foo(txt, nb)
    // Tests for simple toolbox build
    // Syntax
    //  res = foo(A, B)
    // Parameters
    //  res: boolean. Return value
    //  txt: a single string
    //  nb: a numeric value
    // Description
    //  Does nothing but display txt and nb and returns %t
    // Author
    //  Pierre-Aimé Agnel
    disp(txt);
    disp(nb);
    res = %t;
endfunction
