/*
**  -*- C -*-
**
** errmsg.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Tue Nov  6 10:25:35 2007 bruno
** Last update Tue Nov  6 10:26:31 2007 bruno
**
** Copyright INRIA 2007
*/

#ifndef   	__ERRMSG_H__
#define   	__ERRMSG_H__

#include "machine.h"

/*
* errmsg displays the error message
* @param[in] n : error number, if n exceeds the maximum error number this
* routines displays the error message contained in buf
* @param[out] errtyp : error type (recoverable:0 or not:1)
*/
int C2F(errmsg)(integer *n,integer *errtyp);

#endif 	    /* !__ERRMSG_H__ */
