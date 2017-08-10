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

function [sci_expr]=expression2sci(mtlb_expr,lhslist)
    // M2SCI function

    // Global variable for M2SCI
    global("varslist")

    // When lhslist is given, then expression2sci is called from equal2sci
    // else it is called from an other function (clause2sci for example)
    rhs=argn(2);
    // If not given, lhslist is initialized with no name lhs
    if rhs==1 then
        if typeof(mtlb_expr)=="funcall" then
            lhsnb=mtlb_expr.lhsnb;
        else
            lhsnb=1;
        end
        lhslist=list();
        for k=1:lhsnb
            lhslist($+1)=Variable("ans",Infer());
        end
    end

    select typeof(mtlb_expr)
        // --- Expression is an 'operation' tlist ---
    case "operation"
        // Add inference fields to operation
        mtlb_expr=Operation(mtlb_expr.operator,mtlb_expr.operands,lhslist)

        // Translation
        [sci_expr]=operation2sci(mtlb_expr)

        // --- Expression is a 'funcall' tlist ---
    case "funcall"

        // Particular case if expression is a call to 'm2scideclare'
        if mtlb_expr.name=="m2scideclare" then
            sci_expr=list()
            // varslist is updated with user defined data
            sci_m2scideclare(mtlb_expr)

            // Particular case if expression is a call to 'comment'
        elseif mtlb_expr.name=="%comment" then
            // Add inference fields to funcall
            sci_expr=Funcall(mtlb_expr.name,mtlb_expr.lhsnb,mtlb_expr.rhs,lhslist)
            // All other 'funcall' expressions
        else
            // Add inference fields to funcall
            mtlb_expr=Funcall(mtlb_expr.name,mtlb_expr.lhsnb,mtlb_expr.rhs,lhslist)

            // Translation
            [sci_expr]=funcall2sci(mtlb_expr)
        end

        // --- Expression is a 'cste' tlist ---
    case "cste"
        // Convert expression
        sci_expr=Cste(mtlb_expr.value)

        // --- Expression is a 'variable' tlist ---
    case "variable"
        [bval,index]=isdefinedvar(mtlb_expr)

        // Variable already exists
        if bval then
            sci_expr=Variable(varslist(index).sciname,varslist(index).infer)
            // Variable is unknown
        else
            // Try to find what is this variable
            [sci_expr]=getvar2sci(mtlb_expr,lhslist)
            if typeof(sci_expr)=="funcall" then
                if typeof(lhslist(1))=="variable" & sci_expr.name==lhslist(1).name then // Used for commands such as keyboard, cputime...
                    // keyboard in an M-file is represented in tree by an equal with lhs=keyboard and expression=keyboard
                    sci_expr.lhs(1).name="ans"
                elseif typeof(lhslist(1))=="variable" & mtlb_expr.name==lhslist(1).name then // When command converted by mtlb(cmd_name)
                    sci_expr.lhs(1).name="ans"
                end
                if sci_expr.name<>"mtlb" then
                    [sci_expr]=funcall2sci(sci_expr)
                end
            end
        end
        // --- operand is a list (only for operations) ---
    case "list"
        if lstsize(mtlb_expr)==1 & mtlb_expr==list("EOL") then
            sci_expr=mtlb_expr
            return
        end
        for k=1:size(mtlb_expr)
            if typeof(mtlb_expr(k))=="cste" then
                mtlb_expr(k)=Cste(mtlb_expr(k).value)
            elseif typeof(mtlb_expr(k))=="variable" then
                [isvar,index]=isdefinedvar(mtlb_expr(k))
                if isvar then
                    mtlb_expr(k)=Variable(mtlb_expr(k).name,varslist(index).infer)
                else
                    mtlb_expr(k)=Variable(mtlb_expr(k).name,Infer())
                end
            elseif typeof(mtlb_expr(k))=="list" then
                [mtlb_expr(k)]=expression2sci(mtlb_expr(k),lhslist)
            elseif typeof(mtlb_expr(k))=="operation" then
                [mtlb_expr(k)]=expression2sci(mtlb_expr(k),lhslist)
            elseif typeof(mtlb_expr(k))=="funcall" then
                [mtlb_expr(k)]=expression2sci(mtlb_expr(k),lhslist)
            else
                error(msprintf(gettext("recursive extraction with one index of type %s is not yet implemented."),typeof(mtlb_expr(k))))
            end
        end
        sci_expr=mtlb_expr
        // --- operand is a comment ---
    case "comment"
        sci_expr=mtlb_expr
        // --- Expression is a not tolerated tlist ---
    else
        error(msprintf(gettext("%s is not yet implemented."),typeof(mtlb_expr)))
    end

    // Verify if flag for translation improvements has to be set
    if typeof(sci_expr)=="funcall" then
        if part(sci_expr.name,1:4)=="mtlb" then
            m2sci_infos(1)=%t // level 1 = improvements
        end
    end
endfunction
