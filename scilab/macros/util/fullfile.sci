function f=fullfile(varargin)
// Copyright INRIA
// Build a full filename from parts
// V.C.

if lstsize(varargin)<2 then
  error("Wrong number of inputs!");
end

fs = ["/" "\"];
f = varargin(1);
for k=2:lstsize(varargin)
  arg = varargin(k);
  if isempty(f) | isempty(arg)
    f = f+arg;
  else
    if or(part(f,length(f))==fs) & or(part(arg,1)==fs)
      f = f+part(arg,2:length(arg));
    elseif or(part(f,length(f))==fs) | or(part(arg,1)==fs)
      f = f+arg;
    else
      f = f+pathconvert("/")+arg;
    end
  end
end
f=pathconvert(f,%f,%f);
endfunction
