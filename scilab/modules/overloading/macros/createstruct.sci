// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - S. Steer, F. Delebecque, V. Couvert
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function  M=createstruct(index,N)
    // Create a struct with field pointed by index equal to N
    // createstruct(list(indices,fieldname),N)  => M(indices).fieldname = N

    mtlbMode = oldEmptyBehaviour("query")=="off"

    // If index is not a list, creation is done by routines
    if type(index)==15 then

        // When only two indexes
        if size(index)==2 then

            // Second index is a fieldname
            if type(index(2))==10 then

                // First index is a numerical value or a vector of numerical values
                if type(index(1))==1 then
                    // X(p).f = y  -> index=list(p,f)
                    I=index(1)
                    nmax = max(I)
                    if mtlbMode then
                        Dims=[1 nmax]
                    else
                        Dims=[nmax 1]
                    end
                    if nmax==1 then
                        M = struct(index(2), N);
                    else
                        M = cell(Dims(1),Dims(2))
                        M{I} = N
                        M = struct(index(2), M);
                    end
                    return;
                    // First index is a list of numerical values
                elseif type(index(1))==15
                    // X(p,q[,...]).f=z -> index=list(list(p,q[,...]),f)
                    I = index(1)
                    M{I(:)} = N
                    M = struct(index(2), M)
                    return;
                    // First index is also a fieldname
                else
                    // X.f.g=y -> index=list(f,g)
                    M = struct(index(1), struct(index(2), N))
                    return
                end

                // Second index is a fieldname
            else
                // X.f(p[,q,...])=y : index=list(f,list(p[,q,...]))
                if type(index(1)) <> 10 then
                    msg = _("%s: Argument #1(1): string expected.\n")
                    error(msprintf(msg, "createstruct"))
                end
                clear M
                if typeof(N)=="st" then
                    for f = fieldnames(N)'
                        M(index(2)(:))(f) = N(f)
                    end
                else
                    M(index(2)(:)) = N
                end
                M = struct(index(1), M)
                return
            end

            // Any number of indexes <> 2
        else
            // Last index is a fieldname -> init struct
            if type(index($))==10 then
                M = struct(index($), N)
                index($) = null();
                // Last index is a numerical value or a list of numerical values
            elseif type(index($))==1 | type(index($))==15 then

                // If only one index value (happens when recursive calls to createstruct)
                if size(index)==1 then
                    if typeof(N)=="st" then
                        if type(index($))==15 then
                            Dims=list2vec(index($))'
                        else
                            if mtlbMode then
                                Dims=[1 index($)]
                            else
                                Dims=[index($) 1]
                            end
                        end
                        kmax=prod(Dims)
                        if kmax==1 then
                            M = N
                        else
                            M = cell(Dims)
                            f = fieldnames(N)(1)
                            M($,$) = N(f)
                            M = struct(f, M)
                        end
                    else
                        M(index($)(:)) = N;
                    end
                    index($)=null()

                else
                    // More than one index value
                    if typeof(N)=="st" then
                        if type(index($))==15 then
                            Dims=list2vec(index($))'
                        else
                            if mtlbMode then
                                Dims=[1 index($)]
                            else
                                Dims=[index($) 1]
                            end
                        end
                        z = cell(Dims)
                        f = fieldnames(N)(1)
                        z($) = N(f)
                        z = struct(f, z)
                    else
                        z(index($)(:))=N;
                    end
                    M = struct(index($-1), z)
                    index($) = null()
                    index($) = null()
                end
            else
                // This case should not happen (unknown kind of index)
                error(msprintf(_("%s: Not implemented.\n"),"createstruct"));
            end
            if index==list() then
                return
            end
            M=createstruct(index,M);
            return;
        end
    else
        if type(index)==10 then
            M = struct(index($), N)
        else
            error(msprintf(_("%s: Not implemented.\n"),"createstruct"));
        end
    end
endfunction
