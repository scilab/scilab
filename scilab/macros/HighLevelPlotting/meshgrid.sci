function [U,V]=meshgrid(u,v)

// For matlab compatibility

nu=prod(size(u));
nv=prod(size(v));

U=ones(nv,1).*.u(:)';
V=v(:).*.ones(1,nu);

endfunction
