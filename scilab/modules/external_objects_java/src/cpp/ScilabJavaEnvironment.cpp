/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include "windows.h"
#endif

#include <jni.h>

#include "ScilabJavaEnvironment.hxx"
#include "JavaOptionsHelper.hxx"
#include "ScilabClassLoader.hxx"
#include "ScilabJavaClass.hxx"
#include "ScilabJavaObject.hxx"
#include "ScilabJavaArray.hxx"
#include "ScilabJavaCompiler.hxx"
#include "NoMoreScilabMemoryException.hxx"
#include "ScilabAutoCleaner.hxx"

//#include "ScilabJavaObjectHelper.hxx"
extern "C" {
#include "getScilabJavaVM.h"
#include "tmpdir.h"
}

namespace org_scilab_modules_external_objects_java
{

const std::string ScilabJavaEnvironment::environmentName = "Java Environment";
int ScilabJavaEnvironment::envId = -1;
ScilabJavaEnvironment * ScilabJavaEnvironment::instance = 0;
bool ScilabJavaEnvironment::usable = true;

ScilabJavaEnvironment::ScilabJavaEnvironment() :
    helper(*new JavaOptionsHelper()),
    gwOptions(*new ScilabGatewayOptions()),
    wrapper(*new ScilabJavaEnvironmentWrapper(helper)),
    traceEnabled(false),
    isInit(false),
    scilabStream(*new ScilabStream()),
    file(0) { }

//    ScilabJavaEnvironment::ScilabJavaEnvironment() {}
ScilabJavaEnvironment::~ScilabJavaEnvironment()
{
    //    delete &scope;
    delete &helper;
    delete &gwOptions;
    delete &wrapper;

    if (file)
    {
        file->flush();
        file->close();
        delete file;
        file = 0;
    }
}

int ScilabJavaEnvironment::start()
{
    /*    if (!usable)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Due to Java interpreter limitations, Scilab must be restarted"));
            }*/

    if (envId == -1)
    {
        instance = new ScilabJavaEnvironment();
        envId = ScilabEnvironments::registerScilabEnvironment(instance);
        instance->Initialize();
        /*        ScilabJavaOStream::initJavaStreams();
                ScilabJavaOStream::setStdOutStream(&instance->scilabStream);
                ScilabJavaOStream::setStdErrStream(&instance->scilabStream);*/
        instance->helper.setUseLastName(true);
        instance->helper.setNewAllowed(true);
        instance->enabletrace((std::string(getTMPDIR()) + std::string("/eo_java.log")).c_str());
    }

    return envId;
}

void ScilabJavaEnvironment::finish()
{
    if (envId != -1)
    {
        ScilabEnvironments::unregisterScilabEnvironment(envId);
        envId = -1;
        delete instance;
        instance = 0;
        instance->Finalize();
        usable = false;
    }
}

void ScilabJavaEnvironment::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        // No need to init the Java VM. Scilab is already using it
    }
}

void ScilabJavaEnvironment::Finalize()
{
    // Scilab cannot kill the Java VM. It would probably crash the application
}

JavaOptionsHelper & ScilabJavaEnvironment::getOptionsHelper()
{
    return helper;
}

ScilabGatewayOptions & ScilabJavaEnvironment::getGatewayOptions()
{
    return gwOptions;
}

ScilabAbstractEnvironmentWrapper & ScilabJavaEnvironment::getWrapper()
{
    return wrapper;
}

const std::string & ScilabJavaEnvironment::getEnvironmentName()
{
    return environmentName;
}

