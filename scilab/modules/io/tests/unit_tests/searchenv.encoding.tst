// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Antoine Elias - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


ilib_verbose(0);

mydir = TMPDIR + filesep() + "loadlib";
createdir(mydir);
chdir(mydir);

if MSDOS then
	unix_w(jre_path()+"\bin\java.exe -cp "+SCI+"\modules\localization\tests\unit_tests CreateDir");
else
	unix_w(jre_path()+"/bin/java -classpath "+SCI+"/modules/localization/tests/unit_tests CreateDir");
end

fileC = ['void test(){}'];



tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"프로그램",
"프로그램",
"תוכנית"];

PathOrigin = getenv("PATH");

if MSDOS then

for i = 1 : size(tab_ref,'*')
	sz = "dir_" + tab_ref(i);
	a = chdir(sz);
	if(a <> %T) then pause,	end
	szTemp = getshortpathname(mydir + filesep() + sz);
	NewPath = PathOrigin + ";" + szTemp;
	setenv("PATH", NewPath);
	mputl(fileC,'test.c');
	ilib_for_link('test','test.c',[],"c");
	chdir(SCI);
	commandstr = "link(" + """libtest" + getdynlibext() +""")";
	ierr = execstr(commandstr,"errcatch");
	if(ierr <> 0) then pause, end
	ulink();
	setenv("PATH", PathOrigin);
	chdir(mydir);
end

end
sleep(2000);
chdir(TMPDIR);
removedir(mydir);
