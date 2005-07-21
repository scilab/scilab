/* Copyright (C) 2005 Allan CORNET */
#include <Windows.h>
#pragma comment(lib, "../../bin/libscilab.lib")

extern int gp_printfile (HINSTANCE hInstance, HWND hwnd, char *filename, char *port);

int main (int argc, char *argv[])
{
	HWND parent;
	HANDLE x = GetModuleHandleA (0);
	parent = GetActiveWindow ();
	if (argc == 2)
		gp_printfile (x, parent, argv[1], (char *) 0);
	else
		gp_printfile (x, parent, (char *) 0, (char *) 0);
	return (0);
}