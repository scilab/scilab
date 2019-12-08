// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2010 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function outStruct = loadmatfile(varargin)
    // Loads variables from a Matlab binary file, or an Octave native text file,
    // or an Matlab or Octave ASCII file, into Scilab
    // This function has been developed following the 'MAT-File Format' description:
    // www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf

    // 2019 - S. Gougeon:
    //   * "-octave" format added: http://bugzilla.scilab.org/16242
    //   * "-toStruct" option added: http://bugzilla.scilab.org/15680
    //   * Automatic format detection added: http://bugzilla.scilab.org/16271

    Names = []      // Names of read variables
    Values = list() // Values of read variables

    // =====================
    // Parse input arguments
    // =====================
    // Verify that all inputs are character strings
    // --------------------------------------------
    varnames = []
    for k = 1:size(varargin)
        if type(varargin(k))<>10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"loadmatfile", k));
        end
        varnames = [varnames ; varargin(k)(:)]
    end

    // -toStruct" option
    // -----------------
    toStruct = %F
    k = find(convstr(varnames)== "-tostruct")
    if k <> [] then
        toStruct = %T
        varnames(k) = []
    end

    // Exclude the "-regexp" unsupported option
    // ----------------------------------------
    k = find(convstr(varnames)== "-regexp")
    if k <> [] then
        warning(msprintf(gettext("%s: This feature has not been implemented: %s."),"loadmatfile","-regexp"))
        varnames(k) = []
    end

    // Retrieve the type of input data
    // -------------------------------
    ftype = ""  // file type: "mat" | "octave" | "ascii" | ""=> automatic detection
    for t = ["mat" "ascii" "octave"]
        k = find(convstr(varnames)== "-" + t)
        if k <> [] then
            ftype = t
            varnames(k) = []
        end
    end

    // Retrieve the filename
    // ----------------------
    fil = pathconvert(varnames(1), %F, %T)
    if ~isfile(fil) then
        if ~isfile(fil + ".mat") then
            msg = _("%s: The file ''%s'' does not exist.\n")
            error(msprintf(msg, "loadmatfile", fil))
        end
        fil = fil + ".mat"
    end

    // Try to automatically detect the data type if still unknown
    // ----------------------------------------------------------
    if ftype=="" then
        t = mgetl(fil,1)
        if grep(t, "MAT-file") <> [] then
            ftype = "mat"   // including when v6|v7 generated with Octave
        elseif grep(t, "# Created by Octave") <> [] then
            ftype = "octave"
        elseif grep(t, "Octave-1-L") <> []
            ftype = "Octave binary"     // & float-binary. Both unsupported
        elseif grep(t, "‰HDF") <> []
            ftype = "HDF"               // Octave -hdf5. Unsupported
        elseif convstr(fileparts(fil,"extension"))==".mat" | ..
              t=="" | or(ascii(t)<32)
            ftype = "mat"               // Octave -v4
        else
            ftype = "ascii"
        end
    end

    // Get the actual list of variables names
    // --------------------------------------
    varnames(1) = []

    // =================
    // READING VARIABLES
    // =================
    // -------------------
    // --- BINARY FILE ---
    // -------------------
    if ftype == "mat" then
        // Uses MATIO interface
        Name = "";

        //-- Try to open the file
        fd = matfile_open(fil, "r");
        if fd==-1 then
            error(msprintf(gettext("%s: Cannot open file %s.\n"),"loadmatfile",fil));
        end

        //-- Read first variable
        [Name, Value, Class] = matfile_varreadnext(fd);

        //-- Loop on the stored variables
        while Name <> ""
            // Old version compatibility | Name has been given
            if isempty(varnames) | or(Name==varnames) then
                Names = [Names, Name];
                Values($+1) = Value
            end

            [Name, Value, Class] = matfile_varreadnext(fd);
        end

        //-- Close the file
        matfile_close(fd)

    // -----------------------------
    // -- Octave native text file --
    // -----------------------------
    elseif ftype == "octave"
        t = mgetl(fil);
        i = 2
        vname = ""
        while vname <> "-eof"
            [vname, value, i] = get_next_var(t,i)
            if vname <> "-eof" & (varnames==[] | or(vname==varnames))
                Names = [Names vname]
                Values($+1) = value
            end
        end

    // ----------------
    // -- ASCII FILE --
    // ----------------
    elseif ftype == "ascii"
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
        Values = evstr(txt);

        // Output variable name generated from file name
        Names = fileparts(fil, "fname")
        for t = ["." "-" "%" "(" ")"]
            Names = strsubst(Names, t, "_")
        end

    else
        msg = _("%s: Unsupported input file data type: ''%s''.\n")
        error(msprintf(msg, "loadmatfile", ftype))
    end

    // ===========
    // Data return
    // ===========
    //-- Error while reading?
    if isempty(Names) then
        error(msprintf(gettext("%s: No variable read in file ''%s''. Check if your file is not corrupted.\n"),"loadmatfile", fil));
    end

    //-- Return variables in the calling context
    if toStruct then
        outStruct = struct()
        for i = 1:size(Names,"*")
            outStruct(Names(i)) = Values(i)
        end
    else
        outStruct = []
        execstr("["+strcat(Names,",")+"]=resume(Values(:))")
    end
