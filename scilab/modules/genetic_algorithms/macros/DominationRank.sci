// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Rank=DominationRank(All_FObj)
  Rank  = zeros(All_FObj)
  if size(All_FObj,2)==1 then
    [T,Ind]=gsort(All_FObj,'g','i');
    group_length=diff([0 find(diff(T)>0)]);
    Ngroup=size(group_length,'*');
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
