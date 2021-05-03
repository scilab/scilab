// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function funcall_tlist = Funcall(name,lhsnb,rhslist,lhslist)
    // Copyright INRIA
    // Create a new function tlist

    rhs = argn(2)

    // Verify input value
    if rhs<3 | rhs>4 then
        error(gettext("Wrong number of inputs."));
    elseif typeof(name)~="string" then
        error(msprintf(gettext("name must be a string instead of a: %s."),typeof(name)));
    elseif typeof(lhsnb)~="constant" then
        error(msprintf(gettext("lhsnb must be a constant instead of a: %s."),typeof(lhsnb)));
    elseif typeof(rhslist)~="list" & rhslist<>[] then
        error(msprintf(gettext("rhslist must be a list instead of a: %s."),typeof(rhslist)));
    elseif typeof(lhslist)~="list" then
        error(msprintf(gettext("lhslist must be a list instead of a: %s."),typeof(lhslist)));
    end

    if rhs==3
        lhslist = list()
    end
    // Known predefined frequent LHS lists
    if lhslist==list() then
        select name
        case "abs"
            lhslist = Variable("ans",Infer(rhslist(1).dims,Type(Handle,Unknown)))
        case "gcf"
            lhslist = Variable("ans",Infer(list(1,1),Type(Handle,"Figure")))
        case "gca"
            lhslist = Variable("ans",Infer(list(1,1),Type(Handle,"Axes")))
        case "gce"
            lhslist = Variable("ans",Infer(list(Unknown,1),Type(Handle,Unknown)))
        case "scf"
            lhslist = Variable("ans",Infer(list(1,1),Type(Handle,"Figure")))
        case "size"
            lhslist = Variable("ans",Infer(list(1,Unknown),Type(Double,Real)))
        case "type"
            lhslist = Variable("ans",Infer(list(1,1),Type(Double,Real)))
        case "typeof"
            lhslist = Variable("ans",Infer(list(1,1),Type(String,String)))
        case "winsid"
            lhslist = Variable("ans",Infer(list(Unknown,1),Type(Double,Real)))
//        case "isreal"
//            lhslist = Variable("ans",Infer(list(1,1),Type(Boolean,Boolean)))
        case "ones"
        case "zeros"
        case "rand"
        case "bool2s"
        case "real"
        case "getos"
            lhslist = Variable("ans",Infer(list(1,1),Type(String,Char)))
        end
        if lhslist <> list()
            lhslist = list(lhslist)
        end
    end

    // Setting the tlist:
    fields = ["funcall","name","lhsnb","rhs","lhs"]
    funcall_tlist = tlist(fields, name, lhsnb, rhslist, lhslist)
endfunction
