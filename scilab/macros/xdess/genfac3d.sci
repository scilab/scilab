function [xx,yy,zz]=genfac3d(x,y,zmat,mask)
// genfac - transforms  standard 3d data to four sides facets representation
//%Syntax
// [xx,yy,zz]=genfac3d(x,y,zmat,mask)
//%Parameters
// zmat      : matrix , zmat(i,j)= F(x(j),y(i))
// x        : x axis coordinates vector
// y        : y axis coordinates vector
// mask     : boolean optional matrix with same size as zmat used to select 
//            entries of zmat to be represented by facets
// xx,yy,zz :4xn  matrices. xx(:,i),yy(:,i),zz(:,i) are respectively the 
//           x-axis,y-axis and z-axis coordinates of the ith facet
//%Examples
//  genfac3d() 
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs <=0 then
  s_mat=['t=(0:10)''*%pi/5;';
         '  zmat=sin(t)*cos(t'');';
	 '[xx,yy,zz]=genfac3d(t,t,zmat,zmat>=0);';
	 'plot3d(xx,yy,zz)'];
  write(%io(2),s_mat);execstr(s_mat);
  return;
end;
[nr,nc]=size(zmat)
indy=(ones(1,nc-1).*.[0;1;1;0]+(1:nc-1).*.[1;1;1;1]).*.ones(1,nr-1);
indx=ones(1,nc-1).*.(ones(1,nr-1).*.[0;0;1;1]+(1:nr-1).*.[1;1;1;1]);
[nrl,nrc]=size(indx)
indx=matrix(indx,nrl*nrc,1);
indy=matrix(indy,nrl*nrc,1);
indz=indx+(nr)*(indy-1*ones(indy));
[nrl,nrc]=size(indx);
xx=matrix(x(indx),4,nrl*nrc/4);
yy=matrix(y(indy),4,nrl*nrc/4);
zz=matrix(zmat(indz),4,nrl*nrc/4);
if rhs==4 then 
  zl=matrix(mask(indz),4,nrl*nrc/4);
  [xin,yin]=find(zl);
  zz=zz(:,yin);
  xx=xx(:,yin);
  yy=yy(:,yin);
end
endfunction
