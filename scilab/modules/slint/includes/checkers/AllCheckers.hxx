/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SLINT_ALL_CHECKERS_HXX__
#define __SLINT_ALL_CHECKERS_HXX__

#include "checkers/FunctionNameChecker.hxx"
#include "checkers/FunctionArgsChecker.hxx"
#include "checkers/VariablesChecker.hxx"
#include "checkers/UselessArgChecker.hxx"
#include "checkers/UselessRetChecker.hxx"
#include "checkers/VariableNameChecker.hxx"
#include "checkers/RedefinitionChecker.hxx"
#include "checkers/SingleInstrChecker.hxx"
#include "checkers/EmptyBlockChecker.hxx"
#include "checkers/SemicolonAtEOLChecker.hxx"
#include "checkers/MopenMcloseChecker.hxx"
#include "checkers/McCabeChecker.hxx"
#include "checkers/DecimalChecker.hxx"
#include "checkers/PrintfChecker.hxx"
#include "checkers/TodoChecker.hxx"
#include "checkers/LineLengthChecker.hxx"
#include "checkers/IllegalCallsChecker.hxx"
#include "checkers/LinesCountChecker.hxx"
#include "checkers/NaNChecker.hxx"
#include "checkers/EqEqChecker.hxx"
#include "checkers/UselessOpChecker.hxx"
#include "checkers/UnreachableCodeChecker.hxx"
#include "checkers/DeprecatedChecker.hxx"
#include "checkers/SelectChecker.hxx"
#include "checkers/ImplicitListChecker.hxx"
#include "checkers/StructChecker.hxx"
#include "checkers/LoadSaveChecker.hxx"
#include "checkers/OldNotChecker.hxx"
#include "checkers/SpacesAroundOpChecker.hxx"
#include "checkers/BreaksInLoopChecker.hxx"
#include "checkers/NestedBlocksChecker.hxx"
#include "checkers/BracketedExpChecker.hxx"
#include "checkers/NotNotChecker.hxx"
#include "checkers/SpacesInArgsChecker.hxx"
#include "checkers/GlobalKeywordChecker.hxx"
#include "checkers/ExpInCondChecker.hxx"
#include "checkers/CommentRatioChecker.hxx"
#include "checkers/FunctionArgsOrderChecker.hxx"
#include "checkers/FunctionTestReturnChecker.hxx"
#include "checkers/ReturnsCountChecker.hxx"
#include "checkers/NotEqualChecker.hxx"

#endif // __SLINT_ALL_CHECKERS_HXX__
