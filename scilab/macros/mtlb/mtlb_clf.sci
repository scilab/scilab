function mtlb_clf()
global ('%MTLBHOLD')
if %MTLBHOLD==[] then %MTLBHOLD=%f,end
if ~%MTLBHOLD then xbasc(),end
