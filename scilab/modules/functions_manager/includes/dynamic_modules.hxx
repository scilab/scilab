/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        iType = types::Function::EntryPointC;
    }

    std::wstring wstName;
    std::wstring wstFunction;
    types::Function::FunctionType iType;
};

typedef std::vector<GatewayStr> vectGateway;

vectGateway loadGatewaysName(std::wstring _wstModuleName);

class GenericModule
{
protected :
public :
    GenericModule() {};
    ~GenericModule() {};
};

class ScinotesModule : public GenericModule
{
    static bool loadedDep;
    static void LoadDeps(void);
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
    static void LoadDeps(void);
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

class DifferentialEquationsModule : public GenericModule
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


class CacsdModule : public GenericModule
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

class SpecialFunctionModule : public GenericModule
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

#endif /* !__DYNAMIC_MODULES_HXX_ */
