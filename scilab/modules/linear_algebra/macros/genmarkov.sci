
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

function M=genmarkov(rec,tr,flag)
    //Returns in M a random Markov transition probability matrix
    //with size(rec,1) recurrent classes with rec(1),...rec($)
    //entries respectively and tr transient states.
    // If the optional parameter flag='perm' is entered a random
    //permutation of the states is performed.
    [lhs,rhs]=argn(0);
    if rhs==2 then flag="noperm";end
    M=[];r=sum(rec);
    for k=rec
        m=rand(k,k,"u");
        m=m./(sum(m,"c")*ones(1,k));
        M=sysdiag(M,m);
    end
    if type(tr) ~= 15 then
        n=r+tr;
        MT=rand(tr,n,"u");MT=MT./(sum(MT,"c")*ones(1,n));
        M=[[M,zeros(r,tr)];MT];
        if flag=="perm" then [p,q]=gsort(rand(n,1,"u"));M=M(q,q);end
    else
        // tr=list(n1,[a1,a2,...ar],n2,[b1,...br],...)
        l=size(tr)/2;   //2*size(rec,2)
        Q=[];
        for kk=1:l
            nt=tr(1+2*(kk-1));
            Q=sysdiag(Q,rand(nt,nt));
        end
        Nt=size(Q,1);
        L=[];
        nclrec=size(rec,"*");
        for kk=1:l
            L1=[];indi=tr(2+2*(kk-1));nt=tr(1+2*(kk-1));
            for i=1:nclrec
                if indi(i)==0 then L1=[L1,zeros(nt,rec(i))]; else L1=[L1,rand(nt,rec(i))];end
            end
            L=[L;L1]
        end
        LQ=[L,Q];
        LQ=LQ./(sum(LQ,"c")*ones(1,size(LQ,2)));
        M=[[M,zeros(size(M,1),size(Q,2))];LQ];
    end
    if flag=="perm" then [p,q]=gsort(rand(size(M,1),1,"u"));M=M(q,q);end

    //n=size(M,1);[p,q]=gsort(rand(n,1));M1=M(q,q);M=M1

    //Ms=sparse(M);Ms(find(Ms~=0))=1;
    //G=mat_2_graph(Ms,1,'node-node');show_graph(G);
    //[nc,ncomp]=strong_connex(G)
    //[pp,qq]=gsort(ncomp);Mg=M(qq,qq)
    //Mgs=sparse(Mg);
    //Mgs(find(Mgs~=0))=1;Gg=mat_2_graph(Mgs,1,'node-node');show_graph(Gg);
endfunction
