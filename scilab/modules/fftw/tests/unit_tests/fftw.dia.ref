//=================================
//simple vector direct transform
a = rand(50,1)+%i*rand(50,1);
y = fftw(a);
y = fftw(a,-1);
//inverse transform
b = fftw(y,1);
//=================================
//2D transform
a = rand(512,512)+%i*rand(512,512);
y = fftw(a);
//=================================
//M-D transform -old calling sequence-
a = rand(120,1);
y = a;
dim=[5 6 4];incr=[1 5 30];
for i=1:3
 y = fftw(y,-1,dim(i),incr(i));
end
//=================================
//M-D transform -new calling sequence-
//More efficient than old
y = fftw(a,-1,[5 6 4],[1 5 30]);
b = fftw(y,1,[5 6 4],[1 5 30]);
//=================================
