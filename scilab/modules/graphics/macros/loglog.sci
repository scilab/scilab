// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
// along with this program.

function loglog(varargin)
    [lhs,rhs]=argn(0);

    if ~rhs
       w=logspace(-2,2,1000);
       s=%i*w;
       g=[];
       leg=[];
       for alpha=logspace(-2,1,4);
         g=[g;(1)./(s.^2+alpha*s+1)];
       end
       clf("reset")

       loglog(w,abs(g));

       title("$\LARGE \left|s^2/(s^2+\alpha s+1)\right|\mbox{ for }s=i\omega$")
       xlabel("$\LARGE \omega$")
       return
    end

    current_figure = gcf();
    cur_draw_mode = current_figure.immediate_drawing;
    current_figure.immediate_drawing = "off";

    ok = isempty(gca().children) || and(gca().data_bounds([1 3]) > 0);
    set(gca(),"log_flags","nn")
    try
        plot(varargin(:))
        if ok
            set(gca(),"log_flags","ll")
        end
    catch
        current_figure.immediate_drawing = cur_draw_mode;
        error(lasterror(%t))
    end

    current_figure.immediate_drawing = cur_draw_mode;
endfunction
