// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [p, dmin, status] = datafit(iprint, G, varargin)
    //
    //         [p,err]=datafit([iprint,] G [,DG], Data [,Wd] [,Wg],...)
    //
    // Data: matrix where Data(:,i) are the nzc coordinates of the ith measurement point
    //       nz  = size(Data,"c") : number of measured available points.
    //       nzc = size(Data,"r") : number of coordinates for each Data point.
    //    G: Scilab function (e = G(p,Data) (e: ne x nz)
    //   p0: initial guess of parameters values (vector or matrix)
    //       np = length(p0) = length(p) : number of fitting parameters
    //   Wd: Data weights (optionnal). Row of length nz.
    //   Wg: weighting matrix of gaps definitions, of size ng x ng (optional)
    //   DG: partial of G wrt p (optional; S=DG(p,Data), S: ne x np x nz)
    //
    //      p: vector of best parameters values (minimizing dmin)
    // status: integer as returned by optim() when the default "qn" algo is used.
    //   dmin: scalar = average distance of data points to model.

    [lhs,rhs] = argn(0)

    // Processing the 2 first argin imp and G:
    if type(iprint)<>1 then      // 1st argin iprint is actually G (no iprint directive)
        wflag = warning("query") // Disable warnings as the following lines may produce some
        warning("off")
        varargin(0) = G     // so the argin G is the first arg after the true G
        G = iprint          // G gets its true value
        iprint = 0          // iprint gets its default value
        warning(wflag)
    end

    if type(G)==15 then     // G = list(G, params)
        Gparams = G;
        Gparams(1) = null();
        G=G(1)
    else
        Gparams = list()    // no explicit parameter
    end

    // Next argins: optional dG (Jacobian), or mandatory points Data
    DG = varargin(1)
    if type(DG)==10 | type(DG)==13 then   // is a function
        GR = %t      // Jacobian provided
        varargin(1) = null()
    elseif type(DG)==15 then
        msg = gettext("%s: Jacobian cannot be a list, parameters must be set in G.")
        error(msprintf(msg, "datafit"));
    else
        GR = %f  // Jacobian not provided
    end

    Data = varargin(1);          // Data points to fit
    varargin(1) = null()
    if type(Data) <>1 | ~isreal(Data,0) then
        msg = gettext("%s: Data points must be real numbers.");
        error(msprintf(msg, "datafit"));
    end
    [mz,nz] = size(Data);    // nz: Number of points

    // Data weights: Must be a line vector of length nz (Wg can _neither_ be so long)
    if size(varargin(1),2)==nz
        Wd = varargin(1)
        varargin(1) = null()
    else
        Wd = ones(1,nz);
    end
    // Wg assignment / initialization  (from [,Wg][,'b', pinf, psup], p0 [,algo])
    nv = size(varargin);
    if nv==0 then
        msg =  gettext("%s: Initial guess p0 is missing.");
        error(msprintf(msg, "datafit"));
    elseif nv==1 then   // p0 expected
        Wg = 1;
    else
        v1 = varargin(1);
        v2 = varargin(2);
        if v1=="b"| (nv>=5 & or(varargin(5)==["qn" "gc" "nd" "ar" "in"]))
            Wg = 1
        elseif (nv>1 & v2=="b") | ..
               (nv>=6 & or(varargin(6)==["qn" "gc" "nd" "ar" "in"])) | ..
               (type(v1)==1 & issquare(v1) & nv>1 & type(v2)==1 & (size(v2,"*")>1 | nv==2))
            Wg = varargin(1)
            if size(Wg,1) ~= size(Wg,2) then
                msg = gettext("%s: Weighting matrix must be square.");
                error(msprintf(msg, "datafit"));
            end
            varargin(1) = null()
            nv = nv-1
        else
            Wg = 1;
        end
    end
    // next: [,'b', pinf, psup], p0 [,algo])
    if varargin(1)=="b" then
        if nv<4
            msg = gettext("%s: error with parameters bounds or initial guess\n");
            error(msprintf(msg, "datafit"));
        end
        p0 = varargin(4);
    else
        p0 = varargin(1);
    end
    sp0 = size(p0);
    np = size(p0,"*");    // Number of parameters
    _format = format()
    format("v",23)        // for string()

    if type(G)==10 then   // form function to call hard coded external
        if size(Gparams)==0 then
            format(_format(2),_format(1))
            msg = gettext("%s: With hard coded function, user must give output size of G.")
            error(msprintf(msg, "datafit"));
        end
        m = Gparams(1);
        Gparams(1) = null();

        // foo(m,np,p,mz,nz,Data[,Wd],pars,f)
        tmp = "f=call(''" + G + "''," + ..
              "m,1,''i'', np,2,''i'', p,3,''d'', mz,4,''i'', nz,5,''i''," + ..
              "Data,6,''d'', pars,7,''out'',[" + string(m) + ",1],8,''d'')"
        deff("f = G(p,Data)", tmp)

        pars = [];
        for k = 1:size(Gparams)
            p = Gparams(k)
            pars = [pars;p(:)]
        end
        Gparams = list()
    end

    if type(DG)==10 then // form function to call hard coded external
        // dfoo(m,np,p,mz,nz,Data[,Wd],pars,f)
        tmp = "f = call(''" + DG + "''," + ..
          "m,1,''i'', np,2,''i'', p,3,''d'', mz,4,''i'', nz,5,''i'',Data,6,''d''," + ..
          "pars,7,''out'',[" + string(m) + "," + string(np) + "],8,''d'')"
        deff("f = DG(p,Data)", tmp)
    end

    // Is the G function vectorized?
    try
        if Gparams==list() then
            e = G(p0,[Data(:,1) Data(:,1)]);
        else
            e = G(p0,[Data(:,1) Data(:,1)], Gparams(:));
        end
        isGvec = size(e,2)==2
    catch
        isGvec = %f;
    end

    // form square cost gradient function DGG
    if Gparams==list() then
        GP   = "G(p,  Data(:,i))"
        GPPV = "G(p+v,Data(:,i))"
        DGP  = "DG(p, Data(:,i))"
    else
        GP   = "G(p,  Data(:,i), Gparams(:))"
        GPPV = "G(p+v,Data(:,i), Gparams(:))"
        DGP  = "DG(p, Data(:,i), Gparams(:))"
    end

    if ~GR then // finite difference
        // Version vectorized over Data:
        if isGvec then
            DGG = [
            "g  = 0*p"
            "pa = sqrt(%eps)*(1+1d-3*abs(p))"
            "i = : "
            "g1 = " + GP
            "f = sum(sum((g1''*Wg) .* g1'',""c"").*Wd'');"
            "for j = 1:" + msprintf("%d\n",np)
            "    v = 0*p;"
            "    v(j) = pa(j);"
            "    g1 = " + GPPV
            "    e = sum(sum((g1''*Wg) .* g1'',""c"").*Wd'');"
            "    g(j) = e - f;"
            "end"
            "g = g ./ pa;"
            ];
        else
            DGG = [
            "g  = 0*p"
            "pa = sqrt(%eps)*(1+1d-3*abs(p))"
            "f = 0;"
            "for i = 1:" + msprintf("%d\n", nz)
            "    g1 = " + GP
            "    f = f + g1''*Wg*g1 * Wd(i)"
            "end"
            "for j = 1:" + msprintf("%d\n", np)
            "    v = 0*p;"
            "    v(j) = pa(j),"
            "    e = 0;"
            "    for i = 1:" + msprintf("%d\n", nz)
            "        g1 = " + GPPV
            "        e = e + g1''*Wg*g1 * Wd(i)"
            "    end"
            "    g(j) = e - f;"
            "end"
            "g = g ./ pa;"
            ];
        end
    else // using Jacobian of G  (UNVECTORIZED)
        DGG = "g=0; for i=1:nz, g = g + 2*"+DGP+"''*Wg*"+GP+", end"
    end

    // Defining the costf() function for optim:
    if isGvec then
        tmp = [
            "    i = : "
            "    g1 = " + GP
            "    f = sum(sum((g1''*Wg) .* g1'',""c"") .* Wd'');"
            ]
    else
        tmp = [
            "    f = 0;"
            "    for i = 1:" + msprintf("%d\n", nz)
            "        g1 = " + GP
            "        f = f + g1''*Wg*g1 * Wd(i)"
            "    end"
            ]
    end
    deff("[f,g,ind] = costf(p,ind)",[
        "if ind==2 | ind==4 then "
        tmp
        "else"
        "    f = 0;"
        "end";
        "if ind==3 | ind==4 then"
        DGG
        "else"
        "    g = 0*p;"
        "end"
        ])

    format(_format(2), _format(1))
    qn = %t;
    for v = varargin
        if or(v==['gc' 'nd'])
            qn = %f;
            break
        end
    end
    if qn then
        [fmin, p, t,t,t,t, status] = optim(costf, varargin(:), iprint=iprint)
    else
        [fmin, p] = optim(costf, varargin(:), iprint=iprint)
        status = %nan;
    end

    p = matrix(p, sp0);

    // fmin => dmin: average Model-to-data distance
    ng = size(G(p, Data(:,1)),1);
    dmin = sqrt(fmin/ng/sum(Wd));
endfunction
