// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_mandelbrot()

    my_handle             = scf(100001);
    clf(my_handle,"reset");

    // DEMO START

    my_plot_desc          = "mandelbrot";
    my_handle.figure_name = my_plot_desc;
    function M = getMandelbrotPart(xmin, xmax, ymin, ymax, nbPix, nColors, maxIters)
        // M : matrix of integers imaging the targetted local Mandelbrot area
        // M(i,j) gives the number of iterations for which the iterated z(i,j)
        //     suite crosses |z|>=2 (making this point out of the Mandelbrot set)
        ndmin = 0;
        Dx = abs(xmax - xmin);
        Dy = abs(ymax - ymin);
        dp = sqrt(Dx*Dy / nbPix);  // Area of a pixel
        nx = round(Dx/dp);
        ny = round(Dy/dp);
        dx = abs(xmax - xmin)/(nx-1);
        x  = linspace(xmin, xmax, nx);
        y  = ymax:-dx:ymin;
        ny = length(y);
        c  = (ones(ny,1)*x) + %i*(y.'*ones(1,nx));
        z  = c;  // Initial c Matrix
        nd = ones(z)*%inf;  // nb iter for > 2
        inC = 1:size(c,"*");
        i = 1;
        waitId = 0;
        ifin = min(ndmin+nColors, maxIters);
        while i<=ifin,
            if waitId==0
                waitId = waitbar(i/ifin,_("Computing in progress..."));
            else
                waitbar(i/ifin,waitId);
            end
            a = abs(z);
            nd(inC(find(a>2)))=i;
            tmp = find(a<=2);
            z   = z(tmp);
            inC = inC(tmp);
            z   = z.*z+c(inC);
            i = i+1;
        end
        close(waitId)
        M = nd
    endfunction
    // ---------------------

    nColors = 100;   // Number of colors
    maxIters = 500;  // Maximal number of iterations
    nbPix = 200000;  // Total number of pixel per image
    // Targetted Mandelbrot area
    xmin = -0.33;
    xmax = 0.07;
    ymin = 0.6;
    ymax = 1.13;

    // Generating the Mandelbrot local view
    M = getMandelbrotPart(xmin, xmax, ymin, ymax, nbPix, nColors, maxIters);

    // Scaling false colors
    icol  = [color("black")  addcolor(rainbowcolormap(nColors))];
    Mmin = min(M);
    M = M - Mmin + 1;
    M(find(M==%inf | M>nColors))=0;
    A = icol(M+1);

    clf
    drawlater;
    // colordef("black")  // Bug 11369 to be fixed

    // Displaying the area
    Matplot1(matrix(A,size(M)),[xmin ymin xmax ymax]);

    // Post-tuning the graphics
    isoview()
    ca = gca();
    ca.data_bounds = [xmin ymin ; xmax ymax];
    ca.tight_limits = "on";
    ca.axes_visible = ["on" "on" "off"];
    ca.title.font_size = 2;
    Ttxt = _("Matplot1(..) displays a matrix\n(value => color#), with SCALED AXES\n(here is a part of the Mandelbrot set)")
    xtitle(justify(msprintf(Ttxt),"c"))
    xlabel(_("Real part"))
    ylabel(_("Imaginary part"))

    drawnow()

    demo_viewCode("mandelbrot.dem.sce");

endfunction

demo_mandelbrot();
clear demo_mandelbrot;

