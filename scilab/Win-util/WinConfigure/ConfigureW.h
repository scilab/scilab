#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/************************************************************************/
char VC7Path[MAX_PATH];
/************************************************************************/
BOOL IsAFile(char *chainefichier);
void ExecuteCommand(char *cmdline,BOOL WaitEnd);
int ModifyFile(char *fichier,char *motclef,char *chaine);
int ConfigureMakefiles(void);
int ConfigureMakefilePVM3(void);
int ConfigureMakefileIncl(void);
int ConfigureHeaders(void);
int RunVisualDotNETProject(int mode);
int VerifFiles(void);
int SearchVSNET2003(void);
int SearchLibsFortran(void);
void help(void);
/************************************************************************/
#define MakefileInclMak	"Makefile.incl.mak"
#define MakefileInclMakTMP	"Makefile.incl.mak.tmp"
#define MakefilePVM3	".\\PVM3\\Make-PVM3.mak"
#define MakefilePVM3TMP ".\\PVM3\\Make-PVM3.mak.tmp"
#define MSVC71EXE		"devenv.exe"
#define ENVMSVC71		"VS71COMNTOOLS"
#define MSVCPROJECT		" scilab.sln"
#define VERIF			"WinConfigure.verif"
#define PathLibFortran  "c:\\Program Files\\Intel\\Fortran\\Compiler80\\IA32\\LIB\\"
#define FortranLib1		"LIBIFCORE.LIB"
#define FortranLib2		"LIBIFPORT.LIB"
#define FortranLib3		"LIBIRC.LIB"
#define FortranLib4		"LIBM.LIB"

#define ScicosVCHeader	".\\routines\\scicos\\blocks-vc.h"
#define MachineVCHeader	".\\routines\\machine-h.vc"
#define VersionVCHeader	".\\routines\\version-h.vc"
#define ScicosHeader	".\\routines\\scicos\\blocks.h"
#define MachineHeader	".\\routines\\machine.h"
#define VersionHeader	".\\routines\\version.h"

#define switchDebug "-D"
#define switchRelease "-R"
#define switchClean "-C"
#define switchInteractif "-I"

#define DebugMode	" /build debug"
#define ReleaseMode	" /build release"
#define CleanDebugMode	" /clean debug"
#define CleanReleaseMode " /clean release"


/************************************************************************/