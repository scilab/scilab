Put LCC-Win32 in this directory
Edit lcc.reg, modify all paths, save and double click on lcc.reg to update Windows registry
Edit scilab.star and modify Line 150 to LCC=%T
Launch Scilab and convert VC Library to LCC Library with VCtoLCCLib() Macro.

example :
 if you are install scilab in D:\
 
contents of the "lcc" directory:

 Répertoire de D:\scilab\lcc

18/03/2004  09:01    <REP>          .
18/03/2004  09:01    <REP>          ..
18/03/2004  08:52    <REP>          bin
18/03/2004  08:51    <REP>          buildlib
18/03/2004  08:51    <REP>          ChangeLog.txt
18/03/2004  08:51    <REP>          include
18/03/2004  08:52             1 580 lcc.reg
18/03/2004  08:51    <REP>          lib
18/03/2004  08:51    <REP>          projects
18/03/2004  09:01               596 Readme_LCC.txt
18/03/2004  08:51    <REP>          src

