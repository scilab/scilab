//===============================
// Tests fileparts
// INRIA 2008
//===============================
[path,fname,extension] = fileparts(SCI+'/etc/scilab.start');
if (path <> pathconvert(SCI+'/etc/',%f,%f)) then pause,end
if (fname <> 'scilab') then pause,end
if (extension <> '.start') then pause,end
//===============================
r = fileparts(SCI+'/etc/scilab.start','path');
if (r <> pathconvert(SCI+'/etc/',%f,%f)) then pause,end
//===============================
r = fileparts(SCI+'/etc/scilab.start','fname');
if (r <> 'scilab') then pause,end
//===============================
r = fileparts(SCI+'/etc/scilab.start','extension');
if (r <> '.start') then pause,end
//===============================
