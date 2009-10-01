// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM MANDATORY -->

// Regular use
fd=mopen ( 'filetodelete.txt' , 'w' );
mclose(fd); 
computed = deletefile ( 'filetodelete.txt' );
if computed <> %t then pause,end

// Wrong use then correct use
fd=mopen ( 'filetodelete.txt' , 'w' );
computed = deletefile ( 'filetodelete.txt' );
if computed <> %f then pause,end
mclose(fd); 
computed = deletefile ( 'filetodelete.txt' );
if computed <> %t then pause,end

