
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Warning : order !!
// list of  : filename , module

listmacrostoload = ["listfiles","fileio"
"%c_a_c","overloading";
"genlib","functions";
];

dim = size(listmacrostoload);

for i = 1:dim(1)
    clear(listmacrostoload(i,1));
    exec(SCI+"/modules/"+listmacrostoload(i,2)+"/macros/"+listmacrostoload(i,1)+".sci");
end
clear i;
clear dim;
// ------------------------------------