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

function y=char(varargin)
    // Case : One input argument
    // Input : a cell of characters arrays
    // Output : vector (column) of strings,
    // This function returns a vector in  which the rows are the components of the cell characters arrays
    //
    // Input : an array of ascii codes
    // Output :a characters array corresponding into ascii codes
    // If dims input =[n1,n2,n3,n4,....],then returned a value has the dims=[n1,n3,n4,...],
    // returned value have same size as input value instead of second dims !
    //
    // Case : more than one input argument : y=char(st1,st2,st3,....)
    // Inputs must be string : st1,st2,st3,...
    // Outputs : vector (column) of strings
    // This function returns a vector in  which the rows are st1, st2,st3,...,
    // the length of all strings sti is completed by blanks,in order to have the same length for each sti.

    rhs=argn(2)
    y=[]
    // One input argument
    if rhs==1 then
        if typeof(varargin(1))=="ce" then // input argument is a cell of characters arrays
            c=varargin(1)
            if size(c)==1 then // cell contains one element
                lst = c{1};
                if size(lst, 2) > 2 then
                    if type(lst) == 10 then
                        t = emptystr(size(lst,1),1);
                        lst = matrix(lst,[size(lst,1),size(lst,"*")/size(lst,1)]);
                        for j=1:size(lst,2)
                            t = t+lst(:,j);
                        end
                        y = [y;t];
                    else
                        error(msprintf(gettext("%s: Wrong type for input argument: Cell elements must be character arrays.\n"),"char"));
                    end
                elseif type(lst) == 10 then // cell elements are a string matrix
                    t = emptystr(size(lst,1),1);
                    for k=1:size(lst,2)
                        t = t+lst(:,k);
                    end
                    y = [y,t];
                else
                    error(msprintf(gettext("%s: Wrong type for input argument: Cell elements must be character arrays.\n"),"char"));
                end
            else // cell contains more than one element
                for i=1:size(c, "*")
                    if size(c{i}, 2) > 2 then
                        if type(c{i}) == 10 then
                            ctemp = c{i};
                            t = emptystr(size(ctemp,1),1);
                            ctemp = matrix(ctemp,[size(ctemp,1),size(ctemp,"*")/size(ctemp,1)]);
                            for j=1:size(ctemp,2)
                                t = t+ctemp(:,j);
                            end
                            y = [y;t];
                        else
                            error(msprintf(gettext("%s: Wrong type for input argument: Cell elements must be character arrays.\n"),"char"));
                        end
                    elseif type(c{i}) == 10 then
                        t = emptystr(size(c{i},1),1);
                        for k=1:size(c{i},2)
                            t = t+c{i}(:,k);
                        end
                        y = [y;t];
                    else
                        error(msprintf(gettext("%s: Wrong type for input argument: Cell elements must be character arrays.\n"),"char"));
                    end
                end
            end
            // Add blank at the length of strings
            bl=" "
            maxstr = max(length(y));
            for i=1:size(y,"*")
                nb_bl = maxstr-length(y(i));
                if nb_bl>0 then
                    y(i) = y(i)+part(bl,ones(1,nb_bl));
                end
            end
        elseif (type(varargin(1))==1|type(varargin(1))==8) & size(varargin(1),2)<3 then // Input is a matrix of integers (or reals)
            y = asciimat(varargin(1));
        elseif type(varargin(1))==10 & size(varargin(1),2)<3 then // Input is a matrix of strings
            for i=1:size(varargin(1),1)
                y(i) = strcat(varargin(1)(i,:));
            end
        elseif size(varargin(1), 2) > 2 then
            if type(varargin(1)) == 10 then
                y=varargin(1);
            elseif type(varargin(1))==1|type(varargin(1))==8 then // Input is a hypermatrix of integers (or reals)
                y=asciimat(varargin(1));
            else
                error(msprintf(gettext("%s: This feature has not been implemented.\n"),"char"));
            end
        else
            error(msprintf(gettext("%s: Wrong type for input argument: Cell expected.\n"),"char"));
        end
        // More than one input argument
    elseif rhs > 1 then
        for i=1:size(varargin)
            if size(varargin(i), 2) > 2 then
                lst=varargin(i)
                if type(lst) == 10 then
                    t = emptystr(size(lst,1),1);
                    lst = matrix(lst,[size(lst,1),size(lst,"*")/size(lst,1)]);
                    for j=1:size(lst,2)
                        t = t+lst(:,j);
                    end
                    y = [y;t];
                else
                    error(msprintf(gettext("%s: Wrong type for input arguments: String expected.\n"),"char"));
                end
            elseif type(varargin(i))==10 then // Input sti is a matrix of strings
                lst = varargin(i);
                t = emptystr(size(lst,1),1);
                for k=1:size(lst,2)
                    t = t+lst(:,k);
                end
                y = [y;t];
            else
                error(msprintf(gettext("%s: Wrong type for input arguments: String expected.\n"),"char"));
            end
        end
        // Add blanks at the length of strings
        bl=" "
        maxstr = max(length(y))
        for i=1:size(y,"*")
            nb_bl = maxstr-length(y(i));
            if nb_bl>0 then
                y(i) = y(i)+part(bl,ones(1,nb_bl));
            end
        end
    end

endfunction
