function demo_isoSurf()
demo_help demo_isoSurf


//display an isosurface with Gouraud shaded triangles

//cd ENRICO;
//exec loader.sce
//cd ../

f = gcf() ;
SetPosition();
f.color_map = graycolormap(1024);
drawlater();
nx=20; ny=20; nz=20; s=hypermat([nx,ny,nz]);
x=linspace(-4.5,4.5,nx); xq=x'*ones(1,ny);
y=linspace(-4.5,4.5,ny); yq=ones(nx,1)*y;
z=linspace(-4.5,4.5,nz);

for i=1:nz 
  s(:,:,i)=1../((xq-2).^2+yq.^2+(z(i)-2)^2)+1../((xq+2).^2+yq.^2+(z(i)+2)^2); 
end;

[xx,yy,zz]=isosurf3d(x,y,z,-s,-.18);
c= shadecomp(xx,yy,zz,[1,0,1],3,2);

xbasc();
//xtitle('Isosurface resulting of the attraction of two spheres');
plot3d(xx,yy,list(zz,c*(xget('lastpattern')-1)+1))
e = gce();
e.color_mode = 0 ;
e.color_flag = 3;
//drawnow();

f = gcf();
f.pixmap='on';
a = gca();
a.isoview = 'on';
t = a.title ;
t.text = 'Isosurface resulting of the attraction of two spheres' ;
t.font_size = 5 ;
drawnow();
for i=1:45
  a.rotation_angles = a.rotation_angles + [8,0] ;
  show_pixmap() ;
end;

xdel();

nc=30; setcmap([1 2 3 4 6 7],nc)
drawlater();
// generate various 3d objects
[xx1,yy1,zz1]=sphere(rand(2,3),0.1,12);
[xx2,yy2,zz2]=sphere(rand(2,3),0.05,12);
s=(0:0.02:1)';
[xx3,yy3,zz3]=spaghetti(cos(4*%pi*s)/4+s,-sin(4*%pi*s)/4,s);

// calculate their shading
l=[2 0 3];
c1=shadecomp(xx1,yy1,zz1,l,2,2);
c2=shadecomp(xx2,yy2,zz2,l,2,2);
c3=shadecomp(xx3,yy3,zz3,l,2,2);

// group them and plot them (each in a different shade)
data=list(xx1,yy1,zz1,c1,2,xx2,yy2,zz2,c2,6,xx3,yy3,zz3,c3,3);
xbasc();
//xtitle('Shaded 3d objects of different colors') ;
oplot3d(data,nc*ones(1,5),45,80)

f = gcf();
SetPosition();
f.pixmap='on';
a = gca();
a.isoview = 'on';
t = a.title ;
t.text = 'Shaded 3D objects of different colors' ;
t.font_size = 5 ;
drawnow();
for i=1:180
  a.rotation_angles(2) = a.rotation_angles(2) + 2 ;
  show_pixmap() ;
end;


endfunction

function [xx,yy,zz]=isosurf3d(x,y,z,s,s0)
// 
//   xx,yy,zz= (nf,3)  facelet vertices
//   s= real hypermat([length(x),length(y),length(z)])
//   s0 scalar
//
//   hexaedral FEM syntax:
//   [xx,yy,zz]=isosurf3d(dcorvg,kvert,field,f0)
//
//     dcorvg=(nvert,3) coordinates of the vertices of the cells
//     kvert=(ncells,8) list of the vertices of each cell (in a proper order)
//     field=(nvert,1) values of the scalar field at the vertex points
//     f0 scalar 
//
// example: see the demo below.
//
// Plot with:
//    xbasc(); plot3d1({xx;xx(1,:)},{yy;yy(1,:)},{zz;zz(1,:)})
// or
//    xbasc(); shadesurf(xx,yy,zz)
//
// note: this implementation is rather slow, surely due to the
//       nested loops and hypermat operations, which I don't know
//       how to vectorize more readably with scilab syntax (in
//       principle the algorithm is completely parallelizable). 
//       Timings are data dependent, as the work depends on
//       the number of facelets found.

//t=0

[lhs,rhs]=argn(0);
if rhs==0 then
//demo
    disp "demo of isosurf3d(x,y,z,s,s0)"
    disp " "
    democomm=[
    "  nx=20; ny=20; nz=20; s=hypermat([nx,ny,nz]);"
    "  x=linspace(-4.5,4.5,nx); xq=x''*ones(1,ny);"
    "  y=linspace(-4.5,4.5,ny); yq=ones(nx,1)*y;"
    "  z=linspace(-4.5,4.5,nz);"
    "  for i=1:nz; s(:,:,i)=1../((xq-2).^2+yq.^2+(z(i)-2)^2)+"+..
    "1../((xq+2).^2+yq.^2+(z(i)+2)^2); end"
    "  timer();[xx,yy,zz]=isosurf3d(x,y,z,-s,-.18); t=timer();"
    ]
    write(%io(2),democomm); execstr(democomm)
// the -0.2 surface gives a good example of the saddle point hole, btw
    aa=string(size(xx,2))+" triangles, "+string(t)+" sec --> "+..
       string(size(xx,2)/t)+" triang./sec"
    disp "  xbasc(); shadesurf(xx,yy,zz,1,0,60,60,''x@y@z'',[1 6 4])"
    xbasc();  shadesurf(xx,yy,zz,1,0,60,60,'x@y@z',[1 6 4])
    xx=aa;
    return
end;

if rhs==2 & type(x)==17 then
   s=x; s0=y; nx=size(s,1); ny=size(s,2); nz=size(s,3); 
   x=1:nx; y=1:ny; z=1:nz;
end 

if exists('s','local') & type(s)==17 then 
   datastruct='h';
   nx=size(s,1); ny=size(s,2); nz=size(s,3); 
   x=matrix(x,1,size(x,'*')); y=matrix(y,1,size(y,'*')); 
   z=matrix(z,1,size(z,'*'));
   if length(x)<nx | length(y)<ny | length(z)<nz then
     disp 'check the dimensions of the arguments!'; return
   end 
else
   datastruct='c'; 
   dcorvg=x; kvert=y; field=z; f0=s; 
   nvert=size(dcorvg,1); ncell=size(kvert,1)
   if size(field)<>[nvert,1] then
      disp 'wrong dimensions of the scalar field!'; return
   end
end


