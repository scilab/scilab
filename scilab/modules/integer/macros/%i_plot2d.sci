function %i_plot2d(varargin)
for k=1:min(2,size(varargin))
  varargin(k)=double(varargin(k))
end
plot2d(varargin(:))
endfunction
