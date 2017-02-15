// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Paul Bignier
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [
_("Optimal link route")           , "optimal_link.dem.sce"        ; ..
_("Optimal split block position") , "optimal_split_block.dem.sce" ; ..
];

subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);
