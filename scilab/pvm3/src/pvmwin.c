
static char rcsid[] =
	"$Id: pvmwin.c,v 1.1 2001/04/26 07:47:12 scilab Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

#include "pvmwin.h"
#include <fcntl.h>

extern int debugmask;

char *username;


void
Wait(HANDLE id, DWORD *prv)
{
	if (!WaitForSingleObject(id, INFINITE)) {
		*prv = 2;
	} else {
		if (!GetExitCodeThread(id, prv)) {
			*prv = 4;
		}
	}
}


int win32_write_socket (SOCKET s,const char FAR *tosend, int size)
{
	int nSend=0;

	int size2send=size;

	while (size2send > 0) {
		if ((nSend = send(s, tosend, size2send, 0)) == -1)
			return nSend;

		tosend += nSend;
		size2send -= nSend;
	}
/*
	pvmlogprintf("loclinput, to send: %d yes, send: %d\n",
			size,size-size2send);
*/
	return size-size2send;	/* this should be $size in the end */
}


int win32_read_socket (SOCKET s,char FAR *torecv, int size)
{
	int nReceived=0;

	int size2recv=size;

	while (size2recv > 0) {

		if ((nReceived = recv(s, torecv, size2recv, 0)) == -1)
			return nReceived;

		torecv += nReceived;
		size2recv -= nReceived;
	}
/*
	pvmlogprintf("loclinput, to read: %d yes, read: %d\n",
			size,size-size2recv);
*/
	return size-size2recv;
}


int gettimeofday(struct timeval *t, struct timezone *tzp)
{
	struct _timeb timebuffer;

	/* this calls the time and returns sec ** msec */
	_ftime( &timebuffer );

	t->tv_sec=timebuffer.time;
	t->tv_usec=timebuffer.millitm*1000;
 	return 1;
}


void nt_rpc_report( char *s)
{
	printf("Error: %s: %d\n",s,GetLastError());
}


void ErrorHandler(char *s)
{
	printf("Error: %s: %d\n",s,GetLastError());
}


int win32_close_file(HANDLE f)
{
	int success;
	success = CloseHandle(f);
	return success;
}


HANDLE win32_create_file(char *TMP_AREA, int mode)
{
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD;
	PACL pACLNew;
	DWORD cbACL = 1024;
	PSID pSID;
	DWORD cbSID = 1024;
	LPSTR szUserName;
	DWORD dwUserNameLength;
	LPSTR lpszDomain;
	DWORD cchDomainName = 80;
	PSID_NAME_USE psnuType;

	/* Initialize a new security descriptor. */


	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR,
		SECURITY_DESCRIPTOR_MIN_LENGTH);	/* defined in WINNT.H */
	if (pSD == NULL) {
		ErrorHandler("LocalAlloc");
		return FALSE;
	}

	if (!InitializeSecurityDescriptor(pSD,
			SECURITY_DESCRIPTOR_REVISION)) {

		ErrorHandler("InitializeSecurityDescriptor");
		goto Cleanup;
	}

	/* Initialize a new ACL.				 */

	pACLNew = (PACL) LocalAlloc(LPTR, cbACL);
	if (pACLNew == NULL) {
		ErrorHandler("LocalAlloc");
		goto Cleanup;
	}

	if (!InitializeAcl(pACLNew, cbACL, ACL_REVISION2)) {
		if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED) {
			return (HANDLE) -2;
		}
		ErrorHandler("InitializeAcl");
		goto Cleanup;
	}

	/* Retrieve the SID for User		*/
	if (username)
		szUserName=username;
	else
 		szUserName=MyGetUserName();

	pSID = (PSID) LocalAlloc(LPTR, cbSID);
	psnuType = (PSID_NAME_USE) LocalAlloc(LPTR, 1024);
	lpszDomain = (LPSTR) LocalAlloc(LPTR, cchDomainName);
	if (pSID == NULL || psnuType == NULL ||
		lpszDomain == NULL) {
		ErrorHandler("LocalAlloc");
		goto Cleanup;
	}

	if (!LookupAccountName((LPSTR) NULL, /* local name */
			szUserName,
			pSID,
			&cbSID,
			lpszDomain,
			&cchDomainName,
			psnuType)) {
		ErrorHandler("LookupAccountName");
		goto Cleanup;
	}
	if (!IsValidSid(pSID))
		ErrorHandler("SID is not valid.\n");


	/* Allow read but not write access to the file. */

	if (!AddAccessAllowedAce(pACLNew,
			ACL_REVISION2,
			GENERIC_ALL,
			pSID)) {
		ErrorHandler("AddAccessAllowedAce");
		goto Cleanup;
	}

	/* Add a new ACL to the security descriptor. */

	if (!SetSecurityDescriptorDacl(pSD,
			TRUE,			/* fDaclPresent flag */
			pACLNew,
			FALSE)) {		/* not a default disc. ACL */
		ErrorHandler("SetSecurityDescriptorDacl");
		goto Cleanup;
	}

	hFile = CreateFile(TMP_AREA,
					GENERIC_WRITE | GENERIC_READ,
					// FILE_SHARE_READ | FILE_SHARE_WRITE,
					0,
					NULL,
					mode,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		if (mode == CREATE_NEW)
			return (HANDLE) -1;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return (HANDLE) -1;
	}

	/* Apply the new security descriptor to the file. */

	if (!SetFileSecurity(TMP_AREA,
		DACL_SECURITY_INFORMATION,
		pSD)) {
		ErrorHandler("SetFileSecurity");
		goto Cleanup;
	}

	/*
	printf("Successfully added access-allowed ACE to file's DACL.\n");
	*/

	return hFile;

