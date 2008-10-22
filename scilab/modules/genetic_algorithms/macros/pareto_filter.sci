// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [F_out, X_out, Ind_out] = pareto_filter(F_in, X_in)

[nargout,nargin] = argn();

if ~isdef('F_in','local') then
  error(sprintf(gettext("%s: F_in is mandatory"),"pareto_filter"));
end

x_defined   = (nargout>=2);
Ind_defined = (nargout==3);

Ind_out = 1;
for i=2:size(F_in,1)
  Ind_out = [Ind_out i];
  Dominated = %F*ones(size(Ind_out,1),size(Ind_out,2));
  for j=1:length(Ind_out)
    if i==Ind_out(j) then continue; end
    if Dominated(j) then continue; end
    if and(F_in(i,:)<=F_in(Ind_out(j),:)) & or(F_in(i,:)<F_in(Ind_out(j),:)) then
      Dominated(j) = %T;
    elseif and(F_in(Ind_out(j),:)<=F_in(i,:)) & or(F_in(Ind_out(j),:)<F_in(i,:)) then
      Dominated($) = %T;
    end
  end
  Ind_out(find(Dominated)) = [];
end

F_out = F_in(Ind_out,:);

if x_defined & (nargin==2) then
  if typeof(X_in)=='list' then
    X_out = list(X_in(Ind_out));
  else
    X_out = X_in(Ind_out,:);
  end
elseif x_defined &(nargin~=2)
  X_out = [];
end
endfunction
