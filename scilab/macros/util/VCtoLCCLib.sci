// Allan CORNET INRIA 2004
// Export LibScilab for LCC compiler
function VCtoLCCLib()
if with_lcc()==%t then
	if MSDOS==%t then
		printf('\nPlease waiting\n');
		DirCur=pwd();
		chdir(WSCI+'\bin');
		printf('Exports from LibScilab.dll\n');
		unix('pedump /exp LibScilab.dll >'+TMPDIR+'\LibScilab.lcc');
		printf('Converting Library');
		ExportVCLibScilab();
		printf('Build LibScilablcc.lib\n');
		unix('buildLib '+TMPDIR+'\LibScilablcc.exp'+' '+WSCI+'\bin\'+'LibScilablcc.lib');
		if ( with_atlas() == %T ) then
			printf('Exports from atlas.dll\n')
			unix('pedump /exp atlas.dll >'+TMPDIR+'\atlas.lcc');
			printf('Converting Library');
			ExportVCAtlas();
			printf('Build Atlaslcc.lib\n');
			unix('buildLib '+TMPDIR+'\Atlaslcc.exp'+' '+WSCI+'\bin\'+'Atlaslcc.lib');
		end
	
		chdir(DirCur);
	end
end
endfunction

function ExportVCLibScilab()
	fw=mopen(TMPDIR+'\LibScilablcc.exp',"w");
	fr=mopen(TMPDIR+'\LibScilab.lcc',"r");

	if (meof(fr) == 0) then 
		line=mfscanf(1,fr,"%s");
		mfprintf(fw,"%s\n",line);
		printf('.');
	end
	i=0;	
	while ( meof(fr) == 0)
		line=mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s\n",line);
			if (i>55) then printf('.');i=0;end;
			i=i+1;
			end
	end

	mclose(fw);
	mclose(fr);
	
	printf('\n');
endfunction

function ExportVCAtlas()
	printf("\n Warning: Atlas dont work with LCC.\n");
	
	fw=mopen(TMPDIR+'\atlaslcc.exp',"w");
	fr=mopen(TMPDIR+'\atlas.lcc',"r");

	if (meof(fr) == 0) then 
		line=mfscanf(1,fr,"%s");
		mfprintf(fw,"atlas.dll\n");
		printf('.');
	end
	
		
	i=1;	
	while ( meof(fr) == 0)
		line=mfscanf(1,fr,"%s");
		if (line ~= []) then
			mfprintf(fw,"_%s_",line);
			i=i+1;
			printf('.');
			end
	end

	mclose(fw);
	mclose(fr);
	
	printf('\n');
endfunction