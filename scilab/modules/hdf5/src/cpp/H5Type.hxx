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

#ifndef __H5TYPE_HXX__
#define __H5TYPE_HXX__

#include <map>

#include "H5Object.hxx"
#include "H5File.hxx"

namespace org_modules_hdf5
{

class H5Type : public H5Object
{
    hid_t type;

    static std::map<std::string, hid_t> nameToType;

public:

    H5Type(H5Object & _parent, hid_t type);
    H5Type(H5Object & _parent, const std::string & name);
    H5Type(H5Object & _parent, hid_t type, const std::string & name);

    virtual ~H5Type();

    virtual hid_t getH5Id() const
    {
        return type;
    }

    virtual bool isType() const
    {
        return true;
    }

    virtual std::string getClassName() const;
    virtual std::string getTypeName() const;
    virtual unsigned int getTypeSize() const;
    virtual std::string getNativeTypeName() const;
    virtual unsigned int getNativeTypeSize() const;

    virtual void getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;
    virtual std::string toString(const unsigned int indentLevel) const;
    virtual void printLsInfo(std::ostringstream & os) const;
    virtual std::string ls() const;

    static std::string getNameFromType(hid_t type);
    static void printComplexNameFromType(std::ostringstream & os, const unsigned int indentLevel, const hid_t type);

    inline static hid_t getBaseType(double * data)
    {
        return H5Tcopy(H5T_NATIVE_DOUBLE);
    }

    inline static hid_t getBaseType(float * data)
    {
        return H5Tcopy(H5T_NATIVE_FLOAT);
    }

    inline static hid_t getBaseType(char * data)
    {
        return H5Tcopy(H5T_NATIVE_CHAR);
    }

    inline static hid_t getBaseType(unsigned char * data)
    {
        return H5Tcopy(H5T_NATIVE_UCHAR);
    }

    inline static hid_t getBaseType(short * data)
    {
        return H5Tcopy(H5T_NATIVE_SHORT);
    }

    inline static hid_t getBaseType(unsigned short * data)
    {
        return H5Tcopy(H5T_NATIVE_USHORT);
    }

    inline static hid_t getBaseType(int * data)
    {
        return H5Tcopy(H5T_NATIVE_INT);
    }

    inline static hid_t getBaseType(unsigned int * data)
    {
        return H5Tcopy(H5T_NATIVE_UINT);
    }

    inline static hid_t getBaseType(long long * data)
    {
        return H5Tcopy(H5T_NATIVE_LLONG);
    }

    inline static hid_t getBaseType(unsigned long long * data)
    {
        return H5Tcopy(H5T_NATIVE_ULLONG);
    }

    inline static hid_t getBaseType(char ** data)
    {
        hid_t type = H5Tcopy(H5T_C_S1);
        herr_t err = H5Tset_size(type, H5T_VARIABLE);

        return type;
    }

    static hid_t getBaseType(const std::string & type)
    {
        std::string upper(type);
        std::transform(type.begin(), type.end(), upper.begin(), toupper);
        std::map<std::string, hid_t>::iterator it;

        if (upper.find_first_of("H5T_") == std::string::npos)
        {
            upper = "H5T_" + upper;
        }

        it = nameToType.find(upper);
        if (it != nameToType.end())
        {
            return H5Tcopy(it->second);
        }

        return (hid_t) - 1;
    }

