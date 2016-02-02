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

function tree=default_trad(tree)
    // M2SCI function
    // Create a default translation function

    global("mtlbref_fun") //contains the matlab reference functions which not yet converted
    global("mtlbtool_fun")//contains the matlab  toolboxes functions
    global("not_mtlb_fun") // contains the not matlab functions

    if ~exists("mtlbref_fun") then
        mtlb_fun=[]
    end
    if ~exists("mtlbtool_fun") then
        mtlbtool_fun=[]
    end
    if ~exists("not_mtlb_fun") then
        not_mtlb_fun=[]
    end

    name=tree.name

    ispriminame=%f;
    //true if the name function is the name of scilab function primitive
    if funptr(tree.name)<>0 then
        name1="%"+tree.name
        tree.name=name1
        ispriminame=%t;
    end
    //ismtlbfun is true if the function is in a matlab toolbox, mtlbpath is the path where is the function
    [mtlbpath,ismtlbtoolfun]=mtlbtoolfun(name)
    //Matlab reference functions
    if or(name==not_yet_converted()) then
        set_infos(msprintf(gettext("Matlab function %s not yet converted, original calling sequence used."),name),2)
        if ~or(name==mtlbref_fun(:,1)) then
            mtlbref_fun($+1,1)=name
            if ispriminame then
                mtlbref_fun($,2)=msprintf(gettext("(Warning name conflict: function name changed from %s to %s)."),name,name1);
            else
                mtlbref_fun($,2)=""
            end
        end
        //Matlab toolboxes functions
    elseif ismtlbtoolfun then
        set_infos(msprintf(gettext("Matlab toolbox(es) function %s not converted, original calling sequence used"),name),2)
        if ~or(name==mtlbtool_fun(:,1)) then
            mtlbtool_fun($+1,1)=name
            if ispriminame then
                mtlbtool_fun($,2)=msprintf(gettext("Matlab toolbox(es) function %s not converted, original calling sequence used."),name,name1,mtlbpath)
            else
                mtlbtool_fun($,2)=msprintf(gettext("(Find this function in matlab/%s)."),mtlbpath)
            end
        end
    elseif isdefinedvar(Variable(tree.name,Infer())) then
        operands=list()
        operands(1)=Variable(tree.name,Infer())
        for krhs=1:lstsize(tree.rhs)
            operands($+1)=tree.rhs(krhs)
        end
        tree=Operation("ext",operands,tree.lhs)
        tree=operation2sci(tree)

        //Not matlbb function
    else
        set_infos(msprintf(gettext("Unknown function %s not converted, original calling sequence used."),name),2)
        if ~or(name==not_mtlb_fun(:,1)) then
            not_mtlb_fun($+1,1)=name
            if ispriminame then
                not_mtlb_fun($,2)=msprintf(gettext("(Warning name conflict: function name changed from %s to %s)."),name,name1);
            else
                not_mtlb_fun($,2)=""
            end
        end
    end
    if ispriminame then
        set_infos(msprintf(gettext("(Warning name conflict: function name changed from %s to %s)."),name,name1),0)
    end
    [tree]=sci_generic(tree)
endfunction
















