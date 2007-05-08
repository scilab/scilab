// Allan CORNET 2005
//---------------------------------------------------------------
// Tests Interface Scilab Java
//---------------------------------------------------------------
tests = ['Boolean.java';'BooleanArray.java';'Clf.java';'Disp.java';'Exec.java';'Real1.java';'Real2.java';'Real3.java';'String1.java';'Strings1.java';'Strings2.java'];
//---------------------------------------------------------------
// Compilation du fichier Java
function ret=BuildJava(filename)
	if MSDOS then
		Command='javac -deprecation -d '+SCI+'\bin -classpath '+SCI+'\bin '+filename;
	else
		Command='javac -deprecation -classpath ../javasci.jar '+filename;
	end
	[rep,stat]=unix_g(Command);
	if ~(stat==0) then
		disp('Error in the compilation '+filename);
		disp('Command was : '+Command);
		ret=%F;
	else
		ret=%T;
	end
endfunction
//---------------------------------------------------------------
// Execution du fichier Java
function ExecJava(filename,buildref)
	currentdir=pwd();
	repfilename='';
	if ~MSDOS then
		setenv('LD_LIBRARY_PATH','$LD_LIBRARY_PATH:'+pwd());
		setenv('CLASSPATH','$CLASSPATH:'+pwd()+':.');
		setenv('SHLIB_PATH','$SHLIB_PATH:'+pwd()+':.');
	else 
		chdir(SCI+'\bin');
	end
	[path,fname,extension]=fileparts(filename);
	if (buildref == %T) then
		if MSDOS then
			repfilename=currentdir+'\'+fname+'.dia.ref';
		else
			repfilename=currentdir+'/'+fname+'.dia.ref';
		end
	else
		if MSDOS then
			repfilename=currentdir+'\'+fname+'.dia';
		else
			repfilename=currentdir+'/'+fname+'.dia';
		end
	end

	if MSDOS then
	  commandline=SCI+'\java\jre\bin\java '+fname +' > '+repfilename;
	else
	  commandline='java -classpath ../javasci.jar:. -Djava.library.path=../.libs/ '+fname +' > '+repfilename;
	end
	unix(commandline);
	chdir(currentdir);
endfunction
//---------------------------------------------------------------
// Nettoyage apres tests du fichier .class généré
function CleanClass(filename)
	currentdir=pwd();
	chdir(SCI+'\bin');
	[path,fname,extension]=fileparts(filename);
	if MSDOS then
		unix_s('del '+fname+'.class');
	else
		unix_s('rm -f '+fname+'.class');
	end
	chdir(currentdir);
endfunction
//---------------------------------------------------------------
// Comparaison avec la reference
function CompareDiary(filename)
	[path,fname,extension]=fileparts(filename);
	fpdia=mopen(fname+'.dia','r');
	fpref=mopen(fname+'.dia.ref','r');
	dia=mgetl(fpdia);
	mclose(fpdia);
	ref=mgetl(fpref);
	mclose(fpref);
	
	dia=strsubst(dia,' ','');
	ref=strsubst(ref,' ','');
	
	if or(ref<>dia) then
		disp('Test Failed SEE : diff -wu  '+fname+'.dia '+fname+'.dia.ref ');
	else
		disp('Test '+fname+' passed');
	end
endfunction
//---------------------------------------------------------------
// Construction de la reference
function BuildRef(filename)
  ExecJava(filename,%T);
endfunction
//---------------------------------------------------------------
function maketests()
	Dim=size(tests);
	i=Dim(1);
	lines(0);
	for indice=1:i
		disp('//---------------------------------------------------------------');
		if ( BuildJava(tests(indice)) ) then
			ExecJava(tests(indice),%F);
			CleanClass(tests(indice));
			CompareDiary(tests(indice));
		end
	end
	disp('//---------------------------------------------------------------');
endfunction
//---------------------------------------------------------------
function makeref()
	Dim=size(tests);
	i=Dim(1);
	lines(0);
	for indice=1:i
		if ( BuildJava(tests(indice)) ) then
			ExecJava(tests(indice),%T);
			CleanClass(tests(indice));
		end
	end
endfunction
//---------------------------------------------------------------
// Fonction principale
function main()
	maketests();
	quit
endfunction
//---------------------------------------------------------------
main();
//---------------------------------------------------------------
