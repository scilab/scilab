function w=%msp_t(x)
if isempty(x) then
d=size(x);w=mtlb_sparse(d(2),d(1));
else
w=mtlb_sparse(sci_sparse(x)');
end;
endfunction