// Now, we have to generate a set of facelets which approximate
// the isosurface sought. My approach is to scan the hypermatrix
// cell by cell, and a) to identify the pattern of vertices > and
// < s0 (there are 23 possible patterns, which can appear in one
// of 48 possible orientations), then b) to generate a set of
// triangular facelets which represent the isosurface inside the cell.
// Ideally, the function inside the cell would be better
// approximated by shape functions. Shape functions evaluating
// to 1 in one vertex and to 0 in the other 7 would be of higher
// than linear order, and their isosurfacelets wouldn't be
// plane. My (to some extent arbitrary) approximation as
// triangular facelets whose vertices are the linear solutions
// along the sides of the cube, should at least be easier.
// I've found this method to work almost always, the only 
// exception being that it leaves a hole in some 2-cell saddle
// points (of type [[0 1;1 0],[1 0;0 1]]). Treating that would
// require to consider couples of adjacent cells, which is more
// complicate. Alternative approaches would of course be possible
// (e.g., only rectangles parallel to the coordinate planes
//  on the dual lattice,...).
//
//         8----7
//        /|   /|
//       5----6 |     reference order of vertices in the base
//       | 4--|-3     cell
//       |/   |/
//       1----2
//
//the following is obtained with [pp,par]=cubeperm() below, 
// and snippets like
//
//for i=1:48; write(%io(2),string(pp(i,1))+' '+string(pp(i,2))+' '+..
//          string(pp(i,3))+' '+string(pp(i,4))+' '+..
//          string(pp(i,5))+' '+string(pp(i,6))+' '+..
//          string(pp(i,7))+' '+string(pp(i,8))+'; ..' ); end
// time savings are minimal, but one never knows if they don't turn
// out useful
 
 
pp=[8 7 6 5 4 3 2 1; 8 7 3 4 5 6 2 1; 8 5 6 7 4 1 2 3; ..
    8 5 1 4 7 6 2 3; 8 4 3 7 5 1 2 6; 8 4 1 5 7 3 2 6; ..
    7 8 5 6 3 4 1 2; 7 8 4 3 6 5 1 2; 7 6 5 8 3 2 1 4; ..
    7 6 2 3 8 5 1 4; 7 3 4 8 6 2 1 5; 7 3 2 6 8 4 1 5; ..
    6 7 8 5 2 3 4 1; 6 7 3 2 5 8 4 1; 6 5 8 7 2 1 4 3; ..
    6 5 1 2 7 8 4 3; 6 2 3 7 5 1 4 8; 6 2 1 5 7 3 4 8; ..
    5 8 7 6 1 4 3 2; 5 8 4 1 6 7 3 2; 5 6 7 8 1 2 3 4; ..
    5 6 2 1 8 7 3 4; 5 1 4 8 6 2 3 7; 5 1 2 6 8 4 3 7; ..
    4 8 7 3 1 5 6 2; 4 8 5 1 3 7 6 2; 4 3 7 8 1 2 6 5; ..
    4 3 2 1 8 7 6 5; 4 1 5 8 3 2 6 7; 4 1 2 3 8 5 6 7; ..
    3 7 8 4 2 6 5 1; 3 7 6 2 4 8 5 1; 3 4 8 7 2 1 5 6; ..
    3 4 1 2 7 8 5 6; 3 2 6 7 4 1 5 8; 3 2 1 4 7 6 5 8; ..
    2 6 7 3 1 5 8 4; 2 6 5 1 3 7 8 4; 2 3 7 6 1 4 8 5; ..
    2 3 4 1 6 7 8 5; 2 1 5 6 3 4 8 7; 2 1 4 3 6 5 8 7; ..
    1 5 8 4 2 6 7 3; 1 5 6 2 4 8 7 3; 1 4 8 5 2 3 7 6; ..
    1 4 3 2 5 8 7 6; 1 2 6 5 4 3 7 8; 1 2 3 4 5 6 7 8];

ppar=[1 -1 -1  1  1 -1 -1  1  1 -1 -1  1 -1  1  1 -1 -1  1 ..
      1 -1 -1  1  1 -1 -1  1  1 -1 -1  1  1 -1 -1 ..
      1  1 -1  1 -1 -1  1  1 -1 -1  1  1 -1 -1  1];

// "parity" of the permutation of 1:8 above. This is important
// to discriminate the inside and the outside of the isosurface element

[ss,ppinv]=sort(-pp,'c'); 
// this takes up a negligible time, it's not worth to write
//  also ppinv as data


//// base patterns of 0 and 1 on the vertices
//b=zeros(23,8); nft=zeros(23);
//b(1,:)= [0 0 0 0 0 0 0 0];  nft(1)=0;    // 0 t
//b(2,:)= [1 0 0 0 0 0 0 0];  nft(2)=1;    // 1 t
//b(3,:)= [1 1 0 0 0 0 0 0];  nft(3)=2;    // 2 t
//b(4,:)= [1 0 1 0 0 0 0 0];  nft(4)=2;    // 2 t
//b(5,:)= [1 1 1 0 0 0 0 0];  nft(5)=3;    // 3 t
//b(6,:)= [1 0 0 0 0 0 1 0];  nft(6)=2;    // 2 t
//b(7,:)= [1 1 0 0 0 0 1 0];  nft(7)=3;    // 3 t
//b(8,:)= [1 0 1 0 0 0 0 1];  nft(8)=3;    // 3 t
//b(9,:)= [1 1 1 1 0 0 0 0];  nft(9)=2;    // 2 t
//b(10,:)=[1 1 0 0 0 0 1 1];  nft(10)=4;    // 4 t
//b(11,:)=[1 0 1 0 1 0 1 0];  nft(11)=4;    // 4 t
//b(12,:)=[1 0 1 0 0 1 0 1];  nft(12)=4;    // 4 t
//b(13,:)=[1 1 1 0 1 0 0 0];  nft(13)=4;    // 4 t
//b(14,:)=[1 1 1 0 0 1 0 0];  nft(14)=4;    // 4 t 
//b(15,:)=[1 1 1 0 0 0 0 1];  nft(15)=4;    // 4 t
//b(16:23,:)=abs(1-b(8:-1:1,:)); nft(16:23)=nft(8:-1:1);
//bb=b==1; 
//// now that patterns are inlined as data, the array b isn't
//// be needed anymore

nft=[0 1 2 2 3 2 3 3 2 4 4 4 4 4 4 3 3 2 3 2 2 1 0]
// number of triangles for each of the 23 basic patterns

//bp=zeros(23*48,8)==1;
//for l=1:23; bp(l:23:$,:)=matrix(bb(l,pp),48,8); end;
//bp1=bp(:,1); bp2=bp(:,2); bp3=bp(:,3); bp4=bp(:,4); 
//bp5=bp(:,5); bp6=bp(:,6); bp7=bp(:,7); bp8=bp(:,8); 
//// these bp1..bp8 make up the columns of the matrix of
//// all patterns permuted in all possible ways


// definition of the triangular facelets for each pattern:
//  (all but full cell/void cell, which are skipped)
// The entries of vtrian identify, for each triangle, the 
//  three sides of the cell on which a vertex lies. Each of 
//  these sides is identified by the couple of vertices of the
//  cell among which it runs. When the pattern requires less
//  than 4 triangles, the extra entries of vtrian are ones.
// vtrian=ones(45,6*4);
// for l=-23:23
//        nt=0
//        select abs(l)
//          case 2 then 
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,4,1,5];  nt=nt+1 ,
//          case 3 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,3,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,6,2,3];  nt=nt+1 ,
//          case 4 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,4,1,5];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,7,3,4,2,3];  nt=nt+1 ,
//          case 5 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,1,5,2,6];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,2,6,3,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,7,3,4,2,6];  nt=nt+1 ,
//          case 6 then
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,4,1,5];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [6,7,7,8,3,7];  nt=nt+1 ,
//          case 7 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,3,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,6,2,3];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [6,7,7,8,3,7];  nt=nt+1 ,
//          case 8 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,4,1,5];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,7,3,4,2,3];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [5,8,7,8,4,8];  nt=nt+1 ,
//          case 9 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,6,3,7];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,3,7,4,8];  nt=nt+1 ,
//          case 10 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,3,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,6,2,3];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [4,8,6,7,5,8];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [6,7,4,8,3,7];  nt=nt+1 ,
//          case 11 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,4,5,6];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,5,8,5,6];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,3,7,8,3,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,3,6,7,7,8];  nt=nt+1 ,
//          case 12 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,4,1,5];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,7,3,4,2,3];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [4,8,7,8,5,8];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,6,5,6,6,7];  nt=nt+1 ,
//          case 13 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,5,8,5,6];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [5,6,2,6,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,6,3,4,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,6,3,7,3,4];  nt=nt+1 ,
//          case 14 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,5,6,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [5,6,3,4,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [5,6,6,7,3,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,4,6,7,3,7];  nt=nt+1 ,
//          case 15 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,1,5,2,6];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,2,6,3,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [4,8,7,8,5,8];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,6,3,7,3,4];  nt=nt+1 ,
//          case 16 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,5,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,4,3,7,2,3];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [4,8,7,8,5,8];  nt=nt+1 ,
//          case 17 then,
//             vtrian(l+23,6*nt+(1:6)) = [2,3,1,5,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,5,2,6,2,3];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,7,7,8,6,7];  nt=nt+1 ,
//          case 18 then
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,5,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,7,7,8,6,7];  nt=nt+1 ,
//          case 19 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,2,6,1,5];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [1,4,3,4,2,6];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,4,3,7,2,6];  nt=nt+1 ,
//          case 20 then,
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,5,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [3,4,3,7,2,3];  nt=nt+1 ,
//          case 21 then,
//             vtrian(l+23,6*nt+(1:6)) = [2,3,1,5,1,4];  nt=nt+1 ,
//             vtrian(l+23,6*nt+(1:6)) = [2,3,2,6,1,5];  nt=nt+1 ,
//          case 22 then 
//             vtrian(l+23,6*nt+(1:6)) = [1,2,1,5,1,4];  nt=nt+1 ,
//        end
// // switch the order of vertices if the parity of the transformation was
// // negative - this way the 'upper' of any facelet points consistently
// //  in the direction where the function increases
//        if  l<0 & l>-23 then
//           aa=vtrian(l+23,[1,2,7,8,13,14,19,20]); 
//           vtrian(l+23,[1,2,7,8,13,14,19,20])=...
//             vtrian(l+23,[5,6,11,12,17,18,23,24]);
//           vtrian(l+23,[5,6,11,12,17,18,23,24])=aa
//        end
// end
// to produce the output for the inlining
//   for i=1:45;printf('%i ',vtrian(i,:)');end

