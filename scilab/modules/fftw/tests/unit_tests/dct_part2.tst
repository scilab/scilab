// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//============================================================================================
//=================================dct(A ,isn, sel) ============================================
//==============================================================================================
function x=DCT(y,flag,sel)
  x=y;
  Dims=size(y)
  ind=1:ndims(y)
  ind(sel)=[];
  I=list();
  for k=1:ndims(y),I(k)=:;end
  select size(ind,'*')
  case 1 then
    for i1=1:Dims(ind)
      I(ind)=i1;
      x(I(:))=dct(y(I(:)));
    end
  case 2 then
    for i1=1:Dims(ind(1))
      I(ind(1))=i1;
      for i2=1:Dims(ind(2))
        I(ind(2))=i2;
        x(I(:))=dct(y(I(:)));
      end
    end
  case 3 then
    for i1=1:Dims(ind(1))
      I(ind(1))=i1;
      for i2=1:Dims(ind(2))
        I(ind(2))=i2;
        for i3=1:Dims(ind(3))
          I(ind(3))=i3;
          x(I(:))=dct(y(I(:)));
        end
      end
    end

  else
    error('not handled')
  end
endfunction

//*****multiple 1-D transform ******************************************************************
//Real case
x=matrix(1:12,3,4);
y_ref=x;for i2=1:4,y_ref(:,i2)=dct(x(:,i2));end
y=dct(x,-1,1);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,1),x,0,100*%eps);

y_ref=x;for i1=1:3,y_ref(i1,:)=dct(x(i1,:));end
y=dct(x,-1,2);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,2),x,0,100*%eps);

x=matrix([7,1,8,3,5,6,2,8,6,3,10,9,3,1,9,7,9,4,6,7,7,4,1,5],[3,4,2]);
y_ref=x;for i1=1:3,for i3=1:2,y_ref(i1,:,i3)=dct(x(i1,:,i3));end;end
y=dct(x,-1,2);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,2),x,0,100*%eps);

x=matrix([7,1,8,3,5,6,2,8,6,3,10,9,3,1,9,7,9,4,6,7,7,4,1,5],[3,1,4,2]);
y_ref=x;for i1=1:3,for i4=1:2,y_ref(i1,1,:,i4)=dct(x(i1,1,:,i4));end;end
y=dct(x,-1,3);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,3),x,0,100*%eps);

x=matrix([7,1,8,3,5,6,2,8,6,3,10,9,3,1,9,7,9,4,6,7,7,4,1,5],[3,1,4,2]);
y_ref=x;for i1=1:3,for i3=1:4,y_ref(i1,1,i3,:)=dct(x(i1,1,i3,:));end;end
y=dct(x,-1,4);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,4),x,0,100*%eps);

//Complex case
x=matrix( [ 9+%i*9, 8+%i*4, 4+%i*6, 8+%i*8, 1+%i*8, 1+%i*6, 5+%i*4, 4+%i*7, ...
            6+%i*5, 9+%i*6, 1+%i*10, 4+%i*2, 9+%i*5,%i*2, 9+%i*6, 9+%i*4, 9, ...
            8+%i*6,%i*7,%i*9, 5+%i, 4+%i*3, 8,  5+%i*6],[3,1,4,2]);
y_ref=x;for i1=1:3,for i3=1:4,y_ref(i1,1,i3,:)=dct(x(i1,1,i3,:));end;end
y=dct(x,-1,4);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checkalmostequal(dct(y,1,4),x,0,100*%eps);

//*****multiple 2-D transform ******************************************************************
//Real case
x=matrix([7,1,8,3,5,6,2,8,6,3,10,9,3,1,9,7,9,4,6,7,7,4,1,5],[3,4,2]);
sel=[2 3];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

x=matrix([7,1,8,3,5,6,2,8,6,3,10,9,3,1,9,7,9,4,6,7,7,4,1,5],[3,1,4,2]);
sel=[1 3];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

