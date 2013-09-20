// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s2=%s_i_lss(i,j,s1,s2)
    // s2(i,j)=s1
    //!
    if type(i)==10 then  // sl('A'),sl('B'),sl('C'),sl('D'),sl('X'),sl('dt')
        [lhs,rhs]=argn(0)
        if rhs<>3 then  error(21),end
        nams=["A","B","C","D","X","dt"]
        kf=find(convstr(i,"u")==nams)
        if kf==[] then error(21),end
        s2=s1;kf=kf+1
        if size(s2(kf))<>size(j) then
            if kf<>7|prod(size(j))>1 then
                warning("inserted element "+i+" has inconsistent dimension")
            end
        end
        s2(kf)=j
        return
    end
    if s1==[] then  // insertion d'une matrice vide
        row=%f
        col=%f
        [m,n]=size(s2)
        if and(size(i)==[-1 -1]) then
            row=%t
        else
            if and(i(:)==(1:m)') then row=%t,end
        end
        if and(size(j)==[-1 -1]) then
            col=%t
        else
            if and(j(:)==(1:n)') then col=%t,end
        end
        if ~row&~col then error("inserting [] in submatrix --> forbidden!"),end
        if row&col then s2=[],return,end
        if row then
            j1=[]
            for jj=1:n
                if ~or(jj==j) then  j1=[j1 jj] ,end
            end
            s2=s2(:,j1)
        else
            i1=[]
            for ii=1:m
                if ~or(ii==i) then  i1=[i1 ii] ,end
            end
            s2=s2(i1,:)
        end
    else
        s1=tlist(["lss","A","B","C","D","X0","dt"],[],[],[],s1,[],[])
        s2(i,j)=s1
    end
endfunction
