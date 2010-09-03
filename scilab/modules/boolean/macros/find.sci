// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function ret=find(refValue, maxValues)
//return index of true or non null values
    rhs=argn(2);
    if rhs == 1 then
        maxValues = -1;//no check
    end
    ret =[];    
    //double or boolean
    if(type(refValue) <> 1 && type(refValue) <> 4)
	//TODO: Insert error message when localization will be added in Scilab 6
        return;
    end
    
    //boolean case
    if type(refValue) == 4 then
        for i = 1 : size(refValue, "*")
            if refValue(i) <> %f then
                ret = [ret, i];
            end
            
            if size(ret, "*") == maxValues then
            return;
            end
        end
    else //double case
        for i = 1 : size(refValue, "*")
            if refValue(i) <> 0 then
                ret = [ret, i];
            end
            
            if size(ret, "*") == maxValues then
            return;
            end
        end
    end
endfunction
