function x=%i_fft(varargin)
n=size(varargin)
for k=1:n,varargin(k)=double(varargin(k)),end
x=fft(varargin(:))
endfunction
