// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_x_mdialog()

    res = x_mdialog(_(["Message";...
    "(edit the boxes below --> output: edited boxes)"]),...
    _(["first row";"second row";"etc..."]),...
    ["10";"20";"30"]);

    disp(_("Values entered: "+res(1)+" "+res(2)+" "+res(3)));

    n = 5;
    m = 4;
    mat = rand(n, m);
    row = _("row");
    labelv = row(ones(1, n))+string(1:n);
    col = _("col");
    labelh = col(ones(1, m))+string(1:m);
    res = evstr(x_mdialog(_("Matrix to edit"), labelv, labelh, string(mat)));

    disp(res, _("Matrix entered: "));

endfunction

demo_x_mdialog()
clear demo_x_mdialog;
