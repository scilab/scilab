// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_fonts()
    my_handle             = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode("fonts.dem.sce");

    // DEMO START

    my_plot_desc          = _("Font transparency");
    my_handle.figure_name = my_plot_desc;

    gcf().color_map = jet(64);

    // Two curves to overlay text and a legend on
    x = linspace(0, 4 * %pi, 200);
    plot(x, sin(x), "b-", x, cos(x), "r-");
    a = gca();
    a.thickness = 2;
    a.children(1).children(1).thickness = 3;
    a.children(1).children(2).thickness = 3;

    // Title (Label): opaque, kept readable
    xtitle(_("Font transparency demonstration"), _("x"), _("y"));

    // Axis labels (Label): semi-transparent font
    a.x_label.font_size = 4;
    a.x_label.font_foreground_alpha = 0.4;
    a.y_label.font_size = 4;
    a.y_label.font_foreground_alpha = 0.4;

    // Watermark (Text): big string drawn behind, very transparent
    t = xstring(2, 0, "WATERMARK");
    t.font_style = 8;
    t.font_size = 8;
    t.font_foreground = 2;       // blue
    t.font_foreground_alpha = 0.15;
    t.alignment = "center";

    // Legend: semi-transparent text
    lg = legend([_("sin(x)"); _("cos(x)")], 1);
    lg.font_size = 3;
    lg.font_color_alpha = 0.5;

    // DEMO END

endfunction

demo_fonts();
clear demo_fonts;
