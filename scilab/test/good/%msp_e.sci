function w=%msp_e(varargin)
z=sci_sparse(varargin($));
if length(varargin)==2 then
w=mtlb_sparse(z(varargin(1)));
return
end
i=varargin(1);j=varargin(2);
w=mtlb_sparse(z(i,j));
endfunction