vtrian = [ 
1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 2 3 1 5 2 6 2 3 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 1 2 2 3 3 7 3 4 1 4 1 5 1 2 1 4 1 5 1 2;
1 5 2 6 1 4 2 6 3 4 1 4 2 6 3 7 3 4 1 4 1 5 1 2;
1 4 1 5 1 2 6 7 7 8 3 7 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 2 3 2 3 2 6 1 5 6 7 7 8 3 7 1 4 1 5 1 2;
1 4 1 5 1 2 2 3 3 7 3 4 5 8 7 8 4 8 1 4 1 5 1 2;
2 6 1 5 1 4 3 4 2 6 1 4 5 8 7 8 4 8 3 4 3 7 2 6;
1 4 5 6 1 5 1 4 3 4 5 6 3 4 6 7 5 6 3 7 6 7 3 4;
5 6 5 8 1 4 1 4 2 6 5 6 1 4 3 4 2 6 3 4 3 7 2 6;
1 5 1 4 1 2 2 3 3 4 3 7 5 8 7 8 4 8 6 7 5 6 2 6;
5 6 1 4 1 2 5 6 5 8 1 4 3 4 7 8 2 3 7 8 6 7 2 3;
1 4 2 3 1 5 2 3 2 6 1 5 5 8 6 7 4 8 3 7 4 8 6 7;
3 7 2 6 1 5 4 8 3 7 1 5 1 4 1 5 1 2 1 4 1 5 1 2;
1 5 1 4 1 2 2 3 3 4 3 7 4 8 7 8 5 8 1 4 1 5 1 2;
1 4 2 3 1 5 2 3 2 6 1 5 3 7 7 8 6 7 1 4 1 5 1 2;
1 5 1 4 1 2 3 7 7 8 6 7 1 4 1 5 1 2 1 4 1 5 1 2;
2 6 1 5 1 4 3 4 2 6 1 4 2 6 3 4 3 7 1 4 1 5 1 2;
1 5 1 4 1 2 2 3 3 4 3 7 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 2 3 1 5 2 3 2 6 1 5 1 4 1 5 1 2 1 4 1 5 1 2;
1 5 1 4 1 2 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2;
1 2 1 4 1 5 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2;
1 5 2 3 1 4 1 5 2 6 2 3 1 4 1 5 1 2 1 4 1 5 1 2;
1 2 1 4 1 5 3 7 3 4 2 3 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 1 5 2 6 1 4 2 6 3 4 3 7 3 4 2 6 1 4 1 5 1 2;
1 2 1 4 1 5 6 7 7 8 3 7 1 4 1 5 1 2 1 4 1 5 1 2;
1 5 2 3 1 4 1 5 2 6 2 3 6 7 7 8 3 7 1 4 1 5 1 2;
1 2 1 4 1 5 3 7 3 4 2 3 5 8 7 8 4 8 1 4 1 5 1 2;
1 5 2 6 3 7 1 5 3 7 4 8 1 4 1 5 1 2 1 4 1 5 1 2;
1 5 2 3 1 4 1 5 2 6 2 3 4 8 6 7 5 8 6 7 4 8 3 7;
1 2 1 4 5 6 1 4 5 8 5 6 2 3 7 8 3 4 2 3 6 7 7 8;
1 2 1 4 1 5 3 7 3 4 2 3 4 8 7 8 5 8 2 6 5 6 6 7;
1 4 5 8 5 6 5 6 2 6 1 4 2 6 3 4 1 4 2 6 3 7 3 4;
1 5 5 6 1 4 5 6 3 4 1 4 5 6 6 7 3 4 3 4 6 7 3 7;
1 4 1 5 2 6 1 4 2 6 3 4 4 8 7 8 5 8 2 6 3 7 3 4;
1 2 1 5 1 4 3 4 3 7 2 3 4 8 7 8 5 8 1 4 1 5 1 2;
2 3 1 5 1 4 1 5 2 6 2 3 3 7 7 8 6 7 1 4 1 5 1 2;
1 2 1 5 1 4 3 7 7 8 6 7 1 4 1 5 1 2 1 4 1 5 1 2;
1 4 2 6 1 5 1 4 3 4 2 6 3 4 3 7 2 6 1 4 1 5 1 2;
1 2 1 5 1 4 3 4 3 7 2 3 1 4 1 5 1 2 1 4 1 5 1 2;
2 3 1 5 1 4 2 3 2 6 1 5 1 4 1 5 1 2 1 4 1 5 1 2;
1 2 1 5 1 4 1 4 1 5 1 2 1 4 1 5 1 2 1 4 1 5 1 2 ] 
// here I've set the extra vertices beyond 3*nt as repeated 
// 1 4 1 5 1 2 to avoid divisions by zero in vectorized 
// interpolation (doesn't really work yet)

////this is the snippet for generating the magic numbers below.
/// Performance improves if they are inlined as data.
//pfa=zeros(1,256); bfa=pfa; 

pow2=[128 64 32 16 8 4 2 1]';

//for l=0:255 
//       d2b=zeros(1,8)
//       for i=1:8
//         d2b(i)=int((l-d2b*pow2)/pow2(i))
//       end
//       sb8=d2b==1
//       pbf=find(bp1==sb8(1) & bp2==sb8(2) & bp3==sb8(3)..
//              & bp4==sb8(4) & bp5==sb8(5) & bp6==sb8(6)..
//              & bp7==sb8(7) & bp8==sb8(8) )
//      ppf=pbf(1)
//      pfa(l+1)=int((ppf-1)/23)+1; bfa(l+1)=ppf-pfa(l+1)*23+23
//// note that this also works since 48 and 23 happen to be prime
////  among themselves
//end
//disp(int8(pfa))
//disp(int8(bfa))
//
/// The explanation is: for all the 256 possibilities of having
/// each of the 8 vertices of the cell above/below the threshold,
///  specify which is the relevant triangle pattern (pf) and 
/// arrangement (bf)

pfa=[1 1 7 1 3 1 3 1 19 13 7 7 15 13 9 1 20 14 8 8 3 ..
  20 3 1 25 25 9 7 15 13 9 36 4 2 4 2 5 13 5 1 4 ..
  19 4 2 21 13 5 40 16 14 10 2 22 14 6 39 16 14 10 ..
  35 5 47 41 41 23 1 11 1 17 23 11 3 27 13 7 7 27 15 ..
  9 34 28 14 8 8 17 1 11 37 25 25 7 5 27 10 2 34 18 ..
  24 12 4 17 17 5 37 18 2 12 38 17 44 32 38 28 16 10 ..
  33 18 43 31 37 28 9 1 33 7 7 31 43 43 31 7 7 33 1 ..
  9 1 37 31 43 19 33 13 21 28 38 32 44 20 38 32 3 18 ..
  37 25 37 17 37 12 24 18 34 2 10 2 5 1 5 25 37 31 4 ..
  17 33 8 14 28 34 14 22 27 34 7 13 27 38 11 23 17 1 ..
  11 1 23 41 41 47 1 35 35 23 16 39 39 39 22 27 10 14 ..
  16 40 40 40 21 42 4 19 4 37 5 13 5 2 4 2 4 36 36 ..
  24 15 17 9 25 25 41 3 20 3 8 8 14 20 28 9 13 15 7 ..
  7 13 19 1 3 1 3 1 7 1 1 ] 
 
