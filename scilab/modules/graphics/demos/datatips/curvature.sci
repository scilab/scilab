// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function c=curvature(data)
    d1=diff(data,1,1);
    d2=diff(data,2,1);
    if size(data,2)==3 then
        c=sqrt(...
        (d2(:,3).*d1(2:$,2)-d2(:,2).*d1(2:$,3)).^2 +...
        (d2(:,1).*d1(2:$,3)-d2(:,3).*d1(2:$,1)).^2  +...
        (d2(:,2).*d1(2:$,1)-d2(:,1).*d1(2:$,2)).^2)/...
        ((d1(2:$,1)^2+d1(2:$,2)^2+d1(2:$,3)^2)^1.5)
    else
        c=(d1(2:$,1).*d2(:,2)-d1(2:$,2).*d2(:,1))./((d1(2:$,1)^2+d1(2:$,2)^2)^1.5)
    end
endfunction

