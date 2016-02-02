/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Igor GRIDCHYN
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

#ifndef __SCILABABSTRACTENVIRONMENT_HXX__
#define __SCILABABSTRACTENVIRONMENT_HXX__

#include <string>
#include <vector>

#include "ExternalObjects.h"
#include "OptionsHelper.hxx"
#include "ScilabGatewayOptions.hxx"
#include "ScilabAbstractEnvironmentWrapper.hxx"
#include "ScilabAbstractEnvironmentException.hxx"
#include "ScilabAbstractMemoryAllocator.hxx"

namespace org_modules_external_objects
{
class ScilabAbstractEnvironment
{

public:

    virtual OptionsHelper & getOptionsHelper() = 0;

    virtual ScilabGatewayOptions & getGatewayOptions() = 0;

    virtual ScilabAbstractEnvironmentWrapper & getWrapper() = 0;

    virtual const std::string & getEnvironmentName() = 0;

    virtual void addtoclasspath(const char * path) = 0;

    virtual void getclasspath(const ScilabStringStackAllocator & allocator) = 0;

    virtual void getEnvironmentInfos(const ScilabStringStackAllocator & allocator) = 0;

    virtual int extract(int id, int * args, int argsSize) = 0;

    virtual void insert(int id, int * args, int argsSize) = 0;

    virtual void addNamedVariable(int id, const char * varName) = 0;

    virtual int getNamedVariable(const char * varName) = 0;

    virtual void evalString(const char ** code, int nbLines, ScilabStringStackAllocator * allocator) = 0;

    /**
     * Launch a garbage collection
     */
    virtual void garbagecollect() = 0;

    /**
     * Create an multi-dimensional array with base type given by className
     * @param className the name of the base class
     * @param dims an integer array containing the dimensions
     * @param len the number of dimensions
     * @return the id of the corresponding object
     */
    virtual int createarray(char * className, int * dims, int len) = 0;

    /**
     * Load a class with the given name
     * @param className the name of the class
     * @param allowReload if true the class should be reload (in taking into account its eventual modification)
     * @return the id of the corresponding object
     */
    virtual int loadclass(char * className, char * currentSciPath, bool isNamedVarCreated, bool allowReload) = 0;

    /**
     * Get the String representation of the corresponding object (as returned by the method toString() in Java)
     * @param id the id of the corresponding object
     * @return the correspinding string
     */
    virtual void getrepresentation(int id, const ScilabStringStackAllocator & allocator) = 0;

    /**
     * Get the String representation of the corresponding object (as returned by the method toString() in Java)
     * @param id the id of the corresponding object
     * @return the correspinding string
     */
    virtual std::string getrepresentation(int id) = 0;

    /**
     * Test the validity of the corresponding object
     * @param id the id of the object
     * @return true if the object is valid
     */
    virtual bool isvalidobject(int id) = 0;

    /**
     * Create a new instance of the class object with the given id
     * @param id the id of a class object
     * @param args an array containing the id of the arguments
     * @param argsSize the number of arguments
     * @return the id of the newly created object
     */
    virtual int newinstance(int id, int * args, int argsSize) = 0;

    virtual int operation(int idA, int idB, const OperatorsType type) = 0;

    /**
     * Invoke the method named methodName of the object with the corresponding id
     * @param id the object id
     * @param methodName the method name
     * @param args an array containing the id of the corresponding arguments
     * @param argsSize the number of argument
     * @return an array containing the returned values, the first entry is the array length (the returned pointer will be freed)
     */
    virtual int * invoke(int id, const char * methodName, int * args, int argsSize) = 0;

    /**
     * Set the value of the given field
     * @param id the object id
     * @param fieldName the field name
     * @param idarg the id of the value
     */
    virtual void setfield(int id, const char * fieldName, int idarg) = 0;

    /**
     * Get a field value
     * @param id the object id
     * @param fieldName the field name
     * @return the id of the field value
     */
    virtual int getfield(int id, const char * fieldName) = 0;

    /**
     * Get the field type
     * @param id the object id
     * @param fieldName the field name
     * @return -1 if unknown field, 0 if fieldName is a method name and 1 if the field name is a field name !
     */
    virtual int getfieldtype(int id, const char * fieldName) = 0;

    /**
     * Get a multi-dimensional array element
     * @param id the array id
     * @param index the index of the element
     * @param length the number of index
     * @return the id of the corresponding object
     */
    virtual int getarrayelement(int id, int * index, int length) = 0;

    /**
     * Set an element in a multi-dimensional array
     * @param id the array id
     * @param index the index of the element
     * @param length the number of index
     * @param idArg the id of the object to set
     */
    virtual void setarrayelement(int id, int * index, int length, int idArg) = 0;

    /**
     * Cast a given object into an object with the given class name
     * @param id the object id
     * @param className the name of the target class
     * @return the id of the newly casted object
     */
    virtual int cast(int id, char * className) = 0;

    /**
     * Cast a given object into an object with the given class object
     * @param id the object id
     * @param classId the id of the target class
     * @return the id of the newly casted object
     */
    virtual int castwithid(int id, int classId) = 0;

    /**
     * Remove the object with the given id
     * @param id the object id
     */
    virtual void removeobject(int id) = 0;

    /**
     * Remove the object with the given id but without unregister it in ScilabAutoCleaner
     * @param id the object id
     */
    virtual void autoremoveobject(int id) = 0;

    /**
     * Get all the accessible methods (public ones) in the given object
     * @param id the object id
     * @param pos the position in the Scilab stack where to put the names
     */
    virtual void getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator) = 0;

    /**
     * Get all the accessible methods (public ones) in the given object (used in completion)
     * @param id the object id
     * @return the methods
     */
    virtual std::vector<std::string> getCompletion(int id, char ** fieldPath, const int fieldPathLen)
    {
        return std::vector<std::string>();
    }

    /**
     * Get all the accessible fields (public ones) in the given object
     * @param id the object id
     * @param pos the position in the Scilab stack where to put the names
     */
    virtual void getaccessiblefields(int id, const ScilabStringStackAllocator & allocator) = 0;

    /**
     * Get the class name of the given object
     * @param id the object id
     * @return the class name
     */
    virtual std::string getclassname(int id) = 0;

    /**
     * Test if the given object can be unwrapped (as a Scilab variable)
     * @param id the object id
     * @return the type of the variable
     */
    virtual VariableType isunwrappable(int id) = 0;

    /**
     * Compile the given code in a class with the givne class name
     * @param className the class name
     * @param code an array of char* null terminated with the code to compile
     * @param size the number of lines
     * @return the id of the class newly compiled
     */
    virtual int compilecode(char * className, char ** code, int size) = 0;

    virtual void enabletrace(const char * filename) = 0;

    virtual void disabletrace() = 0;

    virtual void writeLog(const std::string & fun, const std::string str, ...) const = 0;

    /**
     * Remove the object with the given id
     * @param id the object id
     */
    virtual void removeobject(const int * ids, const int length)
    {
        for (int i = 0; i < length; i++)
        {
            removeobject(ids[i]);
        }
    }
};
}
#endif //__SCILABABSTRACTENVIRONMENT_HXX__
