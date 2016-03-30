
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [M,Q]=eigenmarkov(P)
    //Returns normalized left and right eigenvectors
    //for the eigenvalue 1 of the Markov transition matrix P.
    //If the multiplicity of this eigenvalue is m and P
    //is N x N, M is a m x N matrix and Q a N x m matrix.
    //M(k,:) is the probability distribution vector associated with the kth
    //ergodic set (recurrent class). M(k,x) is zero if x is not in the
    //k-th recurrent class.
    //Q(x,k) is the probability to end in the k-th recurrent class starting
    //from x.
    [perm,rec1,tr1,indsRec,indsT]=classmarkov(P)
    Mn=P(perm,perm);
    [junk,perminv]=-gsort(-perm);perminv=-perminv;
    nr=sum(rec1);
    T=Mn(nr+1:$,nr+1:$);L=Mn(nr+1:$,1:nr);
    p=0;V=[];
    for k=rec1
        v=L(:,p+1:p+k);V=[V,sum(v,"c")];
        p=p+k;
    end
    LL=zeros(nr,size(rec1,"*"));
    p=0;
    for k=1:size(rec1,"*")
        LL(p+1:p+rec1(k),k)=1;
        p=p+rec1(k);
    end
    Q=[LL;inv(eye()-T)*V];
    Q=Q(perminv,:);
    p=0;M=[];
    for kk=1:size(rec1,"*")
        classe=p+1:p+rec1(kk);
        p=p+rec1(kk);
        Mres=Mn(classe,classe);
        w=kernel((Mres-eye())')';
        M=sysdiag(M,w./sum(w));
    end
    M=[M,zeros(size(M,1),size(P,1)-size(M,2))];
    M=M(:,perminv);
endfunction
