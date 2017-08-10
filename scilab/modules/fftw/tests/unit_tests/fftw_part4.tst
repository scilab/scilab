// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//R2R case  ------------------------------------------------------------------------------------
A=[0   1 2 2 1
   2  -1 3 3 -1
   3   4 5 5  4
   2  -1 3 3 -1];

Dims=[4 5];
Sel=1;
y=fft(A,-1,Sel);
assert_checktrue(isreal(y));
for j=1:Dims(2)
  ind=list(:,j);
  assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
end
y=fft(y,1,Sel);
assert_checktrue(isreal(y));
for j=1:Dims(2)
  ind=list(:,j);
  assert_checkalmostequal(y(ind(:)),A(ind(:)),0,10*%eps);
end

Sel=2;
y=fft(A,-1,Sel);
assert_checktrue(isreal(y));
for i=1:Dims(1)
  ind=list(i,:);
  assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
end
y=fft(y,1,Sel);
assert_checktrue(isreal(y));
for i=1:Dims(1)
  ind=list(i,:);
  assert_checkalmostequal(y(ind(:)),A(ind(:)),0,10*%eps);
end

//ND case
A=[0   1   1
   2  -1  -1
   3   4   4
   2  -1  -1];
A(:,:,2)=A+1;
A(:,:,3)=A(:,:,2);

Dims=size(A);
Sel=1;
y=fft(A,-1,Sel);
assert_checktrue(isreal(y));
for j=1:Dims(2)
  for k=1:Dims(3)
    ind=list(:,j,k);
    assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
  end
end

y=fft(y,1,Sel);
assert_checktrue(isreal(y));
for j=1:Dims(2)
  for k=1:Dims(3)
    ind=list(:,j,k);
    assert_checkalmostequal(y(ind(:)),A(ind(:)),0,10*%eps);
  end
end

Sel=1:2;
y=fft(A,-1,Sel);
assert_checktrue(isreal(y));
for k=1:Dims(3)
  ind=list(:,:,k);
  assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
end

y=fft(y,1,Sel);
assert_checktrue(isreal(y));
for k=1:Dims(3)
  ind=list(:,:,k);
  assert_checkalmostequal(y(ind(:)),A(ind(:)),0,10*%eps);
end

Sel=2:3;
y=fft(A,-1,Sel);
assert_checktrue(isreal(y));
for i=1:Dims(1)
  ind=list(i,:,:);
  assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
end
y=fft(y,1,Sel);
assert_checktrue(isreal(y));
for i=1:Dims(1)
  ind=list(i,:,:);
  assert_checkalmostequal(y(ind(:)),A(ind(:)),0,10*%eps);
end

Dims=[5 4 9 5 6];
A=matrix(rand(1,prod(Dims)),Dims);
y=fft(A,-1,[2 4]);
for i1=1:Dims(1) 
  for i3=1:Dims(3)
    for i5=1:Dims(5)
      ind=list(i1,:,i3,:,i5);;
      assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
    end
  end
end
y1=fft(y,1,[2 4]);
assert_checktrue(isreal(y1));
assert_checkalmostequal(y1,A);


y=fft(A,-1,[2 4 5]);
for i1=1:Dims(1) 
  for i3=1:Dims(3)
    ind=list(i1,:,i3,:,:);
    assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
  end
end
y1=fft(y,1,[2 4 5]);
assert_checktrue(isreal(y1));
assert_checkalmostequal(y1,A);


Dims=[5 4 7 5 6 3];
A=matrix(rand(1,prod(Dims)),Dims);
y=fft(A,-1,[2 5]);
for i1=1:Dims(1) 
  for i3=1:Dims(3)
    for i4=1:Dims(4)
      for i6=1:Dims(6)
        ind=list(i1,:,i3,i4,:,i6);
        assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
      end
    end
  end
end

y1=fft(y,1,[2 5]);
assert_checktrue(isreal(y1));
assert_checkalmostequal(y1,A);


y=fft(A,-1,[2 4 6]);
for i1=1:Dims(1) 
  for i3=1:Dims(3)
    for i5=1:Dims(5)
      ind=list(i1,:,i3,:,i5,:);
      assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
    end
  end
end
y1=fft(y,1,[2 4 6]);
assert_checktrue(isreal(y1));
assert_checkalmostequal(y1,A);

y=fft(A,-1,[2 4 5 6]);
for i1=1:Dims(1) 
  for i3=1:Dims(3)
    ind=list(i1,:,i3,:,:,:);
    assert_checkalmostequal(y(ind(:)),fft(A(ind(:)),-1));
  end
end
y1=fft(y,1,[2 4 5 6]);
assert_checktrue(isreal(y1));
assert_checkalmostequal(y1,A);
