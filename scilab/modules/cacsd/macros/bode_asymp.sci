// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 09/2013 - A. Khorshidi
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [] = bode_asymp(sl, w_min, w_max)
    // Syntax:
    //     syntax: bode_asymp(sl, [w_min, w_max])
    // Arguments:
    //     sl: a linear system given by its transfer function representation or state-space form and defined by syslin.
    //	   w_min, w_max: lower and upper frequency bounds (in rad/s)
    // Description:
    //     This function plots asymptotes for the magnitude and phase graphs.
    // Examples:
    //     s = %s; h = syslin("c", 1/(s+1)); scf(10001); bode(h); bode_asymp(h)
    //     s = %s; h = syslin("c", 1/(s+1)); scf(10002); gainplot(h); bode_asymp(h)
    // Ref:
    //     http://forge.scilab.org/index.php/p/cpge/source/tree/HEAD/macros/REP_FREQ_pre_simulate.sci

    rhs = argn(2);

    if and(typeof(sl) <> ["state-space" "rational"]) then
        msg = _("Wrong type for argument #%d: Rational or State-space matrix expected.\n");
        error(msprintf(msg, 1))
        return;
    end

    typeSL = typeof(sl);

    for elem=1:size(sl, "r") // Individually draw each asymptote of "sl" elements (row vector).
        if typeSL == "rational" then
            h = sl(elem, 1);
        else
            h = clean(ss2tf(sl(elem, 1)), 1e-8);
            // Also removing all the coefficients smaller than < 1e-8
        end

        root_num = roots(h.num);
        root_den = roots(h.den);

        if (find(root_num==0))
            disp("Problem class of system is negative")
        end
        rac_nul = find(root_den==0);
        alpha = length(rac_nul);
        var = "s";
        domain = h.dt;
        if type(domain) == 1 then
            var = "z";
        elseif domain == "c" then
            var = "s";
        elseif domain == "d" then
            var = "z";
        end
        if domain == [] then
            var = "s";
            if type(h.D) == 2 then
                var = varn(h.D);
            end
        end
        s = poly(0, var);
        msg = _("%s: Wrong value for input argument #%d: Evaluation failed.\n")
        try K = horner(h*s^alpha, 0); catch error(msprintf(msg, "bode_asymp")); end

        root_den(rac_nul) = []; // Removing the zeros

        if length([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)]) > 0 then
            disp("Extreme root removed: "+string(root_num([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)])'))
            root_num([find(abs(root_num)>1e8) find(abs(root_num)<1e-8)]) = [];
        end
        if length([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)]) > 0 then
            disp("Extreme root removed: "+string(root_den([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)])'))
            root_den([find(abs(root_den)>1e8) find(abs(root_den)<1e-8)]) = [];
        end

        i = 1;
        puls = [];
        order = [];
        while i<=length(root_num) // Real root
            if isreal(root_num(i), 0) then
                puls = [puls -root_num(i)];
                order = [order 1];
                i = i+1;
            else // Complex root
                xi = 1/sqrt(1+(imag(root_num(i))/real(root_num(i)))^2);
                puls = [puls -real(root_num(i))/xi];
                i = i+2;
                order = [order 2];
            end
        end
        i = 1;
        while i<=length(root_den) // Real root
            if isreal(root_den(i), 0) then
                puls = [puls -root_den(i)];
                order = [order -1];
                i = i+1;
            else // Complex root
                xi = 1/sqrt(1+(imag(root_den(i))/real(root_den(i)))^2);
                puls = [puls -real(root_den(i))/xi];
                order = [order -2];
                i = i+2;
            end
        end

        [puls, ind] = gsort(puls, "g", "i");
        order = order(ind);

        asym = [-20*alpha];
        phas = [-90*alpha];
        i = 1;
        while i<=length(puls)
            new_dir = asym($)+order(i)*20;
            asym = [asym new_dir];
            new_arg = phas($)+order(i)*90;
            phas = [phas new_arg];
            i = i+1;
        end

        // bode(h, w_min, w_max)
        fig = gcf();
        try sca(fig.children(1)); catch
            msg = _("ERROR: %s: bode() or gainplot() must be called before bode_asymp() and graphic window must still be running.\n");
            error(msprintf(msg, "bode_asymp"))
        end

        if length(fig.children) == 1 then
            flag = 1;
        else
            flag = 0;
        end

        if rhs == 1 then
            wmin = fig.children(1).data_bounds(1, 1); // Minimal frequency, w_min
            wmax = fig.children(1).data_bounds(2, 1); // Maximal frequency, w_max
        else
            wmin = w_min;
            wmax = w_max;
        end
        puls_to_plot = [];

        for p=real(puls)
            if p >= wmin & p <= wmax then
                puls_to_plot($+1) = p;
            end
        end
        puls = [wmin puls_to_plot' wmax];
        // End change DV

        eq_asymp = [20*log10(K/wmin^alpha)];
        puls_p = [];
        phas($+1) = phas($);
        eq_phas = [phas(1)];
        i = 2;
        while i<=length(puls)
            eq_asymp = [eq_asymp eq_asymp($)+asym(i-1)*log10(puls(i)/puls(i-1))];
            puls_p = [puls_p puls(i-1) puls(i)];
            eq_phas = [eq_phas phas(i-1) phas(i)];
            i = i+1;
        end

        // Adapt abscissae to current unit (Hertz or rad/s)
        if fig.children(1).x_label.text == "Frequency (Hz)" then
            puls   = puls./(2*%pi);
            puls_p = puls_p./(2*%pi);
        end

        // Change the color of asymptote:
        color_asymp  = ["b--", "g--", "c--", "m--", "r--"];
        rep = modulo(elem, 5); // This line will be useful if the number of systems is more than five.
        if rep == 0 then
            rep = 1;
        end

        if flag == 0 then
            plot(puls_p, eq_phas(1:$-1), color_asymp($-rep));
            sca(fig.children(2));
            plot(puls, eq_asymp, color_asymp(rep));
        elseif flag == 1 then
            plot(puls, eq_asymp, color_asymp(rep));
            fig.children.title.text = "Gainplot";
        end
    end

endfunction