void ScilabJavaEnvironment::getEnvironmentInfos(const ScilabStringStackAllocator & allocator)
{

    /*
        std::vector<char *> version = breakInLines(std::string(Py_GetVersion()));
        std::vector<char *> platform = breakInLines(std::string(Py_GetPlatform()));
        std::vector<char *> copyright = breakInLines(std::string(Py_GetCopyright()));
        std::vector<char *> compiler = breakInLines(std::string(Py_GetCompiler()));
        std::vector<char *> buildInfo = breakInLines(std::string(Py_GetBuildInfo()));

        int nbRows = version.size() + platform.size() + copyright.size() + compiler.size() + buildInfo.size();

        std::vector<char *> all(nbRows, const_cast<char* >(""));
        all[0] = const_cast<char* >("Version");
        all[version.size()] = const_cast<char* >("Platform");
        all[version.size() + platform.size()] = const_cast<char* >("Copyright");
        all[version.size() + platform.size() + copyright.size()] = const_cast<char* >("Compiler");
        all[version.size() + platform.size() + copyright.size() + compiler.size()] = const_cast<char* >("Build info");

        all.insert(all.end(), version.begin(), version.end());
        all.insert(all.end(), platform.begin(), platform.end());
        all.insert(all.end(), copyright.begin(), copyright.end());
        all.insert(all.end(), compiler.begin(), compiler.end());
        all.insert(all.end(), buildInfo.begin(), buildInfo.end());

        allocator.allocate(nbRows, 2, &(all[0]));
    */
}

