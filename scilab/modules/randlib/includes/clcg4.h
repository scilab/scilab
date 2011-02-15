
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef H_CLCG4_H
#define H_CLCG4_H

#include "dynlib_randlib.h"

#define Maxgen  100

typedef  enum {InitialSeed, LastSeed, NewSeed}  SeedType;

RANDLIB_IMPEXP int set_seed_clcg4(int g, double s0, double s1, double s2, double s3);

RANDLIB_IMPEXP void get_state_clcg4(int g, double s[4]);

RANDLIB_IMPEXP void init_generator_clcg4(int g, SeedType Where);

RANDLIB_IMPEXP void advance_state_clcg4(int g, int k);

RANDLIB_IMPEXP int set_initial_seed_clcg4(double s0, double s1, double s2, double s3);

RANDLIB_IMPEXP unsigned int clcg4(int g);

#endif