bfa=[1 2 2 3 2 4 3 5 2 3 4 5 3 5 5 9 2 3 4 5 6 7 7 ..
  13 4 5 8 14 7 13 15 19 2 4 3 5 4 8 5 14 6 7 7 13 ..
  7 15 13 19 3 5 5 9 7 15 13 19 7 13 15 19 11 17 17 ..
  21 2 6 4 7 3 7 5 13 4 7 8 15 5 13 14 19 4 7 8 15 ..
  7 11 15 17 8 15 12 16 15 17 16 20 3 7 5 13 5 15 9 ..
  19 7 11 15 17 13 17 19 21 5 13 14 19 13 17 19 21 15 ..
  17 16 20 17 18 20 22 2 4 6 7 4 8 7 15 3 5 7 13 5 ..
  14 13 19 3 5 7 13 7 15 10 17 5 9 15 19 13 19 17 21 ..
  4 8 7 15 8 12 15 16 7 15 10 17 15 16 17 20 5 14 13 ..
  19 15 16 17 20 13 19 17 21 17 20 18 22 3 7 7 10 5 ..
  15 13 17 5 13 15 17 9 19 19 21 5 13 15 17 13 17 17 ..
  18 14 19 16 20 19 21 20 22 5 15 13 17 14 16 19 20 ..
  13 17 17 18 19 20 21 22 9 19 19 21 19 20 21 22 19 ..
  21 20 22 21 22 22 23 ] 

// also this sits better outside of the loops:
v=ones(255,24);
for l=(2:255)
  v(l,:)=ppinv(pfa(l),vtrian(bfa(l)*ppar(pfa(l))+23,:));
end

nfta=nft(bfa(1:256));

// now scan every cube of the 3d hypermat

nf=0

// a few definitions brought out of the loops
u1=int((0:35)/3 +1)*2-1; u2=u1+1;
v1=1:2:23; 
//v2=v1+1; v12=[v1,v2]; va=1:12; vb=va+12;
uu1=4*u1; uu2=4*u2; 
uv2=[v1*4+1;v1*4+2;v1*4+3]; uv1=uv2-4;

xyz=[];
// to dimension xyz maximally (e.g. (9,4*(nxm)*(nym)*(nzm)) ) 
// from start would be a waste of memory, but to grow it for
// every added triangle would be quite slow. I think that a good 
// compromise is to resize xx,yy,zz at each step of the i loop
// (there xyz needs to grow only from (9,nt-1) to (9,4*nxm*nym+nt-1) )


nt=0

if datastruct=="h" then
  nxm=nx-1; nym=ny-1; nzm=nz-1
  nx1=1:nxm; ny1=1:nym; nz1=1:nzm; ny2=ny1+1; nz2=nz1+1;
  u=1:3; nyzm=nym*nzm; Ny=1:ny; Nz=1:nz; js=(0:7)*nym;
  yy8=[y(ny1);y(ny1);y(ny2);y(ny2);y(ny1);y(ny1);y(ny2);y(ny2)]'
  zz8=[z(nz1);z(nz1);z(nz1);z(nz1);z(nz2);z(nz2);z(nz2);z(nz2)]'
// sort the points of the cube in canonical order - the
//  outer in the loop the faster (?)
  for i=nx1
// I scan the hypermatrix with i outer, try to collapse j and k
   i1=i+1;
   x8=x([i,i1,i1,i,i,i1,i1,i])
   s8i=matrix(s(i,Ny,Nz),ny,nz); //s8i=s8i(:,:)
   s8i1=matrix(s(i1,Ny,Nz),ny,nz); //s8i1=s8i1(:,:)
// s8i1(:,:) etc instead of s8i1 for compatibility with scilab 2.4.1
   ss8=double([s8i(ny1,nz1); s8i1(ny1,nz1); s8i1(ny2,nz1); s8i(ny2,nz1); ..
             s8i(ny1,nz2); s8i1(ny1,nz2); s8i1(ny2,nz2); s8i(ny2,nz2)])'-s0; 
// this matrix ss8 now contains all the necessary values of s in the
// planes i and i+1. I play index tricks, and try to minimize the
// use of hypermatrices, as calculations with them are considerably
// slower. 
   ll=matrix(matrix(bool2s(ss8>0),nyzm,8)*pow2+1,nzm,nym);
// Recognize which pattern of >0 and <0 appears in each cell of 
// this plane: boolean((ny-1)*(nz-1),8) --> integers in a snap, 
// for the whole plane.
// Usually most of the cells have all 8 values either >s0 or <s0,
// and don't need to be considered for isosurfacelets. So the following
//  loop is only on those cells which have some vertices on 
//  opposite sides of the s0 divider.
   [kk,jj]=find(ll>1 & ll<256); njk=length(jj); 
// making space in xyz
   xyz=[xyz(1:9*nt);zeros(36*njk,1)];
   for q=1:njk
       j=jj(q); k=kk(q);
       l=ll(k,j)
       nftbf=nfta(l);
// pick up the relevant vertices for constructing triangles
// and generate the triangular facelets
       X=[x8;yy8(j,:);zz8(k,:);ss8(k,js+j)]
//   X 4x8 stores {x,y,z,s-s0} of the 8 vertices of the cell.
//timer()
       Y=X(:,v(l,:))  // Y(4,24)
//the following criptic lines interpolate linearly along the relevant
// sides, and return nftbf triples of x,y,z, vertices of the triangles.
// Matrix flattening is used to increase speed. Index arrays like
// u,u1,u2,v1,v2,uu1,uu2,uv1,uv2, have been defined above.
       S1=Y(uu1); S2=Y(uu2);
       D=S2-S1; N=S2.*Y(uv1)-S1.*Y(uv2);
// denominators are never zero if Y is formed with the right points
//  and if only the first nftbf triangles are computed
       uu=1:9*nftbf;
       xyz(9*nt+uu)=N(uu)./D(uu)
//t=t+timer()
// There is a redundancy of operations, as points are
// always aligned (have two equal coordinates), so will be their
// mean point, and there would be no need to compute it. I'm afraid 
// that any catch of that will only overburden the snippet.
       nt=nt+nftbf
   end
  end
else
  ll=matrix(bool2s(field(kvert)>f0),ncell,8)*pow2+1
  jj=find(ll>1 & ll<256);
  xyz=zeros(9*sum(nfta(ll(jj))),1);
  for j=jj
       l=ll(j); k=kvert(j,:); nftbf=nfta(l);
       X=[dcorvg(k,1)';dcorvg(k,2)';dcorvg(k,3)';field(k)'-f0]
//   X 4x8 stores {x,y,z,s-s0} of the 8 vertices of the cell.
       Y=X(:,v(l,:))  // Y(4,24)
       S1=Y(uu1); S2=Y(uu2);
       D=S2-S1; N=S2.*Y(uv1)-S1.*Y(uv2);
       uu=1:9*nftbf;
       xyz(9*nt+uu)=N(uu)./D(uu)
       nt=nt+nftbf
  end
end
// output only the xx,yy,zz needed
u=1:nt
xyz=matrix(xyz,9,-1)
xx=xyz(1:3:7,u); yy=xyz(2:3:8,u); zz=xyz(3:3:9,u); 


//write(%io(2),'time spent in the selected routine:'+string(t))

return



