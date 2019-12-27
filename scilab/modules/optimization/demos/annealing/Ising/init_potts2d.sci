function Var = init_potts2d(Size, Level, Proba)
    if ~isdef("Level","local") then
        Level = 2;
    end
    if ~isdef("Proba","local") then
        Proba = 0.5;
    end
    Var = zeros(Size,Size);
    for i=1:Size
        for j=1:Size
            Value = floor(rand(1,1)*Level);
            if (rand(1,1)<Proba) then
                Var(i,j) = -Value;
            else
                Var(i,j) = Value;
            end
        end
    end
endfunction
