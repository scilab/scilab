function R=%xlssheet_e(varargin)
  //extract a subsheet out of an xls sheet
  s=varargin($)
  R=s.value;T=s.text
  R=R(varargin(1:$-1))
  T=T(varargin(1:$-1))
  if and(isnan(R)) then
    R=T
  elseif or(isnan(R)) then
    R=mlist(['xlssheet','name','text','value'],'',T,R)
  end
endfunction
