function mtlb_semilogy(varargin)
global mtlb_log_mod
mtlb_log_mod='nl'
mtlb_plot(varargin(:))
mtlb_log_mod=[]
endfunction
