// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [converted_instr, nblines] = instruction2sci(mtlb_instr,nblines)

    // Convertion of empty lines
    if mtlb_instr==list("EOL") then
        converted_instr=list("EOL")
        nblines = nblines + 1;
        return
    end

    if typeof(mtlb_instr)=="equal" then
        [converted_instr] = equal2sci(mtlb_instr)

    elseif or(typeof(mtlb_instr)==["ifthenelse","selectcase","for","while","trycatch"]) then
        [converted_instr, nblines] = clause2sci(mtlb_instr, nblines)

    elseif or(typeof(mtlb_instr)==["funcall","cste","operation","variable"]) then
        converted_instr = expression2sci(mtlb_instr)

    elseif typeof(mtlb_instr)=="comment" then
        converted_instr = mtlb_instr

    else
        error(gettext("unknown instruction type %s."),typeof(mtlb_instr))
    end

endfunction

// ---------------------------------------------------------------------------

function sci_instr = equal2sci(mtlb_instr)
    // PRIVATE INTERNAL function called only by instruction2sci()
    //
    // Conversion of a Matlab instruction or expression to Scilab
    // Input:
    // - mtlb_instr: Matlab instr or expression to convert
    // Output:
    // - sci_instr: Scilab equivalent for mtlb_instr

    // Trees to insert in converted function tree
    global("m2sci_to_insert_b")
    m2sci_to_insert_b=list()
    global("m2sci_to_insert_a")
    m2sci_to_insert_a=list()
    global("varslist")

    sci_instr=mtlb_instr

    // Add inference data to lhs
    lhslist=list()

    // Inference field added to each lhs argument
    // Get infos in varslist to init
    for k=1:size(mtlb_instr.lhs)
        if typeof(mtlb_instr.lhs(k))=="variable" then
            [bval,index]=isdefinedvar(mtlb_instr.lhs(k))
            if bval then
                INFER=varslist(index).infer
            else
                INFER=Infer()
            end
            lhslist($+1)=Variable(mtlb_instr.lhs(k).name,INFER)
        elseif typeof(mtlb_instr.lhs(k))=="operation" then
            if mtlb_instr.lhs(k).operator<>"ins" then
                error(msprintf(gettext("Wrong LHS ''%s'' operation: Insertion expected.\n"),mtlb_instr.lhs(k).operator))
            end

            [bval,index]=isdefinedvar(mtlb_instr.lhs(k).operands(1))
            if bval then
                INFER=varslist(index).infer
            else
                INFER=Infer()
            end

            lhslist($+1)=Operation(mtlb_instr.lhs(k).operator,..
            mtlb_instr.lhs(k).operands,..
            list(Variable(mtlb_instr.lhs(k).operands(1).name,INFER)))
        else
            error(msprintf(gettext("lhs cannot be a %s."),typeof(mtlb_instr.lhs(k))))
        end
    end

    // Convert expression

    [sci_expr]=expression2sci(mtlb_instr.expression,lhslist);

    if sci_expr==list() then // Conversion made by inserted instructions or 'm2scideclare'
        sci_instr=list()
    else

        sci_instr.expression=sci_expr;

        // Update lhs of instruction
        select typeof(sci_instr.expression)
        case "operation" then
            sci_instr.lhs=sci_expr.out;
        case "funcall" then
            sci_instr.lhs=sci_instr.expression.lhs
            if typeof(mtlb_instr.expression)=="funcall" then
                sci_instr.lhs=sci_expr.lhs;
            end
        case "cste" then
            sci_instr.lhs=lhslist;
            sci_instr.lhs(1).dims=sci_expr.dims;
            sci_instr.lhs(1).type=sci_expr.type;
        case "variable" then
            sci_instr.lhs=lhslist;
            sci_instr.lhs(1).dims=sci_expr.dims;
            sci_instr.lhs(1).type=sci_expr.type;
        else
            error(msprintf(gettext("%s is not yet implemented."),typeof(sci_instr.expression)));
        end

        // If lhs are insertion operation, they also have to be converted
        for k=1:size(sci_instr.lhs)
            if typeof(sci_instr.lhs(k))=="operation" then
                sci_instr.lhs(k).operands($+1)=sci_instr.expression
                // Keep just one inference field in sci_instr.expression (if is a funcall) so that inference can be made in operation2sci()
                if typeof(sci_instr.lhs(k).operands($))=="funcall" then
                    for l=1:size(sci_instr.lhs(k).operands($).lhs)
                        if l<>k then
                            sci_instr.lhs(k).operands($).lhs(l)=list()
                        end
                    end
                    l=1
                    while l<=size(sci_instr.lhs(k).operands($).lhs)
                        if sci_instr.lhs(k).operands($).lhs(l)==list() then
                            sci_instr.lhs(k).operands($).lhs(l)=null()
                        else
                            l=l+1
                        end
                    end
                    // Verify that there is just one lhs kept
                    if size(sci_instr.lhs(k).operands($).lhs)<>1 then pause;end
                end
                // If insertion made in an unknown variable, I add it to varslist
                inservar=sci_instr.lhs(k).operands(1)
                [bval,index]=isdefinedvar(inservar)
                if ~bval then
                    // Variable added to varslist before insertion
                    if funptr(inservar.name)<>0 then
                        matname="%"+inservar.name
                    else
                        matname=inservar.name
                    end
                    if sci_instr.expression.vtype==Struct then
                        // Variable is initialized to struct() in converted script is does not already exist
                        varslist($+1)=M2scivar(matname,inservar.name,Infer(list(0,0),Type(Struct,Unknown)))
                        //m2sci_to_insert_b($+1)=Equal(list(inservar),Funcall("struct",1,list(),list()))
                    elseif sci_instr.expression.vtype==Cell then
                        // Variable is initialized to cell() in converted script is does not already exist
                        varslist($+1)=M2scivar(matname,inservar.name,Infer(list(0,0),Type(Cell,Unknown)))
                        //m2sci_to_insert_b($+1)=Equal(list(inservar),Funcall("cell",1,list(),list()))
                    else
                        // Variable is initialized to [] in converted script is does not already exist
                        varslist($+1)=M2scivar(matname,inservar.name,Infer(list(0,0),Type(Double,Real)))
                        //m2sci_to_insert_b($+1)=Equal(list(inservar),Cste([]))
                    end
                    sci_instr.lhs(k).out(1).infer=varslist($).infer
                else
                    sci_instr.lhs(k).out(1).infer=varslist(index).infer
                end
                [sci_instr.lhs(k)]=operation2sci(sci_instr.lhs(k))
                if typeof(sci_instr.lhs(k))=="operation" then
                    if or(sci_instr.lhs(k).operands($)<>sci_instr.expression) then // Update expression if has been modified while converting lhs
                        sci_instr.expression=sci_instr.lhs(k).operands($)
                    end

                    sci_instr.lhs(k).operands($)=null()
                    updatevarslist(sci_instr.lhs(k).out)
                else
                    // Insertion done by inserted instruction
                    sci_instr=list()
                    return
                end
            end
        end
        // Update varslist
        updatevarslist(sci_instr.lhs);
    end