//function [pp,ppar]=cubeperm()
//// all the topologically equivalent permutations of the vertices:
////  (enumerating is less clever than thinking of an algorithm)
//// ( I'm dummy as they are just all the elements of a discrete SO(3) )
////
//p=zeros(162,8); par=zeros(162,1);
//p(1,:)= [1 2 3 4 5 6 7 8]; 
//// around z
//p(2,:)= [2 3 4 1 6 7 8 5];
//p(3,:)= [3 4 1 2 7 8 5 6];
//p(4,:)= [4 1 2 3 8 5 6 7];
//// around x
//p(5,:)= [4 3 7 8 1 2 6 5];
//p(6,:)= [8 7 6 5 4 3 2 1];
//p(7,:)= [5 6 2 1 8 7 3 4];
//// around y
//p(8,:)= [2 6 7 3 1 5 8 4];
//p(9,:)= [6 5 8 7 2 1 4 3];
//p(10,:)=[5 1 4 8 6 2 3 7]; par(1:10)=1;
//// reflect xy
//p(11:20,:)=p(1:10,[5:8,1:4]);  par(11:20)=-1;
//// reflect yz
//p(21:30,:)=p(1:10,[2 1 4 3 6 5 8 7]);  par(21:30)=-1;
//// reflect xz
//p(31:40,:)=p(1:10,[4 3 2 1 8 7 6 5]);  par(31:40)=-1;
//// this is still missing
//p(41,:)=[7 3 2 6 8 4 1 5];  par(41)=1;
////these are 27 unique -let's try to add more
//p(42:81,:)=p(2:41,p(2,:));  par(42:81)=par(2:41);
//p(82:121,:)=p(2:41,p(5,:));  par(82:121)=par(2:41);
//p(122:161,:)=p(2:41,p(8,:));  par(122:161)=par(2:41);
//// not enough, still this
//p(162,:)=[8 5 1 4 7 6 2 3];   par(162)=1;
//// now there are 48 unique (take any of the 8 vertices as first -
////  then you have 3! ways to permute it's adjacent nodes)
//// now I sort and uniq them via a string sort
//ps=string(p(:,1))+" "+string(p(:,2))+" "+string(p(:,3))+" "+string(p(:,4))..
//   +" "+string(p(:,5))+" "+string(p(:,6))+" "+string(p(:,7))+..
//   " "+string(p(:,8))
//[ps,nn]=gsort(ps); par=par(nn); pss=" ";
//pss(1)=ps(1); ppar(1)=par(1);  j=1;
//for i=1:size(p,1)
//   if ps(i)<>pss(j) then pss(j+1)=ps(i); ppar(j+1)=par(i); j=j+1; end
//end
//pp=evstr(pss); clear ps; clear p; clear par;
//
//return


endfunction


function c=shadecomp(xx,yy,zz,l,shine,m)

// Computes facelet colors for subsequent rendering with
// plot3d. Having the access to the computed colors allows
// further transformations, and assignation of faces to different
// segments of stacked colormaps.
//
// c(1,size(xx,2)) for normal plot3d(xx,yy,list(zz,c)) rendering,
//  while if m=2 (goraud) c has the same size of xx,yy,zz
//  for rendering with Stephane Mottelet's interpolated color plot3d
//  extension


[lhs,rhs]=argn(0);

if rhs==0 then
  return
end
if ~exists('l','local') then l=[1,1,1]; end
if ~exists('shine','local') then shine=1; end
if ~exists('m','local') then m=1; end

ns=size(xx,1); nf=size(xx,2)
i1=1:ns; i2=[2:ns,1]; i3=[3:ns,1,2];

nn1=-((yy(i1,:)-yy(i2,:)).*(zz(i2,:)-zz(i3,:))-...
      (yy(i2,:)-yy(i3,:)).*(zz(i1,:)-zz(i2,:)));
//if ns>3 then
  nn2=((xx(i1,:)-xx(i2,:)).*(zz(i2,:)-zz(i3,:))-...
       (xx(i2,:)-xx(i3,:)).*(zz(i1,:)-zz(i2,:)));
  nn3=-((xx(i1,:)-xx(i2,:)).*(yy(i2,:)-yy(i3,:))-...
        (xx(i2,:)-xx(i3,:)).*(yy(i1,:)-yy(i2,:)));
//else
//   nn2=nn1; nn3=nn1;
//end

// nn1,nn2,nn3 are 3 matrices of size (ns,nf), which store the
//  3 components of the normal at each vertex.
// for triangular (ns=3) faces this calculation is redundant,
// since triangles are always planar and the three vertex
// normals will be identical


[degf1,degf2]=find(abs(nn1)+abs(nn2)+abs(nn3)<10*%eps);
// this can happen for instance if there are coincident vertices;
// it shouldn't, if xx, yy, zz are constructed properly, but
// I want shadesurf to get through it anyway
// the following is capable to handle two consecutive coincident
// vertices. Three or more consecutive coincident vertices are
// partially handled, and a division by zero will result below.
if degf1<>[] then
  q1=degf1+(degf2-1)*ns; q2=modulo(degf1+1,ns)+1+(degf2-1)*ns
  nn1(q1)=nn1(q2); nn2(q1)=nn2(q2); nn3(q1)=nn3(q2)
// I assume that in this case at least the second next vertex of
// the face is not degenerate (if two vertices coincide, then
// two consecutive normals are null)
end

//nn3(degf1,degf2)=1;  // this is bad but works as last resort
nn=sqrt(nn1.^2+nn2.^2+nn3.^2); 
nn(nn==0)=1; // this is a last resort for null faces, with more than
             // two coinciding vertices, or faces degenerate to a
             // segment (e.g., triangles with a zero side)

//cc=sqrt(nn1.^2+nn2.^2+nn3.^2);
// color proportional to the slope

//cc=1../max(a*nn1+b*nn2,1);
// color inversely proportional to the lightened side

