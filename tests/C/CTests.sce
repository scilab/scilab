// Allan CORNET 2005
//---------------------------------------------------------------
// Tests Interface Scilab C
//---------------------------------------------------------------
tests = ['Test1.c'];
//---------------------------------------------------------------
// Compilation du fichier C
function ret=BuildC(filename)
  [path,fname,extension]=fileparts(filename);
  if MSDOS then
    Command='nmake /f '+fname+'.mak all /a';
  else
    Command='make all';;
  end
  [rep,stat]=unix_g(Command);
  if ~(stat==0) then
    disp('error compilation '+filename);
    ret=%F;
  else
  	ret=%T;
  end
endfunction
//---------------------------------------------------------------
// Execution du fichier C
function ExecC(filename,buildref)
  currentdir=pwd();
  if MSDOS then
    chdir(SCI+'\bin');
  else
   chdir(SCI+'/bin');
  end
  [path,fname,extension]=fileparts(filename);

  if MSDOS then
    [rep,stat]=unix_g(fname);
  else
   [rep,stat]=unix_g('./'+fname);
  end

  chdir(currentdir);
  if (buildref == %T) then
  	fd=mopen(fname+'.dia.ref','w');
  else
  	fd=mopen(fname+'.dia','w');
  end
  mputl(rep,fd);
  mclose(fd);
endfunction
//---------------------------------------------------------------
// Nettoyage apres tests du fichier .class généré
function CleanC(filename)
  [path,fname,extension]=fileparts(filename);
  if MSDOS then
    Command='nmake /f '+fname+'.mak distclean /a';
  else
    Command='make distclean ';
  end
  [rep,stat]=unix_g(Command);
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
    disp('Test Failed SEE : diff -w  '+fname+'.dia '+fname+'.ref ');
  else
    disp('Test '+fname+' passed');
  end
endfunction
//---------------------------------------------------------------
// Construction de la reference
function BuildRef(filename)
  ExecC(filename,%T);
endfunction
//---------------------------------------------------------------
function maketests()
  Dim=size(tests);
  i=Dim(1);
	lines(0);
  for indice=1:i
  	disp('//---------------------------------------------------------------');
		if ( BuildC(tests(indice)) ) then
		  ExecC(tests(indice),%F);
		  CleanC(tests(indice));
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
		if ( BuildC(tests(indice)) ) then
		  ExecC(tests(indice),%T);
		  CleanC(tests(indice));
		end
  end
endfunction
//---------------------------------------------------------------
// Fonction principale
function main()
		maketests();
endfunction
//---------------------------------------------------------------
main();
//---------------------------------------------------------------
