// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
