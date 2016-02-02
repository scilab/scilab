// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function sl=cont_frm(num,den)
    //Controllable state-space form of the transfer num/den
    //!
    [lhs,rhs]=argn(0)
    if size(den,"*")<>1 then  error(54,2);end
    [ns,ne]=size(num);
    if type(num)==1 then
        if type(den)==1 then
            sl=syslin([],[],[],[],num./den,[])
            return
        else
            num(1,1)=poly(num(1,1),varn(den),"c"),
        end
    end
    nd=degree(den);
    // normalization
    dnd=coeff(den,nd);den=den/dnd;num=num/dnd
    // D(s)
    for l=1:ns,
        for k=1:ne,
            [nl,dl]=pdiv(num(l,k),den),
            num(l,k)=nl,d(l,k)=dl,
        end,
    end
    if max(degree(d))==0 then d=coeff(d),end
    //matrices a b and c
    if nd<>0 then
        den=coeff(den);c=coeff(num,0:nd-1)
        a=[];
        for k=1:nd,a=[a,-den(k)*eye(ne,ne)];end
        a=[0*ones((nd-1)*ne,ne),eye(ne*(nd-1),ne*(nd-1));a];
        b=[0*ones((nd-1)*ne,ne);eye(ne,ne)]
    else
        a=[];b=[];c=[]
    end;
    [n,n]=size(a);
    sl=syslin([],a,b,c,d,0*ones(n,1))
endfunction
