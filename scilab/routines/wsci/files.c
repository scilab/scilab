
/*
 * From TclWinFCmd.c
 *
 *      This file implements the Windows specific portion of file manipulation 
 *      subcommands of the "file" command. 
 *
 * Copyright (c) 1996-1997 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * SCCS: @(#) SciWinFCmd.c 1.19 97/08/05 15:23:47
 */

#include <windows.h>
#include "dstrings.h"
#include <errno.h>
#ifdef __ABSC__
#include "errno.h"
#endif

/*
 * The following constants specify the type of callback when
 * TraverseWinTree() calls the traverseProc()
 */

#define DOTREE_PRED   1		/* pre-order directory  */
#define DOTREE_POSTD  2		/* post-order directory */
#define DOTREE_F      3		/* regular file */

extern int SciWinGetPlatformId ();
extern void SciWinConvertError (DWORD errCode);
extern void sciprint (char *fmt,...);
/*
 * Prototype for the TraverseWinTree callback function.
 */

typedef int (TraversalProc) (char *src, char *dst, DWORD attr, int type,
			     Sci_DString * errorPtr);

/*
 * Declarations for local procedures defined in this file:
 */


static int TraversalDelete (char *src, char *dst, DWORD attr,
			    int type, Sci_DString * errorPtr);
static int TraverseWinTree (TraversalProc * traverseProc,
			    Sci_DString * sourcePtr, Sci_DString * destPtr,
			    Sci_DString * errorPtr);

static int ScipRemoveDirectory (
				 char *path,	/* Pathname of directory to be removed. */
				 int recursive,		/* If non-zero, removes directories that
							 * are nonempty.  Otherwise, will only remove
							 * empty directories. */
				 Sci_DString * errorPtr);	/* If non-NULL, initialized DString for
								 * error reporting. */


/*
 *---------------------------------------------------------------------------
 *
 * SciCreateDirectory --
 *
 *      Creates the specified directory.  All parent directories of the
 *      specified directory must already exist.  The directory is
 *      automatically created with permissions so that user can access
 *      the new directory and create new files or subdirectories in it.
 *
 * Results:
 *      If the directory was successfully created, returns SCI_OK.
 *      Otherwise the return value is SCI_ERROR and errno is set to
 *      indicate the error.  Some possible values for errno are:
 *
 *      EACCES:     a parent directory can't be read and/or written.
 *      EEXIST:     path already exists.
 *      ENOENT:     a parent directory doesn't exist.
 *
 * Side effects:
 *      A directory is created.
 *
 *---------------------------------------------------------------------------
 */

int
SciCreateDirectory (
		     char *path)	/* Pathname of directory to create */
{
  int error;

  if (CreateDirectory (path, NULL) == 0)
    {
      error = GetLastError ();
      if (SciWinGetPlatformId () == VER_PLATFORM_WIN32s)
	{
	  if ((error == ERROR_ACCESS_DENIED)
	      && (GetFileAttributes (path) != (DWORD) - 1))
	    {
	      error = ERROR_FILE_EXISTS;
	    }
	}
      SciWinConvertError (error);
      if (errno == EEXIST)
	{
	  return SCI_OK;
	}
      sciprint ("Cannot create directory %s\r\n", path);
      return SCI_ERROR;
    }
  return SCI_OK;
}



/*
 *----------------------------------------------------------------------
 *
 * SciRemoveDirectory -- 
 *
 *      Removes directory (and its contents, if the recursive flag is set).
 *
 * Results:
 *      If the directory was successfully removed, returns SCI_OK.
 *      Otherwise the return value is SCI_ERROR, errno is set to indicate
 *      the error, and the pathname of the file that caused the error
 *      is stored in errorPtr.  Some possible values for errno are:
 *
 *      EACCES:     path directory can't be read and/or written.
 *      EEXIST:     path is a non-empty directory.
 *      EINVAL:     path is root directory or current directory.
 *      ENOENT:     path doesn't exist or is "".
 *      ENOTDIR:    path is not a directory.
 *
 *      EACCES:     path is a char device (nul:, com1:, etc.) (95)
 *      EINVAL:     path is a char device (nul:, com1:, etc.) (NT)
 *
 * Side effects:
 *      Directory removed.  If an error occurs, the error will be returned
 *      immediately, and remaining files will not be deleted.
 *
 *----------------------------------------------------------------------
 */

int 
SciRemoveDirectory (
		     char *path)	/* Pathname of directory to be removed. */
{
  Sci_DString errorPtr;
  Sci_DStringInit (&errorPtr);
  if (ScipRemoveDirectory (path, 1, &errorPtr) == SCI_ERROR)
    {
      sciprint ("Cannot remove directory %s\r\n", errorPtr.string);
    }
  Sci_DStringFree (&errorPtr);
  return 0;
}



