// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2010 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function loadmatfile(varargin)
    // Loads variables in a Matlab binary or ASCII file into Scilab
    // This function has been developed following the 'MAT-File Format' description:
    // www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf

    // Verify that all inputs are character strings
    for k=1:size(varargin)
        if type(varargin(k))<>10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"loadmatfile"));
        end
    end

    [lhs,rhs]=argn(0)

    fil=[]
    bin=[]
    varnames=[]

    if rhs==1 then
        fil = varargin(1)
        fileExtension = fileparts(fil, "extension");
        if isempty(fileExtension) then // No extension: looks for a file named fil.mat and treats it as a binary MAT-file
            bin = %T
            fil = file + ".mat";
        elseif convstr(fileExtension, "l") <> ".mat" then // Extension other than .mat: treats the file as ASCII data.
            bin = %F
        else // Compatibility with old loadmatfile version
            bin = %T
        end
    else // Try to find type binary or ASCII ?
        // Filename is the first parameter: loadmatfile(filename[,opts])
        // or the second parameter: loadmatfile(filetype,filename[,opts]) with filetype equal to -ascii or -mat
        // filetype can also be included in opts
        k=1
        while k<=lstsize(varargin)
            select varargin(k)
            case "-mat"
                bin=%T
                k=k+1
            case "-ascii"
                bin=%F
                k=k+1
            case "-regexp"
                warning(msprintf(gettext("%s: This feature has not been implemented: %s."),"loadmatfile","-regexp"));
                while k<=lstsize(varargin) & and(varargin(k)<>["-mat","-ascii"])
                    k=k+1
                end
            else
                if isempty(fil) then // Filename
                    fil=pathconvert(varargin(k),%f,%t);
                    if fileparts(fil,"extension")==".mat" & isempty(bin) then // extension .mat and bin not already fixed by options
                        bin=%T
                    elseif isempty(bin) then
                        bin=%F
                    end
                else // Variable names
                    varnames=[varnames;varargin(k)]
                end
                k=k+1
            end
        end
        if isempty(bin) then
            bin=%T
        end
    end

    if fileparts(fil,"extension")=="" then
        fil=fil+".mat"
    end

    // --- BINARY FILE ---
    if bin then // Uses MATIO interface

        Name="";
        Names=[];
        Matrices=list()

        //-- Try to open the file
        fd = matfile_open(pathconvert(fil, %F, %T), "r");
        if fd==-1 then
            error(msprintf(gettext("%s: Cannot open file %s.\n"),"loadmatfile",fil));
        end

        //-- Read first variable
        [Name, Matrix, Class] = matfile_varreadnext(fd);

        //-- Loop on the stored variables
        while Name<>""

            // Old version compatibility | Name has been given
            if isempty(varnames) | or(Name==varnames) then
                Names=[Names,Name];Matrices($+1)=Matrix
            end

            [Name, Matrix, Class] = matfile_varreadnext(fd);

        end

        //-- Close the file
        matfile_close(fd)

        //-- Error while reading?
        if isempty(Names) then
            error(msprintf(gettext("%s: No variable read in file ''%s''. Check if your file is not corrupted.\n"),"loadmatfile",fil));
        end

        //-- Return variables in the calling context
        execstr("["+strcat(Names,",")+"]=resume(Matrices(:))")

        // --- ASCII FILE ---
    else
        txt = mgetl(fil);

        // Remove comments
        rowIndexes = grep(txt, "%")
        emptyLines = [];
        for k = rowIndexes
            txt(k) = part(txt(k), 1:(strindex(txt(k), "%") - 1));
            if isempty(stripblanks(txt(k))) then
                emptyLines($+1) = k;
            end
        end
        txt(emptyLines) = []; // Remove empty lines

        // Values read
        mat = evstr(txt);

        // Output variable name generated from file name
        name = fileparts(fil, "fname");

        execstr(name + " = resume(mat)")
    end
endfunction