c=(l(1)*nn1+l(2)*nn2+l(3)*nn3)./nn./sqrt(l*l');
c=((c+1)/2).^shine;
// color proportional to the cosine of the angle (local normal)^(sun)
// the direction of the sun is [l(1), l(2), l(3)]

clear nn nn1 nn2 nn3

if abs(m)==1 then
  c=mean(c,'r')
// flat shading -> only one c per face is output, the average of
// all the c values
end

if abs(m)==2
// if facelets are triangular, they are obviously planar
// I construct a vertex c and try to go goraud
//   if ns==4 then
//// recognize if the vertices are laid on a structured grid:
////  try if there is a subset of faces set side to side. 
//       stru1=find(xx(4,1:(nf-1))==xx(1,2:nf) & ..
//            yy(4,1:(nf-1))==yy(1,2:nf) & zz(4,1:(nf-1))==zz(1,2:nf) )
//       stru2=find(xx(2,1:(nf-1))==xx(1,2:nf) & ..
//            yy(2,1:(nf-1))==yy(1,2:nf) & zz(2,1:(nf-1))==zz(1,2:nf) )
//   end
//   if %f then
//     disp('structured!')
//   end
// lex_sort seems the best solution to look for adjacencies
//  arrays are silently flattened here
   [xyz,k]=lex_sort([xx(:),yy(:),zz(:)]);
   clear xx yy zz
   q=[0,find( or(xyz(1:$-1,:)~=xyz(2:$,:),'c') ), ns*nf];
   for i=1:length(q)-1
     qq=k(q(i)+1:(q(i+1)))
     c(qq)=mean(c(qq))
   end
end

// range expansion
if m<0 then
  if max(c)-min(c)>2*%eps then
    c=(c-min(c))/(max(c)-min(c))
  else
// this can happen if the surface is a plane, and I want this to
//  be handled
   c=ones(size(c,1),size(c,2));
  end
end


return

endfunction

function oplot3d(data,nc,theta,alpha,leg,flag,ebox)

// data=list(xx1,yy1,zz1,c1,icontext1,xx2,yy2,zz2,c2,icontext2,...)
// nc vector of number of colors in the stack
// to make things easier to begin with, let's suppose that all
// the objects have facelets with the same number of sides

[lhs,rhs]=argn(0);

if ~exists('nc','local') then nc=xget('lastpattern'); end
// usually one wants to provide nc, this is a last resource escape
if ~exists('theta','local') then theta=45; end
if ~exists('alpha','local') then alpha=35; end
if ~exists('leg','local') then leg='X@Y@Z'; end
if ~exists('flag','local') then flag=[-1 2 4]'; end
if ~exists('ebox','local') then ebox=[0 0 0 1 1 1]'; end

if rhs==0 | type(data)<>15 then
 write(%io(2),..
  ' The first argument of oplot3d has to be a list of xx,yy,zz,c'+..
  ',icontext  tuples!')
 return
end

// scanning the argument "data" for valid entries
lobj=[]; nobj=0; nfj=[]; ncj=[]; ndata=length(data); j=1;
while j<=ndata
   if type(data(j))==15 then
      if length(data(j))==4 then data(j)(5)=1; end
      if length(data(j))<>5 then
         write(%io(2),..
            'graphic object #'+string(nobj+1)+' is a list, but not a 5-tuple!')
         return
      end
      nobj=nobj+1;
      lobj=[lobj,%t]
      nfj=[nfj,size(data(j)(1),1)]
      ncj=[ncj,size(data(j)(4),1)]
      j=j+1
   else
      if ndata < j+4 then
        write(%io(2),..
            'last graphic object is not a 5-tuple!')
        return
      end    
      if ~(type(data(j))==1 & type(data(j+1))==1 &..
           type(data(j+2))==1 & type(data(j+3))==1 &..
           type(data(j+4))==1) then
         write(%io(2),..
            'graphic object #'+string(nobj+1)+' is not a 5-tuple of reals!')
         return
      else          
        nobj=nobj+1;
        lobj=[lobj,%f]
        nfj=[nfj,size(data(j),1)]
        ncj=[ncj,size(data(j+3),1)]
        j=j+5
      end
   end
end

// find the maximum number of sides of all the faceted objects 
nf=max(nfj)
ncv=max(ncj)

if or(ncj<>1 & ncj<>nfj) then
 write(%io(2),..
  '  something wrong with the color arrays dimensions:')
  for k=1:nobj
    if ncj(k)<>1 & ncj(k)<>nfj(k) then
       write(%io(2),'object '+string(k)+' has facelets with '+..
            string(nfj(k))+' sides, but the color array has '+..
            string(ncj(k))+' corresponding entries')
    end
  end
 return
end

// Now we construct a single set xx,yy,zz,cc for all the objects.
// the number of vertices is augmented to nf if nf(j)<nf. This
// allows concatenation of xx,yy,zz; a little memory consuming,
// but easier to code than calling plot3d separately for each object
// Besides, a single call presents no visibility issues.
// One object with vertex colors will force all to have vertex colors.
xx=[];yy=xx;zz=xx;cc=xx;
j=1;
for k=1:nobj
  if lobj(k) then
    xx=[xx,data(j)(1)([1:nfj(k),nfj(k)*ones(1,nf-nfj(k))],:)]
    yy=[yy,data(j)(2)([1:nfj(k),nfj(k)*ones(1,nf-nfj(k))],:)]
    zz=[zz,data(j)(3)([1:nfj(k),nfj(k)*ones(1,nf-nfj(k))],:)]
    ic=min(max(data(j)(5),1),length(nc))
    cc=[cc,data(j)(4)([1:ncj(k),ncj(k)*ones(1,nf-ncj(k))],:)..
       *(nc(ic)-1)+sum(nc(1:(ic-1)))+1]
    j=j+1
  else
    xx=[xx,data(j)([1:nfj(k),nfj(k)*ones(1,nf-nfj(k))],:)]
    yy=[yy,data(j+1)([1:nfj(k),nfj(k)*ones(1,nf-nfj(k))],:)]
    zz=[zz,data(j+2)([1:nfj(k),nfj(k)*ones(1,nf-nfj(k))],:)]
    ic=min(max(data(j+4),1),length(nc))
    cc=[cc,data(j+3)([1:ncj(k),ncj(k)*ones(1,nf-ncj(k))],:)..
        *(nc(ic)-1)+sum(nc(1:(ic-1)))+1]
    j=j+5
  end
end

clear data  // it still occupies memory, not needed any more

plot3d1(xx,yy,list(zz,cc),theta,alpha,leg,flag,ebox)

return

endfunction


function [xx,yy,zz]=sphere(x0,r,n)
// expanded from SCI/surface/surfaces.sci
  [lhs,rhs]=argn(0);
  if rhs<3 then n=16; end
  if rhs<2 then r=1; end
  if rhs<1 then x0=[0,0,0]; end

  if size(x0)==[3,1] then x0=x0'; end
  if size(x0,2)<>3 then
     disp('wrong size of x0'); return
  end
  ns=size(x0,1)
  if length(r)<>ns then r=r(1)*ones(ns,1); end 

  vn=2*n

// prototype sphere
  u = linspace(-%pi/2,%pi/2,n);
  v = linspace(0,2*%pi,vn);  
  x= cos(u)'*cos(v);
  y= cos(u)'*sin(v);
  z= sin(u)'*ones(v);

  in=1:(n-1); inp=2:n; in2=1:(vn-1)
  xxx=[matrix(x(in,in2+1),1,(n-1)*(vn-1));
       matrix(x(in,in2),1,(n-1)*(vn-1));
       matrix(x(inp,in2),1,(n-1)*(vn-1));
       matrix(x(inp,in2+1),1,(n-1)*(vn-1))]
  yyy=[matrix(y(in,in2+1),1,(n-1)*(vn-1));
       matrix(y(in,in2),1,(n-1)*(vn-1));
       matrix(y(inp,in2),1,(n-1)*(vn-1));
       matrix(y(inp,in2+1),1,(n-1)*(vn-1))]
  zzz=[matrix(z(in,in2+1),1,(n-1)*(vn-1));
       matrix(z(in,in2),1,(n-1)*(vn-1));
       matrix(z(inp,in2),1,(n-1)*(vn-1));
       matrix(z(inp,in2+1),1,(n-1)*(vn-1))]

 
//multiplication of spheres

   nf=size(xxx,2);
   xx=zeros(4,nf*ns); yy=xx; zz=xx
   for i=1:ns
     xx(:,((i-1)*nf+1):i*nf)=xxx*r(i)+x0(i,1)
     yy(:,((i-1)*nf+1):i*nf)=yyy*r(i)+x0(i,2)
     zz(:,((i-1)*nf+1):i*nf)=zzz*r(i)+x0(i,3)
   end

return

endfunction

function [xx,yy,zz]=spaghetti(x,y,z,w,nf)

[lhs,rhs]=argn(0);

if rhs==0 then
  disp('demo of spaghetti(x,y,z,w)');disp('');
  comm=[..
     'h=(-25:25)''/20; s=(tanh(h.^2/2)+1)/2; alpha=(-%pi:0.8:%pi);';..
     'nl=length(s); na=length(alpha);';..
     'x=zeros(nl,na); y=x; z=x; w=zeros(nl,3*na);';..
     'for i=1:na;';..
     '   phi=alpha(i);';..
     '   x(:,i)=s*cos(phi); y(:,i)=h; z(:,i)=s*sin(phi);';..
     'end';..
  'xbasc(); [xx,yy,zz]=spaghetti(x,y,z,0.08); shadesurf(xx,yy,zz,10);']
  write(%io(2),comm)
  execstr(comm)
  xtitle('demo for spaghetti()')
  xx=[];yy=[];zz=[];
  return
end

xx=[];

if rhs<5 then nf=11; end

if rhs<3 then 
    disp('spaghetti() wants one or more trajectories x,y,z !')
    return
end

if rhs==3 then w=(max(y)-min(y))/20; end  //just a given value

if size(x)~=size(y) | size(z)~=size(y) | size(x)~=size(z) then
   disp('inconsistency in x,y,z lengths')
   return
end

np=size(x,1); nv=size(x,2);
if np==1 then 
   disp('cannot make spaghetti of single points!')
   return
end

if size(w)==[1,np] then
  w=w'
end
if size(w)==[np,1] then
  w=w*ones(1,nv)
end

if size(w,1)<>np | size(w,2)<>nv then 
   w=w(1)*ones(np,nv);
end

if np==2 then
// "arrows3d" mode: I invent a middle point, so the following
//  assignments work
  x=[x(1,:); (x(1,:)+x(2,:))/2; x(2,:)];
  y=[y(1,:); (y(1,:)+y(2,:))/2; y(2,:)];
  z=[z(1,:); (z(1,:)+z(2,:))/2; z(2,:)];
  np=3
  w=[w(1,:);w]
end

fd=[cos(2*%pi*(1:nf)/nf);sin(2*%pi*(1:nf)/nf)]

xx=zeros(4,nf*np*nv); yy=xx; zz=yy;  // only (np-1)*nv tubelets, but
                                     // the last arrowhead requires
                                     // more facelets


for j=1:nv
//array of versors of the segments
   un=zeros(np-1,3);
   un=[x(2:np,j)-x(1:(np-1),j),y(2:np,j)-y(1:(np-1),j),..
       z(2:np,j)-z(1:(np-1),j)];
   nn=sqrt(sum(un.^2,'c')); 
// take care of possible coincident points: assign an un<>0
// even to those coincident
   isep=find(nn>10*%eps);
   if isep<>[] then   
// if all the points coincide, don't even try to mend anything
// (a division by 0 will result soon)
     un(1:(isep(1)-1),1)=un(isep(1),1);
     un(1:(isep(1)-1),2)=un(isep(1),2);
     un(1:(isep(1)-1),3)=un(isep(1),3);
     nn(1:(isep(1)-1))=nn(isep(1))
//so if the first points were coincident, they take the un of their
//  followers
   else
     write(%io(2),"all the points of line "+string(j)+" coincide!")
     break
   end
// now each nn=0 has for sure a predecessor <>0
   for i=find(nn<=10*%eps); 
     un(i,:)=un(i-1,:); nn(i)=nn(i-1);
   end
// the loop above is not vectorizable! Feedback! 
   nn=[nn,nn,nn];
   un=un./nn;
//compute 2 local normals to each point of the trail
 //intermediate points: first normal=bisectrix of the vertex
   N1=zeros(np,3);
   N1(2:(np-1),:)=un(1:(np-2),:)-un(2:(np-1),:); 
 //extremes: equal that at the next (previous) point
   N1(1,:)=-N1(2,:); N1(np,:)=N1(np-1,:);
//really no idea of why N1(1,:)=-N1(2,:) with minus, but solves a bug.
 //now, we could have N1(:,i)=[0;0;0] either for coincident
 // points or for colinear segments. Let's take care of it.
 // We cannot just set N1 to an arbitrary direction, because
 //  that could be too different from its neighbor.
   nN1=sqrt(sum(N1.^2,'c'));
   if find(abs(nN1)>10*%eps)==[] then
 //if _all_ the points are colinear (this can happen in arrow mode)
      if find(abs(un(:,1))>10*%eps | abs(un(:,2))>10*%eps)==[] then
   //if the whole trail is parallel to the z axis
         N1=[ones(np,1),zeros(np,2)]
      else
         N1(2:np,:)=[un(:,2),-un(:,1),zeros(np-1,1)]       
   //this is for sure perpendicular to un 
         N1(1,:)=N1(2,:); N1(np,:)=N1(np-1,:);
      end
      nN1=sqrt(sum(N1.^2,'c')); // recalculate for the points which
                               // have been taken care of
   else
  //now, if not all points are colinear, lets put the missing N1s
  // (for those which are still colinear). Let's start from the
  // beginning of the trail:
     ibent=find(nN1>10*%eps);
     N1(1:(ibent(1)-1),1)=N1(ibent(1),1);
     N1(1:(ibent(1)-1),2)=N1(ibent(1),2);
     N1(1:(ibent(1)-1),3)=N1(ibent(1),3);
     nN1(1:(ibent(1)-1))=nN1(ibent(1))
  // now each N1=0 has for sure a predecessor <>0
     for i=find(nN1<=10*%eps);
       N1(i,:)=N1(i-1,:); nN1(i)=nN1(i-1);
     end
 // still one thing to do - if the trail is even a little zigzag,
 //  neighboring points can have almost opposite N1. Let's flip
 //  those which have a negative projection on their predecessor
     P1=sign(N1(1:(np-1),1).*N1(2:np,1)+N1(1:(np-1),2).*N1(2:np,2)+..
              N1(1:(np-1),3).*N1(2:np,3))
     iflip=find(cumprod(P1)<0)+1    // I'm really clever!
     N1(iflip,:)=-N1(iflip,:)
   end
//normalization
   N1=N1./[nN1,nN1,nN1];
 //intermediate points: second normal=binormal to the previous
   N2=zeros(np,3);
   N2(2:(np-1),1)=un(1:(np-2),2).*N1(2:(np-1),3)-..
                   un(1:(np-2),3).*N1(2:(np-1),2);
   N2(2:(np-1),2)=un(1:(np-2),3).*N1(2:(np-1),1)-..
                   un(1:(np-2),1).*N1(2:(np-1),3);
   N2(2:(np-1),3)=un(1:(np-2),1).*N1(2:(np-1),2)-..
                   un(1:(np-2),2).*N1(2:(np-1),1);   
 //extremes: equal that at the next (previous) point
   N2(1,:)=N2(2,:); N2(np,:)=N2(np-1,:);
 //normalization
   nN1=sqrt(sum(N2.^2,'c'));
   nN1(find(abs(nN1)<%eps))=1;
   N2=N2./[nN1,nN1,nN1];
//Sometimes the intrinsic twist of the line is such, that it is better
// to further rotate N1 and N2 of an integer number of quarter of turns
// around their commom normal, i.e. send (N1,N2) --> (+-N2,+-N1)
// in order to minimize the angles between corresponding normals of
// neighboring points. This should be done here.

// TO DO

//generate a vector of vertices of the facelet, still to sort
   xt=zeros(np,nf); yt=xt; zt=yt; 
   for k=1:nf  // could I vectorize this?
     xt(:,k)=x(:,j)+(N1(:,1)*fd(1,k)+N2(:,1)*fd(2,k)).*w(:,j)
     yt(:,k)=y(:,j)+(N1(:,2)*fd(1,k)+N2(:,2)*fd(2,k)).*w(:,j)
     zt(:,k)=z(:,j)+(N1(:,3)*fd(1,k)+N2(:,3)*fd(2,k)).*w(:,j)
   end
// generate the faces of the tube
   k1=1:nf; k2=[2:nf,1]; np2=1:(np-2); np2f=(np-2)*nf;
   xx(:,(j-1)*np*nf+(1:(np-2)*nf))=..
      [matrix(xt(np2,k1),1,np2f);matrix(xt(np2+1,k1),1,np2f);..
       matrix(xt(np2+1,k2),1,np2f);matrix(xt(np2,k2),1,np2f)] 
   yy(:,(j-1)*np*nf+(1:(np-2)*nf))=..
      [matrix(yt(np2,k1),1,np2f);matrix(yt(np2+1,k1),1,np2f);..
       matrix(yt(np2+1,k2),1,np2f);matrix(yt(np2,k2),1,np2f)] 
   zz(:,(j-1)*np*nf+(1:(np-2)*nf))=..
      [matrix(zt(np2,k1),1,np2f);matrix(zt(np2+1,k1),1,np2f);..
       matrix(zt(np2+1,k2),1,np2f);matrix(zt(np2,k2),1,np2f)] 
// last segment: arrowhead
   np1=np-1; xc=zeros(1,nf); yc=xc; zc=yc; xp=yc; yp=yc; zp=yc;
   xc(k1)=-x(np1,j)+2*xt(np1,k1)
   yc(k1)=-y(np1,j)+2*yt(np1,k1)
   zc(k1)=-z(np1,j)+2*zt(np1,k1)
   xp(k1)=x(np,j)
   yp(k1)=y(np,j)
   zp(k1)=z(np,j)
   xx(:,(j-1)*np*nf+(((np-2)*nf+1):(np-1)*nf))=..
      [xt(np1,k1);xc(k1);xc(k2);xt(np1,k2)] 
   yy(:,(j-1)*np*nf+(((np-2)*nf+1):(np-1)*nf))=..
      [yt(np1,k1);yc(k1);yc(k2);yt(np1,k2)] 
   zz(:,(j-1)*np*nf+(((np-2)*nf+1):(np-1)*nf))=..
      [zt(np1,k1);zc(k1);zc(k2);zt(np1,k2)] 
   xx(:,(j-1)*np*nf+((np1*nf+1):np*nf))=..
      [xc(k2);xc(k1);xp(k1);xp(k2)] 
   yy(:,(j-1)*np*nf+((np1*nf+1):np*nf))=..
      [yc(k2);yc(k1);yp(k1);yp(k2)] 
   zz(:,(j-1)*np*nf+((np1*nf+1):np*nf))=..
      [zc(k2);zc(k1);zp(k1);zp(k2)] 
//note that only putting equal the 3rd and 4th point, my
// shadesurf works (trick)     
end

endfunction


function nc=setcmap(i,nc,r)
//
// syntax:  setcmap(i,nc,r),  setcmap(i,nc),  setcmap(i)
//
//  i : index of my predefined colormaps
//      the list is printed if i is out of range
//      Negative i gives the complementar colormap
//  nc: number of colors - useful for limited monitors
//      or private colormaps - 32 gives good results
//  r:  the window is redrawn afterwards if r<>0
//      The results are poor if nc has been changed
//
[rhs,lhs]=argn(0);
if lhs==0 then r=0; nc=xget("lastpattern"); i=9999; end
if lhs==1 then r=0; nc=int(xget("lastpattern")/length(i)); end
if lhs==2 then r=0; end
if nc<0 then nc=32; end

i=matrix(i,length(i),1)
//nc=[matrix(nc,length(nc),1);nc($)*ones(length(i)-length(nc),1)]
//would be nice, but as the function is written now, nc has to be a
// scalar

// Here are some colormaps: (crooked way to define them indeed -
//  but it was the first one I figured out long ago -
k=1;
cname(k)='BlackWhite';
     execstr(cname(k)+"=linspace(0,1,nc)''*[1,1,1];") 
k=k+1;
cname(k)='Fluorescine'; 
    execstr(cname(k)+"=[((1:nc).^2)/nc;1:nc;((1:nc).^2)/nc]''/nc;") 
k=k+1;
cname(k)='Caramel'; 
     execstr(cname(k)+"=[(0:nc-1).^0.67*nc^0.33;1:nc;...
    ((1:nc).^3)/nc^2]''/nc;") 
k=k+1;
cname(k)='Cappuccino'; execstr(cname(k)+"=...
   [(0:nc-1).^0.8*nc^0.2;(1:nc).^2/nc;((1:nc).^3)/nc^2]''/nc;") 
k=k+1;
cname(k)='Chocolate'; execstr(cname(k)+"=...
   [(0:nc-1).^0.8*nc^0.2;(1:nc).^2/nc;sin(3*%pi*(1:nc)/nc)/2+1]''/nc;") 
k=k+1;
cname(k)='Hot'; execstr(cname(k)+"=...
  [min(1,max((2*(1:nc)/nc),0));...
   min(1,max((2.5*(1:nc)/nc)-1,0));...
   min(1,max((3*(1:nc)/nc)-2,0))]'';") 
k=k+1;
cname(k)='Hot2';
// this is copied from hotcolormap of scilab 2.4.1
 execstr(cname(k)+"=...
   [(1:fix(3/8*nc))''/fix(3/8*nc)  zeros(fix(3/8*nc),1) ...
                                         zeros(fix(3/8*nc),1); ...
    ones(fix(3/8*nc),1)  (1:fix(3/8*nc))''/fix(3/8*nc)  ...
                                         zeros(fix(3/8*nc),1); ...
    ones((nc-2*fix(3/8*nc)),1)  ones((nc-2*fix(3/8*nc)),1)  ...
           (1:(nc-2*fix(3/8*nc)))''/(nc-2*fix(3/8*nc))];")
k=k+1;
cname(k)='BluRed'; execstr(cname(k)+"=[1:nc;0*(1:nc);nc:-1:1]''/nc") 
k=k+1;
cname(k)='LightBlueRed';execstr(cname(k)+"=[1:nc;nc:-1:1;nc:-1:1]''/nc;") 
k=k+1;
cname(k)='Sunrise'; execstr(cname(k)+..    
    "=[[zeros(1,nc-floor(nc/4)-floor(nc/2)),...
      linspace(1,nc,floor(nc/4))./nc,ones(1,floor(nc/2))]'',..
       [linspace(0,1,ceil(nc/2)),linspace(1,0,floor(nc/2))]'',..
       [ones(1,floor(nc/2)),linspace(nc,1,floor(nc/4))./nc,...
         zeros(1,nc-floor(nc/2)-floor(nc/4))]''];;")
k=k+1;
cname(k)='BluBlackRed'; 
  execstr(cname(k)+"=[zeros(1,ceil(nc/2)),linspace(1,nc,floor(nc/2));...
  zeros(1,nc);linspace(nc,1,floor(nc/2)),zeros(1,ceil(nc/2))]''/nc;") 
k=k+1;
cname(k)='BluBlackRed_II';execstr(cname(k)+"=sqrt(BluBlackRed);")
k=k+1;
cname(k)='BluGreenRed'; 
  execstr(cname(k)+"=[zeros(1,ceil(nc/2)),linspace(1,nc,floor(nc/2));..
        zeros(1,floor(nc/4)),linspace(1,nc,floor(nc/4)),...
        linspace(nc,0,nc-3*floor(nc/4)),zeros(1,floor(nc/4));...
        linspace(nc,1,floor(nc/2)),zeros(1,ceil(nc/2))]''/nc;") 
k=k+1;
cname(k)='BluGreenRed_II'; 
  
execstr(cname(k)+"=[zeros(1,floor(nc/2)),..
                    linspace(1,nc,ceil(nc/2));...
      linspace(0,nc,ceil(nc/2)),linspace(nc,0,floor(nc/2));...
      linspace(nc,0,ceil(nc/2)),zeros(1,floor(nc/2))]''/nc;") 
k=k+1;
cname(k)='BluGreenRed_III'; execstr(cname(k)+"=[1:nc;...
        linspace(0,nc,floor(nc/2)),linspace(nc,0,ceil(nc/2))...
        ;nc:-1:1]''/nc;") 
k=k+1;
cname(k)='Prism'; execstr(cname(k)+"=...
  [ sin((linspace(2,2*nc,nc)+2.*nc/3)*%pi/nc);...
    sin((linspace(2,2*nc,nc)-2.*nc/3)*%pi/nc);...
    sin((linspace(2,2*nc,nc))*%pi/nc)]''/2+1./2;") 
k=k+1;
cname(k)='Prism_II'; execstr(cname(k)+"=Prism.^(1/2)'';")
k=k+1;
cname(k)='Bands'; execstr(cname(k)+"=...
   [1:nc;nc:-1:1;nc*((-sin(15*%pi*(1:nc)/nc)+1)/2)]''/nc;") 
k=k+1;
cname(k)='BlackBands'; execstr(cname(k)+"=...
   ([sin((linspace(2,2*nc,nc)+2.*nc/3)*%pi/nc);...
     sin((linspace(2,2*nc,nc)-2.*nc/3)*%pi/nc);...
     sin((linspace(2,2*nc,nc))*%pi/nc)]''/2+1./2)...
    .*modulo([1:nc;1:nc;1:nc]''-1,nc/8)*7/nc;") 

if lhs==0 then
// demo - interactive choice of colormaps
  currwin=xget('window')
  a=winsid(); xset('window',a($)+1); // note that if a=[], a+1=1
  for j=-k:k;
    setcmap(j,nc);
    subwind(j+k+1,2*k+1,1);
    xstring(0,-0.06,string(j))
    pixmapl(1:nc)
  end
  subwind(1,1,1)
  xtitle('All the colormaps at the current color depth ('+string(nc)+..
         ' colors)')
    disp " "
    i=x_choose(string((-k:k)')+' '+..
     [cname(k:-1:1);'Scilab''s default';cname],..
             'Choose one colormap!','Forget!')-k-1
    xdel(a($)+1); xset('window',currwin); setcmap(i,nc,1)
    return
end


if i==0 then xset("default"); end
if abs(i)>k then 
  write(%io(2)," ")
  write(%io(2),"0 default graphic context")
  for j=1:k
   write(%io(2),string(j)+" "+cname(j));
  end
end  
if abs(i)>=1 & abs(i)<=k then 
  write(%io(2),string(i)+" "+cname(abs(i)))
  ccol=[];
  for j=1:length(i)
//    nc=ncv(j);    //for the future version
    execstr("ccol=[ccol;(sign(-i(j))+1)/2-sign(-i(j))*"+..
             cname(abs(i(j)))+"]"); 
  end
  xset('colormap',ccol)
  if r<>0 then write(%io(2),'redrawing...'); xbasr(xget("window")); end
end

nc=xget("lastpattern")
// reset and returned, just for consistency check

endfunction

