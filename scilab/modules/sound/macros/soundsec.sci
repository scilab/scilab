// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) ???? - ENPC
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aimé AGNEL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [t]=soundsec (n,rate)
    // Return [0,n) seconds of t parameter at frequency = rate (sample/sec)
    // Always excludes the last sample if it arrives exactly at n seconds
    [lhs,rhs]=argn(0);

    //set rate if not defined
    if rhs <=1 ; rate=22050; end;

    //==============================================================================
    // Format testing of the parameters

    //n must be a real positive scalar
    if  typeof(n)=="constant"
        if isreal(n)
            if ~isscalar(n)
                //n is not a real positive scalar
                error(999, msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"soundsec",1,1,1));
            end
        else //n is not real
            error(999, msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"),"soundsec",1));
        end
    else //n is not a constant matrix
        error(999, msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"),"soundsec",1));
    end

    //rate must be a real strictly positive scalar
    if  typeof(rate)=="constant"
        if isreal(rate)
            if ~isscalar(rate)
                //rate is not a real positive scalar
                error(999, msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"soundsec",2,1,1));
            end
        else //rate is not real
            error(999, msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"),"soundsec",2));
        end
    else //rate is not a constant matrix
        error(999, msprintf(_("%s: Wrong type for argument #%d: Real matrix expected.\n"),"soundsec",2));
    end

    //n and rate must be positive
    if n<0
        error(999, msprintf(_("%s: Wrong value for input argument #%d: Must be >= %d.\n"),"soundsec",1,0));
    end

    if rate<0
        error(999, msprintf(_("%s: Wrong value for input argument #%d: Must be >= %d.\n"),"soundsec",2,0));
    end
    //Format test end
    //==============================================================================

    max_sample = floor(n*rate);
    if (max_sample==n*rate)
        //Remove the last sample if it falls exactly at the n-th seconds
        max_sample = max_sample-1
    end

    if rate==0
        //avoid dividing by 0, if rate is 0 the result will be the empty set
        rate=1;
    end

    t = (0:max_sample)/rate;
endfunction
