// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [u,m]=nearly_multiples(v,tol)
    //Find nearly multiple values in v  and their multiplicity
    //v   : complex array, the given values
    //tol : positive relative tolerance, two values v1 and v2 are considered nearly
    //      equals if abs(v1-v2)/abs(v1)<tol
    //u   : the "unique" values. Nearly equal values are replaced by their
    //      mean
    //m   : the multiplicity of the "unique" values/ (u(i) is nearly equal to
    //      m(i) values in v
    v=v(:);
    if argn(2)<2 then
        tol=1e-10
    end
    kz=find(abs(v)<=tol);
    mz=size(kz,"*")
    v(kz)=[]
    ir=find((abs(imag(v))<=tol*abs(v)))
    v(ir)=real(v(ir))
    v=gsort(v(imag(v)>=0),"g","d")
    //[s,k]=gsort([real(v) imag(v)],"lr","d"); v=v(k)
    keq=find(abs(v(2:$)-v(1:$-1))<tol*abs(v(2:$)))
    if keq<>[] then
        //There is nearly mutiple values
        keq=[keq keq($)+2]
        n=size(keq,"*")
        u=[];m=[];del=[];
        //locate clusters of nearly equal values . ith  cluster starts at keq(i)
        //and stops at keq(j) where j is the first value greater than i such as keq(j+1)>keq(j)+1
        //or j=size(keq,"*")
        i=1;
        while i<n
            j=i+1;
            while j<n&keq(j)==keq(j-1)+1 do j=j+1;end
            sel=keq(i):keq(j-1)+1;
            i=j;
            u=[u;mean(v(sel))];
            m=[m;size(sel,"*")];
            del=[del sel];
        end
        v(del)=[];
        u=[u;v];
        m=[m;ones(v)];
    else
        u=v
        m=ones(v)
    end

    //add the complex conjugate
    kc=find(imag(u)>0)
    kr=find(imag(u)==0)
    u=[u(kr);matrix([u(kc).';u(kc)'],-1,1)]
    m=[m(kr);matrix([m(kc)';m(kc)'],-1,1)]

    if mz<>0 then
        u=[0;u]
        m=[mz;m]
    end
endfunction
