function Var = init_ising2d(Size, Proba)
    if (~isdef("Proba","local")) then
        Proba = 0.5;
    end
    Var = 2*(rand(Size,Size)<Proba)-1;
endfunction

