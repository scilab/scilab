% Test file for function close()
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

%m2scideclare %graphicswindow|Unknown Unknown|Double
close
%m2scideclare h|1 1|Double
close(h)
%m2scideclare name|1 Unknown|String
close name
close all
close all hidden

status=close
status=close(h)
status=close(name)
status=close('all')
status=close('all','hidden')

%m2scideclare %graphicswindow|Unknown Unknown|Handle
close
%m2scideclare h1|1 1|Handle
close(h1)
%m2scideclare name|1 Unknown|String
close name
close all
close all hidden

status=close
status=close(h1)
status=close(name)
status=close('all')
status=close('all','hidden')
