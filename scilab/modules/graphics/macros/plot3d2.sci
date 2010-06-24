// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function plot3d2(x,y,z,vect,theta,alpha,leg,flag,ebox)
// plot3d2(x,y,z,vect,T,A,leg,flags,ebox)
// (x,y,z) description of a set of surfaces 
// to Scilab description + call to plot3d 
// (x,y,z) are three matrices which describe a surface 
// the surface is composed of four sided polygons 
// The x-coordinates of a facet are given by x(i,j),x(i+1,j),x(i,j+1),
//	x(i+1,j+1)
// the vect vector is used when multiple surfaces are coded 
// in the same (x,y,z) matrices. vect(j) gives the line 
// at which the coding of the jth surface begins 
// if vect==-1 means that vect is useless
//---------------------------------------------------------
[lhs,rhs]=argn(0);
if rhs<3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "plot3d2", 3))
end
isvect=1
if exists('vect','local')==0 then isvect=1 ;vect=-1,end
if vect<>-1 then
  nobjs=prod(size(vect))+1;
  [rx,cx]=size(x);
  vect1=[0,vect,rx];
  xx=[],yy=[],zz=[];
  for i=1:nobjs; 
    [xxl,yyl,zzl]=nf3d(x(vect1(i)+1:vect1(i+1),:),...
	y(vect1(i)+1:vect1(i+1),:),...
	z(vect1(i)+1:vect1(i+1),:)),...
	xx=[xx,xxl];yy=[yy,yyl];zz=[zz,zzl];
  end;
else 
  [xx,yy,zz]=nf3d(x,y,z)
end

opts=[]
if exists('theta','local')==1 then opts=[opts,'theta=theta'],end
if exists('alpha','local')==1 then opts=[opts,'alpha=alpha'],end
if exists('leg'  ,'local')==1 then opts=[opts,'leg=leg']    ,end
if exists('flag' ,'local')==1 then opts=[opts,'flag=flag']  ,end
if exists('ebox' ,'local')==1 then opts=[opts,'ebox=ebox']  ,end
if size(opts,2)+isvect+3 <rhs then  error('invalid named arguments'),end


execstr('plot3d(xx,yy,zz,'+strcat(opts,',')+')')
endfunction
