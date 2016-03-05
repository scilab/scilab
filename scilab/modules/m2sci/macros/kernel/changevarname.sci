// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function changevarname(fun,oldname,newname)
    // This function replaces the variable (named oldname) of a file by a new name (which is newname). It uses the old2newinstr function
    // INPUTS:
    // -fun: a string, the name (with the whole path) of the modifed file
    // -oldname: a string, the old name of the variable
    // -newname: a string, the new name of the variable after modification

    sep=filesep();

    sep_ind=strindex(fun,sep);
    dot_ind=strindex(fun,".");

    // funname is the function name (without the path and the extension)
    funname=part(fun,sep_ind($)+1:dot_ind($)-1);

    if sep_ind<>[]
        funsave=part(fun,1:sep_ind($))+"save_"+part(fun,sep_ind($)+1:dot_ind($)-1)+".sci";
    else
        funsave="save_"+part(fun,sep_ind($)+1:dot_ind($)-1)+".sci";
    end

    // Compilation
    exec(fun);
    var=who("get");
    indvar=find(var==funname);
    funvect=var(indvar(1):-1:1);
    txt=mgetl(fun);
    mputl(txt,funsave);

    txt=[]
    for j=1:size(funvect,1)
        //Get the tree of the function
        execstr("tree=macr2tree("+funvect(j)+")");
        // Change oldname into newname in the tree
        if tree.name==oldname then
            tree.name=newname;
        end
        for i=1:size(tree.inputs)
            tree.inputs(i)=old2newinstr(tree.inputs(i),oldname,newname);
        end
        for i=1:size(tree.outputs)
            tree.outputs(i)=old2newinstr(tree.outputs(i),oldname,newname);
        end
        for i=1:size(tree.statements)
            tree.statements(i)=old2newinstr(tree.statements(i),oldname,newname);
        end

        // Get the matching code of the tree after modification
        txt=[txt; tree2code(tree,%T)];
    end

    // Replace the old code by the new code (which is txt) in the file fun
    mputl(txt,fun);
endfunction