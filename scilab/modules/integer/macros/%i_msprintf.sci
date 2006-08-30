function r=%i_msprintf(fmt,varargin);
for k=1:size(varargin)
  if type(varargin(k))==8 then 
    varargin(k)=double(varargin(k))
  end
end
r=msprintf(fmt,varargin(:))
endfunction
