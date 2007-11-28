// =============================================================================
// Auhtor : Pierre MARECHAL
//          Scilab Team
// Copyright INRIA 2007
//
// Date   : 28/11/2007
//
// Determines if the path is an absolute name.
// =============================================================================

if is_absolute_path("test_1")                then pause,end
if is_absolute_path("test_2       ")         then pause,end
if is_absolute_path("	test_3		")       then pause,end

if MSDOS then
	
	if is_absolute_path("/")                 then pause,end
	if is_absolute_path("3:")                then pause,end
	if is_absolute_path("c :")               then pause,end
	
	if ~is_absolute_path("a:")               then pause,end
	if ~is_absolute_path("A:")               then pause,end
	if ~is_absolute_path("z:")               then pause,end
	if ~is_absolute_path("Z:")               then pause,end
	if ~is_absolute_path("  a:")             then pause,end
	if ~is_absolute_path("	a:")             then pause,end
	
else
	
	if is_absolute_path("c:\")               then pause,end
	if is_absolute_path("C:\")               then pause,end
	if is_absolute_path("file:\")            then pause,end
	if is_absolute_path("\test1")            then pause,end
	
	if ~is_absolute_path("/test_11")         then pause,end
	if ~is_absolute_path(" /test_12")        then pause,end
	if ~is_absolute_path("	/test_13")       then pause,end
	if ~is_absolute_path("~/test_14")        then pause,end
	if ~is_absolute_path("       ~/test_15") then pause,end
	if ~is_absolute_path("	~/test_16")      then pause,end

end
