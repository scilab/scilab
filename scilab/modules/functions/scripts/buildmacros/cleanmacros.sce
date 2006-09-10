// ------------------------------------
// Allan CORNET
// INRIA 2006
// ------------------------------------
lines(0);

CurrentDirectory=pwd();

chdir ('SCI/modules');
if MSDOS then
  dos('del *.bin /s 2>NUL','-echo');
  dos('del lib /s 2>NUL','-echo');
  dos('del names /s 2>NUL','-echo');
  dos('del *.moc /s 2>NUL','-echo');
else
  unix('rm -Rf *.bin');
  unix('rm -Rf lib');
  unix('rm -Rf names');
  unix('rm -Rf *.moc');
end 

chdir (CurrentDirectory);

exit
// ------------------------------------