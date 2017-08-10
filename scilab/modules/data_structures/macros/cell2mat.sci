// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y= cell2mat(c)

    // This function returns a matrix which is the result of concatenation of all components of the cell c
    // Input : c, a cell. All components of c must have the same data type (strings or doubles or integers or booleans)
    // Moreover all components of c must be scalar, vector or matrix
    // Output : y, a matrix or a hypermatrix
    // F.B

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"cell2mat",1));
    end

    if isempty(c) then
        y=[]
        return
    else
        if typeof(c) <> "ce" then //input argument must be a cell
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Cell expected.\n"),"cell2mat",1));
        end
        cecomptype=typeof(c{1}) //cecomptype is the type of the first component of the cell
        for i=1:prod(size(c))
            if typeof(c{i}) <> cecomptype then //all components of input cell must have the same type
                error(msprintf(gettext("%s: Wrong type for input argument #%d: Same type expected for all cell contents"),"cell2mat",1));
            end
        end
        if  0 < size(size(c),"*") & size(size(c),"*") <= 2 then //size of cell is least or equal to 2
            ytemp=list(1:size(c,1));
            for i=1:size(c,1)
                ytemp(i)=c{i,1};
                for j=2:size(c,2)
                    ytemp(i)=[ytemp(i) c{i,j}];
                end
                if i==1 then
                    y=ytemp(i);
                else
                    y=[y;ytemp(i)];
                end
            end
            if typeof(y)=="string" then
                for i=1:size(y,1)
                    y(i)=strcat(y(i,:))
                end
                y=y(:,1)
            end
        else //size of cell is more than 2
            csize=size(c)
            csizeprod=prod(csize)
            ytemp=list(1:csizeprod/(size(c,1)*size(c,2)))
            for l=1:csizeprod/(size(c,1)*size(c,2))
                ylist=list(1:size(c,1))
                for i=1:size(c,1)
                    ylist(i)=c{i,1,l};
                    for j=2:size(c,2)
                        ylist(i)=[ylist(i) c{i,j,l}];
                    end
                    if i==1 then
                        ytemp(l)=ylist(i);
                    else
                        ytemp(l)=[ytemp(l);ylist(i)];
                    end
                end
            end
            if typeof(ylist(i))=="string" then
                y=[]
                for l=1:csizeprod/(size(c,1)*size(c,2))
                    for i=1:size(ytemp(l),1)
                        y($+1)=strcat(ytemp(l)(i,:))
                    end
                end
                n=size(csize,"*")
                ysize=[size(ytemp(1),1),csize(3:n)]
                y=matrix(y,ysize)
            else
                n=size(csize,"*")
                ysize=[size(ytemp(1)),csize(3:n)]
                y=matrix(1:prod(ysize),ysize)
                for l=1:csizeprod/(size(c,1)*size(c,2))
                    y(:,:,l)=ytemp(l)
                end
            end
        end
    end
endfunction
