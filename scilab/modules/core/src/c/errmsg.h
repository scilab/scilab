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

#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "localization.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "inffic.h"
#include "stackinfo.h"
#include "core_math.h"

extern int C2F(showstack)();
extern int C2F(cvname)();
extern int C2F(prntid)();
static void strip_blank(char *source);
static void msgout_string(const char *msg);

int C2F(errmsg)(integer *n,integer *errtyp);

#endif 	    /* !__ERRMSG_H__ */
