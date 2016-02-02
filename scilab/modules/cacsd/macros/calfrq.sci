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


function [frq, bnds, splitf] = calfrq(h, fmin, fmax)
    //!

    eps = 1.d-14   //minimum absolute lower frequency
    k = 0.001      // Minimum relative prediction error in the nyquist plan
    epss = 0.002   // minimum frequency distance with a singularity
    nptmax = 5000  //maximum number of discretization points
    tol = 0.01     // Tolerance for testing pure imaginary numbers

    // Check inputs
    // ------------
    if and(typeof(h) <> ["state-space" "rational"])
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"), "calfrq", 1))
    end
    if typeof(h) == "state-space" then
        h = ss2tf(h)
    end

    [m, n] = size(h.num)
    dom = h("dt")
    select dom
    case "d" then
        dom = 1
    case [] then
        error(96, 1)
    case 0 then
        error(96, 1)
    end;

    if type(dom) == 1 then
        nyq_frq = 1/2/dom;
        if fmax > nyq_frq then
            warning(msprintf(gettext("%s: Frequencies beyond Nyquist frequency are ignored.\n"), "calfrq"));
            fmax = min(fmax, nyq_frq)
        end
        if fmin < -nyq_frq then
            warning(msprintf(gettext("%s: Negative frequencies below Nyquist frequency are ignored.\n"), "calfrq"));
            fmin = max(fmin, -nyq_frq)
        end
    end
    // Use symmetry to reduce the range
    // --------------------------------
    if fmin < 0 & fmax >= 0 then
        [frq, bnds, splitf] = calfrq(h, eps, -fmin)
        ns1 = size(splitf, "*")-1;
        nsp = size(frq, "*");
        bnds = [bnds(1), bnds(2), -bnds(4), -bnds(3)];

        if fmax > eps then
            if fmax == -fmin then
                splitf = [1, (nsp+2)*ones(1,ns1)-splitf($:-1:2), nsp*ones(ns1)+splitf(2:$)];
                bnds = [bnds(1), bnds(2), min(bnds(3), -bnds(3)), max(bnds(4), -bnds(4))];
                frq = [-frq($:-1:1), frq]
            else
                [frq2, bnds2, splitf2] = calfrq(h, eps, fmax);
                ns2 = size(splitf2,"*")-1
                splitf = [1, (nsp+2)*ones(1,ns1)-splitf($:-1:2), nsp*ones(ns2)+splitf2(2:$)];
                bnds = [min(bnds(1), bnds2(1)), max(bnds(2), bnds2(2)),...
                min(bnds(3), bnds2(3)), max(bnds(4), bnds2(4))];
                frq = [-frq($:-1:1), frq2]
            end
            return
        else
            frq = -frq($:-1:1);
            nsp = size(frq, "*");

            splitf = [1, (nsp+2)*ones(1, ns1)-splitf($:-1:2)]
            bnds = bnds;
            return;
        end
    elseif fmin < 0 & fmax <= 0 then
        [frq, bnds, splitf] = calfrq(h, -fmax, -fmin)
        ns1 = size(splitf, "*")-1;
        frq = -frq($:-1:1);
        nsp = size(frq, "*");
        splitf = [1, (nsp+2)*ones(1, ns1)-splitf($:-1:2)]
        bnds = [bnds(1), bnds(2), -bnds(4), -bnds(3)];
        return;
    elseif fmin >= fmax then
        error(msprintf(gettext("%s: Wrong value for input arguments #%d and #%d: %s < %s expected.\n"),..
        "calfrq", 2, 3, "fmin", "fmax"));
    end

    // Compute dicretisation over a given range
    // ----------------------------------------


    splitf = []
    if fmin == 0 then fmin = min(1d-14, fmax/10);end
    //
    denh = h("den"); numh = h("num")
    l10 = log(10)

    // Locate singularities to avoid them
    // ----------------------------------
    if dom == "c" then
        c = 2*%pi;
        // selection function for singularities in the frequency range
        deff("f=%sel(r, fmin, fmax, tol)",["f = [],";
        "if prod(size(r)) == 0 then return, end";
        "f = imag(r(find((abs(real(r))<=tol*abs(r))&(imag(r)>=0))))";
        "if f <> [] then f = f(find((f>fmin-tol)&(f<fmax+tol))); end"]);
    else
        c = 2*%pi*dom
        // selection function for singularities in the frequency range
        deff("[f] = %sel(r, fmin, fmax, dom, tol)",["f = [],";
        "if prod(size(r)) == 0 then return, end";
        "f = r(find( ((abs(abs(r)-ones(r)))<=tol)&(imag(r)>=0)))";
        "if f <> [] then ";
        "  f = atan(imag(f), real(f)); nf = prod(size(f))";
        "  for k=1:nf,";
        "    kk = int((fmax-f(k))/(2*%pi))+1;";
        "    f = [f; f(1:nf)+2*%pi*kk*ones(nf, 1)];";
        "  end;"
        "  f = f(find((f>fmin-tol)&(f<fmax+tol)))";
        "end"]);
    end

    sing = [];zers = [];
    fmin = c*fmin, fmax = c*fmax;

    for i=1:m
        sing = [sing; %sel(roots(denh(i), "e"), fmin, fmax, tol)];
    end

    pp = gsort(sing', "g", "i"); npp = size(pp, "*");//'

    // singularities just on the left of the range
    kinf = find(pp<fmin)
    if kinf <> [] then
        fmin = fmin+tol
        pp(kinf) = []
    end

    // singularities just on the right of the range
    ksup = find(pp>=fmax)
    if ksup <> [] then
        fmax = fmax-tol
        pp(ksup) = []
    end

    // check for nearly multiple singularities
    if pp <> [] then
        dpp = pp(2:$)-pp(1:$-1)
        keq = find(abs(dpp)<2*epss)
        if keq <> [] then pp(keq) = [], end
    end

    if pp <> [] then
        frqs = [fmin real(matrix([(1-epss)*pp; (1+epss)*pp], 2*size(pp, "*"), 1)') fmax]
        //'
    else
        frqs = [fmin fmax]
    end
    nfrq = size(frqs, "*");

    // Evaluate bounds of nyquist plot
    //-------------------------------
    xt = []; Pas = []
    for i=1:2:nfrq-1
        w = logspace(log(frqs(i))/log(10), log(frqs(i+1))/log(10), 100);
        xt = [xt, w]
        Pas = [Pas w(2)-w(1)]
    end
    if dom == "c" then
        rf = freq(h("num"), h("den"), %i*xt);
    else
        rf = freq(h("num"), h("den"), exp(%i*xt));
    end
    //
    xmin = min(real(rf)); xmax = max(real(rf));
    ymin = min(imag(rf)); ymax = max(imag(rf));
    bnds = [xmin xmax ymin ymax];
    dx = max([xmax-xmin, 1]); dy = max([ymax-ymin, 1]);

    // Compute discretization with a step adaptation method
    // ----------------------------------------------------
    frq = [];
    i = 1;
    nptr = nptmax; // number of unused discretization points
    l10last = log10(frqs($));
    while i<nfrq
        f0 = frqs(i); fmax = frqs(i+1);
        while f0==fmax do
            i = i+2;
            f = frqs(i); fmax = frqs(i+1);
        end
        frq = [frq, f0];
        pas = Pas(floor(i/2)+1)
        splitf = [splitf size(frq, "*")];

        f = min(f0+pas, fmax);

        if dom == "c" then // Continuous case
            while f0<fmax
                rf0 = freq(h("num"), h("den"), (%i*f0));
                rfc = freq(h("num"), h("den"), %i*f);
                // compute prediction error
                epsd = pas/100; // epsd = 1.d-8

                rfd = (freq(h("num"), h("den"), %i*(f0+epsd))-rf0)/(epsd);
                rfp = rf0+pas*rfd;

                e = max([abs(imag(rfp-rfc))/dy; abs(real(rfp-rfc))/dx])
                if e > k then rf0 = freq(h("num"), h("den"), (%i*f0));
                    rfc = freq(h("num"), h("den"), %i*f);
                    // compute prediction error
                    epsd = pas/100; // epsd = 1.d-8

                    rfd = (freq(h("num"), h("den"), %i*(f0+epsd))-rf0)/(epsd);
                    rfp = rf0+pas*rfd;

                    e = max([abs(imag(rfp-rfc))/dy; abs(real(rfp-rfc))/dx])
                    // compute minimum frequency logarithmic step to ensure a maximum
                    //of nptmax points to discretize
                    pasmin = f0*(10^((l10last-log10(f0))/(nptr+1))-1)
                    pas = pas/2
                    if pas < pasmin then
                        pas = pasmin
                        frq = [frq, f]; nptr = max([1, nptr-1])
                        f0 = f; f = min(f0+pas, fmax)
                    else
                        f = min(f0+pas, fmax)
                    end
                elseif e < k/2 then
                    pas = 2*pas
                    frq = [frq, f]; nptr = max([1, nptr-1])
                    f0 = f; f = min(f0+pas, fmax),
                else
                    frq = [frq, f];nptr = max([1, nptr-1])
                    f0 = f; f = min(f0+pas, fmax),
                end
            end
        else  // Discrete case
            pas = pas/dom
            while f0<fmax
                rf0 = freq(h("num"), h("den"), exp(%i*f0))
                rfd = dom*(freq(h("num"), h("den"), exp(%i*(f0+pas/100)))-rf0)/(pas/100);
                rfp = rf0+pas*rfd
                rfc = freq(h("num"), h("den"), exp(%i*f));
                e = max([abs(imag(rfp-rfc))/dy; abs(real(rfp-rfc))/dx])
                if e > k then
                    pasmin = f0*(10^((l10last-log10(f0))/(nptr+1))-1)
                    pas = pas/2
                    if pas < pasmin then
                        pas = pasmin
                        frq = [frq, f]; nptr = max([1, nptr-1])
                        f0 = f; f = min(f0+pas, fmax)
                    else
                        f = min(f0+pas, fmax)
                    end
                elseif e < k/2 then
                    pas = 2*pas
                    frq = [frq, f]; nptr = max([1, nptr-1])
                    f0 = f; f = min(f0+pas, fmax),
                else
                    frq = [frq, f]; nptr = max([1, nptr-1])
                    f0 = f; f = min(f0+pas, fmax),
                end
            end
        end
        i = i+2
    end
    frq(size(frq, "*")) = fmax
    frq = frq/c;
endfunction
