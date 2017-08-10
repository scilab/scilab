// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function testOk=m2sciCompareResults(sciRes,matRes)

    // Define tolerance on error
    //prec=1000000*%eps;
    prec=1000*%eps
    my_inf=1D300

    res_type=type(sciRes);

    if type(sciRes)<>10 then
        if size(sciRes)<>size(matRes) then
            testOk=%F
            return
        end
    end

    testOk=%T

    if matRes==[]|sciRes==[] then
        testOk=and(sciRes==matRes);
    elseif res_type==10 then
        testOk=and(sciRes==matRes)
    elseif or(res_type==[4,6]) | type(matRes)==4 then
        matRes(find(bool2s(matRes)<>0))=1;
        testOk=and(bool2s(sciRes)==matRes)
    else
        // Looking for Nan values in matRes
        ind=find(isnan(matRes));
        ind1=find(isnan(real(sciRes)));// Real part because Scilab can return %Nan+%Nan*%i
        if ind<>ind1 then
            testOk=%F;
        end
        // Looking for Inf values in matRes
        ind=find(isinf(matRes));
        if isreal(sciRes) then
            sciRes(find(sciRes>my_inf))=%inf
            sciRes(find(sciRes<-my_inf))=-%inf
        else
            sciRes(find(abs(sciRes)>my_inf))=%inf
        end
        ind1=find(isinf(sciRes));
        if ind<>ind1 then
            testOk=%F;
        end
        //Indexes corresponding to other values than Nan and Inf
        ind=find((~isinf(matRes))&(~isnan(matRes)));
        diff_mat=abs(sciRes-matRes);
        if (max(abs(matRes(ind)))>1)&(find(matRes(ind)==0)==[]) then
            testOk=and(max(abs(diff_mat(ind)./matRes(ind)))<prec)
        else
            testOk=and(abs(diff_mat(ind))<prec);
        end
    end

endfunction
