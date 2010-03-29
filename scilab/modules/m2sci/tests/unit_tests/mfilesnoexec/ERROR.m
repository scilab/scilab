% Test file for function error()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% F.B.

error('Hello')
error('The matrix must be square, the size is %dx%d',2,3)
error('MyToolbox:SquareMatrix','Matrix must be square')
error('MyToolbox:SquareMatrix','The matrix must be square, the size is %dx%d',5,7)

