// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_nargin(tree)
    // File generated from sci_PROTO15.g: PLEASE DO NOT EDIT !
    // M2SCI function
    // Conversion function for Matlab nargin()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree

    global("varslist")

    if rhs==0 then // Number of arguments of local function
        tree=Variable("%nargin",Infer(list(1,1),Type(Double,Real)))
        varslist($+1)=M2scivar("nargin","%nargin",Infer(list(1,1),Type(Double,Real)))
    else // Number of arguments of M-file
        if "nargin"=="nargin" then
            fieldnb=1
        else
            fieldnb=2
        end
        set_infos(msprintf(gettext("%s considered to be a Scilab macro."),rhs2code(tree.rhs)),2);
        evstrtree=Funcall("evstr",1,tree.rhs,list())
        macrovartree=Funcall("macrovar",1,Rhs_tlist(evstrtree),list())
        getfieldtree=Funcall("getfield",1,Rhs_tlist(fieldnb,macrovartree),list())
        tree=Funcall("size",1,Rhs_tlist(getfieldtree,"*"),tree.lhs)
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Double,Real)
    end
endfunction
