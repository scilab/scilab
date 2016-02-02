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

function spcho=chfact(A)
    //cholesky factors, returned in a tlist
    //spcho  = {xlnz, nnzl, xsuper, xlindx, lindx, snode,
    //          split, tmpsiz, perm, invp, lnz}.
    //
    //  invp=spcho('invp');xlnz=spcho('xlnz');
    //  xlindx=spcho('xlindx');lindx=spcho('lindx');lnz=spcho('lnz');
    //  P=sparse([(1:m)',invp],ones(invp),[m,m]);
    //  adjncy = spcompack(xlnz,xlindx,lindx);
    //  R=adj2sp(xlnz,adjncy,lnz);
    //  =>P*R*R'*P'=A
    m = size(A,1);
    if size(A,2) ~= m | type(A)~=5 | A == [] then,
        error("Matrix must be square, sparse and nonempty.");
    end;
    neqns=m;
    [xadj,adjncy,v]=sp2adj(A-diag(diag(A)));
    [perm,invp,nofsub]=ordmmd(xadj,adjncy,neqns);
    cachsz = 16;
    spcho=symfct(xadj,adjncy,perm,invp,cachsz,neqns);
    [xadjf,adjncyf,v]=sp2adj(A);
    spcho=inpnv(xadjf,adjncyf,v,spcho);
    level=8;
    spcho=blkfc1(spcho,level);
endfunction

function [spcho]= blkfc1(spcho,level)
    //retrieves Fortran variables (see sfinit.f,bfinit.f,symfct.f )
    //[xlnz,nnzl,xsuper,xlindx,lindx,snode,split,tmpsiz,perm,invp,lnz]=spcho(2:12);
    xsuper=spcho("xsuper");
    nsuper=size(xsuper,1)-1;
    snode=spcho("snode");
    neqns=size(snode,1);
    lnz=spcho("lnz");
    nnzl=size(lnz,1);
    iwsiz = 2*neqns+2*nsuper;
    iwork = zeros(iwsiz,1);
    tmpsiz = spcho("tmpsiz");
    tmpvec= zeros(tmpsiz,1);
    iflag=0;
    // calling blkfc1i primitive
    [lnz,iflag]=blkfc1i(neqns,nsuper,xsuper,snode,spcho("split"),spcho("xlindx"),spcho("lindx"),spcho("xlnz"),lnz,iwsiz,iwork,tmpsiz,tmpvec,iflag,level);
    //
    if max(abs(lnz)) > 5d63 then
        warning(gettext("  Possible matrix is not positive definite."));
    end;

    select iflag
    case -1 then,
        error(gettext("Non-positive diagonal encountered, matrix is not positive definite.")),
    case -2 then,
        error(gettext("Insufficient working storage in blkfct, temp(*).")),
    case -3 then,
        error(gettext("Insufficient working storage in blkfct, iwork(*).")),
    end;
    //
    spcho("lnz")=lnz;
endfunction

function rhs=blkslv(spcho,rhs)
    //
    //[xlnz,nnzl,xsuper,xlindx,lindx,snode,split,tmpsiz,perm,invp,lnz]=spcho(2:12);
    xsuper=spcho("xsuper");
    nsuper=size(xsuper,1)-1;
    neqns =size(rhs,1);
    rhs=blkslvi(nsuper,xsuper,spcho("xlindx"),spcho("lindx"),spcho("xlnz"),...
    spcho("lnz"),rhs);
endfunction

function [spcho]=inpnv(xadjf,adjf,anzf,spcho)
    //
    //[xlnz,nnzl,xsuper,xlindx,lindx,snode,split,tmpsiz,perm,invp,lnz]=spcho(2:12);
    //
    xsuper=spcho("xsuper");
    neqns=size(xadjf,1)-1,
    nsuper=size(xsuper,1)-1,
    //
    offset=zeros(neqns,1);
    lnz=zeros(spcho("nnzl"),1);
    //
    lnz = inpnvi(neqns,xadjf,adjf,anzf,spcho("perm"),spcho("invp"),nsuper,...
    xsuper,spcho("xlindx"),spcho("lindx"),spcho("xlnz"),lnz,offset);
    //
    spcho("lnz")=lnz;
endfunction

function [spcho] = symfct(xadj,adjncy,perm,invp,cachsz,neqns)
    //
    // sfinit - input
    //
    nnza=size(adjncy,1);
    iwsiz  = 7*neqns+4;
    iwork=zeros(iwsiz,1);
    ///
    if size(perm)~= [neqns,1] then, error(gettext(" SYMFCT requires PERM to be neqns x 1")),
    end;
    if size(invp)~= [neqns,1] then, error(gettext(" SYMFCT requires INVN to be neqns x 1")),
    end;
    if size(cachsz)~= [1,1] then, error(gettext(" SYMFCT requires CACHSZ  to be 1 x 1")),
    end;
    //
    [perm,invp,colcnt,nnzl,nsub,nsuper,snode,xsuper,iflag]=...
    sfinit(neqns,nnza,xadj,adjncy,perm,invp,iwsiz,iwork);
    //
    if iflag == -1 then error(gettext(" Insufficient working storage in sfinit")),end;
    //
    bb=xsuper(1:nsuper+1,1);
    xsuper=bb
    iwsiz  = 2*nsuper+2*neqns+1;
    iwork=zeros(iwsiz,1);
    //
    //
    [xlindx,lindx,xlnz,iflag]=symfcti(neqns,nnza,xadj,adjncy,perm,...
    invp,colcnt,nsuper,xsuper,snode,nsub,iwsiz,iwork)
    if iflag == -1 then error(" Insufficient working storage in symfct"),end;
    if iflag == -2 then error(" Inconsistancy in the input in symfct"),end;
    //
    [tmpsiz,split]=bfinit(neqns,nsuper,xsuper,snode,xlindx,lindx,cachsz)

    spcho = tlist(["chol","xlnz","nnzl","xsuper","xlindx","lindx","snode","split",...
    "tmpsiz","perm","invp","lnz"],...
    xlnz,nnzl,xsuper,xlindx,lindx,snode,split,tmpsiz,perm,invp,[])
endfunction
