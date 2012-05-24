// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->


filename = pathconvert(TMPDIR+"/filetodelete_1.txt",%F);

// First test-case : Regular use
fd=mopen ( filename , "w" );
mclose(fd); 
computed = deletefile ( filename );
if computed <> %t then pause,end

// Second test-case : Wrong use then correct use
if getos() == 'Windows' then
	fd=mopen ( filename , "w" );
	computed = deletefile ( filename );
	if computed <> %f then pause,end
	mclose(fd); 
	computed = deletefile ( filename );
	if computed <> %t then pause,end
end

// Third test-case : try to delete a non-existing file
filename = pathconvert(TMPDIR+"/filetodelete_2.txt",%F);
computed = deletefile ( filename );
if computed <> %f then pause,end
