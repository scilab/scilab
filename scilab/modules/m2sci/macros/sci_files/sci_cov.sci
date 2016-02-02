// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_cov(tree)
    // M2SCI function
    // Conversion function for Matlab cov()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==1 then
        A=getrhs(tree)
        if A.dims(1) == 1 then
            if A.dims(2) == 1 then
                tree.name="mvvacov"
                tree.lhs(1).dims=list(1,1)
            elseif A.dims(2) > 1 then
                tree.name="mvvacov"
                tree.rhs(1) = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
                n = A.dims(2)
                n_minus = n-1
                tree.lhs(1).dims=list(n,n)
                op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
                tree = Operation("*",list(op1,tree),tree.lhs)
            elseif A.dims(2) == Unknown then
                tree.name = "mtlb_cov"
            end
        elseif A.dims(1) > 1 then
            tree.name = "mvvacov"
            n = A.dims(1)
            n_minus = n-1
            tree.lhs(1).dims=list(n,n)
            op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
            tree = Operation("*",list(op1,tree),tree.lhs)
        elseif A.dims(1) == Unknown & A.dims(2) == Unknown
            tree.name = "mtlb_cov"
        end
    elseif rhs == 2 then
        [A,B] = getrhs(tree)
        if B.dims(1) == 1 & B.dims(2) == 1
            if typeof(B) == "cste" & B.value == 1 then
                if A.dims(1) == 1 then
                    if A.dims(2) == 1 then
                        tree.name = "mvvacov"
                        tree.rhs(2)=null()
                        tree.lhs(1).dims=list(1,1)
                    elseif A.dims(2) > 1 then
                        tree.name = "mvvacov"
                        tree.rhs(1) = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
                        tree.rhs(2)=null()
                        tree.lhs(1).dims=list(A.dims(2),A.dims(2))
                    elseif A.dims(2) == Unknown then
                        tree.name = "mtlb_cov"
                    end
                elseif A.dims(1) > 1 then
                    tree.name = "mvvacov"
                    tree.rhs(2)=null()
                    tree.lhs(1).dims=list(A.dims(1),A.dims(1))
                elseif A.dims(1) == Unknown & A.dims(2) == Unknown
                    tree.name = "mtlb_cov"
                end
            elseif typeof(B) == "cste" & B.value == 0 then
                if A.dims(1) == 1 then
                    if A.dims(2) == 1 then
                        tree.name="mvvacov"
                        tree.rhs(2)=null()
                        tree.lhs(1).dims=list(1,1)
                    elseif A.dims(2) > 1 then
                        tree.name="mvvacov"
                        tree.rhs(2)=null()
                        tree.rhs(1) = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
                        n = A.dims(2)
                        n_minus = n-1
                        tree.lhs(1).dims=list(n,n)
                        op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
                        tree = Operation("*",list(op1,tree),tree.lhs)
                    elseif A.dims(2) == Unknown then
                        tree.name = "mtlb_cov"
                    end
                elseif A.dims(1) > 1 then
                    tree.name = "mvvacov"
                    tree.rhs(2)=null()
                    n = A.dims(1)
                    n_minus = n-1
                    tree.lhs(1).dims=list(n,n)
                    op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
                    tree = Operation("*",list(op1,tree),tree.lhs)
                elseif A.dims(1) == Unknown & A.dims(2) == Unknown
                    tree.name = "mtlb_cov"
                end
            else
                tree.name = "mtlb_cov"
            end
        elseif B.dims(1)>1 | B.dims(2)>1 then
            tree.name = "mvvacov"
            n = B.dims(1)*B.dims(2)
            n_minus = n-1
            tree.lhs(1).dims=list(n,n)
            rhs1 = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
            rhs2 = Funcall("matrix",1,list(B,Cste(-1),Cste(1)),list())
            tree.rhs(1) = Operation("rc",list(rhs1,rhs2),list())
            tree.rhs(2) = null()
            op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
            tree = Operation("*",list(op1,tree),tree.lhs)
        else
            tree.name = "mtlb_cov"
        end
    elseif rhs==3
        [A,B,C]=getrhs(tree)
        if typeof(C) == "cste" & C.value == 1 then
            if B.dims(1)>1 | B.dims(2)>1 | A.dims(1)>1 | A.dims(2)>1
                tree.name = "mvvacov"
                rhs1 = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
                rhs2 = Funcall("matrix",1,list(B,Cste(-1),Cste(1)),list())
                tree.rhs(1) = Operation("rc",list(rhs1,rhs2),list())
                tree.rhs(3) = null()
                tree.rhs(2) = null()
                if A.dims(1)<> Unknown & A.dims(2)<>Unknown
                    tree.lhs(1).dims = list(A.dims(1)*A.dims(2),A.dims(1)*A.dims(2))
                elseif B.dims(1)<>Unknown & B.dims(2)<>Unknown
                    tree.lhs(1).dims = list(B.dims(1)*B.dims(2),B.dims(1)*B.dims(2))
                end
            else
                tree.name = "mtlb_cov"
            end
        elseif typeof(C) == "cste" & C.value == 0 then
            if B.dims(1)>1 | B.dims(2)>1 | A.dims(1)>1 | A.dims(2)>1
                if A.dims(1)<>Unknown  & A.dims(2)<>Unknown
                    tree.name = "mvvacov"
                    n = A.dims(1)*A.dims(2)
                    n_minus = n-1
                    tree.lhs(1).dims = list(n,n)
                    rhs1 = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
                    rhs2 = Funcall("matrix",1,list(B,Cste(-1),Cste(1)),list())
                    tree.rhs(1) = Operation("rc",list(rhs1,rhs2),list())
                    tree.rhs(3)=null()
                    tree.rhs(2)=null()
                    op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
                    tree = Operation("*",list(op1,tree),tree.lhs)
                elseif B.dims(1)<>Unknown  & B.dims(2)<>Unknown
                    tree.name = "mvvacov"
                    n = B.dims(1)*B.dims(2)
                    n_minus = n-1
                    tree.lhs(1).dims = list(n,n)
                    rhs1 = Funcall("matrix",1,list(A,Cste(-1),Cste(1)),list())
                    rhs2 = Funcall("matrix",1,list(B,Cste(-1),Cste(1)),list())
                    tree.rhs(1) = Operation("rc",list(rhs1,rhs2),list())
                    tree.rhs(3)=null()
                    tree.rhs(2)=null()
                    op1 = Operation("/",list(Cste(n),Cste(n_minus)),list())
                    tree = Operation("*",list(op1,tree),tree.lhs)
                else
                    tree.name = "mtlb_cov"
                end
            else
                tree.name = "mtlb_cov"
            end
        else
            tree.name = "mtlb_cov"
        end
    end

endfunction