% Test file for function warning()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

s = warning;
[s,f] = warning;
warning('on');
s = warning('backtrace');
warning('message');
s = warning('message');
warning('message_id:message_id','message');
warning('message_id:message_id','message',1,2);
