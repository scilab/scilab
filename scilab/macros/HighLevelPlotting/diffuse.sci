function d=diffuse(nx,ny,nz,lightVect)

[ly,lx]=size(nx);

d=computeLight([nx(:)';ny(:)';nz(:)'],lightVect);
d=matrix(d,ly,lx);
