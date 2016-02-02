// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Rank=DominationRank(All_FObj)
    Rank  = zeros(All_FObj)
    if size(All_FObj,2)==1 then
        [T,Ind]=gsort(All_FObj,"g","i");
        group_length=diff([0 find(diff(T)>0)]);
        Ngroup=size(group_length,"*");
        k=1;
        for Count=1:Ngroup
            Rank(k:k+group_length(Count)-1)=Count;
            k=k+group_length(Count);
        end
        Rank(Ind)=Rank;
    else
        MO_All_FObj = All_FObj;
        // Compute the domination rank on all the population
        Index = 1:size(MO_All_FObj,1);
        Count = 1;
        while size(MO_All_FObj,1)>1
            [tmp1,tmp2,Index_List]  = pareto_filter(MO_All_FObj);
            Rank(Index(Index_List)) = Count;
            Count = Count + 1;
            MO_All_FObj(Index_List,:) = [];
            Index(Index_List)          = [];
        end
    end
endfunction
