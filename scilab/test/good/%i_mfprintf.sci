function %i_mfprintf(id,fmt,varargin);
for k=1:size(varargin)
  if type(varargin(k))==8 then 
    varargin(k)=double(varargin(k))
  end
end
mfprintf(id,fmt,varargin(:))
endfunction
