% Test file for function error()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

a1=0;
a2=0;
error('message')
error('message',a1,a2)
error('message_id','message')
error('message_id','message',a1,a2)