endfunction

// ===========================================================================

// Internal function called by loadmatfile()(Octave)
// -------------------------------------------------
function [vname, value, i] = get_next_var(t, i)
    // 2019 - Samuel GOUGEON
    st = size(t,1)
    while i <= st & t(i)=="", i = i + 1, end
    if i > st then
        vname = "-eof"
        value = []
        return
    end
    msg = _("loadmatfile: Error in input file at line #%d: ''%s'' expected.\n")
    // Variable name
    if grep(t(i), "/^# name: /", "r")==[] then
        error(msprintf(msg, i, "# name: "))
    end
    vname = stripblanks(part(t(i), 9:$))
    //disp(vname)
    i = i + 1
    // Variable type
    if grep(t(i), "/^# type: /", "r")==[] then
        error(msprintf(msg, i, "# type: "))
    end
    vtype = stripblanks(part(t(i), 9:$))
    i = i + 1

    // Sparses
    // -------
    if or(vtype == ["sparse matrix" "sparse complex matrix" "sparse bool matrix"]) then
        if grep(t(i), "/^# nnz: /", "r")==[] then
            error(msprintf(msg, i, "# nnz: "))
        end
        NNZ = strtod(part(t(i), 7:$))
        [vsize, i] = get_size(t, i+1, vtype)
        if NNZ > 0
            tmp = t(i:i+NNZ-1)
            tmp = strsubst(tmp, "NA","NaN")
            if or(vtype == ["sparse matrix" "sparse bool matrix"])
                tmp = csvTextScan(tmp, " ")
                value = sparse(tmp(:,1:2), tmp(:,3), vsize)
                if grep(vtype, "bool") <> []
                    value = value==1
                end
            else // complex
                tmp = strsubst(tmp, "(","");
                tmp = strsubst(tmp, ","," ");
                tmp = strsubst(tmp, ")","");
                tmp = csvTextScan(tmp, " ")
                value = sparse(tmp(:,1:2), complex(tmp(:,3),tmp(:,4)), vsize);
            end
            i = i + NNZ
        else
            value = spzeros(vsize(1), vsize(2));
            if grep(vtype, "bool") <> []
                value = value==1
            elseif grep(vtype, "complex") <> []
                value = value * %i
            end
        end
        if grep(vtype, "float") <> []
            value = set_single_precision(value)
        end
        return
    end
    [vsize, i] = get_size(t, i, vtype)
    if length(vsize) > 2 | grep(vtype,"int") <> []
        nr = prod(vsize)
    else
        nr = vsize(1)
    end

    // Booleans (dense) & Integers & Reals (dense)
    // -------------------------------------------
    if (grep(vtype, ["bool" "int" "float"]) <> [] | ..
        or(vtype == ["scalar" "matrix" "null_matrix"])) & ..
         grep(vtype, "complex")==[] then
        tmp = t(i:i+nr-1)
        tmp = strsubst(tmp, "NA", "NaN")
        if tmp <> []
            value = csvTextScan(tmp, " ")
        else
            value = []
        end

        if vtype <> "bool" & grep(vtype, "scalar") == []
            value = value(:,2:$)    // trims Nan in column #1
        end
        if length(vsize) > 2
            value = matrix(value, vsize);
        end
        if grep(vtype, "bool") <> []
            value = value == 1
        elseif grep(vtype, "int") <> []
            tmp = tokens(vtype);
            execstr(msprintf("value = %s(value);",tmp(1)));
            value = matrix(value, vsize);
        elseif grep(vtype, "float") <> []
            value = set_single_precision(value)
        end
        i = i + nr
        return
    end

    // Complex numbers (dense)
    // -----------------------
    if grep(vtype, "complex") <> []
        tmp = t(i:i+nr-1);
        tmp = strsubst(tmp, "NA", "NaN")
        tmp = strsubst(tmp, "(","");
        tmp = strsubst(tmp, ","," ");
        tmp = strsubst(tmp, ")","");
        tmp = csvTextScan(tmp, " ");
        if modulo(length(tmp(1,:)),2)==1 then
            tmp(:,1) = []
        end
        tmp = complex(tmp(:,1:2:$), tmp(:,2:2:$));
        value = matrix(tmp, vsize)
        if grep(vtype, "float") <> []
            value = set_single_precision(value)
        end
        i = i + nr
        return
    end

    // Strings
    // -------
    if or(vtype == ["string" "sq_string" "null_string"]) then
        n = vsize(1)
        if n==0
            value = ""
            i = i + 1
        else
            value = emptystr(n,1);
            for p = 1:n
                tmp = ""
                if grep(t(i), "/^# length:/", "r") == []
                    error(msprintf(msg, i, "# length:"))
                end
                L = strtod(part(t(i), 10:$));   // expected length
                i = i + 1
                tmp = tmp + t(i)
                while length(tmp) < L
                    i = i + 1
                    tmp = tmp + ascii(10) + t(i)
                end
                value(p) = tmp
                i = i + 1
            end
        end
        return
    end

    // Cells
    // -----
    if vtype=="cell" then
        value = cell(vsize);
        ofStrings = %t
        for p = 1:prod(vsize)
            [vn, val, j] = get_next_var(t,i)
            if vn <> "<cell-element>"
                msg = _("loadmatfile: Error in input file at line #%d: ''%s'' expected.\n")
                error(msprintf(msg, i, "# name: <cell-element>"))
            end
            value{p} = val
            ofStrings = ofStrings && type(val)==10 && size(val,"*")==1
            i = j
        end
        // Converting an array of celles of strings into a strings matrix
        if ofStrings & prod(vsize) > 0
            tmp = []
            for j = 1:prod(vsize)
                tmp(j) = value{j}
            end
            value = matrix(tmp, vsize)
        end
        return
    end

    // Structures
    // ----------
    if grep(vtype, "struct") <> [] then
        if grep(vtype, "scalar") <> []
            i = i + 2
        end
        // Number of fields
        if grep(t(i), "/^# length:/", "r") <> []
            nfields = strtod(part(t(i),10:$))
            len = prod(vsize);
            i = i + 1
            value = struct()
            for ifield = 1:nfields
                [fname, fval, i] = get_next_var(t,i)
                if len > 0
                    value(fname) = fval;
                end
            end
            f = fieldnames(value)'
            if len > 1 & f <> []
                tmp = [""""+f+""""
                       "value("""+f+""")"];
                tmp = "value=struct(" + strcat(tmp, ",")+");"
                execstr(tmp)
            end
        else
            msg = _("loadmatfile: Error in input file at line #%d: ''%s'' expected.\n")
            error(msprintf(msg, i, "# length:"))
        end
    end
endfunction

// -----------------------------------------------------------------------

// Internal function called by loadmatfile()(Octave) ..> get_next_var()
// --------------------------------------------------------------------
function [vsize, i] = get_size(t, i, vtype)
    // 2019 - Samuel GOUGEON
    if grep(vtype, "scalar") <> [] | vtype=="bool" then
        vsize = [1 1]
    elseif grep(vtype, "string") <> []
        if grep(t(i), "/^# elements:/", "r") <> []
            vsize = [strtod(part(t(i),12:$)) 1]
            i = i + 1
        else
            msg = _("loadmatfile: Error in input file at line #%d: ''%s'' expected.\n")
            error(msprintf(msg, i, "# elements:"))
        end
    else
        if grep(t(i), "/^# rows:/", "r") <> []
            vsize = strtod([part(t(i),8:$), part(t(i+1),11:$)])
            i = i + 2
        elseif grep(t(i), "/^# ndims:/", "r") == []
            msg = _("loadmatfile: Error in input file at line #%d: ''%s'' expected.\n")
            error(msprintf(msg, i, "# rows: or # ndims:"))
        else
            vsize = strtod(tokens(t(i+1))')
            i = i + 2
        end
    end
endfunction

// -----------------------------------------------------------------------

// Internal function called by loadmatfile()(Octave) ..> get_next_var()
// --------------------------------------------------------------------
function r = set_single_precision(r)
    // 2019 - Samuel GOUGEON
    if r==[] then
        return
    end
    s = size(r)
    if ~isreal(r) then
        r2 = [real(r(:)) imag(r(:))]
    else
        r2 = r
    end
    biggestFloat = 2^127*(2-2^-23); // (normal full accuracy)
    k = find(r2 > biggestFloat); r2(k) = %inf
    k = find(r2 < -biggestFloat); r2(k) = -%inf
    tiniestFloat = 2^-126;          // (normal full accuracy)
    k = find(abs(r2) < tiniestFloat); r2(k) = 0;
    k = find(r2 <> 0 & ~isinf(r2) & ~isnan(r2))
    if k==[] then
        return
    end
    L = log10(abs(r2(k)))
    p = round(10.^(8-ceil(L)))
    r2(k) = round(r2(k) .* p) ./ p
    if ~isreal(r) then
        r2 = complex(r2(:,1), r2(:,2));
    end
    r = matrix(r2, s)
endfunction
