/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __SCILABPYTHONENVIRNOMENT_HXX__
#define __SCILABPYTHONENVIRNOMENT_HXX__

#ifdef _MSC_VER
#define PATHSEPARATOR ";"
#else
#define PATHSEPARATOR ":"
#endif

extern "C"
{
#include "dynlib_external_objects_java_scilab.h"
#include <jni.h>
#ifdef _MSC_VER
    //#include <Windows.h>
#endif
#include "os_string.h"
}

#include "ScilabAbstractEnvironment.hxx"
#include "ScilabEnvironments.hxx"
#include "ScilabJavaException.hxx"
#include "ScilabJavaEnvironmentWrapper.hxx"
#include "JavaOptionsHelper.hxx"
#include "ScilabStream.hxx"

#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#define LOG_BUFFER_SIZE 4096

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{

class EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP ScilabJavaEnvironment : public ScilabAbstractEnvironment
{
    static const std::string environmentName;
    static int envId;
    static ScilabJavaEnvironment * instance;
    static bool usable;

    bool traceEnabled;
    bool isInit;
    ScilabStream & scilabStream;
    JavaOptionsHelper & helper;
    ScilabGatewayOptions & gwOptions;
    ScilabJavaEnvironmentWrapper & wrapper;

public :

    ~ScilabJavaEnvironment();

    void Initialize();

    void Finalize();

    static int start();

    static void finish();

    static ScilabJavaEnvironment* getInstance()
    {
        return instance;
    }

    JavaOptionsHelper & getOptionsHelper();

    ScilabGatewayOptions & getGatewayOptions();

    ScilabAbstractEnvironmentWrapper & getWrapper();

    const std::string & getEnvironmentName();

    int extract(int id, int * args, int argsSize);

    void insert(int id, int * args, int argsSize);

    void addNamedVariable(int id, const char * varName);

    int getNamedVariable(const char * varName);

    void evalString(const char ** code, int nbLines, ScilabStringStackAllocator * allocator);

    void getEnvironmentInfos(const ScilabStringStackAllocator & allocator);

    void garbagecollect();

    void addtoclasspath(const char * path);

    void getclasspath(const ScilabStringStackAllocator & allocator);

    int createarray(char * className, int * dims, int len);

    int loadclass(char * className, char * currentSciPath, bool isNamedVarCreated, bool allowReload);

    void getrepresentation(int id, const ScilabStringStackAllocator & allocator);

    std::string getrepresentation(int id);

    bool isvalidobject(int id);

    int newinstance(int id, int * args, int argsSize);

    int operation(int idA, int idB, const OperatorsType type);

    int * invoke(int id, const char * methodName, int * args, int argsSize);

    void setfield(int id, const char * fieldName, int idarg);

    int getfield(int id, const char * fieldName);

    int getfieldtype(int id, const char * fieldName);

    int getarrayelement(int id, int * index, int length);

    void setarrayelement(int id, int * index, int length, int idArg);

    int cast(int id, char * className);

    int castwithid(int id, int classId);

    void removeobject(int id);

    void removeobject(const int * id, const int length);

    void autoremoveobject(int id);

    void getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator);

    void getaccessiblefields(int id, const ScilabStringStackAllocator & allocator);

    std::vector<std::string> getCompletion(int id, char ** fieldPath, const int fieldPathLen);

    std::string getclassname(int id);

    VariableType isunwrappable(int id);

    int compilecode(char * className, char ** code, int size);

    void enabletrace(const char * filename);

    void disabletrace();

    void writeLog(const std::string & fun, const std::string str, ...) const;

    inline int getEnvId() const
    {
        return envId;
    }

    inline bool isTraceEnabled() const
    {
        return traceEnabled;
    }

    int createJarArchive(char *jarFilePath, char **filePaths, int filePathsSize, char *filesRootPath, char *manifestFilePath);

private:
    ScilabJavaEnvironment();

    void getMethodResult(JavaVM * jvm_, const char * const methodName, int id, const ScilabStringStackAllocator & allocator);

    // template <typename T, typename U, class V> void unwrapMat(JavaVM * jvm_, const bool methodOfConv, const int javaID, const ScilabStringStackAllocator & allocator);
    // template <typename T, typename U, class V> void unwrapRow(JavaVM * jvm_, const bool methodOfConv, const int javaID, const ScilabStringStackAllocator & allocator);
    // template <typename T, typename U, class V> void unwrapSingle(JavaVM * jvm_, const bool methodOfConv, const int javaID, const ScilabStringStackAllocator & allocator);


    inline void getAccessibleFields(int id, const ScilabStringStackAllocator & allocator, const bool isField)
    {
    }

    static inline std::vector<char *> breakInLines(const std::string & str)
    {
        std::vector<char *> buf;
        std::size_t prev(0);
        std::size_t pos = str.find_first_of("\n");
        while (pos != std::string::npos)
        {
            buf.push_back(os_strdup(str.substr(prev, pos - prev).c_str()));
            prev = pos + 1;
            pos = str.find_first_of("\n", prev);
        }
        buf.push_back(os_strdup(str.substr(prev).c_str()));

        std::vector<char *>::iterator last = buf.end();
        for (std::vector<char *>::iterator i = buf.end() - 1; i >= buf.begin(); i--)
        {
            if (strlen(*i) == 0)
            {
                last = i;
                free(*i);
            }
            else
            {
                break;
            }
        }
        buf.erase(last, buf.end());

        return buf;
    }

    static inline const char * getOpNameFromType(const OperatorsType type)
    {
        switch (type)
        {
            case Transp :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: \'"));
            case Add :
                return "add";
            case Sub :
                return "sub";
            case Mul :
                return "mul";
            case Div :
                return "div";
            case Backslash :
                return "truediv";
            case Pow :
                return "pow";
            case DotMul :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: .*"));
            case DotDiv :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: ./"));
            case DotBackslash :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: .\\"));
            case DotMulDot :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: .*."));
            case DotDivDot :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: ./."));
            case DotBackslashDot :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: .\\."));
            case Eq :
                return "eq";
            case Neq :
                return "ne";
            case Or :
                return "or";
            case And :
                return "and";
            case DotPow :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: .^"));
            case Not :
                return "not_";
            case DotTransp :
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid operator: .\'"));
            case Lt :
                return "lt";
            case Gt :
                return "gt";
            case Leq :
                return "le";
            case Geq :
                return "ge";
        }

        return "";
    }
};

}

#endif // __SCILABJAVAENVIRONMENT_HXX__
