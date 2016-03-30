// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
