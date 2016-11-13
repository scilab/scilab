/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2016 - Scilab Enterprises - Clement DAVID
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*
 * swig -java -c++ -directors -package org.scilab.modules.xcos -outdir src/java/org/scilab/modules/xcos/ src/jni/JavaXMIResource.i
 * or make swig
 */

%module(director="0") JavaXMIResource;

%{
#include "utilities.hxx"
#include "XMIResource.hxx"

using namespace org_scilab_modules_scicos;
%}
typedef long long ScicosID;
#define SCICOS_IMPEXP

%inline %{
inline int save(const char* uri, ScicosID id) {
    return XMIResource(id).save(uri);
}

inline int load(const char* uri, ScicosID id) {
    return XMIResource(id).load(uri);
}
%}

