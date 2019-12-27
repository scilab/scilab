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
