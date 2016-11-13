// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function C=expression2code(e)
    // Translate an expression tree to macro code (called by tree2code)
    // Input:
    // - e: expression 'tree'
    // Output:
    // - C: Scilab code corresponding to e
    // V.C.

    // Change format for constants
    fmtsav=format();
    format(16);

    // Tables of symbols arranged by priority in computations
    sumops=["+","-","&","|"] //1
    prodops=["*","/",".*","./","\",".\","^",".^"] //2
    othops=["==",">=","<=","~=",">","<","~",".''",".''",":"] //3

    C=""
    select typeof(e)
        // ---------
        // OPERATION
        // ---------
    case "operation" then
        operator=e.operator
        operands=[]
        nb_op=size(e.operands)
        if and(operator<>["cc","cceol"]) then
            for i=1:nb_op
                if e.operands(i)==list("EOL") then
                    continue
                end
                expression = expression2code(e.operands(i));
                if expression == "eye()" then
                    expression = ":";
                end
                operands=[operands;expression]
            end
        end

        // Row concatenation
        if operator=="rc" then
            for i=1:nb_op
                if typeof(e.operands(i))=="operation" then
                    if e.operands(i).operator=="rc" then
                        operands(i)=part(operands(i),2:length(operands(i))-1)
                    elseif or(e.operands(i).operator==["cceol"]) then
                        operands(1)=part(operands(1),2:length(operands(1)))
                        operands($)=part(operands($),1:length(operands($))-1)
                    end
                end
            end
            C="["+strcat(operands,",")+"]"
            // Multi-line column concatenation
        elseif operator=="cceol" then
            for i=1:nb_op
                opi=expression2code(e.operands(i))
                if typeof(e.operands(i))=="operation" then
                    if e.operands(i).operator=="rc" then
                        opi=part(opi,2:length(opi)-1)
                    elseif or(e.operands(i).operator==["cc","cceol"]) then
                        opi(1)=part(opi(1),2:length(opi(1)))
                        opi($)=part(opi($),1:length(opi($))-1)
                    end
                end

                if i==1 then
                    C="["
                    if size(opi,"*")>1 then
                        C = [C+opi(1);opi(2:$)]
                    else
                        C = C+opi
                    end
                    C($)=C($)+";"
                    C=[C;""]
                else
                    if size(opi,"*")>1 then
                        C = [C(1:$-1);C($)+opi(1);opi(2:$)]
                    else
                        C = [C(1:$-1);C($)+opi]
                    end
                    C($)=C($)+"]"
                end
            end
            // Column concatenation
        elseif operator=="cc" then
            C="["
            for i=1:nb_op
                opi=expression2code(e.operands(i))
                // Delete [ and ] if there are...
                if typeof(e.operands(i))=="operation" then
                    if e.operands(i).operator=="rc" then
                        opi=part(opi,2:length(opi)-1)
                    elseif or(e.operands(i).operator==["cc","cceol"]) then
                        opi(1)=part(opi(1),2:length(opi(1)))
                        opi($)=part(opi($),1:length(opi($))-1)
                    end
                end
                if i==1 then
                    if size(opi,"*")>1 then
                        C = [C+opi(1);opi(2:$)]
                    elseif opi=="(EOL)" then
                        C = [C;""];
                    else
                        C = C+opi
                    end
                    C($)=C($)+";"
                else
                    if size(opi,"*")>1 then
                        C = [C(1:$-1);C($)+opi(1);opi(2:$)]
                    elseif opi=="(EOL)" then
                        C = [C;""]
                    else
                        C = [C(1:$-1);C($)+opi]
                    end
                end
            end
            C($)=C($)+"]"
            // Extraction
        elseif operator=="ext" then
            if size(e.operands)==1 then
                C=e.operands(1).name
                return
            end
            if type(e.operands(2))==15 then // Recursive extraction
                C=operands(1)+operands(2)
            else
                // Deal with :
                for k=2:size(operands,"*")
                    if operands(k)==""":""" then
                        operands(k)=":"
                    elseif operands(k)=="""$""" then
                        operands(k)="$"
                    elseif operands(k)=="""*""" then // Only used for M2SCI
                        operands(k)="*"
                    end
                end
                val = part(operands(2),1)=="""" & part(operands(2),length(operands(2)))==""""
                if val then // struct field
                    C=operands(1)+"."+evstr(operands(2))
                    if size(operands,"*")>=3 then
                        C=C+"("
                    end
                    for k=3:size(operands,"*")
                        C=C+","+operands(k)
                    end
                    if size(operands,"*")>=3 then
                        C=C+")"
                    end
                else
                    C=operands(1)+"("+operands(2)
                    for k=3:size(operands,"*")
                        C=C+","+operands(k)
                    end
                    C=C+")"
                end
            end
            // Insertion
        elseif operator=="ins" then
            if type(e.operands(2))==15 then // Recursive insertion
                C=operands(1)+operands(2)
            else
                // Deal with :
                for k=2:size(operands,"*")
                    if operands(k)==""":""" then
                        operands(k)=":"
                    elseif operands(k)=="""$""" then
                        operands(k)="$"
                    elseif operands(k)=="""*""" then // Only used in M2SCI
                        operands(k)="*"
                    end
                end
                val = part(operands(2),1)=="""" & part(operands(2),length(operands(2)))==""""
                if val then // struct field
                    C=operands(1)+"."+evstr(operands(2))
                    if size(operands,"*")>=3 then
                        C=C+"("
                    end
                    for k=3:size(operands,"*")
                        C=C+","+operands(k)
                    end
                    if size(operands,"*")>=3 then
                        C=C+")"
                    end
                else
                    C=operands(1)+"("+operands(2)
                    for k=3:size(operands,"*")
                        C=C+","+operands(k)
                    end
                    C=C+")"
                end
            end
            // Unary Operators
        elseif size(operands,"*")==1 then
            if or(operator==["''",".''"]) then
                if typeof(e.operands(1))=="operation" then
                    if and(e.operands(1).operator<>["rc","cc","-","+"]) then
                        operands="("+operands+")"
                    end
                end
                C=operands+operator
            elseif or(operator==["+","-"]) then
                if typeof(e.operands(1))=="operation" then
                    if or(e.operands(1).operator==["-","+"]) then
                        operands="("+operands+")"
                    end
                end
                C=operator+operands
            else
                C=operator+operands
            end
            // Other operators
        else
            // Parenthesize
            if or(operator==["+","-"]) then
                for i=1:nb_op
                    if typeof(e.operands(i))=="operation" then
                        if or(e.operands(i).operator==othops) then
                            operands=[operands(1:i-1) "("+operands(i)+")" operands(i+1:$)]
                        end
                    end
                end
                for i=2:nb_op
                    if typeof(e.operands(i))=="operation" then
                        if or(e.operands(i).operator==sumops) then
                            operands=[operands(1:i-1) "("+operands(i)+")" operands(i+1:$)]
                        end
                    end
                end
            end
            if or(operator==[prodops,othops]) & (operator<>":") then
                if typeof(e.operands(1))=="operation" then
                    if or(e.operands(1).operator==[sumops,prodops,othops]) then
                        operands(1)="("+operands(1)+")"
                    end
                end
                if typeof(e.operands(2))=="operation" then
                    if or(e.operands(2).operator==[sumops,prodops,othops]) then
                        operands(2)="("+operands(2)+")"
                    end
                end
            end
            if part(operator,1)=="." & part(operator,length(operator))=="." | or(operator==["&","|"]) then
                C=strcat(operands," "+operator+" ")
            elseif part(operator,1)=="." then
                C=strcat(operands," "+operator)
            elseif part(operator,length(operator))=="." then
                C=strcat(operands,operator+" ")
            else
                C=strcat(operands,operator)
            end
        end
        // --------
        // CONSTANT
        // --------
    case "cste" then
        C=sci2exp(e.value)
        if C==""":""" then
            //C=":"
        elseif C=="""$""" then
            C="$"
        elseif C=="""*""" then // Only used in M2SCI
            C="""*"""
            //  C="*"
        end
        // --------
        // VARIABLE
        // --------
    case "variable" then
        C=e.name
        // ----------------
        // CHARACTER STRING
        // ----------------
    case "string" then
        C=e
        // -------
        // FUNCALL
        // -------
    case "funcall" then
        if size(e.rhs)==0 then
            I=I; // I is defined in instruction2code
            C=e.name
            [l,mac]=where()
            if size(grep(mac,"expression2code"),"*")>1 then
                C=C+"()"
            elseif typeof(I)=="equal" & ( (typeof(I.lhs(1))=="variable" & I.lhs(1).name<>"ans") | typeof(I.lhs(1))=="operation") then
                C=C+"()"
            end
        else
            rhscode = rhs2code(e.rhs);
            if size(rhscode,"*")==1 then
                C=[e.name+"("+rhscode+")"]
            else
                C=[e.name+"("+rhscode(1);rhscode(2:($-1));rhscode($)+")"]
            end
        end
        // ----
        // LIST
        // ----
    case "list"
        // Recursive extraction
        C=""
        for k=1:lstsize(e)
            ind=expression2code(e(k))
            if type(e(k))==15 then // Recursive extraction in recursive extraction
                ind=strsubst(ind,")(",",")
                if part(ind, 1)=="(" then
                    ind = part(ind, 2:length(ind))
                end
                if part(ind, length(ind))==")" then
                    ind = part(ind, 1:(length(ind)-1))
                end
            end
            if ind==""":""" then
                ind=":"
            elseif ind=="""$""" then
                ind="$"
            elseif ind=="""*""" then // Only used in M2SCI
                ind="*"
            end
            val = part(ind,1)=="""" & part(ind,length(ind))==""""
            if val then
                C=C+"."+evstr(ind)
            else
                C=C+"("+ind+")"
            end
        end
        C=strsubst(C,")(",",")
        // -----
        // EQUAL (can occur fir disp(a=1) for example)
        // -----
    case "equal"
        C=instruction2code(e)
        // ------
        // INLINE
        // ------
    case "inline"
        C=["function " + e.prototype;"  " + e.definition;"endfunction"];
        // -------
        // COMMENT (inside a matrix declaration for example)
        // -------
    case "comment"
        C=instruction2code(e)
    else
        error(msprintf(gettext("%s: This feature has not been implemented: %s.\n"),"expression2code",typeof(e)));
    end

    // Restore format
    if fmtsav(1)==1 then
        format("v",fmtsav(2));
    else
        format("e",fmtsav(2));
    end
endfunction


