// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
