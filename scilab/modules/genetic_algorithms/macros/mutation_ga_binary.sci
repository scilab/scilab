// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function Mut_Indiv = mutation_ga_binary(Indiv,param)
if ~isdef('param','local') then
  param = [];
end
// We deal with some parameters to take into account the boundary of the domain and the neighborhood size
[BinLen,err]     = get_param(param,'binary_length',8);
[MultiMut,err]   = get_param(param,'multi_mut',%F);
[MultiMutNb,err] = get_param(param,'multi_mut_nb',2);

if ~MultiMut then
  pos = ceil((length(Indiv)-1)*rand(1,1))+1;
  Mut_Indiv = Indiv;
  if part(Indiv,pos)=='0' then 
    Mut_Indiv = strcat([part(Indiv,1:pos-1) '1' part(Indiv,pos+1:length(Indiv))]);
  end
  if part(Indiv,pos)=='1' then 
    Mut_Indiv = strcat([part(Indiv,1:pos-1) '0' part(Indiv,pos+1:length(Indiv))]);
  end
else
  pos = ceil((length(Indiv)-1)*rand(MultiMutNb,1))+1;
  pos = -unique(gsort(-pos));
  Mut_Indiv = Indiv;
  for i=1:length(pos)
    if part(Mut_Indiv,pos(i))=='0' then 
      Mut_Indiv = strcat([part(Mut_Indiv,1:pos(i)-1) '1' part(Mut_Indiv,pos(i)+1:length(Mut_Indiv))]);
    end
    if part(Mut_Indiv,pos(i))=='1' then 
      Mut_Indiv = strcat([part(Mut_Indiv,1:pos(i)-1) '0' part(Mut_Indiv,pos(i)+1:length(Mut_Indiv))]);
    end
  end
end
endfunction
