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

function [scitree,crp]=mtlbtree2sci(mtlbtree,prettyprintoutput)
    // Conversion of a Matlab function tree to Scilab (and code generation)
    // Input arguments:
    //  - mtlbtree: tree (returned by macr2tree) representing Matlab function compiled code
    //  - prettyprintoutput: boolean flag for pretty printed output file if TRUE
    // Output arguments:
    //  - scitree: Scilab equivalent for mtlbtree
    //  - crp: Scilab equivalent function code (function body)

    // Global variables for M2SCI
    global("m2sci_to_insert_b") // To insert before current instruction
    global("m2sci_to_insert_a") // To insert after current instruction
    global("tmpvarnb") // Index for temporary variables
    m2sci_to_insert_b=list()
    m2sci_to_insert_a=list()
    tmpvarnb=0

    if typeof(mtlbtree)<>"program" then
        error(gettext("wrong type of input."))
    end

    // Init Scilab tree
    scitree=tlist(["program","name","outputs","inputs","statements"],mtlbtree.name,mtlbtree.outputs,mtlbtree.inputs,list())

    ninstr=1 // Index of Matlab tree
    if batch then // defined in m2sci.sci
        nblines=0
    else
        nblines=1 // Number of converted lines
    end

    m2sci_info(gettext("Conversion of M-tree..."),-1);

    // Default value
    rhs = argn(2);
    if rhs<2 then
        prettyprintoutput=%F
    end

    crp=""

    // Function prototype
    lhsstr=[]
    rhsstr=[]
    if scitree.name<>"" then // Not a batch file
        for k=1:size(scitree.outputs)
            lhsstr=[lhsstr,expression2code(scitree.outputs(k))]
        end
        if ~isempty(lhsstr) then
            lhsstr="["+strcat(lhsstr,",")+"]"
        else
            lhsstr = "[]";
        end

        for k=1:size(scitree.inputs)
            rhsstr=[rhsstr,expression2code(scitree.inputs(k))]
        end
        if ~isempty(rhsstr) then
            rhsstr="("+strcat(rhsstr,",")+")"
        else
            rhsstr = "()";
        end

        crp=lhsstr+" = "+scitree.name+rhsstr;
    end


    // Convert Matlab instruction tree to Scilab
    while ninstr<=size(mtlbtree.statements)-3
        //Case : sup_equal instruction
        // Add converted tree to scitree and also inserted instructions
        if typeof(mtlbtree.statements(ninstr))=="sup_equal"

            sci_stat=list();
            for i=1:size(mtlbtree.statements(ninstr).sup_instr)
                [converted_tree,nblines]=instruction2sci(mtlbtree.statements(ninstr).sup_instr(i),nblines);

                sci_stat=update_instr_list(sci_stat,converted_tree);
            end

            scitree.statements($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_stat,mtlbtree.statements(ninstr).nb_opr);
        else

            [converted_tree,nblines]=instruction2sci(mtlbtree.statements(ninstr),nblines);

            // Add converted tree to scitree and also inserted instructions

            scitree.statements=update_instr_list(scitree.statements,converted_tree);

            // Generate code corresponding to scitree.statements
        end
        for k=1:size(scitree.statements)
            if k<size(scitree.statements)
                crp = cat_code(crp,instruction2code(scitree.statements(k),prettyprintoutput));
                crp = format_txt(crp,scitree.statements(k),prettyprintoutput,scitree.statements(k+1));
            end
        end

        scitree.statements=list(scitree.statements($));

        // Disp percentage of conversion done
        msprintf(gettext("%s line %s out of %s..."),margin, string(nblines), string(mtlbtree.nblines));
        ninstr=ninstr+1;
        tmpvarnb=0;
    end

    if scitree.statements(1)<>list("EOL") then
        crp = cat_code(crp,instruction2code(scitree.statements(1),prettyprintoutput));
        crp = format_txt(crp,scitree.statements(1),prettyprintoutput,list("EOL"));
    end

    if scitree.name<>"" then // Not a batch file
        crp=cat_code(crp,"");
        crp=cat_code(crp,"endfunction"); // Replace last return
        crp=cat_code(crp,"");
    end

    m2sci_info(gettext("Conversion of M-tree: Done"),-1);


    clearglobal("m2sci_to_insert_b")
    clearglobal("m2sci_to_insert_a")
    clearglobal("tmpvarnb")

endfunction