    static std::map<std::string, hid_t> initMap()
    {
        std::map<std::string, hid_t> nameToType;

        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I8BE", H5T_STD_I8BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I8LE", H5T_STD_I8LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I16BE", H5T_STD_I16BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I16LE", H5T_STD_I16LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I32BE", H5T_STD_I32BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I32LE", H5T_STD_I32LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I64BE", H5T_STD_I64BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_I64LE", H5T_STD_I64LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U8BE", H5T_STD_U8BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U8LE", H5T_STD_U8LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U16BE", H5T_STD_U16BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U16LE", H5T_STD_U16LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U32BE", H5T_STD_U32BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U32LE", H5T_STD_U32LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U64BE", H5T_STD_U64BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_U64LE", H5T_STD_U64LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_SCHAR", H5T_NATIVE_SCHAR));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UCHAR", H5T_NATIVE_UCHAR));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_SHORT", H5T_NATIVE_SHORT));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_USHORT", H5T_NATIVE_USHORT));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT", H5T_NATIVE_INT));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT", H5T_NATIVE_UINT));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_LONG", H5T_NATIVE_LONG));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_ULONG", H5T_NATIVE_ULONG));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_LLONG", H5T_NATIVE_LLONG));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_ULLONG", H5T_NATIVE_ULLONG));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT8", H5T_NATIVE_INT8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT8", H5T_NATIVE_UINT8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_LEAST8", H5T_NATIVE_INT_LEAST8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_LEAST8", H5T_NATIVE_UINT_LEAST8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_FAST8", H5T_NATIVE_INT_FAST8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_FAST8", H5T_NATIVE_UINT_FAST8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT16", H5T_NATIVE_INT16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT16", H5T_NATIVE_UINT16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_LEAST16", H5T_NATIVE_INT_LEAST16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_LEAST16", H5T_NATIVE_UINT_LEAST16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_FAST16", H5T_NATIVE_INT_FAST16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_FAST16", H5T_NATIVE_UINT_FAST16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT32", H5T_NATIVE_INT32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT32", H5T_NATIVE_UINT32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_LEAST32", H5T_NATIVE_INT_LEAST32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_LEAST32", H5T_NATIVE_UINT_LEAST32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_FAST32", H5T_NATIVE_INT_FAST32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_FAST32", H5T_NATIVE_UINT_FAST32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT64", H5T_NATIVE_INT64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT64", H5T_NATIVE_UINT64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_LEAST64", H5T_NATIVE_INT_LEAST64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_LEAST64", H5T_NATIVE_UINT_LEAST64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_INT_FAST64", H5T_NATIVE_INT_FAST64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_UINT_FAST64", H5T_NATIVE_UINT_FAST64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_I8", H5T_INTEL_I8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_I16", H5T_INTEL_I16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_I32", H5T_INTEL_I32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_I64", H5T_INTEL_I64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_U8", H5T_INTEL_U8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_U16", H5T_INTEL_U16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_U32", H5T_INTEL_U32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_U64", H5T_INTEL_U64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_I8", H5T_ALPHA_I8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_I16", H5T_ALPHA_I16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_I32", H5T_ALPHA_I32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_I64", H5T_ALPHA_I64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_U8", H5T_ALPHA_U8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_U16", H5T_ALPHA_U16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_U32", H5T_ALPHA_U32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_U64", H5T_ALPHA_U64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_I8", H5T_MIPS_I8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_I16", H5T_MIPS_I16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_I32", H5T_MIPS_I32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_I64", H5T_MIPS_I64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_U8", H5T_MIPS_U8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_U16", H5T_MIPS_U16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_U32", H5T_MIPS_U32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_U64", H5T_MIPS_U64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_HADDR", H5T_NATIVE_HADDR));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_HSIZE", H5T_NATIVE_HSIZE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_HSSIZE", H5T_NATIVE_HSSIZE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_HERR", H5T_NATIVE_HERR));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_HBOOL", H5T_NATIVE_HBOOL));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_IEEE_F32BE", H5T_IEEE_F32BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_IEEE_F32LE", H5T_IEEE_F32LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_IEEE_F64BE", H5T_IEEE_F64BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_IEEE_F64LE", H5T_IEEE_F64LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_VAX_F32", H5T_VAX_F32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_VAX_F64", H5T_VAX_F64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_FLOAT", H5T_NATIVE_FLOAT));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_DOUBLE", H5T_NATIVE_DOUBLE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_NATIVE_LDOUBLE", H5T_NATIVE_LDOUBLE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_F32", H5T_INTEL_F32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_F64", H5T_INTEL_F64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_F32", H5T_ALPHA_F32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_F64", H5T_ALPHA_F64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_VAX_F32", H5T_VAX_F32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_VAX_F64", H5T_VAX_F64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_F32", H5T_MIPS_F32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_F64", H5T_MIPS_F64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D32BE", H5T_UNIX_D32BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D32LE", H5T_UNIX_D32LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D64BE", H5T_UNIX_D64BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D64LE", H5T_UNIX_D64LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B8BE", H5T_STD_B8BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B8LE", H5T_STD_B8LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B16BE", H5T_STD_B16BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B16LE", H5T_STD_B16LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B32BE", H5T_STD_B32BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B32LE", H5T_STD_B32LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B64BE", H5T_STD_B64BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_B64LE", H5T_STD_B64LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_B8", H5T_INTEL_B8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_B16", H5T_INTEL_B16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_B32", H5T_INTEL_B32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_INTEL_B64", H5T_INTEL_B64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_B8", H5T_ALPHA_B8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_B16", H5T_ALPHA_B16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_B32", H5T_ALPHA_B32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ALPHA_B64", H5T_ALPHA_B64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_B8", H5T_MIPS_B8));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_B16", H5T_MIPS_B16));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_B32", H5T_MIPS_B32));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_MIPS_B64", H5T_MIPS_B64));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STD_REF_OBJ", H5T_STD_REF_OBJ));
        /*
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D32BE", H5T_UNIX_D32BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D32LE", H5T_UNIX_D32LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D64BE", H5T_UNIX_D64BE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_UNIX_D64LE", H5T_UNIX_D64LE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_STRING", H5T_STRING));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_OPAQUE", H5T_OPAQUE));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_COMPOUND", H5T_COMPOUND));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ENUM", H5T_ENUM));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_VLEN", H5T_VLEN));
        nameToType.insert(std::pair<std::string, hid_t>("H5T_ARRAY", H5T_ARRAY));
        */

        return nameToType;
    }

    static void cleanMap()
    {
        nameToType.clear();
    }


private:
    void init();
};
}

#endif // __H5TYPE_HXX__