int ScilabJavaEnvironment::extract(int id, int * args, int argsSize)
{
    JavaVM * vm = getScilabJavaVM();
    const int ret = ScilabJavaObject::extract(vm, id, args, argsSize);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

void ScilabJavaEnvironment::insert(int id, int * args, int argsSize)
{
    /*
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        if (argsSize != 2)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot insert more than one element in a dictionary"));
        }

        PyObject * key = scope.getObject(args[0]);
        if (!obj)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid key object"));
        }

        PyObject * value = scope.getObject(args[1]);
        if (!value)
        {
            throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid value object"));
        }

        if (PyDict_Check(obj))
        {
            PyDict_SetItem(obj, key, value);

            writeLog("insert", "success.");

            return;
        }
        else
        {
            if (!PyString_Check(key))
            {
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid key object: A string expected"));
            }

            int ret = PyObject_SetAttr(obj, key, value);
            if (ret == -1)
            {
                if (PyErr_Occurred())
                {
                    PyObject *type, *value, *traceback;
                    PyErr_Fetch(&type, &value, &traceback);
                    PyErr_NormalizeException(&type, &value, &traceback);
                    PyErr_Clear();

                    throw ScilabJavaException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot evaluate the code"));
                }
                throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot set the attribute."));
            }

            writeLog("insert", "success.");

            return;
        }
    */
    throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot insert in Java object"));
}

void ScilabJavaEnvironment::garbagecollect() { }

void ScilabJavaEnvironment::addtoclasspath(const char * path)
{
    /*
        PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
        PyObject * _path = PyString_FromString(path);
        PyList_Append(syspath, _path);
    */
}

void ScilabJavaEnvironment::getclasspath(const ScilabStringStackAllocator & allocator)
{
    /*
        PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
        int size = PyList_Size(syspath);
        char ** arr = new char*[size];

        for (int i = 0; i < size; i++)
        {
            PyObject * item = PyList_GetItem(syspath, i);
            arr[i] = strdup(PyString_AsString(item));
        }

        allocator.allocate(size, 1, arr);
        for (int i = 0; i < size; i++)
        {
            free(arr[i]);
        }
        delete arr;
    */
}

void ScilabJavaEnvironment::addNamedVariable(int id, const char * varName)
{
    // Useless in Java environment
}

int ScilabJavaEnvironment::getNamedVariable(const char * varName)
{
    return 0;
}

void ScilabJavaEnvironment::evalString(const char ** code, int nbLines, ScilabStringStackAllocator * allocator)
{
    // Useless in Java (it is not a script language !)
}

int ScilabJavaEnvironment::createarray(char * className, int * dims, int len)
{
    JavaVM *vm = getScilabJavaVM();
    const int ret = ScilabJavaArray::newInstance(vm, className, dims, len);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

int ScilabJavaEnvironment::loadclass(char * className, char * currentSciPath, bool isNamedVarCreated, bool allowReload)
{
    JavaVM *vm = getScilabJavaVM();
    const int ret = ScilabClassLoader::loadJavaClass(vm, className, allowReload);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

void ScilabJavaEnvironment::getrepresentation(int id, const ScilabStringStackAllocator & allocator)
{
    JavaVM *vm = getScilabJavaVM();
    char *str = ScilabJavaObject::getRepresentation(vm, id);
    allocator.allocate(1, 1, &str);
}

std::string ScilabJavaEnvironment::getrepresentation(int id)
{
    JavaVM *vm = getScilabJavaVM();
    return std::string(ScilabJavaObject::getRepresentation(vm, id));
}

/* Used by jexists */
bool ScilabJavaEnvironment::isvalidobject(int id)
{
    JavaVM *vm = getScilabJavaVM();
    return ScilabJavaObject::isValidJavaObject(vm, id);
}

int ScilabJavaEnvironment::newinstance(int id, int * args, int argsSize)
{
    JavaVM *vm = getScilabJavaVM();
    const int ret = ScilabJavaClass::newInstance(vm, id, args, argsSize);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

int ScilabJavaEnvironment::operation(int idA, int idB, const OperatorsType type)
{
    // TODO: plug String concatenation and maybe others things like operations on double, int, ...

    return 0;
}

int * ScilabJavaEnvironment::invoke(int id, const char * methodName, int * args, int argsSize)
{
    // TODO: In Java, an array can be passed as a reference so we need to "return" it
    // for example, stream.read(buf, ...), the bytes are put in buf so we need to get it !

    JavaVM *vm = getScilabJavaVM();
    int * invokedId = new int[2];
    invokedId[0] = 1 ; //1 object returned
    invokedId[1] = ScilabJavaObject::invoke(vm, id, methodName, args, argsSize);

    ScilabAutoCleaner::registerVariable(envId, invokedId[1]);

    return invokedId;
}

void ScilabJavaEnvironment::setfield(int id, const char * fieldName, int idarg)
{
    if (*fieldName == '\0')
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    try
    {
        JavaVM * vm = getScilabJavaVM();
        ScilabJavaObject::setField(vm, id, fieldName, idarg);
    }
    catch (const GiwsException::JniCallMethodException & e)
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Cannot set the field: %s"), fieldName);
    }
}

int ScilabJavaEnvironment::getfield(int id, const char * fieldName)
{
    if (*fieldName == '\0')
    {
        throw ScilabJavaException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    JavaVM * vm = getScilabJavaVM();
    const int ret = ScilabJavaObject::getField(vm, id, fieldName);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

int ScilabJavaEnvironment::getfieldtype(int id, const char * fieldName)
{
    JavaVM * vm = getScilabJavaVM();
    return ScilabJavaObject::getFieldType(vm, id, fieldName);
}

int ScilabJavaEnvironment::getarrayelement(int id, int * index, int length)
{
    JavaVM * vm = getScilabJavaVM();
    const int ret = ScilabJavaObject::getArrayElement(vm, id, index, length);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

void ScilabJavaEnvironment::setarrayelement(int id, int * index, int length, int idArg)
{
    JavaVM * vm = getScilabJavaVM();
    ScilabJavaObject::setArrayElement(vm, id, index, length, idArg);
}

int ScilabJavaEnvironment::cast(int id, char * className)
{
    JavaVM *vm = getScilabJavaVM();
    const int ret = ScilabJavaObject::javaCast(vm, id, className);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

int ScilabJavaEnvironment::castwithid(int id, int classId)
{
    JavaVM *vm = getScilabJavaVM();
    const int ret = ScilabJavaObject::javaCast(vm, id, classId);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

void ScilabJavaEnvironment::removeobject(int id)
{
    JavaVM *vm = getScilabJavaVM();
    ScilabJavaObject::removeScilabJavaObject(vm, id);
    ScilabAutoCleaner::unregisterVariable(envId, id);
}

void ScilabJavaEnvironment::autoremoveobject(int id)
{
    JavaVM *vm = getScilabJavaVM();
    ScilabJavaObject::removeScilabJavaObject(vm, id);
}

void ScilabJavaEnvironment::getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator)
{
    JavaVM *vm = getScilabJavaVM();
    getMethodResult(vm, "getAccessibleMethods", id, allocator);
}

void ScilabJavaEnvironment::getaccessiblefields(int id, const ScilabStringStackAllocator & allocator)
{
    JavaVM *vm = getScilabJavaVM();
    getMethodResult(vm, "getAccessibleFields", id, allocator);
    getAccessibleFields(id, allocator, true);
}

std::vector<std::string> ScilabJavaEnvironment::getCompletion(int id, char ** fieldsPath, const int fieldPathLen)
{
    JavaVM * vm = getScilabJavaVM();
    int len = 0;
    char ** fields = ScilabJavaObject::getCompletion(vm, id, fieldsPath + 1 , fieldPathLen - 1, &len);
    std::vector<std::string> v;

    for (int i = 0; i < len; i++)
    {
        v.push_back(fields[i]);
    }

    return v;
}

std::string ScilabJavaEnvironment::getclassname(int id)
{
    JavaVM *vm = getScilabJavaVM();
    return std::string(ScilabJavaObject::getClassName(vm, id));
}

VariableType ScilabJavaEnvironment::isunwrappable(int id)
{
    return wrapper.isunwrappable(id);
}

int ScilabJavaEnvironment::compilecode(char * className, char ** code, int size)
{
    std::ostringstream os;
    for (int i = 0; i < size; i++)
    {
        os << code[i] << std::endl;
    }
    os.flush();

    JavaVM *vm = getScilabJavaVM();
    const int ret = ScilabJavaCompiler::compileCode(vm, className, code, size);

    ScilabAutoCleaner::registerVariable(envId, ret);

    return ret;
}

void ScilabJavaEnvironment::enabletrace(const char * filename)
{
    JavaVM *vm = getScilabJavaVM();
    ScilabJavaObject::enableTrace(vm, filename);
    traceEnabled = true;
}

void ScilabJavaEnvironment::disabletrace(void)
{
    JavaVM *vm = getScilabJavaVM();
    ScilabJavaObject::disableTrace(vm);
    traceEnabled = false;
}

void ScilabJavaEnvironment::writeLog(const std::string & fun, const std::string str, ...) const
{
    if (traceEnabled)
    {
        JavaVM * vm = getScilabJavaVM();
        char _str[LOG_BUFFER_SIZE];
        va_list args;

        va_start(args, str);
        vsnprintf(_str, LOG_BUFFER_SIZE, str.c_str(), args);
        va_end(args);

        ScilabJavaObject::writeLog(vm, _str);
    }
}

#define SCILABJAVAOBJECT "org/scilab/modules/external_objects_java/ScilabJavaObject"

void ScilabJavaEnvironment::getMethodResult(JavaVM * jvm_, const char * const methodName, int id, const ScilabStringStackAllocator & allocator)
{
    JNIEnv * curEnv = NULL;
    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(SCILABJAVAOBJECT);
    if (cls == NULL)
    {
        throw GiwsException::JniClassNotFoundException(curEnv, SCILABJAVAOBJECT);
    }

    jmethodID jobjectArray_getAccessibleMethodsjintID = curEnv->GetStaticMethodID(cls, methodName, "(I)[Ljava/lang/String;");
    if (jobjectArray_getAccessibleMethodsjintID == NULL)
    {
        throw GiwsException::JniMethodNotFoundException(curEnv, methodName);
    }

    jobjectArray res = static_cast<jobjectArray>(curEnv->CallStaticObjectMethod(cls, jobjectArray_getAccessibleMethodsjintID, id));
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
    jint lenRow = curEnv->GetArrayLength(res);
    jboolean isCopy = JNI_FALSE;

    char **addr = new char*[lenRow];
    jstring *resString = new jstring[lenRow];

    for (jsize i = 0; i < lenRow; i++)
    {
        resString[i] = reinterpret_cast<jstring>(curEnv->GetObjectArrayElement(res, i));
        addr[i] = const_cast<char *>(curEnv->GetStringUTFChars(resString[i], &isCopy));
    }
    int lenCol = lenRow == 0 ? 0 : 1;
    allocator.allocate(lenRow, lenCol, addr);
    /*
            SciErr err = createMatrixOfString(pvApiCtx, pos, lenCol, lenRow, addr);

            for (jsize i = 0; i < lenRow; i++)
            {
                curEnv->ReleaseStringUTFChars(resString[i], addr[i]);
                curEnv->DeleteLocalRef(resString[i]);
                }*/
    delete[] addr;
    delete[] resString;

    // if (err.iErr)
    // {
    //     throw org_scilab_modules_external_objects_java::NoMoreScilabMemoryException();
    // }

    curEnv->DeleteLocalRef(res);
    curEnv->DeleteLocalRef(cls);
    if (curEnv->ExceptionCheck())
    {
        throw GiwsException::JniCallMethodException(curEnv);
    }
};
}
