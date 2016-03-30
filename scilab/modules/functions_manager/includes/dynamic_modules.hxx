/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#ifndef __DYNAMIC_MODULES_HXX_
#define __DYNAMIC_MODULES_HXX_
#include <string>
#include <vector>
#include "function.hxx"

struct GatewayStr
{
    GatewayStr()
    {
        wstName = L"";
        wstFunction = L"";
        iType = types::Function::EntryPointOldC;
    }

    std::wstring wstName;
    std::wstring wstFunction;
    types::Function::FunctionType iType;
};

typedef std::vector<GatewayStr> vectGateway;

class GenericModule
{
protected :
public :
    GenericModule() {};
    ~GenericModule() {};
    static int Unload()
    {
        return 1;
    }
};

class ScinotesModule : public GenericModule
{
    static bool loadedDep;
    static int LoadDeps(const std::wstring& _functionName);
public :
    static int Load();
};

class FunctionsModule : public GenericModule
{
public :
    static int Load();
};

class StatisticsModule : public GenericModule
{
public :
    static int Load();
};

class SignalProcessingModule : public GenericModule
{
public :
    static int Load();
};

class HelptoolsModule : public GenericModule
{
    static bool loadedDep;
    static int LoadDeps(const std::wstring& _functionName);
public :
    static int Load();
};

class MatioModule : public GenericModule
{
public :
    static int Load();
};

class Hdf5Module : public GenericModule
{
public :
    static int Load();
};

class ActionBindingModule : public GenericModule
{
public :
    static int Load();
};

class SpreadsheetModule : public GenericModule
{
public :
    static int Load();
};

class RandlibModule : public GenericModule
{
public :
    static int Load();
};

class InterpolationModule : public GenericModule
{
public :
    static int Load();
};

class SoundModule : public GenericModule
{
public :
    static int Load();
};

class UmfpackModule : public GenericModule
{
public :
    static int Load();
};

class OptimizationModule : public GenericModule
{
public :
    static int Load();
};

class SpecialFunctionsModule : public GenericModule
{
public :
    static int Load();
};

class GraphicExportModule : public GenericModule
{
public :
    static int Load();
};

class ArnoldiModule : public GenericModule
{
public :
    static int Load();
};

class CallScilabModule : public GenericModule
{
public :
    static int Load();
};

class CompletionModule : public GenericModule
{
public :
    static int Load();
};

class XmlModule : public GenericModule
{
public :
    static int Load();
};

class ScicosModule : public GenericModule
{
public :
    static int Load();
};

class XcosModule : public GenericModule
{
    static bool loadedDep;
    static int LoadDeps(const std::wstring& _functionName);
public :
    static int Load();
};

class MPIModule : public GenericModule
{
public :
    static int Load();
};

class ExternalObjectsModule : public GenericModule
{
public :
    static int Load();
};

class ExternalObjectsJavaModule : public GenericModule
{
public :
    static int Load();
};

class SlintModule : public GenericModule
{
public :
    static int Load();
};

#endif /* !__DYNAMIC_MODULES_HXX_ */
