function %msp_p(x)
[ij,v,mn]=spget(x)
ij=string(ij);mn=string(mn);v=string(v);
n=size(v,1)
lp='('
rp=')        '
comma=',    '

t=['('+mn(1,1)+comma+mn(1,2)+') m sparse matrix'
   ' '
    lp(ones(n,1))+ij(:,1)+comma(ones(n,1))+ij(:,2)+rp(ones(n,1))+v(:)]
write(%io(2),t,'(a)')
endfunction
