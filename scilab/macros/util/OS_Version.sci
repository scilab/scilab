function Version=OS_Version()
// return OS version
// Copyright INRIA
// Allan CORNET 2004
Version="";
if MSDOS then
	filever=TMPDIR+'\version.dos';
	cmd='ver >'+TMPDIR+'\version.dos';
	unix(cmd);
	msg=read(TMPDIR+'\version.dos',-1,1,'(a)')
	Version=tokens(msg(2),'[');
	Version=Version(1);
	Version=part(Version,1:length(Version)-1);
	host('del '+filever);
else
	Version="Not MS-DOS";
end

endfunction