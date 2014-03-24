// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_messagebox()

    res = messagebox(_(["Simple example of message";
    "First row of comments";
    "Second row";
    "etc...";]), _("Title"), "info", _(["Button 1" "Button 2"]), "modal");

    if res == 1 then
        disp(_("Button 1 clicked."));
    else
        disp(_("Button 2 clicked"));
    end

endfunction

demo_messagebox()
clear demo_messagebox;
