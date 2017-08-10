// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function  variablename=variablesearch(instr,variablename)
    //  VARIABLESEARCH recursive function (used by "translatepaths" function)
    //  Searches names of declared variables for each instruction of mtlbtree
    //  Output
    //  -variablename : a vector which contains the names of declared variables
    //  -instr : mtlbtree instruction

    // case : ifthenelse instruction
    if typeof(instr) == "ifthenelse" then
        for i=1:size(instr.then)
            [variablename]=variablesearch((instr.then(i)),variablename)
        end
        for i=1:size(instr.elseifs)
            for k=1:size(instr.elseifs(i).then)
                [variablename]=variablesearch((instr.elseifs(i).then(k)),variablename)
            end
        end
        for i=1:size(instr.else)
        [variablename]=variablesearch((instr.else(i)),variablename)
        end
        // case : selectcase instruction
    elseif typeof(instr) == "selectcase" then
        for i=1:size(instr.cases)
            [variablename]=variablesearch(instr.cases(i).expression,variablename)
            for j=1:size(instr.cases(i).then)
                [variablename]=variablesearch((instr.cases(i).then(j)),variablename)
            end
        end
        for i=1:size(instr.else)
        [variablename]=variablesearch(instr.else(i),variablename)
        end
        // case : while instruction
    elseif typeof(instr) == "while" then
        for i=1:size(instr.statements)
            [variablename]=variablesearch(instr.statements(i),variablename)
        end
        // case : for instruction
    elseif typeof(instr) == "for" then
        [variablename]=variablesearch(instr.expression,variablename)
        for i=1:size(instr.statements)
            [variablename]=variablesearch(instr.statements(i),variablename)
        end
        // case : equal instruction
    elseif typeof(instr) == "equal" then
        for i=1:size(instr.lhs)
            [variablename]=variablesearch(instr.lhs(i),variablename)
        end
        // case : operation instruction
    elseif typeof(instr) == "operation" then
        if instr.operator=="ins" then
            if find(instr.operands(1).name==variablename)==[] then
                variablename($+1)=instr.operands(1).name
            end
        end
        // case : variable instruction
    elseif typeof(instr) == "variable" then
        if find(instr.name==variablename)==[] & instr.name<>"ans" then
            variablename($+1)=instr.name
        end
    end

endfunction
