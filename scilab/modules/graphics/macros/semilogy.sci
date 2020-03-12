// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
// along with this program.

function semilogy(varargin)
    [lhs,rhs]=argn(0);

    if ~rhs
       w=logspace(-2,2,1000);
       s=%i*w;
       g=1../(s.^2+s+1);
       clf("reset")

       semilogy(w,abs(g));

       title("$\LARGE \left|1/(s^2+s+1)\right|\mbox{ for }s=i\omega$")
       xlabel("$\LARGE \omega$")
       return
    end

    current_figure = gcf();
    cur_draw_mode = current_figure.immediate_drawing;
    current_figure.immediate_drawing = "off";

    ok = isempty(gca().children) || gca().data_bounds(3) > 0;
    set(gca(),"log_flags","nn")
    try
        plot(varargin(:))
        if ok
            set(gca(),"log_flags","nl")
        end
    catch
        current_figure.immediate_drawing = cur_draw_mode;
        error(lasterror(%t))
    end

    current_figure.immediate_drawing = cur_draw_mode;
endfunction
