lines(0);
CurrentDirectorym2sci=pwd();
 Directoriesm2sci=["kernel",
 "percent",
 "sci_files"];

stacksize(5000000);
 
Dim=size(Directoriesm2sci);
for i=1:Dim(1) do 
  chdir(Directoriesm2sci(i));
  disp('-------- Creation of '+Directoriesm2sci(i)+' (Macros) for m2sci --------');
  exec('buildmacros.sce');
  chdir(CurrentDirectorym2sci);
end

clear CurrentDirectorym2sci Directoriesm2sci Dim
