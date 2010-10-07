// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Automotive Suspension")                    , "susp.dem.sce"];

subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);