x=matrix([7,1,8,3,5,6,2,8,6,3,10,9,3,1,9,7,9,4,6,7,7,4,1,5],[1,3,4,2]);
sel=[2 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

x=matrix([6,4,5,5,1,9,7,1,1,5,8,1,7,2,2,4,4,2,3,1,4,7,1,6,4,...
          2,2,6,9,7,3,2,2,7,6,9,6,8,1,7,6,8,8,1,2,3,5,7],[3,2,1,4,2]);
sel=[2 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

//Complex case
x=matrix([6+%i,4,5,5,1,9,7,1,1,5,8,1,7,2,2,4,4,2,3,1,4,7,1,6,4,...
          2,2,6,9,7,3,2,2,7,6,9,6,8,1,7,6,8,8-%i,1,2,3,5,7],[3,2,1,4,2]);
sel=[2 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

//*****multiple N-D transform ******************************************************************
//Real case


x=matrix([2,8,1,3,7,6,8,7,9,1,6,7,7,2,5,2,2,2,9,7,3,9,2,3,...
          4,3,6,5,3,6,5,4,3,6,4,9,11,5,3,4,3,1,8,2,1,7,2,7,...
          8,4,4,9,1,2,6,6,7,9,5,3,4,9,9,3,4,7,3,5,3,5,5,1,...
          2,6,8,5,7,2,4,8,6,5,2,8,1,3,9,8,5,10,6,10,1,7,4,6],[3,4,2 4]);
sel=[1 3 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

sel=[1 2 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);


x=matrix([5,8,8,4,2,2,2,9,0,2,6,7,6,0,2,9,1,3,4,6,4,3,1,8,0,4,7,2,9,7,2,3,9,1,5,7,..
          8,6,4,7,1,2,9,3,7,0,2,10,9,1,9,5,9,9,2,0,0,7,5,1,5,8,4,4,8,0,10,10,7,10,...
          8,5,2,9,0,7,2,8,4,3,6,8,9,6,6,9,6,5,7,9,7,5,9,6,4,7,3,6,0,9,10,6,6,3,10,...
          8,9,5,1,7,9,0,8,6,1,8,5,2,8,8,7,4,3,6,0,9,3,2,5,1,10,7,8,5,7,5,7,1,7,3,9,...
          8,5,4,9,0,5,2,1,10,5,2,1,9,9,6,7,2,9,8,3,2,6,2,5,6,4,1,5,1,3,9,2,2,1,7,10,...
          9,7,8,8,10,2,5,7,4,5,8,3,9,2,9,9,10,1,8,9,7,6,8,1,5,0,10,8,6,8,4,8,4,6,9,3,...
          9,4,2,4,3,5,4,3,3,0,5,7,7,3,6,7,4,2,9,9,4,4,6,7,9,3,10,1,1,7,8,8,3,5,4,3,3,...
          9,10,1,6,2,0,6,0,3,1,2,2,4,1,3,1,8,6,3,4,5,1,10,7,9,10,3,6,2,6,6,5,2,1,8,6,...
          6,7,1,1,8,6,4,3,9,10,1,4,7,2,10,7,1,4,5,5,5,8,3,9,7,2,1,3,1,3,4,2,0,4,5,1,10,...
          7,1,9,8,5,9,5,3,6,4,9,0,8,3,1,6,9,4,3,4,8,2,5,1,5,3,0,2,3,1,10,3,1,9,9,8,2],[3 2 3 4 5]);
sel=[2 3 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

x=matrix([5,8,8,4,2,2,2,9,0,2,6,7,6,0,2,9,1,3,4,6,4,3,1,8,0,4,7,2,9,7,2,3,9,1,5,7,..
          8,6,4,7,1,2,9,3,7,0,2,10,9,1,9,5,9,9,2,0,0,7,5,1,5,8,4,4,8,0,10,10,7,10,...
          8,5,2,9,0,7,2,8,4,3,6,8,9,6,6,9,6,5,7,9,7,5,9,6,4,7,3,6,0,9,10,6,6,3,10,...
          8,9,5,1,7,9,0,8,6,1,8,5,2,8,8,7,4,3,6,0,9,3,2,5,1,10,7,8,5,7,5,7,1,7,3,9,...
          8,5,4,9,0,5,2,1,10,5,2,1,9,9,6,7,2,9,8,3,2,6,2,5,6,4,1,5,1,3,9,2,2,1,7,10,...
          9,7,8,8,10,2,5,7,4,5,8,3,9,2,9,9,10,1,8,9,7,6,8,1,5,0,10,8,6,8,4,8,4,6,9,3,...
          9,4,2,4,3,5,4,3,3,0,5,7,7,3,6,7,4,2,9,9,4,4,6,7,9,3,10,1,1,7,8,8,3,5,4,3,3,...
          9,10,1,6,2,0,6,0,3,1,2,2,4,1,3,1,8,6,3,4,5,1,10,7,9,10,3,6,2,6,6,5,2,1,8,6,...
          6,7,1,1,8,6,4,3,9,10,1,4,7,2,10,7,1,4,5,5,5,8,3,9,7,2,1,3,1,3,4,2,0,4,5,1,10,...
          7,1,9,8,5,9,5,3,6,4,9,0,8,3,1,6,9,4,3,4,8,2,5,1,5,3,0,2,3,1,10,3,1,9,9,8,2],[3 2 1 3 4 5]);

sel=[2 4 5];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);

x=matrix([9,8,8,8,6,5,5,9,8,1,1,9,4,10,8,6,0,7,2,5,6,8,2,1,7,1,1,6,8,6,4,3],[2 2 2 2 2]);
sel=[2 4];
y_ref=DCT(x,-1,sel);
y=dct(x,-1,sel);
assert_checkalmostequal(y,y_ref,0,100*%eps);
assert_checktrue(isreal(y));
assert_checkalmostequal(dct(y,1,sel),x,0,100*%eps);
