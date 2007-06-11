#include "wtext.h"

extern void SendCTRLandAKey(int code);

void CleanCurrentLine(char *line)
{
	SendCTRLandAKey(CTRLU);
	write_scilab(line);
}