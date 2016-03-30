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


function [X,dims,J,Y,k,Z]=cainv(Sl,Alfa,Beta,flag)
    //CA invariant subspace: dual of abinv
    //Finds orthogonal bases X and Y and output injection J
    //such that the abcd matrices of Sl in bases (X,Y) are displayed as:
    //
    //                  [A11,*,*,*,*,*]                [*]
    //                  [0,A22,*,*,*,*]                [*]
    //   X'*(A+J*C)*X = [0,0,A33,*,*,*]   X'*(B+J*D) = [*]
    //                  [0,0,0,A44,*,*]                [0]
    //                  [0,0,0,0,A55,*]                [0]
    //                  [0,0,0,0,0,A66]                [0]
    //
    //          Y*C*X = [0,0,C13,*,*,*]          Y*D = [*]
    //                  [0,0,0,0,0,C26]                [0]
    //
    // dims=[nd1,nu1,dimS,dimSg,dimN] defines the partition of A matrix and k the partition
    // of [C,D] matrix.
    // eigenvalues of A11 (nd1 x nd1) are unstable
    // eigenvalues of A22 (nu1-nd1 x nu1-nd1) are stable
    // pair (A33, C13) (dimS-nu1 x dimS-nu1, k x dimS-nu1) is observable, eigenvalues of A33 set to Alfa
    // A44 (dimSg-dimS x dimSg-dimS) is unstable
    // A55 (dimN-dimSg,dimN-dimSg) is stable
    // pair (A66,C26) (nx-dimN x nx-dimN, ) is observable, eigenvalues of A66 set to Beta.
    //
    // dimS first columns of X span S= smallest (C,A) invariant
    // subspace which contains Im(B).
    // dimSg first columns of X span Sg
    // dimN first columns of X span N=S+V
    // dimS=0 iff B(ker(D))=0
    //
    // DDEP: dot(x)=A x + Bu + Gd
    //           y= Cx   (observation)
    //           z= Hx    (z=variable to be estimated, d=disturbance)
    //  Find: dot(w) = Fw + Ey + Ru such that
    //          zhat = Mw + Ny
    //           z-Hx goes to zero at infinity
    //  Solution exists iff Ker H contains Sg(A,C,G) inter KerC
    //i.e. H is such that:
    // For any W which makes a column compression of [Xp(1:dimSg,:);C]
    // with Xp=X' and [X,dims,J,Y,k,Z]=cainv(syslin('c',A,G,C));
    // [Xp(1:dimSg,:);C]*W = [0 | *] one has
    // H*W = [0 | *]  (with at least as many columns as above).

    [LHS,RHS]=argn(0);
    if RHS==1 then Alfa=-1;Beta=-1;flag="ge";end
    if RHS==2 then Beta=Alfa;flag="ge";end
    if RHS==3 then flag="ge";end
    if RHS==4 then
        if type(flag)~=10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"cainv",4));
        end
        if size(flag,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"cainv",4));
        end
    end
    [X,ddims,F,U,k,Z]=abinv(Sl',Beta,Alfa,flag);
    [nx,nx]=size(X);
    select flag
    case "ge"
        nr=ddims(1);nvg=ddims(2);nv=ddims(3);noc=ddims(4);nos=ddims(5);
        nd1=nx-nos;nu1=nx-noc;dimS=nx-nv;dimSg=nx-nvg;dimN=nx-nr;
        n6=1+ddims(5):nx;
        n5=1+ddims(4):ddims(5);
        n4=1+ddims(3):ddims(4);
        n3=1+ddims(2):ddims(3);
        n2=1+ddims(1):ddims(2);
        n1=1:ddims(1);
        //nr=1:nr;nzs=nr+1:nr+nvg;nzi=nvg+1:nv;
        X=[X(:,n6),X(:,n5),X(:,n4),X(:,n3),X(:,n2),X(:,n1)];
        J=F';Z=Z';Y=U';Y=[Y(k+1:$,:);Y(1:k,:)];
        dims=[nd1,nu1,dimS,dimSg,dimN];
        return;
    case "st"
        dims=nx-ddims;dims=dims($:-1:1);
        n5=1+ddims(4):nx;
        n4=1+ddims(3):ddims(4);
        n3=1+ddims(2):ddims(3);
        n2=1+ddims(1):ddims(2);
        n1=1:ddims(1);
        X=[X(:,n5),X(:,n4),X(:,n3),X(:,n2),X(:,n1)];
        J=F';Z=Z';Y=U';Y=[Y(k+1:$,:);Y(1:k,:)];
        return;
    case "pp"
        dims=nx-ddims;dims=dims($:-1:1);
        n4=1+ddims(3):nx;
        n3=1+ddims(2):ddims(3);
        n2=1+ddims(1):ddims(2);
        n1=1:ddims(1);
        X=[X(:,n4),X(:,n3),X(:,n2),X(:,n1)];
        J=F';Z=Z';Y=U';Y=[Y(k+1:$,:);Y(1:k,:)];
        return;
    end
endfunction
