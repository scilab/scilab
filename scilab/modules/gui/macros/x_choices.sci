// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [rep] = x_choices(Title,listOfChoices)
    [lhs,rhs] = argn(0)

    // Tests/Demo code
    if rhs<=0 then
        s_mat = ["l1=list(''choice 1'',1,[''toggle c1'',''toggle c2'',''toggle c3'']);";
        "l2=list(''choice 2'',2,[''toggle d1'',''toggle d2'',''toggle d3'']);";
        "l3=list(''choice 3'',3,[''toggle e1'',''toggle e2'']);";
        "rep=x_choices(''Toggle Menu'',list(l1,l2,l3));"];
        // Commands display
        for l=1:size(s_mat,1)
            mprintf(s_mat(l) + "\n");
        end
        // Commands execution
        execstr(s_mat);
        return;
    end;

    // Check number of inputs
    if rhs ~=2 then
        error(msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "x_choices", 2));
    end

    // Type for title
    if typeof(Title)~="string" then
        error(msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "x_choices", 1));
    end;
    // Type for items
    if typeof(listOfChoices)~="list" then
        error(msprintf(_("%s: Wrong type for input argument #%d: A list expected.\n"), "x_choices", 2));
    end;

    n = size(listOfChoices);
    // Labels & buttons names
    items = "void";
    // Initial selected buttons
    defv = [];
    for i = 1:n
        currentList = listOfChoices(i);

        // Check that first choose is a list
        if typeof(currentList)~="list" then
            error(msprintf(_("%s: Wrong type for %s: A list expected.\n"),"x_choices(t,x)", "x("+string(i)+")"))
        end

        // Check that first item (row label in the GUI) is a string
        if typeof(currentList(1))~="string" then
            error(msprintf(_("%s: Wrong type for %s: string expected.\n"),"x_choices(t,x)", "x("+string(i)+")(1)"))
        end
        items = [items,currentList(1)];

        // Button names must be a row string vector
        if typeof(currentList(3))~="string" then
            error(msprintf(_("%s: Wrong type for %s: A row vector of strings expected.\n"),"x_choices(t,x)", "x("+string(i)+")(3)"))
        end
        [nbLines,nbCols] = size(currentList(3));
        if nbLines~=1 then
            error(msprintf(_("%s: Wrong size for %s: A row vector of strings expected.\n"),"x_choices(t,x)", "x("+string(i)+")(3)"))
        end
        items = [items,currentList(3)];

        // Default button selected
        if typeof(currentList(2))~="constant" then
            error(msprintf(_("%s: Wrong type for %s: A real expected.\n"),"x_choices(t,x)", "x("+string(i)+")(2)"))
        end
        if size(currentList(2), "*")~=1 then
            error(msprintf(_("%s: Wrong size for %s: A real expected.\n"),"x_choices(t,x)", "x("+string(i)+")(2)"))
        end
        defv = [defv,currentList(2)];

        // Add separator between labels of each line
        if n~=i then
            items = [items,"[--sep--]"];
        end
    end
    // Remove "void" from the list of items
    items = items(2:prod(size(items)))

    // Call the interface to Java
    rep = xchoicesi(defv,Title,items)
endfunction

