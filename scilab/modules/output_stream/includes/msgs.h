/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

/**
 * @TODO : comment
 *
 * @param n 
 * @param ierr  
 * @return <ReturnValue>
 */
int C2F(msgs)(int *n, int *ierr);
/*
 * print a message
 * @param n int message number see modules/output_stream/src/c/msgs.c 
 * for the list of messages 
 * some message append a buffer to the message (@see printToBuffer)
 */
void Msgs(int n,int ierr);

/*
 * print to the char buffer used to add custom info to some messages
 * (@see Msgs). Buffer is of size bsize as #defined in stack-def.h (i.e. 4096)
 *
 * @param same as sprintf
 * @return same as sprintf
 */
int printToBuffer(const char* format,...);
