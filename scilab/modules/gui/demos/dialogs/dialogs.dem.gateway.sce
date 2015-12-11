// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("messagebox"), "messagebox.sce"; ...
_("x_choose"),  "x_choose.sce"; ...
_("x_dialog"),  "x_dialog.sce"; ...
_("x_mdialog"), "x_mdialog.sce"];

subdemolist(:,2) = SCI + "/modules/gui/demos/dialogs/"+ subdemolist(:,2);
