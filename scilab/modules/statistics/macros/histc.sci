// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [heights, jokers, binsOut, ind] = histc(data, bins, options)
// Scilab 5.5:
//  1) histc(bins, data, normalization)
//  2) By défaut, histc() worked in "densityNorm,normWith:all" mode
// Scilab 6.0:
//  1) histc(data, bins, options)
//  2) By default, histc() works in "counts" mode

//    histc(data)
//    histc(data, nbins)
//    histc(data, binsWidth<0)
//    histc(data, binsEdges)
//    histc(data, binsValues [, "discrete"])
//    histc(data, binsAlgo)
//    histc(data, ..., options)
//    histc(nbins, data)                        // deprecated
//    histc(binsEdges, data)                    // deprecated
//    histc(binsEdges, data, normalization=%t)  // deprecated
//     heights                        = histc(data ..)
//    [heights, jokers]               = histc(data ..)
//    [heights, jokers, binsOut]      = histc(data ..)
//    [heights, jokers, binsOut, ind] = histc(data ..)
//
//    Binning algorithm:
//      "sqrt" | "sturges" | "freediac"
//    Binning options:
//      "discrete"  // , "smartbins"
//    Scale options:
//      "counts" (default) | "countsNorm" | "density" | "densityNorm"
//    Normalization "normWith: " options:  Priorities: "all" > "out" > ..
//     decimal, complex numbers: "leftout rightout out inf nan zeros all"
//                     integers: "leftout rightout (out=) all"
//                         Text: "empty out all"
//                  polynomials: "(out=) all"
//       vector or single string with space-separated flags (case-unsensitive)
//
//    Complex numbers are sorted by real parts, then by imaginary parts
//    jokers =
//      * integers: [leftout, rightout] (continuous) or [out] (discrete)
//      * decimal or complex numbers, full or sparse:
//            continuous: [leftout, rightout, zeros, nan, inf]
//              discrete: [out, 0, zeros, nan, inf]
//      * polynomials: [out, 0, 0, nan, inf]
//      * texts: continuous: [leftout, rightout, ""]; discrete: [out, 0, ""]
//

    [lhs, rhs] = argn();
    fname = "histc"
    [heights, jokers, binsOut, ind] = ([],[],[],[])
    [Nout, Nleftout, Nzeros, Nrightout, Ninf, Nnan] = (0,0,0,0,0,0)

    // PARSING INPUT ARGUMENTS
    // =======================
    if rhs<1 | rhs>3 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 1, 3))
    end
    // Old 5.5.x mode management
    // -------------------------
    mode55 = %f
    // Check for Scilab 5.5 back-compatibility
    if (isdef("data","l") & isdef("bins", "l") & type(bins)>0) & ..
        size(data,"*")< size(bins,"*") & or(type(data)==[1 8]) & type(bins)~=10
        msg = _("%s: data are now expected in argument #1 => arguments #1 and #2 switched\n")
        warning(msprintf(msg, fname))
        mode55 = %t
        [data, bins] = (bins, data)
        tmp = isdef("options","l") && type(options)==4
        normarg = %f
        norma = %f
        if isdef("normalization","l")
            normarg = %t
            if type(normalization)==4 then
                norma = normalization(1)
            end
        else
            if rhs<3 then
                norma = %t
            else
                if isdef("options","l") & type(options)==4
                    normarg = %t
                    norma = options(1)
                end
            end
        end
        if norma then
            options = ["densityNorm" "normWith:all"]
        else
            options = "counts"
        end
        if normarg
            msg = _("%s: ""normalization"" boolean argument #3 is obsolete. ""%s"" option used.\n")
            warning(msprintf(msg, fname, options(1)))
        end
    end
    // -----------------------
    if ~isdef("data","l") | type(data)==0 then
        msg = _("%s: Argument #%d: %s\n")
        error(msprintf(msg, fname, 1, "data array missing"))
    end
    if ~isdef("bins","l") | type(bins)==0 then
        bins = []
    end
    if ~isdef("options","l") then
        options = "counts"
    end
    // Now, bins, data and options are defined. Let's check their types and sizes

    // CHECKING INPUT ARGUMENTS AND SETTING DEFAULTS
    // =============================================
    // Options
    // -------
    if options~=[]
        if type(options)~=10 then
            msg = _("%s: Argument #%d: Text expected.\n")
            error(msprintf(msg, fname, 3))
        end
        options = tokens(strcat(convstr(options),","),",")
    end
    discrete = grep(options, "discrete")~=[] | type(data)==2 // polynomials

    // Data
    // ----
    if isempty(data) then
        return
    end
    if ~or(type(data)==[1 2 5 8 10]) then
        msg = _("%s: Data in argument #%d: Numbers or polynomials or texts expected.\n")
        error(msprintf(msg, fname, 1))
    end
    data0 = data    // Saving
    // If encoded integers (including u-int64): => decimal (for dsearch(), etc)
    if type(data) == 8 then
        data = double(data)
    end
    // If sparse numbers: we remove zeros
    if type(data)==5
        [IJ, data] = spget(data)
    end
    // If there are only %inf or %nan => return ==!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if ~discrete & or(type(data0)==[1 5]) then
        L = data
        tmp = isnan(data)
        Nnan = sum(tmp)
        L(tmp) = []
        tmp = isinf(data)
        Ninf = sum(tmp)
        L(tmp) = []
        if data==[]
            msg = _("%s: data have only %inf or/and %nan values.\n")
            warning(msprintf(msg, fname))
            if type(data0)==1
                Nzeros = sum(data==0)
            else
                Nzeros = size(data0,"*") - Nnan - Ninf
            end
            Nleftout = 0
            Nrightout = 0
            if length(bins)>1
                b = bins(~isinf(bins))
                if 0<min(b)
                    Nleftout = Nzeros
                end
                if 0 > max(b)
                    Nrightout = Nzeros
                end
            end
            jokers = [Ninf Nnan Nzeros Nleftout Nrightout]
            return
        end
    end
    // Polynomials: setting Nan and Ninf
    if type(data)==2
        L = data
        tmp = isnan(L)
        Nnan = sum(tmp)
        L(tmp) = []     // to ensure excluding any intersection
        tmp = isinf(L)
        Ninf = sum(tmp)
    end

    // Bins
    // ----
    // Setting a default binning mode
    if bins==[] then
        // histc(data): Default bins:
        //  * strings: discrete, from unique()
        //  * polynomials: discrete, from defBins set hereabove
        //  * encoded integers: "sqrt". if strange() < "sqrt" => binsWidth = 1
        //  * decimal, complex: binMethod option or default "sqrt"
        if type(data)==10       // texts
            discrete = %t
            bins = unique(data)(:)'
            bins = bins(:)'

        elseif type(data)==2    // Polynomials
            L = data
            tmp = isnan(L)
            L(tmp) = []
            bins = []
            while L~=[]
                bins = [bins L(1)]
                L(L==L(1)) = []
            end
            clear L

        else    // decimals, complexes, encoded integers
            if ~discrete
                bins = "sqrt"
            else
                bins = unique(data)(:).'
            end
        end
    end
    //
    if ~discrete
        if type(bins)==8 | (type(bins)==1 & ~isreal(bins))
            bins = real(double(bins))
        end
        if size(bins,"*")==1 & type(bins)==10 then
            // binsAlgo
            // --------
            if ~or(convstr(bins)==["sqrt" "sturges" "freediac"])
                msg = _("%s: Argument #%d: wrong value for binning algo")
                error(msprintf(msg, fname, 2))
            end
            // We set the number of bins. Their edges are set later.
            Nvalid = size(data,"*") - Nnan - Ninf
            select bins
            case "sturges"
                bins = ceil(1 + log2(Nvalid))
            case "freediac"
                tmp = real(data)
                binWidth = 2*iqr(tmp)* Nvalid^(-1/3)
                if binWidth~=0
                    bins = round((max(tmp) - min(tmp)) / binWidth)
                else
                    bins = 2
                end
            else  // "sqrt"
                bins = max(1, round(sqrt(Nvalid)))
            end
        end

        if length(bins)==1 & type(bins)==1  then
            // The number of bins or their width is provided
            if type(data)==10   // texts
                msg = _("%s: Argument #2: Please provide bins edges or values or leave choosing default bins.\n")
                error(msprintf(msg, fname))
            end
            // Case polynomials: not possible here <= discrete previously set to %t
            // Cases: Encoded integers, decimal or complex numbers
            if bins>=0 & (bins~=floor(bins) | bins==0) | isnan(bins) | bins==%inf
                msg = _("%s: Argument #%d: non-zero decimal integer expected.\n")
                error(msprintf(msg, fname, 2))
            elseif bins==-%inf
                msg = _("%s: Argument #%d: decimal number > -Inf expected.\n")
                error(msprintf(msg, fname, 2))
            end

            // If data are complex-encoded, they will be sorted by increasing
            //  real parts, then by increasing imaginary parts.
            b = data(~isnan(data) & ~isinf(data))
            b = real(b)
            mind = min(b)
            maxd = max(b)
            if type(data0)==1
                if bins > 0
                    // Required number of bins
                    if (mind == maxd) then
                        mind = mind*(1 - 0.5/bins);
                        maxd = maxd*(1 + 0.5/bins);
                    end
                    bins = linspace(mind, maxd, bins+1);          // Bins edges
                else
                    // Required bins width
                    if (mind == maxd) then
                        bins = mind + [1 -1]*bins/2
                    else
                        // Edges set à 0.0, width = |bins|, from mind- to maxd+
                        tmp = -bins
                        bins = (floor(mind/tmp):ceil(maxd/tmp))*tmp
                    end
                end
            else
                // Encoded integers
                if bins > 0
                    // Required number of bins
                    tmp = maxd - mind + 1
                    if tmp <= bins & tmp <= 2^16  // integer-centered bins of width==1
                        bins = (mind:(maxd+1)) - 0.5              // Bins edges
                    else
                        ds = (maxd - mind)/bins/2
                        bins = linspace(mind-ds, maxd+ds, bins+1) // Bins edges
                    end
                else
                    // Required bins width
                    tmp = max(round(-bins),1) // Bins edges @ -0.5 modulo |int(bins)|
                    bins = (floor(mind/tmp):ceil(maxd/tmp))*tmp - 0.5 // Bins edges
                end
            end

        else
            // Bins edges are provided: we check that edges are sorted
            // Texts
            // -----
            if type(data0)==10
                if type(bins)~=10
                    msg = _("%s: Arguments #%d and #%d: Same types expected.\n")
                    error(msprintf(msg, fname, 1, 2))
                end
                if ~and(bins==unique(bins))
                    msg = _("%s: Argument #%d: Elements must be sorted in increasing order.\n")
                    error(msprintf(msg, fname, 1))
                end

            // Numbers
            // -------
            else
                if ~or(type(bins)==[1 8])
                    msg = _("%s: Arguments #%d and #%d: Same types expected.\n")
                    error(msprintf(msg, fname, 1, 2))
                end
                bins = bins(~isnan(bins))
                if min(diff(bins)) <= 0 then
                    msg = _("%s: Argument #%d: Elements must be in increasing order.\n")
                    error(msprintf(msg, fname, 1))
                end
                // We manage -%inf open left bin
                if bins(1)==-%inf
                    tmp = min(data(~isinf(data)))
                    if bins(2)<=tmp
                        bins(1) = []
                    else
                        bins(1) = tmp
                    end
                end
                // We manage %inf open right bin
                if bins($)==%inf
                    tmp = max(data(~isinf(data)))
                    if bins($-1)>=tmp
                        bins($) = []
                    else
                        bins($) = tmp
                    end
                end
            end
        end
         // if ~discrete

    else    // if discrete
        if type(data0)==10
            if type(bins)~=10
                msg = _("%s: Arguments #%d and #%d: Same types expected.\n")
                error(msprintf(msg, fname, 1, 2))
            end
            [bins, dataOrder] = unique(bins)

        elseif or(type(data)==[1 8])
            if ~or(type(bins)==[1 8])
                msg = _("%s: Arguments #%d and #%d: Same types expected.\n")
                error(msprintf(msg, fname, 1, 2))
            end
            bins = real(double(bins))
            bins(isnan(bins)) = []
            [bins, dataOrder] = unique(bins)
        end
    end
    binsOut = bins

    // PROCESSING
    // ==========
    // jokers =
    //  * integers: [leftout, rightout] (continuous) or [out] (discrete)
    //  * decimal or complex numbers, full or sparse:
    //       continuous: [inf, nan, zeros, leftout, rightout]
    //         discrete: [inf, nan, zeros, out]
    //  * polynomials: [inf, nan, out]
    //  * texts: continuous: [empty, leftout, rightout]; discrete: [empty, out]
    if type(data0)~=2 then
        hmode = "c"            // continuous
        if discrete then
            hmode = "d"        // discrete
        end
        if or(type(data0)==[1 5])
            if type(data0)==1
                tmp = data0
            else
                tmp = data     // sparse
            end
            tmp1 = isinf(real(tmp))
            tmp2 = isinf(imag(tmp))
            tmp3 = isnan(tmp) | (tmp1 & tmp2)
            if ~discrete & ~isreal(tmp,0)
                data(tmp1 | tmp2) = %inf
                data(tmp3) = %nan
            end
            clear tmp tmp1 tmp2 tmp3
            data = real(data)
        end
        [ind, heights] = dsearch(data, bins, hmode)
                                     // ind: bin's number of each data component

        if discrete
            // We restore the original order of binsValues:
            [tmp, k]  = gsort(dataOrder, "g","i")
            [tmp, k2] = gsort(k,"g","i")
            bins = bins(k)
            binsOut = bins
            heights = heights(k)
            tmp2 = ind~=0
            ind(tmp2) = k2(ind(tmp2))
        end

    else // Polynomials
        // Counts (and location)
         ind = zeros(data)
         heights = []
         for i = 1:size(bins,"*")
             tmp = data==bins(i)
             heights(1,i) = sum(tmp)
             ind(tmp) = i
         end
         tmp = Ninf - sum(heights(isinf(bins)))     // inf out of the selection
         Nout = size(data,"*") - sum(heights) - Nnan - tmp// excludes all nan and inf not in the selection
         jokers = [Nout, 0, 0, Nnan, Ninf]
    end

    // POST-PROCESSING
    // ===============
    // Counting jokers
    // ---------------
    if or(type(data0)==[1 5])        // Real and complex numbers, sparse or not
        inf = isinf(data)
        Ninf = sum(inf)
        Nnan = sum(isnan(data))
        if type(data0)==1
            Nzeros = sum(data==0)
        else
            Nzeros = size(data0,"*") - nnz(data0)
        end
        if ~discrete                 // Sparses: 0 is not taken into account
            Nleftout  = sum(data(~inf) < bins(1))
            Nrightout = sum(data(~inf) > bins($))
            jokers = [Nleftout Nrightout Nzeros Nnan Ninf]
        else
            Nout = size(data0,"*") - sum(heights) - Nnan - Ninf
            binsinf = isinf(bins)
            if or(binsinf)
                Nout = Nout + sum(heights(binsinf))
            end
            if type(data0)==5
                Nout = Nout - Nzeros
            end
            jokers = [Nout 0 Nzeros Nnan Ninf]
        end

    elseif type(data0)==8            // Encoded integers
        if ~discrete
            Nleftout  = sum(data < bins(1))
            Nrightout = sum(data > bins($))
            jokers = [Nleftout Nrightout]
            Nout = Nleftout + Nrightout
        else
            Nout = size(data,"*") - sum(heights)
            jokers = Nout
        end

    elseif type(data0)==10           // Texts
        Nempty = sum(data0=="")
        Nout = size(data0,"*") - sum(heights)
        if ~discrete
            tmp = gsort(data,"g","i")
            Nleftout = sum(find(strcmp(tmp,bins(1))<0)($)) - Nempty
            tmp = sum(find(strcmp(tmp,bins($))>0)(1))
            if ~tmp
                Nrightout = 0
            else
                Nrightout = size(data,"*") - tmp + 1
            end
            clear tmp
            jokers = [Nleftout, Nrightout, Nempty]
        else
            if ~or(bins=="")
                Nout = Nout - Nempty
            end
            jokers = [Nout, 0, Nempty]
        end

    // Polynomials: jokers already set when setting bins
    end

    // Tuning the histogram scale
    // --------------------------
    histmode = "counts"
    if discrete then
        tmp = grep(options, ["counts" "countsnorm"])
    else
        tmp = grep(options, ["counts" "countsnorm" "density" "densitynorm"])
    end
    if tmp~=[] then
        histmode = options(tmp($))
    end
    // jokers =
    //  * integers: [leftout, rightout] (continuous) or [out] (discrete)
    //  * decimal or complex numbers, full or sparse:
    //       continuous: [leftout, rightout, zeros, nan, inf]
    //         discrete: [out, 0, zeros, nan, inf]
    //  * polynomials: [out, 0, 0, nan, inf].
    //  * texts: continuous: [leftout, rightout, empty]; discrete: [out, empty]

    // Number of components to take into account:
    // Parsing the option "normWith:
    //  Continuous: " leftout rightout inf nan zeros"
    //    Discrete: " out inf nan zeros"
    if grep(histmode, "norm") then
        N = sum(heights)
        tmp = grep(options, "normwith")
        normMode = options(tmp)
        if normMode~=[]
            gh = "/[\:|\s+]"    // grep() header
            if ~discrete
                if or(type(data0)==[1 2 5])
                    if grep(normMode, gh+["inf" "all"]+"/", "r")~=[]
                        N = N + Ninf
                    end
                    if grep(normMode, gh+["nan" "all"]+"/", "r")~=[]
                        N = N + Nnan
                    end
                    if type(data0)==5 & ..
                       grep(normMode,gh+["zeros" "all"]+"/", "r")~=[]
                        N = N + Nzeros
                    end
                end
                if grep(normMode, gh+["leftout" "out" "all"]+"/", "r")~=[]
                    N = N + Nleftout
                end
                if grep(normMode, gh+["rightout" "out" "all"]+"/", "r")~=[]
                    N = N + Nrightout
                end
                if type(data0)==10 & grep(normMode, gh+["empty" "all"]+"/", "r")~=[]
                    N = N + Nempty
                end
            // Normalized discrete mode:
            else
                if grep(normMode, gh+"all/", "r")
                    N = size(data0,"*")
                else
                    if grep(normMode, gh+"out/", "r")
                        N = N + Nout
                    end
                    if or(type(data0)==[1 2 5])
                        if grep(normMode, gh+"nan/", "r")
                            N = N + Nnan
                        end
                        if grep(normMode, gh+"inf/", "r")
                            N = N + Ninf
                            // Inf already in bins must not be counted twice:
                            binsinf = isinf(bins)
                            if or(binsinf)
                                N = N - sum(heights(binsinf))
                            end
                        end
                        if type(data0)==5 & and(bins~=0) & ..
                           grep(normMode, gh+"zeros/", "r")~=[]
                            N = N + Nzeros
                        end
                    end
                    if type(data0)==10 & ~or(bins=="") & ..
                        grep(normMode, gh+"empty/", "r")~=[]
                        N = N + Nempty
                    end
                end
            end
        //else    // all (but zeros for sparses)
        //    N = size(data,"*")
        end
        heights = heights / N
        jokers = jokers / N
    end
    if grep(histmode, "density") then
        heights = heights ./ (bins(2:$) - bins(1:($-1)))
    end

    // Formatting outputs for sparse input data
    // ----------------------------------------
    if type(data0)==5 then
        ind = sparse(IJ, ind, size(data0))
    end

    // Shifting argouts in Scilab 5.5 compatibility mode
    // -------------------------------------------------
    if mode55 then
        jokers = ind
    end
endfunction
