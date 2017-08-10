// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [h]=buttmag(order,omegac,sample_vector)
    //<h>=buttmag(order,omegac,sample_vector)
    //Squared Magnitude response of a Butterworth filter
    //omegac = cutoff frequency ; sample_vector = sample of frequencies
    //  order  :Filter order
    //  omegac :Cut-off frequency in Hertz
    //  sample_vector :Vector of frequency where buttmag is evaluated
    //  h      :Butterworth filter values at sample points
    //
    //!

    // For ascendant compatibility before bug 4618 fix
    // http://bugzilla.scilab.org/show_bug.cgi?id=4618
    // In case the users calls buttmag with named arguments
    if exists("sample","local")==1 then
        sample_vector = sample;
    end

    [n1,n2]=size(sample_vector);
    un=ones(n1,n2);
    h=un./(un+(sample_vector/omegac).^(2*order));
endfunction
