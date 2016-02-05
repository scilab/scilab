// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_x_choose()

    res = x_choose(["item1";"item2";"item3";"item4";"item5";"item6";..
    "item7";"item8";"item9";"item10"],..
    _("Message: Selection (double-click on an item --> returns the item number)"),..
    _("Cancel"));

    disp(_("Item selected: "+string(res)));

endfunction

demo_x_choose()
clear demo_x_choose;