endfunction

// ---------------------------------------------------------------------------

function [sci_clause, nblines] = clause2sci(mtlb_clause, nblines, leveltemp)
    // M2SCI function

    // Global variables for M2SCI
    global("m2sci_to_insert_b")
    global("varslist")

    // Increment level of clause indentation
    level

    // Temp variable used to store instructions to insert before clause
    to_insert=list()
    select typeof(mtlb_clause)
        // --- TRYCATCH ---
    case "trycatch"
        level=[level;0]

        // Get instructions to insert if there are
        if m2sci_to_insert_b<>list() then
            to_insert=m2sci_to_insert_b
            m2sci_to_insert_b=list()
        end

        // Convert try
        sci_try=list()
        level($)=level($)+1
        for k=1:size(mtlb_clause.trystat)
            if typeof(mtlb_clause.trystat(k))=="sup_equal" then
                sci_try_temp=list()
                for i=1:size(mtlb_clause.trystat(k).sup_instr)
                    [instr,nblines]=instruction2sci(mtlb_clause.trystat(k).sup_instr(i),nblines)
                    sci_try_temp=update_instr_list(sci_try_temp,instr)
                end
                sci_try($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_try_temp,mtlb_clause.trystat(k).nb_opr)
            else
                [instr,nblines]=instruction2sci(mtlb_clause.trystat(k),nblines)
                sci_try=update_instr_list(sci_try,instr)
            end
        end

        // Convert catch
        sci_catch=list()
        level($)=level($)+1
        for k=1:size(mtlb_clause.catchstat)
            if typeof(mtlb_clause.catchstat(k))=="sup_equal" then
                sci_catch_temp=list()
                for i=1:size(mtlb_clause.catchstat(k).sup_instr)
                    [instr,nblines]=instruction2sci(mtlb_clause.catchstat(k).sup_instr(i),nblines)
                    sci_catch_temp=update_instr_list(sci_catch_temp,instr)
                end
                sci_catch($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_catch_temp,mtlb_clause.catchstat(k).nb_opr)
            else
                [instr,nblines]=instruction2sci(mtlb_clause.catchstat(k),nblines)
                sci_catch=update_instr_list(sci_catch,instr)
            end
        end

        // Create Scilab trycatch
        sci_clause=tlist(["trycatch","trystat","catchstat"],sci_try,sci_catch)
        level($)=level($)+1
        updatevarslist("END OF CLAUSE")

        // --- IF ---
    case "ifthenelse"
        level=[level;0]

        // Convert expression
        [sci_expr]=expression2sci(mtlb_clause.expression)

        // Get instructions to insert if there are
        if m2sci_to_insert_b<>list() then
            to_insert=m2sci_to_insert_b
            m2sci_to_insert_b=list()
        end

        // Convert then statements
        sci_then=list()
        level($)=level($)+1
        for k=1:size(mtlb_clause.then)
            if typeof(mtlb_clause.then(k))=="sup_equal" then
                sci_then_temp=list()
                for i=1:size(mtlb_clause.then(k).sup_instr)
                    [instr,nblines]=instruction2sci(mtlb_clause.then(k).sup_instr(i),nblines)
                    sci_then_temp=update_instr_list(sci_then_temp,instr)
                end
                sci_then($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_then_temp,mtlb_clause.then(k).nb_opr)
            else
                [instr,nblines]=instruction2sci(mtlb_clause.then(k),nblines)
                sci_then=update_instr_list(sci_then,instr)
            end
        end

        // Convert elseifs
        sci_elseifs=list()
        for k=1:size(mtlb_clause.elseifs)
            level($)=level($)+1

            // Convert expression
            [sci_exprn]=expression2sci(mtlb_clause.elseifs(k).expression)
            // Get instructions to insert if there are
            if m2sci_to_insert_b<>list() then
                to_insert=m2sci_to_insert_b
                m2sci_to_insert_b=list()
            end

            // Convert statements
            sci_stat=list()
            for l=1:size(mtlb_clause.elseifs(k).then)
                if typeof(mtlb_clause.elseifs(k).then(l))=="sup_equal" then
                    sci_stat_temp=list()
                    for i=1:size(mtlb_clause.elseifs(k).then(l).sup_instr)
                        [instr,nblines]=instruction2sci(mtlb_clause.elseifs(k).then(l).sup_instr(i),nblines)
                        sci_stat_temp=update_instr_list(sci_stat_temp,instr)
                    end
                    sci_stat($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_stat_temp,mtlb_clause.elseifs(k).then(l).nb_opr)
                else
                    [instr,nblines]=instruction2sci(mtlb_clause.elseifs(k).then(l),nblines)
                    sci_stat=update_instr_list(sci_stat,instr)
                end
            end
            sci_elseifs($+1)=tlist(["elseif","expression","then"],sci_exprn,sci_stat)
        end

        // Convert else
        sci_else=list()
        if size(mtlb_clause.else)<>0 then
            level($)=level($)+1
        end
        for k=1:size(mtlb_clause.else)
            if typeof(mtlb_clause.else(k))=="sup_equal" then
                sci_else_temp=list()
                for i=1:size(mtlb_clause.else(k).sup_instr)
                [instr,nblines]=instruction2sci(mtlb_clause.else(k).sup_instr(i),nblines)
                    sci_else_temp=update_instr_list(sci_else_temp,instr)
                end
            sci_else($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_else_temp,mtlb_clause.else(k).nb_opr)
            else
                [instr, nblines] = instruction2sci(mtlb_clause.else(k),nblines)
                sci_else = update_instr_list(sci_else,instr)
            end
        end

        // Create Scilab ifthenelse
        sci_clause=tlist(["ifthenelse","expression","then","elseifs","else"],sci_expr,sci_then,sci_elseifs,sci_else)
        level($)=level($)+1
        updatevarslist("END OF CLAUSE")

        // --- SELECT ---
    case "selectcase"
        level=[level;0]
        // Convert expression
        sci_expr=list()
        [sci_expr(1)]=expression2sci(mtlb_clause.expression(1))
        for i=2:size(mtlb_clause.expression)
            sci_expr(i)=mtlb_clause.expression(i) // EOL or comment
        end

        // Get instructions to insert if there are
        if m2sci_to_insert_b<>list() then
            to_insert=m2sci_to_insert_b
            m2sci_to_insert_b=list()
        end

        // Convert cases
        sci_cases=list()
        k=0
        while k<size(mtlb_clause.cases)
            k=k+1
            level($)=level($)+1
            // Convert expression
            if typeof(mtlb_clause.cases(k).expression)=="funcall" then
                if mtlb_clause.cases(k).expression.name=="makecell" then
                    // Insert new cases
                    for nbcas=size(mtlb_clause.cases):-1:k+1
                        mtlb_clause.cases(nbcas+size(mtlb_clause.cases(k).expression.rhs))=mtlb_clause.cases(nbcas)
                    end
                    for nbrhs=1:size(mtlb_clause.cases(k).expression.rhs)
                        mtlb_clause.cases(nbrhs+k)=tlist(["case","expression","then"],mtlb_clause.cases(k).expression.rhs(nbrhs),mtlb_clause.cases(k).then)
                    end
                    mtlb_clause.cases(k)=null()
                end
            end
            [sci_exprn]=expression2sci(mtlb_clause.cases(k).expression)
            // Get instructions to insert if there are
            if m2sci_to_insert_b<>list() then
                to_insert=m2sci_to_insert_b
                m2sci_to_insert_b=list()
            end

            // Convert statements
            sci_stat=list()
            for l=1:size(mtlb_clause.cases(k).then)
                if typeof(mtlb_clause.cases(k).then(l))=="sup_equal" then
                    sci_stat_temp=list()
                    for i=1:size(mtlb_clause.cases(k).then(l).sup_instr)
                        [instr,nblines]=instruction2sci(mtlb_clause.cases(k).then(l).sup_instr(i),nblines)
                        sci_stat_temp=update_instr_list(sci_stat_temp,instr)
                    end
                    sci_stat($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_stat_temp,mtlb_clause.cases(k).then(l).nb_opr)
                else
                    [instr,nblines]=instruction2sci(mtlb_clause.cases(k).then(l),nblines)
                    sci_stat=update_instr_list(sci_stat,instr)
                end
            end
            sci_cases($+1)=tlist(["case","expression","then"],sci_exprn,sci_stat)
        end

        // Convert else
        sci_else=list()
        if size(mtlb_clause.else)<>0 then
            level($)=level($)+1
        end
        for k=1:size(mtlb_clause.else)
            if typeof(mtlb_clause.else(k))=="sup_equal" then
                sci_else_temp=list();
                for i=1:size(mtlb_clause.else(k).sup_instr)
                [instr,nblines]=instruction2sci(mtlb_clause.else(k).sup_instr(i),nblines)
                    sci_else_temp=update_instr_list(sci_else_temp,instr)
                end
            sci_else($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_else_temp,mtlb_clause.else(k).nb_opr)
            else
            [instr,nblines]=instruction2sci(mtlb_clause.else(k),nblines)
                sci_else=update_instr_list(sci_else,instr)
            end
        end
        // Create Scilab selectcase
        sci_clause=tlist(["selectcase","expression","cases","else"],sci_expr,sci_cases,sci_else)
        level($)=level($)+1
        updatevarslist("END OF CLAUSE")

        // --- WHILE ---
    case "while"
        level=[level;0]
        sci_do=list()
        // Convert expression
        [sci_expr]=expression2sci(mtlb_clause.expression)
        // If there are instructions to insert, while is modified so that inserted instruction is evaluated in each loop
        if m2sci_to_insert_b<>list() then
            newif=tlist(["ifthenelse","expression","then","elseifs","else"],sci_expr,list(Funcall("break",1,list(),list())),list(),list())
            m2sci_to_insert_b($+1)=newif
            sci_expr=Cste(%T)
            sci_do=m2sci_to_insert_b
            m2sci_to_insert_b=list()
        end

        // Convert all do instructions
        level($)=level($)+1
        for k=1:size(mtlb_clause.statements)
            if typeof(mtlb_clause.statements(k))=="sup_equal" then
                sci_do_temp=list()
                for i=1:size(mtlb_clause.statements(k).sup_instr)
                    [instr,nblines]=instruction2sci(mtlb_clause.statements(k).sup_instr(i),nblines)
                    // If inserted instruction is an initialisation, it has to be done just one time and before loop
                    l=1;
                    while l <= size(m2sci_to_insert_b)
                        if typeof(m2sci_to_insert_b(l))=="equal" & ..
                            (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
                            and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
                            and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
                            to_insert($+1)=m2sci_to_insert_b(l)
                            m2sci_to_insert_b(l)=null()
                            if size(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
                                to_insert($+1)=m2sci_to_insert_b(l)
                                m2sci_to_insert_b(l)=null()
                            end
                        else
                            l=l+1;
                        end
                    end
                    sci_do_temp=update_instr_list(sci_do_temp,instr)
                end
                sci_do($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_do_temp,mtlb_clause.statements(k).nb_opr)
            else
                [instr,nblines]=instruction2sci(mtlb_clause.statements(k),nblines)
                // If inserted instruction is an initialisation, it has to be done just one time and before loop
                l=1;
                while l <= size(m2sci_to_insert_b)
                    if typeof(m2sci_to_insert_b(l))=="equal" & ..
                        (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
                        and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
                        and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
                        to_insert($+1)=m2sci_to_insert_b(l)
                        m2sci_to_insert_b(l)=null()
                        if size(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
                            to_insert($+1)=m2sci_to_insert_b(l)
                            m2sci_to_insert_b(l)=null()
                        end
                    else
                        l=l+1;
                    end
                end
                sci_do=update_instr_list(sci_do,instr)
            end
        end

        // Create Scilab while
        sci_clause=tlist(["while","expression","statements"],sci_expr,sci_do)
        level($)=level($)+1
        updatevarslist("END OF CLAUSE")

        // --- FOR ---
    case "for"
        //level=[level;1]
        // Convert expression
        [sci_expr,nblines]=instruction2sci(mtlb_clause.expression,nblines)
        if typeof(sci_expr)=="equal" then
            [bval,pos]=isdefinedvar(sci_expr.lhs(1))
            if bval then
                varslist(pos).infer.dims=list(varslist(pos).infer.dims(1),1)
            end
        end
        // Get instructions to insert if there are
        if m2sci_to_insert_b<>list() then
            to_insert=m2sci_to_insert_b
            m2sci_to_insert_b=list()
        end
        sci_instr=list()
        // Convert all do instructions
        for k=1:size(mtlb_clause.statements)
            if typeof(mtlb_clause.statements(k))=="sup_equal" then
                sci_instr_temp=list()
                for i=1:size(mtlb_clause.statements(k).sup_instr)
                    [instr,nblines]=instruction2sci(mtlb_clause.statements(k).sup_instr(i),nblines)
                    // If inserted instruction is an initialisation, it has to be done just one time and before loop
                    l=1;
                    while l <= size(m2sci_to_insert_b)
                        if typeof(m2sci_to_insert_b(l))=="equal" & ..
                            (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
                            and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
                            and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
                            to_insert($+1)=m2sci_to_insert_b(l)
                            m2sci_to_insert_b(l)=null()
                            if size(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
                                to_insert($+1)=m2sci_to_insert_b(l)
                                m2sci_to_insert_b(l)=null()
                            end
                        else
                            l=l+1;
                        end
                    end
                    sci_instr_temp=update_instr_list(sci_instr_temp,instr)
                end
                sci_instr($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_instr_temp,mtlb_clause.statements(k).nb_opr)
            else
                [instr,nblines]=instruction2sci(mtlb_clause.statements(k),nblines)
                // If inserted instruction is an initialisation, it has to be done just one time and before loop
                l=1;
                while l <= size(m2sci_to_insert_b)
                    if typeof(m2sci_to_insert_b(l))=="equal" & ..
                        (and(m2sci_to_insert_b(l).expression==Cste([])) | ..
                        and(m2sci_to_insert_b(l).expression==Funcall("struct",1,list(),list())) | ..
                        and(m2sci_to_insert_b(l).expression==Funcall("cell",1,list(),list())) ) then
                        to_insert($+1)=m2sci_to_insert_b(l)
                        m2sci_to_insert_b(l)=null()
                        if size(m2sci_to_insert_b)>=l & m2sci_to_insert_b(l)==list("EOL") then
                            to_insert($+1)=m2sci_to_insert_b(l)
                            m2sci_to_insert_b(l)=null()
                        end
                    else
                        l=l+1;
                    end
                end
                sci_instr=update_instr_list(sci_instr,instr)
            end
        end
        // Create Scilab while
        sci_clause=tlist(["for","expression","statements"],sci_expr,sci_instr)
    else
        error(msprintf(gettext("unknown clause type: %s."),typeof(mtlb_clause)))
    end
    m2sci_to_insert_b=to_insert
    if m2sci_to_insert_b<>list() then
        m2sci_to_insert_b($+1)=list("EOL");
    end
endfunction

// ---------------------------------------------------------------------------

function updatevarslist(instr_lhs)
    // PRIVATE INTERNAL function called only by equal2sci() and clause2sci()
    //                  that are also private to instruction2sci()
    //
    // (2 functions in this file: merge_vars() at the end)
    // Update list of M2SCI variables with converted instruction lhs
    // Input:
    // - instr_lhs: list of lhs of current instruction
    // - in_a_clause: boolean value
    //   Set to 1 if instruction is in a clause
    //   In this case, type and dimensions are set to unknown if differ from those already stored in varslist
    //   (Default value is %F)

    // Global variable for M2SCI
    global("varslist")
    if isempty(varslist)
        varslist = list()
    end
    // level is declared in m2sci.sci and modified in clause2sci.sci
    level;

    rhs=argn(2)
    if rhs==2 then
        in_a_clause=%F
    end

    // Merge infered data from the last two parts of clause which are above the current part
    // if we are in the third part of clause (current), then : merge the first and second part of clause
    // when end of conversion of a clause : merge infered data from the last two parts of clause
    levelsize=size(level,1)
    changepartclause=%F

    for i=size(varslist):-1:1
        if size(varslist(i).level,1)==levelsize then
            varlevel=varslist(i).level
            if varlevel($)<>level($)
                changepartclause=%T
            else
                changepartclause=%F
                break
            end
        end
    end
    if changepartclause | instr_lhs=="END OF CLAUSE" then
        index=[] // Search variables from two part above current part clause
        for k=size(varslist):-1:1
            if size(varslist(k).level,1)==levelsize then
                varlevel=varslist(k).level
                if and(varlevel(1:$-1)==level(1:$-1)) & varlevel($)==level($)-2 then
                    index=[index;k]
                end
            end
        end
        if index<>[] then  // Found variables from the second part above current part of a clause
            for k=1:size(index,1)
                boolmerge =%F
                for i=size(varslist):-1:1 // Search variables from the first part above current part of a clause, and which have the same name than variables from the second part above current part of a clause
                    varlevel=varslist(i).level
                    if varslist(i).matname==varslist(index(k)).matname & and(varlevel(1:$-1)==level(1:$-1)) &  varlevel($)==level($)-1 then
                        boolmerge =%T // Found the same variable name from the last two parts above the current part : Merge
                        merge_vars(index(k),varslist(i))
                        varslist(i)=null()
                        break
                    end
                end
                if ~boolmerge then
                    varslist(index(k)).level=[level(1:$-1);level($)-1]
                end
            end
        end
    end

    // Special case when end of conversion of a clause
    // Merge infered data from clause and those from level-1
    if instr_lhs=="END OF CLAUSE" then // Search variables in the last part of a clause (above end of conversion of a clause)
        index=[] //
        for k=size(varslist):-1:1 // Search variables from level-1 which have the same name than variables from the last part of current level
            varlevel=varslist(k).level
            if varlevel==[level(1:$-1);level($)-1] then
                index=[index;k]
            end
        end
        if index<>[] then
            for j=1:size(index,1)
                boolmerge=%F
                for k=size(varslist):-1:1 //
                    varlevel=varslist(k).level
                    if varslist(k).matname==varslist(index(j)).matname  & and(varlevel==level(1:$-1)) then
                        boolmerge=%T // Found variables from level-1 which have the same name than variables from the last part of current level : Merge
                        index_lower_level=k
                        merge_vars(index_lower_level,varslist(index(j)))
                        varslist(k).level=level(1:$-1)
                        varslist(index(j))=null()
                        break
                    end
                end
                if boolmerge==%F then
                    varslist(index(j)).level=level(1:$-1)
                end
            end
        end
        return
    end

    // Expression: lhs name is empty => nothing to do
    if instr_lhs==list() then
        return
    end

    // Remove lhs which are not variables
    k=1
    while k<=size(instr_lhs)
        // Insertion operation
        if typeof(instr_lhs(k))=="operation" then
            instr_lhs(k)=null()
        else
            k=k+1
        end
    end

    if size(instr_lhs)==0 then
        return
    end

    // Update varslist
    for k=1:size(instr_lhs)
        [bval,index]=isdefinedvar(instr_lhs(k))
        ierr=execstr("zz=instr_lhs(k).contents.index","errcatch")
        if ierr<>0 then pause;end
        // Remove multiple entries from contents
        for kcont = size(instr_lhs(k).contents.index):-1:1
            [infertlist,pos]=get_contents_infer(instr_lhs(k),instr_lhs(k).contents.index(kcont))
            if pos<>0 & pos<>kcont then
                instr_lhs(k).contents.index(pos)=null()
                instr_lhs(k).contents.data(pos)=null()
            end
        end
        // If variable exists for the current level in the same part of clause then update exixting variable
        if bval
            boolupdate=%F
            for l=1:size(varslist)
                if varslist(l).matname==instr_lhs(k).name & varslist(l).level==level then
                    varslist(l)=M2scivar(varslist(l).sciname,..
                    varslist(l).matname,..
                    Infer(instr_lhs(k).infer.dims,instr_lhs(k).infer.type,instr_lhs(k).infer.contents),..
                    varslist(l).level)
                    boolupdate=%T
                    break
                end
            end
            // If variable exists, but not for the current level or not in the same part of clause then Update variable then create new variable
            if ~boolupdate then
                varslist($+1)=M2scivar(varslist(index).sciname,..
                varslist(index).matname,..
                instr_lhs(k).infer,..
                level)
            end
        else
            // Variable added to varslist if as a name (not done for expressions
            if execstr("f=instr_lhs(k).name","errcatch")<>0 then pause;end;errclear();
            if instr_lhs(k).name<>"ans" then
                varslist($+1)=M2scivar(instr_lhs(k).name,..
                instr_lhs(k).name,..
                instr_lhs(k).infer,..
                level)
            end
        end
    end
endfunction

// ---------------------------------------------------------------------------

function merge_vars(oldvarindex, newvar)
    // PRIVATE INTERNAL function called only by updatevarslist() hereabove.
    // M2SCI function
    // Merge two variables inference properties, if different then set to Unknown
    // Input:
    // - oldvarindex: index of old variable in varslist
    // - newvar: new variable to take in account to update oldvar properties

    // Global variable for M2SCI
    global("varslist")
    oldvar=varslist(oldvarindex)

    olddims=oldvar.dims
    oldvtype=oldvar.vtype
    oldprop=oldvar.property

    newdims=newvar.dims
    newvtype=newvar.vtype
    newprop=newvar.property

    // Verify dims
    for l=1:min(size(newdims),size(olddims))
        if newdims(l)<>olddims(l) then
            newdims(l)=Unknown
        end
    end
    if size(newdims)>size(olddims) then
        for l=size(olddims):size(newdims)
            newdims(l)=null()
        end
    end

    // Verify vtype
    if newvtype<>oldvtype then
        newvtype=Unknown
    end

    // Verify property
    if newprop<>oldprop then
        newprop=Unknown
    end

    // Verify contents
    for k = 1:size(newvar.contents.index)
        olddata=get_contents_infer(oldvar,newvar.contents.index(k))
        newdata=newvar.contents.data(k)

        if or(olddata<>newdata) then
            newvar.infer.contents.data(k)=Infer()
        end
    end

    // Write result in varslist
    varslist(oldvarindex)=M2scivar(oldvar.sciname,..
    oldvar.matname,..
    Infer(newdims,Type(newvtype,newprop),newvar.contents),..
    newvar.level)
endfunction
