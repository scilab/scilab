// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tree = sci_isa(tree)
    // M2SCI function
    // Conversion function for Matlab isa()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    [OBJ,class] = getrhs(tree)

    if typeof(class)=="cste" then

        typetree = Funcall("type",1,Rhs_tlist(OBJ))
        typeoftree = Funcall("typeof",1,Rhs_tlist(OBJ))

        select class.value
        case "logical"
            eqtree = Operation("==", list(typetree,Cste([4 6])))
            tree = Funcall("or",1, list(eqtree),list(tree.lhs(1)))
        case "char"
            tree = Operation("==", list(typetree,Cste(10)),tree.lhs)
        case "numeric"
            eqtree = Operation("==", list(typetree,Cste([1 5 8])))
            tree = Funcall("or",1, list(eqtree),list(tree.lhs(1)))
        case "integer"
            tree = Operation("==", list(typetree,Cste(8)), tree.lhs)
        case "int8"
            tree = Operation("==", list(typeoftree,Cste("int8")), tree.lhs)
        case "uint8"
            tree = Operation("==", list(typeoftree,Cste("uint8")), tree.lhs)
        case "int16"
            tree = Operation("==", list(typeoftree,Cste("int16")), tree.lhs)
        case "uint16"
            tree = Operation("==", list(typeoftree,Cste("uint16")), tree.lhs)
        case "int32"
            tree = Operation("==", list(typeoftree,Cste("int32")), tree.lhs)
        case "uint32"
            tree = Operation("==", list(typeoftree,Cste("uint32")), tree.lhs)
        case "float"
            tree = Operation("==", list(typetree,Cste(1)), tree.lhs)
        case "single"
            tree = Operation("==", list(typetree,Cste(1)), tree.lhs)
        case "double"
            tree = Operation("==", list(typetree,Cste(1)), tree.lhs)
        case "cell"
            tree = Operation("==", list(typeoftree,Cste("ce")), tree.lhs)
        case "struct"
            tree = Operation("==", list(typeoftree,Cste("st")), tree.lhs)
        case "function_handle"
            eqtree = Operation("==", list(typetree,Cste([13 130])), tree.lhs)
            tree = Funcall("or",1, list(eqtree),list(tree.lhs(1)))
        case "sparse"
            tree = Operation("==", list(typetree,Cste(5)),tree.lhs)
        case "lti"
            tree = Operation("==", list(typeoftree,Cste("state-space")),tree.lhs)
        else
            set_infos(msprintf(gettext("Unhandled class: %s."),class.value),2)
        end
        if typeof(tree)=="operation"
            tree.out(1).dims = list(1,1)
            tree.out(1).type = Type(Boolean,Boolean)
        else    // funcall
            tree.lhs(1).dims = list(1,1)
            tree.lhs(1).type = Type(Boolean,Boolean)
        end
    else
        tree.name = "mtlb_isa"
        tree.lhs(1).dims = list(1,1)
        tree.lhs(1).type = Type(Boolean,Boolean)
    end
endfunction