Cleanup:
		FreeSid(pSID);
		if(pSD != NULL)
			LocalFree((HLOCAL) pSD);
		if(pACLNew != NULL)
			LocalFree((HLOCAL) pACLNew);
		if(psnuType != NULL)
			LocalFree((HLOCAL) psnuType);
		if(lpszDomain != NULL)
			LocalFree((HLOCAL) lpszDomain);
	return 0;
}


int win32_write_file(HANDLE f, char *s,int size)
{
	int sizewritten=0;

	if (!WriteFile(f,s,size,&sizewritten,NULL)) {
		pvmlogprintf("Could not write to file :%d \n",GetLastError());
		return 0;
	}
	return sizewritten;

}


int win32_read_file(HANDLE f, char *s,int size)
{
	int sizeread=0;
	if (!ReadFile(f,s,size,&sizeread,NULL)) {
		pvmlogprintf("Could not read data from file :%d \n",
				GetLastError());
		return -1;
	}
	return sizeread;

}


HANDLE win32_open_file(char *TMP_AREA)
{
	HANDLE hF;

	hF= win32_create_file(TMP_AREA,OPEN_EXISTING);
	if (hF == INVALID_HANDLE_VALUE)
		printf("Could not open file %s: %d \n",TMP_AREA,GetLastError());
	return hF;
}


int kill( int pid, int p_handle, int signal)
{
	HANDLE hProcess;

	// DebugBreak();
	if (p_handle)
	if (!TerminateProcess((HANDLE)p_handle,signal)) {
		pvmlogprintf("Unable to terminate process: %d \n",p_handle);
	} else return TRUE;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);

	if (hProcess) {
		if (!TerminateProcess(hProcess,signal)) {
			pvmlogprintf("Unable to terminate process: %d \n",hProcess);
		} else {CloseHandle (hProcess);return TRUE;}
	}
	CloseHandle (hProcess);
	return TRUE;
}


char *MyGetUserName()
{
	char *thisname=0;
	char myuser[64];
	DWORD szuser=64;
	if (!GetUserName(myuser,&szuser)) {
		pvmlogprintf("You are not logged on to this machine: %d\n",
				GetLastError());
		pvmlogprintf("Continued by provided username\n");
		return NULL;
	}
	thisname = malloc (16 *sizeof(char));
	strncpy(thisname,myuser,16);
	return thisname;

}


int win32_delete_file(char *TMP_AREA)
{
	return (!DeleteFile(TMP_AREA));
}

