% Test file for function waitforbuttonpress()
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

w=waitforbuttonpress

if w==0
  disp('Button press')
else
    disp('Key press')
end