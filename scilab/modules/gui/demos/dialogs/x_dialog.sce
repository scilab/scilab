// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_x_dialog()

    res = x_dialog(_("Message: enter a value"), "3.1592653");

    disp(_("Value entered: "+string(res)));

endfunction

demo_x_dialog()
clear demo_x_dialog;
