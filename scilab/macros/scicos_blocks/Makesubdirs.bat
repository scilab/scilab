@echo off
REM Makesubdirs.bat all
cd Branching
echo Macros compilation scicos_blocks/Branching
nmake /C /f Makefile.mak %1 
cd ..
cd Linear
echo Macros compilation scicos_blocks/Linear
nmake /C /f Makefile.mak %1 
cd ..

cd Sinks
echo Macros compilation scicos_blocks/Sinks
nmake /C /f Makefile.mak %1 
cd ..

cd Misc
echo Macros compilation scicos_blocks/Misc
nmake /C /f Makefile.mak %1 
cd ..

cd Sources
echo Macros compilation scicos_blocks/Sources
nmake /C /f Makefile.mak %1 
cd ..

cd Events
echo Macros compilation scicos_blocks/Events
nmake /C /f Makefile.mak %1 
cd ..

cd NonLinear
echo Macros compilation scicos_blocks/NonLinear
nmake /C /f Makefile.mak %1 
cd ..

cd Threshold
echo Macros compilation scicos_blocks/Threshold
nmake /C /f Makefile.mak %1 
cd ..

cd Electrical
echo Macros compilation scicos_blocks/Electrical
nmake /C /f Makefile.mak %1 
cd ..

cd Hydraulics
echo Macros compilation scicos_blocks/Hydraulics
nmake /C /f Makefile.mak %1 
cd ..


cd ..
