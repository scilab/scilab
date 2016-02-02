// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=isoview(varargin)
    // setting default plotting to be isoview

    if size(varargin)<>4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "isoview", 4));
    end

    for i=1:4 do
        if type(varargin(i)) <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "isoview", i));
        end
    end;

    xmin = varargin(1);
    xmax = varargin(2);
    ymin = varargin(3);
    ymax = varargin(4);

    a = gca();
    a.isoview = "on";
    a.data_bounds = [xmin, ymin ; xmax, ymax];
    a.clip_state = "clipgrf";

endfunction
