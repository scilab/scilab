// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_datafit()
    function g = Gap(p, Data)
        // p: a, b, center xc, yc, tilt °
        C = cosd(p(5));
        S = -sind(p(5));
        x = Data(1,:) - p(3);
        y = Data(2,:) - p(4);
        g = ((x*C + y*S )/p(1)).^2 + ((-x*S + y*C)/p(2)).^2 - 1;
    endfunction

    // Generating the data
    // -------------------
    // Actual parameters :
    //    a   b   xc  yc  tilt
    a = grand(1,1,"unf",1,10);
    b = grand(1,1,"unf",a/6, a);
    C = grand(1,2,"unf",-10,10);
    tilt = grand(1,1,"unf", -85,85);
    Pa = [a b C(1) C(2) tilt];
    Np = 30;            // Number of data points
    Theta_min = grand(1,1,"unf",-180,-90)
    Theta = grand(1,Np, "unf",Theta_min, Theta_min+270);
    // untilted centered noisy arc
    x = Pa(1)*(cosd(Theta) + grand(Theta, "unf",-0.07, 0.07));
    y = Pa(2)*(sind(Theta) + grand(Theta, "unf",-0.07, 0.07));
    // Tilting and off-centering the arc
    A = Pa(5);
    C = cosd(A); S = sind(A);
    xe =  C*x + y*S + Pa(3);
    ye = -S*x + y*C + Pa(4);
    Data = [xe ; ye];

    // Retrieving parameters from noisy data
    // -------------------------------------
    // Initial estimation
    ab0 = (strange(xe)+strange(ye))/2;
    xc0 = mean(xe);
    yc0 = mean(ye);
    A0 = -atand(reglin(xe,ye));
    P0 = [ab0 ab0/2 xc0 yc0 A0];
    // Parameters bounds
    Pinf = [ 1     0.2     xc0-ab0/2, yc0-ab0/2 -89];
    Psup = [1.2*ab0 1.2*ab0  xc0+ab0/2, yc0+ab0/2  89];// Fitting

    // FITTING
    [P, dmin] = datafit(Gap, Data, 'b',Pinf,Psup, P0);

    // P(1) is the longest axis: postprocessing the orientation angle
    if P(1)<P(2) then
        P([1 2]) = P([2 1]);
        if P(5)>0
            P(5) = 90 - P(5);
        else
            P(5) = 90 + P(5);
        end
    end
    if (P(5)*Pa(5))<0 & abs(-P(5)/Pa(5)-1)<0.3 & P(5)>5 then
        P(5) = -P(5)
    end

    // COMPUTING THE FITTING CURVE
    Theta = 0:2:360;
    x = P(1) * cosd(Theta);
    y = P(2) * sind(Theta);
    A = P(5);
    [x, y] = (x*cosd(A)+y*sind(A)+P(3), -x*sind(A)+y*cosd(A)+P(4));

    // COMPUTING THE TRUE ELLIPSE CURVE
    xt = Pa(1) * cosd(Theta);
    yt = Pa(2) * sind(Theta);
    At = Pa(5);
    [xt, yt] = (xt*cosd(At)+yt*sind(At)+Pa(3), -xt*sind(At)+yt*cosd(At)+Pa(4));

    // DISPLAY
    // -------
    my_handle = scf(100001);
    drawlater
    clf(my_handle, "reset");
    gcf().figure_name = _("datafit() random demo : ellipse fitting (unweighted)");
    demo_viewCode(get_absolute_file_path("ellipse.dem.sce")+"ellipse.dem.sce");

    plot(xt, yt, "g")   // True ellipse
    plot(xe, ye, "+")   // Data
    plot(x, y, "r")     // Fitting ellipse
    isoview
    legend([_("True elliptical law")
            _("Data = true + noise dx|y = +/-7%")
            _("Best ellipse fitting Data")], "in_lower_right");
    // Parameters values:
    Patxt = msprintf("%5.2f   %5.2f   %5.2f   %5.2f  %5.1f", Pa);
    Ptxt  = msprintf("%5.2f   %5.2f   %5.2f   %5.2f  %5.1f", P);
    xstring(P(3), P(4), ["" "    a           b        xc       yc     A°"
    "True:" Patxt
    "Fit:" Ptxt
    "" "    dmin = "+ msprintf("%5.3f", dmin)]);
    set(gce(),"text_box",[0 0], "text_box_mode", "centered");
    title(_("datafit: Fitting on 30 random unweighted data points"),"fontsize",3)
    drawnow
endfunction

demo_datafit();
clear demo_datafit;
