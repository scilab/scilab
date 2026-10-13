/* ---------------------------------------------------------------------------*
 * fmuTemplate.h
 * Definitions used in fmiModelFunctions.c and by the includer of this file
 * (c) 2010 QTronic GmbH 
 * ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "fmiModelFunctions.h"
/*#include "SmuConverter.h"*/

/* macros used to define variables */
#define  r(vr) comp->r[vr]
#define  i(vr) comp->i[vr]
#define  c(vr) comp->c[vr]
#define  b(vr) comp->b[vr]
#define  s(vr) comp->s[vr]
#define pos(z) comp->isPositive[z]
#define copy(vr, value) setString(comp, vr, value)

#define not_modelError (modelInstantiated|modelInitialized|modelTerminated)

typedef enum {
    modelInstantiated = 1<<0,
    modelInitialized  = 1<<1,
    modelTerminated   = 1<<2,
    modelError        = 1<<3
} ModelState;

typedef struct {
    void       *context;
    void       **r;
    void       **i;
    void       **c;
    void       **b;
    void       **isPositive;
    void       *pConverter;
    fmiReal time;
    fmiString instanceName;
    fmiString GUID;
    fmiCallbackFunctions functions;
    fmiBoolean loggingOn;
    ModelState state;
} ModelInstance;

fmiStatus setString(fmiComponent comp, fmiValueReference vr, fmiString value){
    return fmiSetString(comp, &vr, 1, &value);
}

