//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [A,description,ref,mtype] = ReadHBSparse(filename)
    //
    //  PURPOSE
    //     An utility to read the Harwell-Boeing sparse matrix
    //     format. Currently don't work for unassembled matrix.
    //     Also possible rhs presents in the file are not red.
    //
    //  ARGUMENTS
    //     filename   : (optional) a string given the filename
    //                  (eventually preceeding by the path), if
    //                  filename is not given then the function
    //                  use uigetfile to get filename
    //     A          : the sparse matrix
    //     description: a string given some information about the
    //                  matrix
    //     ref        : a string given the reference of the matrix
    //     mtype      : a string given the "type" of the matrix
    //
    //  COMMENTS
    //     Generally the file name is of the form ref.matrixtype
    //     where mtype is a 3 letters word given some
    //     information (already inside the file) on the matrix :
    //        1st letter : R|C|P   for real|complex|pattern (no values given)
    //        2d  letter : S|H|Z|U for symmetric|hermitian|skew symmetric|unsymmetric
    //        3d  letter : A|E     for assembled|unassembled matrix
    //                             (case E is not treated by this func)
    //  REFERENCES
    //     Users' Guide for the Harwell-Boeing Sparse Matrix Collection
    //     Iain S. Duff, Roger G. Grimes, John G. Lewis
    //
    //     You may found this guide and numerous sparse
    //     matrices (in the Harwell-Boeing format) at the
    //     University of Florida Sparse Matrix Collection
    //     web site :
    //
    //        http://www.cise.ufl.edu/research/sparse/matrices/
    //
    //     maintained by Tim Davis <http://www.cise.ufl.edu/~davis/>
    //
    //  AUTHOR
    //     Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
    //
    [lhs, rhs] = argn()

    if rhs == 0 then
        filename = uigetfile("*.[rc][shzu]a", title=[gettext("Choose a sparse matrix"); ...
        gettext("   then click on OK   ")])
    elseif rhs == 1 then
        if typeof(filename) ~= "string" then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"ReadHBSparse",1));
        end
    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"ReadHBSparse",0));
    end

    unit = file("open", filename, "old")

    // 1)  read the 4 or 5 header lines

    line1 = read(unit,1,1,"(A)")
    ref = stripblanks(part(line1,73:80))
    ref = convstr(ref)
    description = stripblanks(part(line1,1:72))
    description = convstr(description)

    // normaly the line2 contains 5 numbers and if the last is zero
    // the file contains no rhs but in this case in some HB file
    // we have only 4 numbers. So the following is a trick to take
    // into account this pb
    line2 = read(unit,1,1,"(A)") // read the line 2 as a string
    line2 = evstr(line2)         // this string is then transform is a row vector
    if length(line2) < 5 then
        Rhs_in_file = %f
    else
        if line2(5) == 0 then
            Rhs_in_file = %f
        else
            Rhs_in_file = %t
            warning(msprintf(gettext("%s: The file contains a rhs but it will not be read !"),"ReadHBSparse"));

        end
    end

    line3 = read(unit,1,1,"(A)")
    mtype = convstr(part(line3,1:3))

    Dimensions = evstr(part(line3,4:80))
    if part(mtype,3)=="e" then
        error(msprintf(gettext("%s: currently don''t read unassembled (elemental) sparse matrix."),"ReadHBSparse"));
    end
    TypeValues = part(mtype,1) // r for real, c for complex, p for pattern

    m     = Dimensions(1)   // number of rows
    n     = Dimensions(2)   // number of columns
    nb_nz = Dimensions(3)   // number of non zeros


    line4 = read(unit,1,1,"(A)")  // these are the formats
    form1 = stripblanks(part(line4,1:16))
    form2 = stripblanks(part(line4,17:32))
    form3 = stripblanks(part(line4,33:52))
    // for the 2 first replace Ix by Fx.0 : the read func uses only float formats
    form1 = replace_Ix_by_Fx(form1)
    form2 = replace_Ix_by_Fx(form2)

    if Rhs_in_file then  // a 5 header line to read (but ignored)
        line5 = read(unit,1,1,"(A)")
    end


    // 2) read the data

    col_ptr = read(unit,1,n+1,form1)
    ind_row = read(unit,1,nb_nz,form2)
    select TypeValues
    case "p" // values given
        warning(msprintf(gettext("%s: No values for this matrix (only non zero pattern) : put some 1."),"ReadHBSparse"));
        val = ones(1,nb_nz)
    case "r" // values are real
        val = read(unit,1,nb_nz,form3)
    case "c" // values are complex
        valc = matrix( read(unit,1,2*nb_nz,form3) , 2, nb_nz )
        val = valc(1,:) + %i*(valc(2,:))
        clear valc
    end

    file("close", unit)

    // 3) form the sparse scilab matrix

    // 3-1/ form the basic matrix
    ind_col = ones(1,nb_nz)
    for i = 2:n
        ind_col(col_ptr(i):col_ptr(i+1)-1) = i
    end

    A = sparse([ind_row' ind_col'], val, [m n])
    clear ind_row ind_col col_ptr val  // to regain some memory

    // 3-2/ complete the matrix depending the symetry property
    MatrixSymetry = part(mtype,2)
    select MatrixSymetry
    case "s"  // (real or complex) symmetric matrix
        A = A - diag(diag(A)) + A.'
    case "h"  // complex hermitian matrix
        A = A - diag(diag(A)) + A'
    case "z"  // skew symmetric matrix
        A = A - A'
    end

endfunction
