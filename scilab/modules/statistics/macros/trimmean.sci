// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function t=trimmean(x,discard,orien, verbose)
    //
    //A  trimmed mean  is calculated  by discarding  a certain
    //percentage of the lowest and the highest scores and then
    //computing  the  mean   of  the  remaining  scores.   For
    //example, a  mean trimmed  50% is computed  by discarding
    //the lower  and higher 25%  of the scores and  taking the
    //mean of the remaining scores.
    //
    //The median  is the mean trimmed 100%  and the arithmetic
    //mean is the mean trimmed 0%.
    //
    //A  trimmed mean  is  obviously less  susceptible to  the
    //effects of  extreme scores than is  the arithmetic mean.
    //It is therefore less susceptible to sampling fluctuation
    //than the mean for  extremely skewed distributions. It is
    //less  efficient (The  efficiency of  a statistic  is the
    //degree to  which the statistic is stable  from sample to
    //sample.    That  is,  the   less  subject   to  sampling
    //fluctuation  a  statistic  is,  the  more  efficient  it
    //is. The efficiency of statistics is measured relative to
    //the  efficiency  of other  statistics  and is  therefore
    //often called the relative efficiency. If statistic A has
    //a  smaller   standard  error  than   statistic  B,  then
    //statistic  A is  more efficient  than statistic  B.  The
    //relative efficiency of two  statistics may depend on the
    //distribution involved.   For instance, the  mean is more
    //efficient than  the median for  normal distributions but
    //not  for  some   extremely  skewed  distributions.   The
    //efficiency of a statistic can  also be thought of as the
    //precision  of  the  estimate:  The  more  efficient  the
    //statistic,  the  more precise  the  statistic  is as  an
    //estimator         of         the         parameter.[from
    //http://davidmlane.com/hyperstat/A12977.html])  than  the
    //mean for normal distributions.
    //
    //Trimmed  means  are often  used  in  Olympic scoring  to
    //minimize the effects  of extreme ratings possibly caused
    //by            biased            judges.            [from
    //http://davidmlane.com/hyperstat/A11971.html]
    //
    //
    //For a vector  or matrix x, t=trimmean(x,discard) returns
    //in  scalar t the  mean of  all the  entries of  x, after
    //discarding discard/2 highest values and discard/2 lowest
    //values.
    //
    //t=trimmean(x,discard,'r')       (or,       equivalently,
    //t=trimmean(x,discard,1))  returns in  each entry  of the
    //row vector t the trimmed mean of each column of x.
    //
    //t=trimmean(x,discard,'c')       (or,       equivalently,
    //t=trimmean(x,discard,2))  returns in  each entry  of the
    //column vector t the trimmed mean of each row of x.
    //
    //References:  Luis   Angel  Garcia-Escudero  and  Alfonso
    //Gordaliza,  Robustness Properties  of Means  and Trimmed
    //Means, JASA, Volume 94, Number 447, Sept 1999, pp956-969
    //

    //
    // modified by Bruno Pincon 2006-08-12 (to fix bug 2083)

    [lhs,rhs]=argn()
    if rhs < 1 | rhs > 4 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"trimmean",1,4))
    end

    if exists("discard","local")==0 then
        discard = 50.
    else
        if type(discard)~=1 | ~isreal(discard) | length(discard) ~=1 | discard > 100 | discard < 0 then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Real number between %d to %d expected.\n"),"trimmean",2,0,100))
        end
    end
    if exists("orien","local")==0 then
        orien = "all"
    end
    if exists("verbose","local")==0 then
        verbose=0;
    end
    // Compute sizx
    if (orien=="r" | orien==1) then
        sizx=size(x,"r")
    elseif (orien=="c" | orien==2) then
        sizx=size(x,"c")
    elseif (orien == "all") then
        sizx = length(x)
    else
        error(msprintf(gettext("Wrong value for input argument orien: %s.\n"),string(orien)))
    end

    if sizx==0 then
        if (verbose==1) then
            printf(gettext("Size of x is zero : returning NaN.\n"));
        end
        t=%nan
        return
    end

    nomdis = floor(sizx*discard/200)
    k1 = 1 + nomdis
    k2 = sizx - nomdis
    if k2 < k1 then
        [k1,k2] = (k2,k1)
    end
    nb = k2-k1+1
    if (verbose==1) then
        printf(gettext("discard=%s\n"),string(discard));
        printf(gettext("orien=%s\n"),string(orien));
        printf(gettext("Size of x:%i\n"),sizx);
        printf(gettext("Keeping %i values from %i to %i in sorted order\n"),nb,k1,k2);
    end
    if orien == "all" then
        x = gsort(x)
        t = sum(x(k1:k2)) / nb
    elseif (orien=="r" | orien==1) then
        x = gsort(x,"r")
        t = sum(x(k1:k2,:),"r") / nb
    elseif (orien=="c" | orien==2) then
        x = gsort(x,"c")
        t = sum(x(:,k1:k2),"c") / nb
    else
        error(msprintf(gettext("Unexpected value of orien : %s\n"),string(orien)))
    end
endfunction

