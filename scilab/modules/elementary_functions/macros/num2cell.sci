function  y=num2cell(x,dimens)
// Copyright INRIA
//
// NUM2CELL function converts an array of double or string or boolean into a cell array
// if there is just one input argument x, then it returns a cell which has the same size and the same components than x.
// if there are two input arguments x and dimens, y is obtained after reduction to 1 of the x dimensions sizes contained in the vector dimens. 
// The dimensions sizes of components of y (i.e y(i).entries) are equal to the dimensions sizes of x whose the numbers are in dimens vector
// if size(x)=[2 3 4 5 6] and dimens=[2 4] -> size(y)=[2 1 4 1 6] , the sizes of the second (=dimens(1)) and the fourth (=dimens(2)) dimensions of x are reduced to 1 to obtain y. And size(y(i))=[1,3,1,5,1], the sizes of the second (=dimens(1)) and the fourth (=dimens(2)) dimensions of y(i) are equal to the sizes of the second and the fourth dimensions of x (i.e 3 and 5), the others dimensions are equal to 1  
// 
// Output 
// -y : a cell
// Input
// -x : a numeric array
// -dimens : a scalar or a vector of positive integers. It contains the numbers of dimensions of x to reduce   
// F.B

// case : x is empty, returns an empty cell
if isempty(x) then
  y=cell()
  return
end
xsize=size(x)
// check the number of input arguments
if argn(2)<1 then
  error("wrong number of input arguments")
elseif argn(2)==1 then
    for i=1:size(x,"*")
      y(i).entries=x(i)
    end 
    y=matrix(y,xsize)
  return
// check the second input argument is a scalar (or a vector) of positive integers
else
  if type(dimens)<>1 | or(dimens<=0) | or(dimens-floor(dimens)<>0) | ndims(dimens)>2 then
    error("second argument must be a positive integer")
  end
  dimens=matrix(dimens,1,-1)
end


// xdimens : vector which contains the sizes of x dimensions which are in the vectordimens
// nxdimens : vector which contains the sizes of x dimensions which are not in the vector dimens
inddims=find(dimens<=ndims(x))
xdimens=dimens(inddims)
prodxdimens=prod(xsize(xdimens))
// ydims : dimension of y cell (it's the new size of x after reduction to one of the dimensions contained in 'dimens' vector)
// ycpdims : dimension of y components (equal to dimensions of x contained in 'dimens' vector)
ycpdims=ones(1:ndims(x))
ydims=ones(1:ndims(x))
ycpdims(xdimens)=xsize(xdimens)
nxdimens=1:ndims(x)
nxdimens(xdimens)=[]
ydims(nxdimens)=xsize(nxdimens)
y=cell()
xdimens=[xdimens,nxdimens]
x=permute(x,xdimens)

// delete the value of 'ycpdims' vector equal to one which are in last positions
// if ycpdims=[2 3 1 4 1 1]  -> ycpdims=[2 3 1 4]  
i=prod(size(ycpdims))
while i>2 & ycpdims(i)==1
  ycpdims(i)=[]
  i=i-1
end
// delete the value of 'ydims' vector equal to one which are in last positions
// if ydims=[2 3 1 4 1 2 1 1]  -> ydims=[2 3 1 4 1 2]
i=prod(size(ydims))
while i>2 & ydims(i)==1
  ydims(i)=[]
  i=i-1
end

// fill the cell y 
for i=1:prod(ydims) 
  y(i).entries=matrix(x(1+(i-1)*prodxdimens:prodxdimens*i),ycpdims)
end
y=matrix(y,ydims)

endfunction
