function mtlb_semilogx(varargin)
global mtlb_log_mod
mtlb_log_mod='ln'
mtlb_plot(varargin(:))
mtlb_log_mod=[]
endfunction
