// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->


filename = TMPDIR+'/filetodelete.txt';

// Regular use
fd=mopen ( filename , 'w' );
mclose(fd); 
computed = deletefile ( filename );
if computed <> %t then pause,end

// Wrong use then correct use
fd=mopen ( filename , 'w' );
computed = deletefile ( filename );
if computed <> %f then pause,end
mclose(fd); 
computed = deletefile ( filename );
if computed <> %t then pause,end

