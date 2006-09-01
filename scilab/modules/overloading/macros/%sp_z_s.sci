function r=%sp_z_s(a,b)
// a.\.b with a  sparse b full
// Copyright INRIA
if b==[] then r=[],return,end
r=a.\.sparse(b)
endfunction
