// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function txt = %p_string(p)
    Sizes = size(p)
    p = p(:)             // 1 polynomial per row
    N = size(p,1)        // N rows

    C = coeff(p)          // Matrix of coefficients
    signPs = emptystr(C)  // Pure reals or imaginary: Signs
                          // Mixed complex: Signs of real parts
    valuePs = emptystr(C) // reals: |real|
                          // Pure imaginaries: |imag|i
                          // Mixed complex: mantissa (|real|+-imag i)
    r0 = real(C)
    im0 = imag(C)

    // Special case: null polynomials
    k = (p==0)
    valuePs(k) = "0"

    // Pure real coeffs
    k = find((r0~=0 | isnan(r0)) & im0==0)
    if k~=[] then
        r = r0(k)
        kp = find(r>0 | isnan(r))
        signPs(k(kp)) = "+"
        valuePs(k(kp)) = string(r(kp))
        km = find(r<0)
        signPs(k(km)) = "-"
        valuePs(k(km)) = string(-r(km))
    end

    // pure imag coeffs
    k = find((im0~=0 | isnan(im0)) & r0==0)
    if k~=[] then
        im = im0(k)
        kp = find(im>0 | isnan(im))
        if kp~=[]
            signPs(k(kp)) = "+"
            valuePs(k(kp)) = string(im(kp))+"i"
        end
        km = find(im<0)
        if km~=[]
            signPs(k(km)) = "-"
            valuePs(k(km)) = string(-im(km))+"i"
        end
        kIsOne = find(valuePs(k)==string(1)+"i")
        valuePs(k(kIsOne)) = "i"
    end

    // coeffs with real and imag parts
    k = find((r0~=0 | isnan(r0)) & (im0~=0 | isnan(im0)))
    if k~=[] then
        r = r0(k)
        kp = find(r>0 | isnan(r))
        signPs(k(kp)) = "+"
        km = find(r<0)
        signPs(k(km)) = "-"
        C(k(km)) = -C(k(km))  // + (-1.2+3i) => - (1.2-3i)
        valuePs(k) = "("+string(real(C(k)));

        s = ""
        if format()(1)==0   // format("e")
            s = " "
        end
            // sign of imag part:
        im = imag(C(k))
        kp = find(im>0 | isnan(im))
        if kp~=[]
            valuePs(k(kp)) = valuePs(k(kp)) + s+"+"+s
        end
        km = find(im<0)
        if km~=[]
            valuePs(k(km)) = valuePs(k(km)) + s+"-"+s
            C(k(km)) = conj(C(k(km)))
            im = imag(C(k))
        end
            // 1i => i
        kNoOne = find(abs(im)~=1)
        if kNoOne~=[]
            valuePs(k(kNoOne)) = valuePs(k(kNoOne))+string(im(kNoOne))+"i)"
        end
        kIsOne = find(abs(im)==1)
        if kIsOne~=[]
            valuePs(k(kIsOne)) = valuePs(k(kIsOne))+"i)"
        end
    end

    // Adding the name of the variable
    s = stripblanks(varn(p))
    k = find(C~=0)
    k2 = find(k>N)  // skip z^0 terms
    if k2~=[] then
        valuePs(k(k2)) = valuePs(k(k2))+s
    end

    // Simplifying 1z => z
    k = find(valuePs==string(1)+s)
    valuePs(k) = s
    // Integrating signs to values
    k = find(valuePs~="" & valuePs~="0")
    valuePs2 = valuePs
    if k~=[] then
        valuePs2(k) = " "+signPs(k)+" "+valuePs(k)
    end

    // Exponents
    E = emptystr(C)
    k = find(valuePs~="")
    k2 = find(k>2*N)        // skipping z^0 and z^1 terms
    if k2~=[] then
        k = k(k2)'
        E(k) = msprintf("%d\n",(k-1)/N)

        // Prepending blanks to exponents.
        LE = length(E)
        Ls = length(valuePs2)   // Length of spaces: init
        Ls(:,4:$) = Ls(:,4:$) - LE(:,3:$-1)
        Ls0 = Ls;
        c = Ls(:,2);
        for j = 3:size(Ls,2)
            k = find(Ls(:,j)>0 & c<0);
            Ls(k,j) = Ls(k,j)+c(k);
            c(k) = Ls(k,j);
            k = find(Ls0(:,j)<0);
            c(k) = Ls0(k,j);
        end
        Ls(Ls<0) = 0
        S = strsplit(part(" ",ones(1,3+sum(Ls))), [1 cumsum(Ls)(:)'+1])
        S = matrix(S(2:size(E,"*")+1),N,-1)
        E = S+E
    end

    // Gluing terms
    ps = strcat(valuePs2,"","c")
    es = strcat(E,"","c")

    // Removing heading "+ " and " "
    k = grep(ps,"/^ \+ /","r")
    ps = strsubst(ps, "/^ \+ /", "", "r")
    es(k) = part(es(k),4:$)
    k = grep(ps,"/^ /","r")
    ps = strsubst(ps, "/^ /", "", "r")
    es(k) = part(es(k),2:$)

    // Final reshaping
    txt = [es' ; ps']
    txt = txt(:)
    Sizes(1) = Sizes(1)*2
    txt = matrix(txt,Sizes)
endfunction
