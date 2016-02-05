// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %_unwrap(typexample)
    if argn(2)==0 then
        typexample = ""
    end
    if convstr(typexample)~="2d" then
        // 1-D EXAMPLES
        // -----------
        f = scf();
        f.figure_size = [800 1000];
        f.figure_position(2) = 0;
        f.figure_name = "unwrap() & ""unfold""" + _(": 1-D examples ");
        ax = gda();
        ax.y_label.font_size=2;
        drawlater()

        // Original 1-D profile
        t = linspace(-4,4.2,800);
        alpha = t.^2 + t -1;
        subplot(5,2,1)
        titlepage("unwrap(): unwrap | unfold")
        subplot(5,2,2)
        plot(t,alpha)
        t2 = "$\text{Original profile: } \alpha=t^2+t-1$";
        ax = gca();
        ax.tight_limits = "on";
        yT = max(alpha) - strange(alpha)*0.1;
        xstring(0,yT,t2)
        e = gce();
        e.text_box_mode = "centered";
        e.font_size = 2;

        // Loop over cases
        for i=1:4
            subplot(5,2,2*i+1)
            if i==1 then
                // Wrapping by atan(tan())
                ralpha = atan(tan(alpha));            // raw recovered alpha [pi]
                ylabel("$\mbox{Y= }atan(tan(\alpha))$")
                [u, K] = unwrap(ralpha, %pi);         // arctan
                t2 = "$\text{unwrap(Y, \%pi)}$";
            elseif i==2
                // Wrapping by modulo() + Y-shift
                c = (rand(1,1)-0.5)*4;
                ralpha = pmodulo(alpha, 5) + c;
                ylabel("$\mbox{Y= }modulo(\alpha,\ 5)"+sprintf("%+5.2f",c)+"$")
                [u, K] = unwrap(ralpha, 0);
                t2 = "$\text{unwrap(Y, 0)}$";
            elseif i==3
                // Folding by asin(sin()) + Y-shift
                ralpha = 1+asin(sin(alpha));          // raw recovered alpha [2.pi]
                ylabel("$\mbox{Y= }1+asin(sin(\alpha))$")
                [u, K] = unwrap(ralpha, "unfold");
                t2 = "$\text{unwrap(Y,""unfold"")}$";
            else
                // Folding by acos(cos()) + trend
                ralpha = 1+alpha/10+acos(cos(alpha)); // raw recovered alpha [2.pi]
                ylabel("$\mbox{Y= }1+\frac{\alpha}{10}+acos(cos(\alpha))$")
                [u, K] = unwrap(ralpha, "unfold");
                t2 = "$\text{unwrap(Y,""unfold"")}$";
            end
            // Plotting the profile to be processed
            plot(t, ralpha)
            // Staring the breakpoints or the cusp points on the curve:
            if K~=[]
                plot(t(K), ralpha(K),"*")
            end
            // Plotting the processed (unwrapped/unfolded) profile:
            subplot(5,2,2*i+2)
            plot(t,u)
            ax = gca();
            ax.tight_limits = "on";
            // Adding a legend:
            yT = max(u) - strange(u)*0.2;
            xstring(0,yT,t2)
            e = gce();
            e.text_box_mode = "centered";
            e.font_size = 2;
        end
        sda();
        drawnow()

    else
        // 2D EXAMPLES
        // -----------
        ax = gda();
        ax.title.font_size = 2;
        f = scf();
        f.color_map = hotcolormap(100);
        f.figure_size = [475 1050];
        f.figure_position(2) = 0;
        f.figure_name = "unwrap()" + _(": 2-D examples");
        drawlater()

        nx = 300;
        ny = 400;
        rmax = 8.8;
        x = linspace(-rmax/2, rmax/2, nx)-1;
        y = linspace(-rmax/2, rmax/2, ny)+1;
        [X, Y] = meshgrid(x,y);
        for ex=0:1   // examples
            // Original surface
            // Generating the surface
            if ex==0 then
                z = X.^2 + Y.^2;
            else
                z = sqrt(0.3+sinc(sqrt(z)*3))*17-7;
            end
            // PLotting it in 3D
            subplot(4,2,1+ex)
            surf(x, y, z)
            title("Original profile Z")
            e = gce();
            e.thickness = 0; // removes the mesh
            e.parent.tight_limits = "on";

            // WRAPPED surface (flat)
            m = 2.8;
            zw = pmodulo(z, m); // wraps it
            subplot(4,2,3+ex)
            grayplot(x, y, zw')
            title(msprintf("Zw = pmodulo(Z, %g)  (flat)",m))
            ax0 = gca();
            ax0.tight_limits = "on";

            // UNWRAPPED surfaces (flat):
            // in both directions:
            u = unwrap(zw, 0);
            subplot(4,2,5+ex)
            grayplot(x, y, u')
            title(msprintf("unwrap(Zw, %g)  (flat)", 0))
            ax3 = gca();
            ax3.tight_limits = "on";

            if ex==0, direc="r", else direc="c", end
            // along a single direction:
            u = unwrap(zw, m, direc);
            subplot(4,2,7+ex)
            grayplot(x, y, u')
            title(msprintf("unwrap(Zw, %g, ""%s"")  (flat)",m,direc))
            ax1 = gca();
            ax1.tight_limits = "on";
        end
        sda();
        drawnow()
    end
endfunction
