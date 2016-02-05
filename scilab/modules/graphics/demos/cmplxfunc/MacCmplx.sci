//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - Bruno PINCON
// Copyright (C) 2005 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file is distributed under the same license as the Scilab package.
//

function [xr,yr,zr,xi,yi,zi] = CmplxFacets(R,e,TypeDomain,TypeCut,n,StrFunc)

    //  A function to compute the facets for drawing a complex function
    //  on a square or a disk with branch(es) cut(s) on Ox or Oy
    //
    //  TypeDomain : "Square" or "Disk"
    //     TypeCut : "Ox" or "Oy"
    //           R : length of half a side of the square or radius of the disk
    //           e : thin layer to avoid the branch(es) cut(s)
    //           n : a scalar (for Square) or a 2-vector = [ntheta, nr]
    //               (for Disk) for discretization
    //     StrFunc : the string which names the complex function (this is
    //               because primitive don't pass as function argument)
    //
    // Bruno (10/10/2001): macros for the complex function dem

    if TypeDomain == "Square" then
        if TypeCut == "Ox" then
            x1 = linspace(-R, R, n);
            y1 = linspace( e, R, floor(n/2));
        else  // for TypeCut = "Oy" ...
            x1 = linspace( e, R, floor(n/2));
            y1 = linspace(-R, R, n);
        end
        X1 = ones(y1')*x1 ; Y1 = y1'*ones(x1);
    else // for TypeDomain = "Disk"
        r = linspace(0,R, n(2));
        if TypeCut == "Ox" then
            theta = linspace(0,%pi,n(1))';
            X1 = cos(theta)*r;
            Y1 = e + sin(theta)*r;
        else // for TypeCut = "Oy"
            theta = linspace(-%pi/2,%pi/2,n(1))';
            X1 = e + cos(theta)*r;
            Y1 = sin(theta)*r;
        end
    end
    X2 = -X1 ; Y2 = -Y1;
    Z1 = evstr(StrFunc+"(X1 + %i*Y1)");
    Z2 = evstr(StrFunc+"(X2 + %i*Y2)");
    [xr1,yr1,zr1] = nf3d(X1,Y1,real(Z1));
    [xr2,yr2,zr2] = nf3d(X2,Y2,real(Z2));
    xr = [xr1 xr2]; yr = [yr1 yr2]; zr = [zr1 zr2];
    [xi1,yi1,zi1] = nf3d(X1,Y1,imag(Z1));
    [xi2,yi2,zi2] = nf3d(X2,Y2,imag(Z2));
    xi = [xi1 xi2]; yi = [yi1 yi2]; zi = [zi1 zi2];
endfunction


function []=PlotCmplxFunc(R,e,TypeDomain,TypeCut,n,StrFunc,theta,alpha,DomReal)

    //  A function to draw on a square or a disk a complex function
    //  with branch(es) cut(s) on Ox or Oy
    //
    //  TypeDomain : "Square" or "Disk"
    //     TypeCut : "Ox" or "Oy"
    //           R : length of half a side of the square or radius of the disk
    //           e : thin layer to avoid the branch(es) cut(s)
    //           n : a scalar (for Square) or a 2-vector = [ntheta, nr]
    //               (for Disk) for discretization
    //     StrFunc : the string which names the complex function (this is
    //               because primitive don't pass as function argument)
    // theta,alpha : usual parameters for plot3d
    //     DomReal : interval for which the real restriction is drawn

    // Bruno (10/10/2001): macros for the complex function dem

    // Adapted for new graphic by Pierre MARECHAL ( 16/12/2005 )

    // computes the facets

    [xr,yr,zr,xi,yi,zi] = CmplxFacets(R,e,TypeDomain,TypeCut,n,StrFunc)

    // draw
    // ============================================

    current_figure_hdl = scf(100001);
    clf(current_figure_hdl,"reset");

    drawlater();

    // Title
    // ============================================

    my_title_axes = newaxes();

    // make axes transparent
    my_title_axes.filled = "off";

    Rs = string(R);

    if TypeDomain == "Square" then
        end_title = " Function on [-"+Rs+","+Rs+"]x[-"+Rs+","+Rs+"]"
    else
        end_title = " Function on D(0,R="+Rs+")"
    end

    if StrFunc == "f" then
        the_title = "Your Custom (named f) Complex" + end_title;
    else
        the_title = "The Complex " + StrFunc + end_title;
    end

    xtitle(the_title);

    my_title_axes.title.text       = the_title;
    my_title_axes.title.font_size  = 3;
    my_title_axes.title.font_style = 2;
    my_title_axes.margins = [ 0.08 0.08 0.08 0.08 ]

    // plot Im(z)
    // ============================================

    subplot(1,2,1);
    plot3d(xi,yi,zi,theta,alpha,"Re(z)@Im(z)@",[2 6 4]);
    xtitle("Im("+StrFunc+"(z))");

    // plot Re(z) + the real restriction
    // ============================================

    subplot(1,2,2);
    plot3d(xr,yr,zr,theta,alpha,"Re(z)@Im(z)@",[ 2 6 4]);
    xtitle("Re("+StrFunc+"(z))");

    // real function in yellow
    // ============================================

    if DomReal(2) > DomReal(1) then
        //xstring(0.1,-0.15," In yellow : the real "+StrFunc+" function")
        xx = linspace(DomReal(1),DomReal(2),40)';
        yy = zeros(xx);
        zz = evstr(StrFunc+"(xx)");
        param3d1(xx,yy,list(zz,32),theta,alpha,flag=[0,0]);
        yellow_line = get("hdl");
        yellow_line.thickness = 3;
        captions(yellow_line, "the real "+StrFunc+" function", "lower_caption");
    end



    drawnow();

endfunction
