function %msp_p(x)
[ij,v,mn]=spget(x);
IJ=string(ij);MN=string(mn);
if v==[] then disp(v);return;end
V=string(v);
n=size(V,1)
lp='('
rp=')        '
comma=',    '

t=['('+MN(1,1)+comma+MN(1,2)+') m sparse matrix'
   ' '
    lp(ones(n,1))+IJ(:,1)+comma(ones(n,1))+IJ(:,2)+rp(ones(n,1))+V(:)]
write(%io(2),t,'(a)')
endfunction
