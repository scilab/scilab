function r=%i_sign(m)
it=inttype(m)
r=m;
r(1:$)=iconvert(1,it)
r(find(m<iconvert(0,it)))=iconvert(-1,it)
endfunction
