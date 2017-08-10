// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function  C=instruction2code(I, bprettyprintformat)
    // Translate an instruction tlist to Scilab code (called by tree2code)
    // Input:
    // - I: instruction 'tree'
    // - bprettyprintformat: boolean value, if FALSE (default value), generated code is not formated else it is
    // Output:
    // - C: Scilab code corresponding to I

    // Default value
    rhs=argn(2)
    if rhs==1 then
        bprettyprintformat=%F
    end

    C=[]

    // -----------
    // Empty lines
    // -----------
    if I==list("EOL") then
        C=""
        return
    end

    // ---------------------------------------------
    // Generate code corresponding to a TRY-CATCH
    // ---------------------------------------------
    if typeof(I)=="trycatch" then

        //TRYCATCH
        C="try "
        [C,indent_space] = format_txt(C,I.trystat(1),bprettyprintformat); // Add EOL after while if needed and returns indent_space
        for k=1:size(I.trystat)
            C=cat_code(C,indent_space+instruction2code(I.trystat(k)))
            if k<size(I.trystat) then // Add EOL between statements if needed
                C = format_txt(C,I.trystat(k),bprettyprintformat,I.trystat(k+1));
            end
        end
        C = format_txt(C,I.trystat($),bprettyprintformat); // Add EOL after last statement if needed
        C=cat_code(C,"catch ")
        for k=1:size(I.catchstat)
            C=cat_code(C,indent_space+instruction2code(I.catchstat(k)))
            if k<size(I.catchstat) then // Add EOL between statements if needed
                C = format_txt(C,I.catchstat(k),bprettyprintformat,I.catchstat(k+1));
            end
        end
        C = format_txt(C,I.catchstat($),bprettyprintformat); // Add EOL after last statement if needed
        C=cat_code(C,"end")
        C($)=C($)+";"
        return
    end

    // ---------------------------------------------
    // Generate code corresponding to a IF-THEN-ELSE
    // ---------------------------------------------
    if typeof(I)=="ifthenelse" then

        // IF
        C="if "+expression2code(I.expression)+" then"
        [C,indent_space] = format_txt(C,I.then(1),bprettyprintformat); // Add EOL after then if needed and returns indent_space
        for k=1:size(I.then)
            C=cat_code(C,indent_space+instruction2code(I.then(k)))
            if k<size(I.then) then // Add EOL between then statements if needed
                C = format_txt(C,I.then(k),bprettyprintformat,I.then(k+1));
            end
        end
        C = format_txt(C,I.then($),bprettyprintformat); // Add EOL after last then statement if needed

        // ELSEIF
        if size(I.elseifs)<>0 then
            for k=1:size(I.elseifs)
                C=cat_code(C,"elseif "+expression2code(I.elseifs(k).expression)+" then")
                [C,indent_space] = format_txt(C,I.elseifs(k).then(1),bprettyprintformat); // Add EOL after then if needed and returns indent_space
                for l=1:size(I.elseifs(k).then)
                    C=cat_code(C,indent_space+instruction2code(I.elseifs(k).then(l)))
                    if l<size(I.elseifs(k).then) then // Add EOL between then statements
                        C = format_txt(C,I.elseifs(k).then(l),bprettyprintformat,I.elseifs(k).then(l+1));
                    end
                end
                C = format_txt(C,I.elseifs(k).then($),bprettyprintformat); // Add EOL after last then statement if needed
            end
        end

        // ELSE
        if size(I.else)<>0 then
            C=cat_code(C,"else")
        [C,indent_space] = format_txt(C,I.else(1),bprettyprintformat); // Add EOL after else if needed and returns indent_space
            for k=1:size(I.else)
            C=cat_code(C,indent_space+instruction2code(I.else(k)))
                if k<size(I.else) then // Add EOL between else statements if needed
                C = format_txt(C,I.else(k),bprettyprintformat,I.else(k+1));
                end
            end
        C = format_txt(C,I.else($),bprettyprintformat); // Add EOL after last else statement if needed
        end
        C=cat_code(C,"end")
        C($)=C($)+";"
        return
    end

    // --------------------------------------------
    // Generate code corresponding to a SELECT-CASE
    // --------------------------------------------
    if typeof(I)=="selectcase" then

        // SELECT
        C="select "+expression2code(I.expression(1))

        if size(I.expression)==1 // Not EOL and not comment after the expression
            if bprettyprintformat then
                C = cat_code(C,"") // Add EOL after expression
            end
        else
            for i=2:size(I.expression)
                C=cat_code(C," "+ instruction2code(I.expression(i)))
            end
        end

        // CASES
        if size(I.cases)<>0 then
            for k=1:size(I.cases)
                C=cat_code(C,"  case "+expression2code(I.cases(k).expression)+" then")
                [C,indent_space] = format_txt(C,I.cases(k).then(1),bprettyprintformat); // Add EOL after then if needed and returns indent_space
                if indent_space=="  " then // indent_space is modified because indentation differs from others control instructions
                    indent_space="    "
                end
                for l=1:size(I.cases(k).then)
                    C=cat_code(C,indent_space+instruction2code(I.cases(k).then(l)))
                    if l<size(I.cases(k).then) then // Add EOL between then statements if needed
                        C = format_txt(C,I.cases(k).then(l),bprettyprintformat,I.cases(k).then(l+1));
                    end
                end
                C = format_txt(C,I.cases(k).then($),bprettyprintformat); // Add EOL after last then statement if needed
            end
        end
        // ELSE
        if size(I.else)<>0 then
            C=cat_code(C,"  else")
        [C,indent_space] = format_txt(C,I.else(1),bprettyprintformat); // Add EOL after else if needed and returns indent_space
            if indent_space=="  " then // indent_space is modified because indentation differs from others control instructions
                indent_space="    "
            end
            for k=1:size(I.else)
            C=cat_code(C,indent_space+instruction2code(I.else(k)))
                if k<size(I.else) then // Add EOL between else statements if needed
                C = format_txt(C,I.else(k),bprettyprintformat,I.else(k+1));
                end
            end
        C = format_txt(C,I.else($),bprettyprintformat); // Add EOL after last else statement if needed
        end
        C=cat_code(C,"end")
        C($)=C($)+";"
        return
    end

    // --------------------------------------
    // Generate code corresponding to a WHILE
    // --------------------------------------
    if typeof(I)=="while" then

        C="while "+expression2code(I.expression)
        [C,indent_space] = format_txt(C,I.statements(1),bprettyprintformat); // Add EOL after while if needed and returns indent_space
        for k=1:size(I.statements)
            C=cat_code(C,indent_space+instruction2code(I.statements(k)))
            if k<size(I.statements) then // Add EOL between statements if needed
                C = format_txt(C,I.statements(k),bprettyprintformat,I.statements(k+1));
            end
        end
        C = format_txt(C,I.statements($),bprettyprintformat); // Add EOL after last statement if needed
        C=cat_code(C,"end")
        C($)=C($)+";"
        return
    end

    // ------------------------------------
    // Generate code corresponding to a FOR
    // ------------------------------------
    if typeof(I)=="for" then

        C="for "+instruction2code(I.expression)
        [C,indent_space] = format_txt(C,I.statements(1),bprettyprintformat); // Add EOL after while if needed and returns indent_space
        for k=1:size(I.statements)
            C=cat_code(C,indent_space+instruction2code(I.statements(k)))
            if k<size(I.statements) then // Add EOL between statements if needed
                C = format_txt(C,I.statements(k),bprettyprintformat,I.statements(k+1));
            end
        end
        C = format_txt(C,I.statements($),bprettyprintformat); // Add EOL after last statement if needed
        C=cat_code(C,"end")
        C($)=C($)+";"
        return
    end

    // --------------------------------------
    // Generate code corresponding to a EQUAL
    // --------------------------------------
    if typeof(I)=="equal" then

        // Comments
        if typeof(I.expression)=="funcall" then
            if I.expression.name=="%comment" then
                I.expression.rhs(1).value=strsubst(I.expression.rhs(1).value,"""""","""")
                I.expression.rhs(1).value=strsubst(I.expression.rhs(1).value,"''''","''")
                C="//"+I.expression.rhs(1).value
                //C($)=C($)+";"
                return
            end
        end

        // Other EQUAL instruction
        if size(I.lhs)==1 then
            if typeof(I.lhs(1))=="variable" then
                if I.lhs(1).name=="ans" then // expression
                    C=rhs2code(I.expression)
                else
                    RHS=rhs2code(I.expression)
                    if size(RHS,"*")==1 then
                        C=I.lhs(1).name+" = "+rhs2code(I.expression)
                    else // Multi-line definition
                        C=[I.lhs(1).name+" = "+RHS(1);"     "+RHS(2:$)]
                    end
                end
            else // Insertion...
                C=expression2code(I.lhs(1))+" = "+rhs2code(I.expression)
            end
        else
            lhsnames=[]
            for lhsind=1:size(I.lhs)
                lhsnames=[lhsnames,expression2code(I.lhs(lhsind))]
            end
            if strcat(lhsnames,",")<>"" then
                C="["+strcat(lhsnames,",")+"] = "+rhs2code(I.expression)
            else
                C=rhs2code(I.expression)
            end
        end
        C($)=C($)+I.endsymbol
        //C($)=C($)+";";
        return
    end

    // --------------------------------------
    // Generate code corresponding to a comment
    // --------------------------------------
    if typeof(I)=="comment" then
        C="//"+I.text
        //C = cat_code(C,"//"+I.text)
        return
    end

    // ---------------------------------------
    // Generate code corresponding to sup_equal
    // ---------------------------------------
    if typeof(I)=="sup_equal" then
        while typeof(I.sup_instr(1))=="equal" | I.sup_instr(1)==list("EOL")
            if I.sup_instr(1)==list("EOL") then //Instruction is an EOL
                I.sup_instr(1)=null()
            elseif typeof(I.sup_instr(1))=="equal" then //Instruction is acomment
                if typeof(I.sup_instr(1).expression)=="funcall" then
                    break
                end
            end
        end
        //Optimize the code if all sup_intr are equal tlists and expression of this equal tlists are temporaries variables (not a function)
        if size(I.sup_instr)==I.nb_opr+1 then
            for i=size(I.sup_instr):-1:2
                optim_instr=%f
                if typeof(I.sup_instr(i))=="equal" then
                    if typeof(I.sup_instr(i).expression)=="variable" then
                        j=0
                        while ~optim_instr & j<=size(I.sup_instr(1).lhs)
                            j=j+1
                            optim_instr=I.sup_instr(i).expression.name==I.sup_instr(1).lhs(j).name
                        end
                    end
                end
                if optim_instr then
                    I.sup_instr(1).lhs(j)=I.sup_instr(i).lhs(1)
                    I.sup_instr(i)=null()
                end
            end
        end
        for i=1:size(I.sup_instr)
            C($+1)=instruction2code(I.sup_instr(i))
        end
        return
    end

    // ----------------------------------------------------
    // Generate code corresponding to a function definition
    // ----------------------------------------------------
    if typeof(I)=="inline" then
        C = "function "+I.prototype;
        C = cat_code(C,I.definition)
        C($+1) = "endfunction";
        return
    end

    // -------
    // Command
    // -------
    if and(typeof(I)<>["funcall" "variable", "comment"]) then
        disp("instruction2code: bug in macr2tree() !");
        pause
    end
    C=expression2code(I);
    C($)=C($)+";"
endfunction