/*
 *---------------------------------------------------------------------------
 *
 * ScipDeleteFile --
 *
 *      Removes a single file (not a directory).
 *
 * Results:
 *      If the file was successfully deleted, returns SCI_OK.  Otherwise
 *      the return value is SCI_ERROR and errno is set to indicate the
 *      error.  Some possible values for errno are:
 *
 *      EACCES:     a parent directory can't be read and/or written.
 *      EISDIR:     path is a directory.
 *      ENOENT:     path doesn't exist or is "".
 *
 *      EACCES:     exists an open file already referring to path.
 *      EACCES:     path is a char device (nul:, com1:, etc.)
 *
 * Side effects:
 *      The file is deleted, even if it is read-only.
 *
 *---------------------------------------------------------------------------
 */

int
ScipDeleteFile (
		 char *path)	/* Pathname of file to be removed. */
{
  DWORD attr;

  if (DeleteFile (path) != FALSE)
    {
      return SCI_OK;
    }
  SciWinConvertError (GetLastError ());
  if (path[0] == '\0')
    {
      /*
       * Win32s thinks that "" is the same as "." and then reports EISDIR
       * instead of ENOENT.
       */

      errno = ENOENT;
    }
  else if (errno == EACCES)
    {
      attr = GetFileAttributes (path);
      if (attr != (DWORD) - 1)
	{
	  if (attr & FILE_ATTRIBUTE_DIRECTORY)
	    {
	      /*
	       * Windows NT reports removing a directory as EACCES instead
	       * of EISDIR.
	       */

	      errno = EISDIR;
	    }
	  else if (attr & FILE_ATTRIBUTE_READONLY)
	    {
	      SetFileAttributes (path, attr & ~FILE_ATTRIBUTE_READONLY);
	      if (DeleteFile (path) != FALSE)
		{
		  return SCI_OK;
		}
	      SciWinConvertError (GetLastError ());
	      SetFileAttributes (path, attr);
	    }
	}
    }
  else if (errno == ENOENT)
    {
      attr = GetFileAttributes (path);
      if (attr != (DWORD) - 1)
	{
	  if (attr & FILE_ATTRIBUTE_DIRECTORY)
	    {
	      /*
	       * Windows 95 reports removing a directory as ENOENT instead 
	       * of EISDIR. 
	       */

	      errno = EISDIR;
	    }
	}
    }
  else if (errno == EINVAL)
    {
      /*
       * Windows NT reports removing a char device as EINVAL instead of
       * EACCES.
       */

      errno = EACCES;
    }

  return SCI_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * ScipRemoveDirectory -- 
 *
 *      Removes directory (and its contents, if the recursive flag is set).
 *
 * Results:
 *      If the directory was successfully removed, returns SCI_OK.
 *      Otherwise the return value is SCI_ERROR, errno is set to indicate
 *      the error, and the pathname of the file that caused the error
 *      is stored in errorPtr.  Some possible values for errno are:
 *
 *      EACCES:     path directory can't be read and/or written.
 *      EEXIST:     path is a non-empty directory.
 *      EINVAL:     path is root directory or current directory.
 *      ENOENT:     path doesn't exist or is "".
 *      ENOTDIR:    path is not a directory.
 *
 *      EACCES:     path is a char device (nul:, com1:, etc.) (95)
 *      EINVAL:     path is a char device (nul:, com1:, etc.) (NT)
 *
 * Side effects:
 *      Directory removed.  If an error occurs, the error will be returned
 *      immediately, and remaining files will not be deleted.
 *
 *----------------------------------------------------------------------
 */

int
ScipRemoveDirectory (
		      char *path,	/* Pathname of directory to be removed. */
		      int recursive,	/* If non-zero, removes directories that
					 * are nonempty.  Otherwise, will only remove
					 * empty directories. */
		      Sci_DString * errorPtr)	/* If non-NULL, initialized DString for
						 * error reporting. */
{
  int result;
  Sci_DString buffer;
  DWORD attr;

  if (RemoveDirectory (path) != FALSE)
    {
      return SCI_OK;
    }
  SciWinConvertError (GetLastError ());
  if (path[0] == '\0')
    {
      /*
       * Win32s thinks that "" is the same as "." and then reports EACCES
       * instead of ENOENT.
       */

      errno = ENOENT;
    }
  if (errno == EACCES)
    {
      attr = GetFileAttributes (path);
      if (attr != (DWORD) - 1)
	{
	  if ((attr & FILE_ATTRIBUTE_DIRECTORY) == 0)
	    {
	      /* 
	       * Windows 95 reports calling RemoveDirectory on a file as an 
	       * EACCES, not an ENOTDIR.
	       */

	      errno = ENOTDIR;
	      goto end;
	    }

	  if (attr & FILE_ATTRIBUTE_READONLY)
	    {
	      attr &= ~FILE_ATTRIBUTE_READONLY;
	      if (SetFileAttributes (path, attr) == FALSE)
		{
		  goto end;
		}
	      if (RemoveDirectory (path) != FALSE)
		{
		  return SCI_OK;
		}
	      SciWinConvertError (GetLastError ());
	      SetFileAttributes (path, attr | FILE_ATTRIBUTE_READONLY);
	    }

	  /* 
	   * Windows 95 and Win32s report removing a non-empty directory 
	   * as EACCES, not EEXIST.  If the directory is not empty,
	   * change errno so caller knows what's going on.
	   */

	  if (SciWinGetPlatformId () != VER_PLATFORM_WIN32_NT)
	    {
	      HANDLE handle;
	      WIN32_FIND_DATA data;
	      Sci_DString buffer;
	      char *find;
	      int len;

	      Sci_DStringInit (&buffer);
	      find = Sci_DStringAppend (&buffer, path, -1);
	      len = Sci_DStringLength (&buffer);
	      if ((len > 0) && (find[len - 1] != '\\'))
		{
		  Sci_DStringAppend (&buffer, "\\", 1);
		}
	      find = Sci_DStringAppend (&buffer, "*.*", 3);
	      handle = FindFirstFile (find, &data);
	      if (handle != INVALID_HANDLE_VALUE)
		{
		  while (1)
		    {
		      if ((strcmp (data.cFileName, ".") != 0)
			  && (strcmp (data.cFileName, "..") != 0))
			{
			  /*
			   * Found something in this directory.
			   */

			  errno = EEXIST;
			  break;
			}
		      if (FindNextFile (handle, &data) == FALSE)
			{
			  break;
			}
		    }
		  FindClose (handle);
		}
	      Sci_DStringFree (&buffer);
	    }
	}
    }
  if (errno == ENOTEMPTY)
    {
      /* 
       * The caller depends on EEXIST to signify that the directory is
       * not empty, not ENOTEMPTY. 
       */

      errno = EEXIST;
    }
  if ((recursive != 0) && (errno == EEXIST))
    {
      /*
       * The directory is nonempty, but the recursive flag has been
       * specified, so we recursively remove all the files in the directory.
       */

      Sci_DStringInit (&buffer);
      Sci_DStringAppend (&buffer, path, -1);
      result = TraverseWinTree (TraversalDelete, &buffer, NULL, errorPtr);
      Sci_DStringFree (&buffer);
      return result;
    }

end:
  if (errorPtr != NULL)
    {
      Sci_DStringAppend (errorPtr, path, -1);
    }
  return SCI_ERROR;
}

/*
 *---------------------------------------------------------------------------
 *
 * TraverseWinTree --
 *
 *      Traverse directory tree specified by sourcePtr, calling the function 
 *      traverseProc for each file and directory encountered.  If destPtr 
 *      is non-null, each of name in the sourcePtr directory is appended to 
 *      the directory specified by destPtr and passed as the second argument 
 *      to traverseProc() .
 *
 * Results:
 *      Standard Sci result.
 *
 * Side effects:
 *      None caused by TraverseWinTree, however the user specified 
 *      traverseProc() may change state.  If an error occurs, the error will
 *      be returned immediately, and remaining files will not be processed.
 *
 *---------------------------------------------------------------------------
 */

static int
TraverseWinTree (
		  TraversalProc * traverseProc,		/* Function to call for every file and
							 * directory in source hierarchy. */
		  Sci_DString * sourcePtr,	/* Pathname of source directory to be
						 * traversed. */
		  Sci_DString * targetPtr,	/* Pathname of directory to traverse in
						 * parallel with source directory. */
		  Sci_DString * errorPtr)	/* If non-NULL, an initialized DString for
						 * error reporting. */
{
  DWORD sourceAttr;
  char *source, *target, *errfile;
  int result, sourceLen, targetLen, sourceLenOriginal, targetLenOriginal;
  HANDLE handle;
  WIN32_FIND_DATA data;

  result = SCI_OK;
  source = Sci_DStringValue (sourcePtr);
  sourceLenOriginal = Sci_DStringLength (sourcePtr);
  if (targetPtr != NULL)
    {
      target = Sci_DStringValue (targetPtr);
      targetLenOriginal = Sci_DStringLength (targetPtr);
    }
  else
    {
      target = NULL;
      targetLenOriginal = 0;
    }

  errfile = NULL;

  sourceAttr = GetFileAttributes (source);
  if (sourceAttr == (DWORD) - 1)
    {
      errfile = source;
      goto end;
    }
  if ((sourceAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
    {
      /*
       * Process the regular file
       */

      return (*traverseProc) (source, target, sourceAttr, DOTREE_F, errorPtr);
    }

  /*
   * When given the pathname of the form "c:\" (one that already ends
   * with a backslash), must make sure not to add another "\" to the end
   * otherwise it will try to access a network drive.  
   */

  sourceLen = sourceLenOriginal;
  if ((sourceLen > 0) && (source[sourceLen - 1] != '\\'))
    {
      Sci_DStringAppend (sourcePtr, "\\", 1);
      sourceLen++;
    }
  source = Sci_DStringAppend (sourcePtr, "*.*", 3);
  handle = FindFirstFile (source, &data);
  Sci_DStringSetLength (sourcePtr, sourceLen);
  if (handle == INVALID_HANDLE_VALUE)
    {
      /* 
       * Can't read directory
       */

      SciWinConvertError (GetLastError ());
      errfile = source;
      goto end;
    }

  result = (*traverseProc) (source, target, sourceAttr, DOTREE_PRED, errorPtr);
  if (result != SCI_OK)
    {
      FindClose (handle);
      return result;
    }

  if (targetPtr != NULL)
    {
      targetLen = targetLenOriginal;
      if ((targetLen > 0) && (target[targetLen - 1] != '\\'))
	{
	  target = Sci_DStringAppend (targetPtr, "\\", 1);
	  targetLen++;
	}
    }

  while (1)
    {
      if ((strcmp (data.cFileName, ".") != 0)
	  && (strcmp (data.cFileName, "..") != 0))
	{
	  /* 
	   * Append name after slash, and recurse on the file. 
	   */

	  Sci_DStringAppend (sourcePtr, data.cFileName, -1);
	  if (targetPtr != NULL)
	    {
	      Sci_DStringAppend (targetPtr, data.cFileName, -1);
	    }
	  result = TraverseWinTree (traverseProc, sourcePtr, targetPtr,
				    errorPtr);
	  if (result != SCI_OK)
	    {
	      break;
	    }

	  /*
	   * Remove name after slash.
	   */

	  Sci_DStringSetLength (sourcePtr, sourceLen);
	  if (targetPtr != NULL)
	    {
	      Sci_DStringSetLength (targetPtr, targetLen);
	    }
	}
      if (FindNextFile (handle, &data) == FALSE)
	{
	  break;
	}
    }
  FindClose (handle);

  /*
   * Strip off the trailing slash we added
   */

  Sci_DStringSetLength (sourcePtr, sourceLenOriginal);
  source = Sci_DStringValue (sourcePtr);
  if (targetPtr != NULL)
    {
      Sci_DStringSetLength (targetPtr, targetLenOriginal);
      target = Sci_DStringValue (targetPtr);
    }

  if (result == SCI_OK)
    {
      /*
       * Call traverseProc() on a directory after visiting all the
       * files in that directory.
       */

      result = (*traverseProc) (source, target, sourceAttr,
				DOTREE_POSTD, errorPtr);
    }
end:
  if (errfile != NULL)
    {
      SciWinConvertError (GetLastError ());
      if (errorPtr != NULL)
	{
	  Sci_DStringAppend (errorPtr, errfile, -1);
	}
      result = SCI_ERROR;
    }

  return result;
}

/*
 *----------------------------------------------------------------------
 *
 * TraversalDelete --
 *
 *      Called by procedure TraverseWinTree for every file and
 *      directory that it encounters in a directory hierarchy. This
 *      procedure unlinks files, and removes directories after all the
 *      containing files have been processed.
 *
 * Results:
 *      Standard Sci result.
 *
 * Side effects:
 *      Files or directory specified by src will be deleted. If an
 *      error occurs, the windows error is converted to a Posix error
 *      and errno is set accordingly.
 *
 *----------------------------------------------------------------------
 */

static int
TraversalDelete (
		  char *src,	/* Source pathname. */
		  char *ignore,	/* Destination pathname (not used). */
		  DWORD srcAttr,	/* File attributes for src (not used). */
		  int type,	/* Reason for call - see TraverseWinTree(). */
		  Sci_DString * errorPtr)	/* If non-NULL, initialized DString for
						 * error return. */
{
  switch (type)
    {
    case DOTREE_F:
      if (ScipDeleteFile (src) == SCI_OK)
	{
	  return SCI_OK;
	}
      break;

    case DOTREE_PRED:
      return SCI_OK;

    case DOTREE_POSTD:
      if (ScipRemoveDirectory (src, 0, NULL) == SCI_OK)
	{
	  return SCI_OK;
	}
      break;

    }

  if (errorPtr != NULL)
    {
      Sci_DStringAppend (errorPtr, src, -1);
    }
  return SCI_ERROR;
}
