function w=%msp_e(varargin)
z=sci_sparse(varargin($));
i=varargin(1);j=varargin(2);
w=mtlb_sparse(z(i,j));
endfunction
