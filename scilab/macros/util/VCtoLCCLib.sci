// Allan CORNET INRIA 2004
// Export LibScilab for LCC compiler
function VCtoLCCLib()
if with_lcc()==%t then
	if MSDOS==%t then
	DirCur=pwd();
	chdir(WSCI+'\bin');
	unix('pedump /exp LibScilab.dll >'+TMPDIR+'\LibScilab.lcc');
	EditExportVC();
	unix('buildLib '+TMPDIR+'\LibScilablcc.exp'+' '+WSCI+'\bin\'+'LibScilablcc.lib');
	chdir(DirCur);
	end
end
endfunction

function EditExportVC()
	fw=mopen(TMPDIR+'\LibScilablcc.exp',"w");
	fr=mopen(TMPDIR+'\LibScilab.lcc',"r");

	if (meof(fr) == 0) then 
		line=mfscanf(1,fr,"%s");
		mfprintf(fw,"%s\n",line);
	end
		
	while ( meof(fr) == 0)
		line=mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s\n",line);
			end
	end

	mclose(fw);
	mclose(fr);
	
endfunction