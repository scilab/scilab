// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_datafit()
    function Y = model(P,X)
        Y = P(3) * exp(-(X-P(1)).^2 / (2*P(2)*P(2))) + P(4)*X + P(5);
    endfunction
    // -------------------------------
    // Gap function
    function r = gap(P,XY)
        r = XY(2,:) - model(P,XY(1,:))
    endfunction
    // -------------------------------
    // True parameters
    Pg = [3.5 1.5 7 0.4 -0.5]; // mean stDev ymax a(.x) b

    // Generating data
    // ---------------
    X = 0:0.2:10;
    Np = length(X);
    Y0 = model(Pg, X);              // True law
    noise = (rand(Y0)-.5)*Pg(3)/4;
    Y = Y0 + noise                  // Noised data
    Z = [X ; Y];

    // Let's try to find the original parameters from the (noisy) data:
    // ---------------------------------------------------------------
    // Performing the fitting = parameters optimization
    [v, k] = max(Y0);
    P0 = [X(k) 1 v 1 1];
    [P, fmin, s]  = datafit(gap, Z, P0);
    Wz =  1./abs(noise);
    [Pw, fminw, s] = datafit(gap, Z, Wz, P0);

    // Computing fitting curves from found parameters
    fY = model(P, X);
    fYw = model(Pw,X);

    // Display
    // -------
    my_handle = scf(100001);
    clf(my_handle, "reset");
    gcf().figure_name = "datafit() demo : Sloping gaussian";
    demo_viewCode(get_absolute_file_path("slopingGaussian.dem.sce")+"slopingGaussian.dem.sce");
    xsetech([0 0 1 0.75])

    plot2d(X, Y0, 24)  // True underlaying law
    plot2d(X, Y,  -1)  // Noisy data
    plot2d(X, fY, 15)  // best unweighted fit
    plot2d(X, fYw,18)  // best weighted fit
    legend(["True law" "Noisy Data" ..
           msprintf("Unweighted fit (%5.2f /%5.2f)",mean(abs(fY-Y0)), sqrt(fmin/Np)) ..
           msprintf("Weighted fit (%5.2f /%5.2f)", mean(abs(fYw-Y0)), sqrt(fminw/sum(Wz)))],1)
           // Average vertical Model-Data distance: True one / according to the residual cost
    // Params: row#1: true params row#2: from unweighted fit row#3: from weighted fit
    [xs, ys] = meshgrid(linspace(2,6,5), linspace(0.5,-0.5,3));
    xnumb(xs, ys, [Pg ; P ; Pw])
    xstring(2,1.5,"$\Large p_3\,.\,exp\left({-(x-p_1)^2}\over{2\,p_2^2} \right)+p_4.x+p_5$")

    // Plotting residus:
    xsetech([0 .75 1 0.25])
    plot2d(X, model(P ,X)-Y0, 15)  // remaining gap with best unweighted fit
    plot2d(X, model(Pw,X)-Y0, 18)  // remaining gap best weighted fit
    ylabel("Fit - True law")
    ax = gca();
    ax.x_location = "top";
    tmp = ax.x_ticks.labels;
    gca().x_ticks.labels = emptystr(size(tmp,"*"),1);
    xgrid(color("grey70"))
endfunction

demo_datafit();
clear demo_datafit;
