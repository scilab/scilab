function [E_Value, M_Value] = ising2d(Var, J, H, Magnet, Connect)

    [nargout, nargin] = argn();

    M_Value_defined = (nargout==2);

    if (~isdef("Connect","local")) then
        Connect = %F;
    end

    if (~isdef("Magnet","local")) then
        Magnet = %F;
    end

    if (~isdef("H","local")) then
        H = 1.0;
    end

    Value = 0;
    //J = J/2;
    Size = size(Var,1);
    for i=2:Size-1
        for j=2:Size-1
            Value = Value + J*Var(i,j)*(Var(i-1,j) + Var(i+1,j) + Var(i,j-1) + Var(i,j+1));
        end
    end
    if (~Connect) then
        // First line
        for j=2:Size-1
            Value = Value + J*Var(1,j)*(Var(2,j) + Var(1,j-1) + Var(1,j+1));
        end
        // Last line
        for j=2:Size-1
            Value = Value + J*Var(Size,j)*(Var(Size-1,j) + Var(Size,j-1) + Var(Size,j+1));
        end
        // First column
        for i=2:Size-1
            Value = Value + J*Var(i,1)*(Var(i-1,1) + Var(i+1,1) + Var(i,2));
        end
        // Second column
        for i=2:Size-1
            Value = Value + J*Var(i,Size)*(Var(i-1,Size) + Var(i+1,Size) + Var(i,Size-1));
        end
        // First corner
        Value = Value + J*Var(1,1)*(Var(2,1) + Var(1,2));
        // Second corner
        Value = Value + J*Var(Size,1)*(Var(Size-1,1) + Var(1,2));
        // Third corner
        Value = Value + J*Var(1,Size)*(Var(2,Size) + Var(1,Size-1));
        // Fourth corner
        Value = Value + J*Var(Size,Size)*(Var(Size-1,Size) + Var(Size,Size-1));
    else
        // First line
        for j=2:Size-1
            Value = Value + J*Var(1,j)*(Var(Size,j) + Var(2,j) + Var(1,j-1) + Var(1,j+1));
        end
        // Last line
        for j=2:Size-1
            Value = Value + J*Var(Size,j)*(Var(Size-1,j) + Var(1,j) + Var(Size,j-1) + Var(Size,j+1));
        end
        // First column
        for i=2:Size-1
            Value = Value + J*Var(i,1)*(Var(i-1,1) + Var(i+1,1) + Var(i,Size) + Var(i,2));
        end
        // Second column
        for i=2:Size-1
            Value = Value + J*Var(i,Size)*(Var(i-1,Size) + Var(i+1,Size) + Var(i,Size-1) + Var(i,1));
        end
        // First corner
        Value = Value + J*Var(1,1)*(Var(Size,1) + Var(2,1) + Var(1,Size) + Var(1,2));
        // Second corner
        Value = Value + J*Var(Size,1)*(Var(Size-1,1) + Var(1,1) + Var(1,Size) + Var(1,2));
        // Third corner
        Value = Value + J*Var(1,Size)*(Var(Size,Size) + Var(2,Size) + Var(1,1) + Var(1,Size-1));
        // Fourth corner
        Value = Value + J*Var(Size,Size)*(Var(Size-1,Size) + Var(1,Size) + Var(Size,Size-1) + Var(Size,1));
    end

    E_Value = - Value;

    if (Magnet) then
        M_Value = - H * sum(Var);
        if (M_Value_defined) then
            E_Value = E_Value + M_Value;
        end
    end

endfunction
