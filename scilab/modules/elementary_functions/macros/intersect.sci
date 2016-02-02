// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x_out, ka_out, kb_out] = intersect(a_in, b_in, orient)
    // returns the vector of common values of two vectors

    rhs = argn(2);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "intersect", 2));
    end

    if ( (a_in == [])| (b_in == []) ) then
        x_out = [];
        ka_out = [];
        kb_out = [];
        return
    end
    if argn(2)<3 then
        //remove duplicate values in a_in and b_in
        [a, ka] = unique(matrix(a_in, 1, -1));
        [b, kb] = unique(matrix(b_in, 1, -1));
        kab = [ka, -kb];
        //find duplicated values in [a_in,b_in],
        [x,ksort] = gsort([a, b], "g", "i"); //sort the array

        kab = kab(ksort); //apply [a_in,b_in] sorting permutation to kab
        keq = find( x(2:$) == x(1:$-1) ); // find consecutive equal values index

        if keq == [] then
            //the intersection is empty
            x_out = [];
            ka_out = [];
            kb_out = [];
        else
            x_out = x(keq); //the intersection values in increasing order
            if argn(1) > 1 then //build the output index
                // each duplicated value appear twice  and only twice and in
                // consecutive positions keq(i) and keq(i)+1 in the sorted array x
                kab = kab([keq keq+1]);

                //the positive values correspond to a_in index while the negative to b_in index.
                ka_out = kab(kab > 0); //select index of intersection elements in a_in
                kb_out = -kab(kab < 0); //select index of intersection elements in b_in

                //insure that a_in(ka_out)==x_out and b_in(kb_out)==x_out.
                //I was'nt able to find a simple way.
                [s, k] = gsort(a_in(ka_out), "g", "i");
                ka_out = ka_out(k);
                [s, k] = gsort(b_in(kb_out),"g","i");
                kb_out = kb_out(k);
            end
        end
    elseif  orient==1|orient=="r" then
        //remove duplicate rows in a_in and b_in
        [a, ka] = unique(a_in, "r");
        [b, kb] = unique(b_in, "r");
        kab = [ka; -kb];
        //find duplicated rows in [a_in;b_in],

        [x,ksort] = gsort([a; b], "lr", "i"); //sort the rows

        kab = kab(ksort);//apply [a_in,b_in] sorting permutation to kab
        keq = find(and(x(2:$,:) == x(1:$-1,:),"c")) // find index of consecutive equal values

        if keq == [] then
            //the intersection is empty
            x_out = [];
            ka_out = [];
            kb_out = [];
        else
            x_out = x(keq,:); //the intersection values in increasing order

            if argn(1)>1 then //build the output index

                // each duplicated value appear twice  and only twice and in
                // consecutive positions keq(i) and keq(i)+1 in the sorted array x
                kab = kab([keq keq+1]);

                //the positive values correspond to a_in index while the negative to b_in index.
                ka_out = kab(kab>0); //select index of intersection elements in a_in
                kb_out = -kab(kab<0); //select index of intersection elements in b_in

                //insure that a_in(ka_out,:)==x_out and b_in(kb_out,:)==x_out.
                //I was'nt able to find a simple way.
                [s,k]=gsort(a_in(ka_out,:),"lr","i"); ka_out=ka_out(k)
                [s,k]=gsort(b_in(kb_out,:),"lr","i"); kb_out=kb_out(k)


            end
        end
    elseif  orient==2|orient=="c" then
        //remove duplicate columns in a_in and b_in
        [a,ka]=unique(a_in,"c");

        [b,kb]=unique(b_in,"c");
        kab=[ka, -kb];
        //find duplicated rows in [a_in;b_in],
        [x,ksort] = gsort([a b],"lc","i"); //sort the rows

        kab = kab(ksort);//apply [a_in,b_in] sorting permutation to kab
        keq = find(and(x(:,2:$) == x(:,1:$-1),"r")) // find index of consecutive equal values

        if keq == [] then
            //the intersection is empty
            x_out = [];
            ka_out = [];
            kb_out = [];
        else
            x_out =x(:,keq); //the intersection values in increasing order

            if argn(1)>1 then //build the output index

                // each duplicated value appear twice  and only twice and in
                // consecutive positions keq(i) and keq(i)+1 in the sorted array x
                kab=kab([keq keq+1]);

                //the positive values correspond to a_in index while the negative to b_in index.
                ka_out = kab(kab>0); //select index of intersection elements in a_in
                kb_out = -kab(kab<0); //select index of intersection elements in b_in

                //insure that a_in(ka_out,:)==x_out and b_in(kb_out,:)==x_out.
                //I was'nt able to find a simple way.
                [s,k]=gsort(a_in(:,ka_out),"lc","i"); ka_out=ka_out(k)
                [s,k]=gsort(b_in(:,kb_out),"lc","i"); kb_out=kb_out(k)
            end
        end

    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %d,''%s'',%d or ''%s'' expected\n"),"intersect",3,1,"r",2,"c"));
    end

endfunction
