function [n]=parametricNormals(xu,yu,zu,xv,yv,zv)

dim=prod(size((xu)));

n=[yu(:).*zv(:)-zu(:).*yv(:) zu(:).*xv(:)-xu(:).*zv(:) xu(:).*yv(:)-yu(:).*xv(:)]';

n=n*sparse([1:dim;1:dim]',1 ./sqrt(sum(n.^2,'r')));

endfunction
