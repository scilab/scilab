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

#include "H5Type.hxx"

namespace org_modules_hdf5
{

std::map<std::string, hid_t> H5Type::nameToType = initMap();

void H5Type::init()
{
    type = H5Topen(getParent().getH5Id(), name.c_str(), H5P_DEFAULT);
    if (type < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid H5Type name: %s."), name.c_str());
    }
}

H5Type::H5Type(H5Object & _parent, const hid_t _type) : H5Object(_parent), type(_type)
{

}

H5Type::H5Type(H5Object & _parent, const hid_t _type, const std::string & _name) : H5Object(_parent, _name), type(_type)
{

}

H5Type::H5Type(H5Object & _parent, const std::string & _name) : H5Object(_parent, _name)
{
    init();
}

H5Type::~H5Type()
{
    if (type >= 0)
    {
        H5Tclose(type);
    }
}

std::string H5Type::getClassName() const
{
    switch (H5Tget_class(type))
    {
        case H5T_INTEGER:
            return "integer";
        case H5T_FLOAT:
            return "float";
        case H5T_TIME:
            return "time";
        case H5T_STRING:
            return "string";
        case H5T_BITFIELD:
            return "bitfield";
        case H5T_OPAQUE:
            return "opaque";
        case H5T_COMPOUND:
            return "compound";
        case H5T_REFERENCE:
            return "reference";
        case H5T_ENUM:
            return "enum";
        case H5T_VLEN:
            return "vlen";
        case H5T_ARRAY:
            return "array";
        default:
            return "unknown";
    }
}

unsigned int H5Type::getTypeSize() const
{
    return (unsigned int)H5Tget_size(type);
}

std::string H5Type::getTypeName() const
{
    return getNameFromType(type);
}

unsigned int H5Type::getNativeTypeSize() const
{
    const hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);
    unsigned int size = (unsigned int)H5Tget_size(nativeType);
    H5Tclose(nativeType);

    return size;
}

std::string H5Type::getNativeTypeName() const
{
    const hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);
    std::string name = getNameFromType(nativeType);
    H5Tclose(nativeType);

    return name;
}

std::string H5Type::getNameFromType(hid_t type)
{
    std::string sorder, ssign;
    std::ostringstream os;
    H5T_sign_t sign;
    H5T_order_t order;

    switch (H5Tget_class(type))
    {
        case H5T_INTEGER:
            if (H5Tequal(type, H5T_STD_I8BE) > 0)
            {
                return "H5T_STD_I8BE";
            }
            else if (H5Tequal(type, H5T_STD_I8LE) > 0)
            {
                return "H5T_STD_I8LE";
            }
            else if (H5Tequal(type, H5T_STD_I16BE) > 0)
            {
                return "H5T_STD_I16BE";
            }
            else if (H5Tequal(type, H5T_STD_I16LE) > 0)
            {
                return "H5T_STD_I16LE";
            }
            else if (H5Tequal(type, H5T_STD_I32BE) > 0)
            {
                return "H5T_STD_I32BE";
            }
            else if (H5Tequal(type, H5T_STD_I32LE) > 0)
            {
                return "H5T_STD_I32LE";
            }
            else if (H5Tequal(type, H5T_STD_I64BE) > 0)
            {
                return "H5T_STD_I64BE";
            }
            else if (H5Tequal(type, H5T_STD_I64LE) > 0)
            {
                return "H5T_STD_I64LE";
            }
            else if (H5Tequal(type, H5T_STD_U8BE) > 0)
            {
                return "H5T_STD_U8BE";
            }
            else if (H5Tequal(type, H5T_STD_U8LE) > 0)
            {
                return "H5T_STD_U8LE";
            }
            else if (H5Tequal(type, H5T_STD_U16BE) > 0)
            {
                return "H5T_STD_U16BE";
            }
            else if (H5Tequal(type, H5T_STD_U16LE) > 0)
            {
                return "H5T_STD_U16LE";
            }
            else if (H5Tequal(type, H5T_STD_U32BE) > 0)
            {
                return "H5T_STD_U32BE";
            }
            else if (H5Tequal(type, H5T_STD_U32LE) > 0)
            {
                return "H5T_STD_U32LE";
            }
            else if (H5Tequal(type, H5T_STD_U64BE) > 0)
            {
                return "H5T_STD_U64BE";
            }
            else if (H5Tequal(type, H5T_STD_U64LE) > 0)
            {
                return "H5T_STD_U64LE";
            }
            else if (H5Tequal(type, H5T_NATIVE_SCHAR) > 0)
            {
                return "H5T_NATIVE_SCHAR";
            }
            else if (H5Tequal(type, H5T_NATIVE_UCHAR) > 0)
            {
                return "H5T_NATIVE_UCHAR";
            }
            else if (H5Tequal(type, H5T_NATIVE_SHORT) > 0)
            {
                return "H5T_NATIVE_SHORT";
            }
            else if (H5Tequal(type, H5T_NATIVE_USHORT) > 0)
            {
                return "H5T_NATIVE_USHORT";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT) > 0)
            {
                return "H5T_NATIVE_INT";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT) > 0)
            {
                return "H5T_NATIVE_UINT";
            }
            else if (H5Tequal(type, H5T_NATIVE_LONG) > 0)
            {
                return "H5T_NATIVE_LONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG) > 0)
            {
                return "H5T_NATIVE_ULONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_LLONG) > 0)
            {
                return "H5T_NATIVE_LLONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_ULLONG) > 0)
            {
                return "H5T_NATIVE_ULLONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT8) > 0)
            {
                return "H5T_NATIVE_INT8";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT8) > 0)
            {
                return "H5T_NATIVE_UINT8";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST8) > 0)
            {
                return "H5T_NATIVE_INT_LEAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST8) > 0)
            {
                return "H5T_NATIVE_UINT_LEAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST8) > 0)
            {
                return "H5T_NATIVE_INT_FAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST8) > 0)
            {
                return "H5T_NATIVE_UINT_FAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT16) > 0)
            {
                return "H5T_NATIVE_INT16";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT16) > 0)
            {
                return "H5T_NATIVE_UINT16";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST16) > 0)
            {
                return "H5T_NATIVE_INT_LEAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST16) > 0)
            {
                return "H5T_NATIVE_UINT_LEAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST16) > 0)
            {
                return "H5T_NATIVE_INT_FAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST16) > 0)
            {
                return "H5T_NATIVE_UINT_FAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT32) > 0)
            {
                return "H5T_NATIVE_INT32";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT32) > 0)
            {
                return "H5T_NATIVE_UINT32";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST32) > 0)
            {
                return "H5T_NATIVE_INT_LEAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST32) > 0)
            {
                return "H5T_NATIVE_UINT_LEAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST32) > 0)
            {
                return "H5T_NATIVE_INT_FAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST32) > 0)
            {
                return "H5T_NATIVE_UINT_FAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT64) > 0)
            {
                return "H5T_NATIVE_INT64";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT64) > 0)
            {
                return "H5T_NATIVE_UINT64";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST64) > 0)
            {
                return "H5T_NATIVE_INT_LEAST64";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST64) > 0)
            {
                return "H5T_NATIVE_UINT_LEAST64";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST64) > 0)
            {
                return "H5T_NATIVE_INT_FAST64";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST64) > 0)
            {
                return "H5T_NATIVE_UINT_FAST64";
            }
            else if (H5Tequal(type, H5T_INTEL_I8) > 0)
            {
                return "H5T_INTEL_I8";
            }
            else if (H5Tequal(type, H5T_INTEL_I16) > 0)
            {
                return "H5T_INTEL_I16";
            }
            else if (H5Tequal(type, H5T_INTEL_I32) > 0)
            {
                return "H5T_INTEL_I32";
            }
            else if (H5Tequal(type, H5T_INTEL_I64) > 0)
            {
                return "H5T_INTEL_I64";
            }
            else if (H5Tequal(type, H5T_INTEL_U8) > 0)
            {
                return "H5T_INTEL_U8";
            }
            else if (H5Tequal(type, H5T_INTEL_U16) > 0)
            {
                return "H5T_INTEL_U16";
            }
            else if (H5Tequal(type, H5T_INTEL_U32) > 0)
            {
                return "H5T_INTEL_U32";
            }
            else if (H5Tequal(type, H5T_INTEL_U64) > 0)
            {
                return "H5T_INTEL_U64";
            }
            else if (H5Tequal(type, H5T_ALPHA_I8) > 0)
            {
                return "H5T_ALPHA_I8";
            }
            else if (H5Tequal(type, H5T_ALPHA_I16) > 0)
            {
                return "H5T_ALPHA_I16";
            }
            else if (H5Tequal(type, H5T_ALPHA_I32) > 0)
            {
                return "H5T_ALPHA_I32";
            }
            else if (H5Tequal(type, H5T_ALPHA_I64) > 0)
            {
                return "H5T_ALPHA_I64";
            }
            else if (H5Tequal(type, H5T_ALPHA_U8) > 0)
            {
                return "H5T_ALPHA_U8";
            }
            else if (H5Tequal(type, H5T_ALPHA_U16) > 0)
            {
                return "H5T_ALPHA_U16";
            }
            else if (H5Tequal(type, H5T_ALPHA_U32) > 0)
            {
                return "H5T_ALPHA_U32";
            }
            else if (H5Tequal(type, H5T_ALPHA_U64) > 0)
            {
                return "H5T_ALPHA_U64";
            }
            else if (H5Tequal(type, H5T_MIPS_I8) > 0)
            {
                return "H5T_MIPS_I8";
            }
            else if (H5Tequal(type, H5T_MIPS_I16) > 0)
            {
                return "H5T_MIPS_I16";
            }
            else if (H5Tequal(type, H5T_MIPS_I32) > 0)
            {
                return "H5T_MIPS_I32";
            }
            else if (H5Tequal(type, H5T_MIPS_I64) > 0)
            {
                return "H5T_MIPS_I64";
            }
            else if (H5Tequal(type, H5T_MIPS_U8) > 0)
            {
                return "H5T_MIPS_U8";
            }
            else if (H5Tequal(type, H5T_MIPS_U16) > 0)
            {
                return "H5T_MIPS_U16";
            }
            else if (H5Tequal(type, H5T_MIPS_U32) > 0)
            {
                return "H5T_MIPS_U32";
            }
            else if (H5Tequal(type, H5T_MIPS_U64) > 0)
            {
                return "H5T_MIPS_U64";
            }
            else if (H5Tequal(type, H5T_NATIVE_HADDR) > 0)
            {
                return "H5T_NATIVE_HADDR";
            }
            else if (H5Tequal(type, H5T_NATIVE_HSIZE) > 0)
            {
                return "H5T_NATIVE_HSIZE";
            }
            else if (H5Tequal(type, H5T_NATIVE_HSSIZE) > 0)
            {
                return "H5T_NATIVE_HSSIZE";
            }
            else if (H5Tequal(type, H5T_NATIVE_HERR) > 0)
            {
                return "H5T_NATIVE_HERR";
            }
            else if (H5Tequal(type, H5T_NATIVE_HBOOL) > 0)
            {
                return "H5T_NATIVE_HBOOL";
            }
            else
            {
                if (H5Tget_size(type) > 1)
                {
                    order = H5Tget_order(type);
                    if (H5T_ORDER_LE == order)
                    {
                        sorder = " little-endian";
                    }
                    else if (H5T_ORDER_BE == order)
                    {
                        sorder = " big-endian";
                    }
                    else if (H5T_ORDER_VAX == order)
                    {
                        sorder = " mixed-endian";
                    }
                    else
                    {
                        sorder = " unknown-byte-order";
                    }
                }
                else
                {
                    sorder = "";
                }

                sign = H5Tget_sign(type);
                if (sign >= 0)
                {
                    if (sign == H5T_SGN_NONE)
                    {
                        ssign = " unsigned";
                    }
                    else if (sign == H5T_SGN_2)
                    {
                        ssign = "";
                    }
                    else
                    {
                        ssign = " unknown-sign";
                    }
                }
                else
                {
                    ssign = " unknown-sign";
                }

                os << (unsigned long)(8 * H5Tget_size(type)) << "-bit"
                   << sorder
                   << ssign
                   << " integer";

                return os.str();
            }
            break;
        case H5T_FLOAT:
            if (H5Tequal(type, H5T_IEEE_F32BE) > 0)
            {
                return "H5T_IEEE_F32BE";
            }
            else if (H5Tequal(type, H5T_IEEE_F32LE) > 0)
            {
                return "H5T_IEEE_F32LE";
            }
            else if (H5Tequal(type, H5T_IEEE_F64BE) > 0)
            {
                return "H5T_IEEE_F64BE";
            }
            else if (H5Tequal(type, H5T_IEEE_F64LE) > 0)
            {
                return "H5T_IEEE_F64LE";
            }
            else if (H5Tequal(type, H5T_VAX_F32) > 0)
            {
                return "H5T_VAX_F32";
            }
            else if (H5Tequal(type, H5T_VAX_F64) > 0)
            {
                return "H5T_VAX_F64";
            }
            else if (H5Tequal(type, H5T_NATIVE_FLOAT) > 0)
            {
                return "H5T_NATIVE_FLOAT";
            }
            else if (H5Tequal(type, H5T_NATIVE_DOUBLE) > 0)
            {
                return "H5T_NATIVE_DOUBLE";
            }
            else if (H5Tequal(type, H5T_NATIVE_LDOUBLE) > 0)
            {
                return "H5T_NATIVE_LDOUBLE";
            }
            else if (H5Tequal(type, H5T_INTEL_F32) > 0)
            {
                return "H5T_INTEL_F32";
            }
            else if (H5Tequal(type, H5T_INTEL_F64) > 0)
            {
                return "H5T_INTEL_F64";
            }
            else if (H5Tequal(type, H5T_ALPHA_F32) > 0)
            {
                return "H5T_ALPHA_F32";
            }
            else if (H5Tequal(type, H5T_ALPHA_F64) > 0)
            {
                return "H5T_ALPHA_F64";
            }
            else if (H5Tequal(type, H5T_VAX_F32) > 0)
            {
                return "H5T_VAX_F32";
            }
            else if (H5Tequal(type, H5T_VAX_F64) > 0)
            {
                return "H5T_VAX_F64";
            }
            else if (H5Tequal(type, H5T_MIPS_F32) > 0)
            {
                return "H5T_MIPS_F32";
            }
            else if (H5Tequal(type, H5T_MIPS_F64) > 0)
            {
                return "H5T_MIPS_F64";
            }
            else
            {
                if (H5Tget_size(type) > 1)
                {
                    order = H5Tget_order(type);
                    if (order == H5T_ORDER_LE)
                    {
                        sorder = " little-endian";
                    }
                    else if (order == H5T_ORDER_BE)
                    {
                        sorder = " big-endian";
                    }
                    else if (order == H5T_ORDER_VAX)
                    {
                        sorder = " mixed-endian";
                    }
                    else
                    {
                        sorder = " unknown-byte-order";
                    }
                }
                else
                {
                    sorder = "";
                }

                os << (unsigned long)(8 * H5Tget_size(type)) << "-bit"
                   << sorder
                   << " floating-point";

                return os.str();
            }
            break;
        case H5T_TIME:
            if (H5Tequal(type, H5T_UNIX_D32BE) > 0)
            {
                return "H5T_UNIX_D32BE";
            }
            else if (H5Tequal(type, H5T_UNIX_D32LE) > 0)
            {
                return "H5T_UNIX_D32LE";
            }
            else if (H5Tequal(type, H5T_UNIX_D64BE) > 0)
            {
                return "H5T_UNIX_D64BE";
            }
            else if (H5Tequal(type, H5T_UNIX_D64LE) > 0)
            {
                return "H5T_UNIX_D64LE";
            }
            else
            {
                return "Unknown time format";
            }
            break;
        case H5T_STRING:
            return "H5T_STRING";
        case H5T_BITFIELD:
            if (H5Tequal(type, H5T_STD_B8BE) > 0)
            {
                return "H5T_STD_B8BE";
            }
            else if (H5Tequal(type, H5T_STD_B8LE) > 0)
            {
                return "H5T_STD_B8LE";
            }
            else if (H5Tequal(type, H5T_STD_B16BE) > 0)
            {
                return "H5T_STD_B16BE";
            }
            else if (H5Tequal(type, H5T_STD_B16LE) > 0)
            {
                return "H5T_STD_B16LE";
            }
            else if (H5Tequal(type, H5T_STD_B32BE) > 0)
            {
                return "H5T_STD_B32BE";
            }
            else if (H5Tequal(type, H5T_STD_B32LE) > 0)
            {
                return "H5T_STD_B32LE";
            }
            else if (H5Tequal(type, H5T_STD_B64BE) > 0)
            {
                return "H5T_STD_B64BE";
            }
            else if (H5Tequal(type, H5T_STD_B64LE) > 0)
            {
                return "H5T_STD_B64LE";
            }
            else if (H5Tequal(type, H5T_INTEL_B8) > 0)
            {
                return "H5T_INTEL_B8";
            }
            else if (H5Tequal(type, H5T_INTEL_B16) > 0)
            {
                return "H5T_INTEL_B16";
            }
            else if (H5Tequal(type, H5T_INTEL_B32) > 0)
            {
                return "H5T_INTEL_B32";
            }
            else if (H5Tequal(type, H5T_INTEL_B64) > 0)
            {
                return "H5T_INTEL_B64";
            }
            else if (H5Tequal(type, H5T_ALPHA_B8) > 0)
            {
                return "H5T_ALPHA_B8";
            }
            else if (H5Tequal(type, H5T_ALPHA_B16) > 0)
            {
                return "H5T_ALPHA_B16";
            }
            else if (H5Tequal(type, H5T_ALPHA_B32) > 0)
            {
                return "H5T_ALPHA_B32";
            }
            else if (H5Tequal(type, H5T_ALPHA_B64) > 0)
            {
                return "H5T_ALPHA_B64";
            }
            else if (H5Tequal(type, H5T_MIPS_B8) > 0)
            {
                return "H5T_MIPS_B8";
            }
            else if (H5Tequal(type, H5T_MIPS_B16) > 0)
            {
                return "H5T_MIPS_B16";
            }
            else if (H5Tequal(type, H5T_MIPS_B32) > 0)
            {
                return "H5T_MIPS_B32";
            }
            else if (H5Tequal(type, H5T_MIPS_B64) > 0)
            {
                return "H5T_MIPS_B64";
            }
            else
            {
                return "undefined bitfield";
            }
        case H5T_OPAQUE:
            return "H5T_OPAQUE";
        case H5T_COMPOUND:
            return "H5T_COMPOUND";
        case H5T_REFERENCE:
            if (H5Tequal(type, H5T_STD_REF_DSETREG) > 0)
            {
                return "H5T_STD_REF_DSETREG";
            }
            else
            {
                return "H5T_STD_REF_OBJECT";
            }
        case H5T_ENUM:
            return "H5T_ENUM";
        case H5T_VLEN:
            return "H5T_VLEN";
        case H5T_ARRAY:
            return "H5T_ARRAY";
        default:
            return _("Unknown datatype");
    }
    return _("Unknown datatype");
}

void H5Type::printComplexNameFromType(std::ostringstream & os, const unsigned int indentLevel, const hid_t type)
{
    std::string indent = H5Object::getIndentString(indentLevel);
    if (H5Tget_class(type) == H5T_COMPOUND)
    {
        unsigned int nmembers = H5Tget_nmembers(type);

        os << indent << "H5T_COMPOUND {" << std::endl;

        for (unsigned int i = 0; i < nmembers; i++)
        {
            char * mname = H5Tget_member_name(type, i);
            hid_t mtype = H5Tget_member_type(type, i);

            os << indent;
            H5Type::printComplexNameFromType(os, indentLevel + 1, mtype);
            os << " \"" << mname << "\";" << std::endl;
            os << std::endl;

            free(mname);
        }

        os << indent << "}";
    }
    else if (H5Tget_class(type) == H5T_ARRAY)
    {
        hid_t super = H5Tget_super(type);
        unsigned int ndims = H5Tget_array_ndims(type);
        hsize_t * dims = new hsize_t[ndims];
        H5Tget_array_dims(type, dims);

        os << indent << "H5T_ARRAY { ";

        for (unsigned int i = 0; i < ndims; i++)
        {
            os << "[" << (unsigned int)dims[i] << "]";
        }

        os << " ";
        H5Type::printComplexNameFromType(os, indentLevel + 1, super);
        os <<  " }";
        H5Tclose(super);
    }
    else if (H5Tget_class(type) == H5T_VLEN)
    {
        hid_t super = H5Tget_super(type);
        os << indent << "H5T_VLEN { ";
        H5Type::printComplexNameFromType(os, indentLevel + 1, super);
        os << " }";
        H5Tclose(super);
    }
    else
    {
        os << getNameFromType(type);
    }
}

void H5Type::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    SciErr err;
    std::string lower(_name);
    std::transform(_name.begin(), _name.end(), lower.begin(), tolower);

    if (lower == "class")
    {
        std::string _class = getClassName();
        const char * __class = _class.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &__class);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "type")
    {
        std::string type = getTypeName();
        const char * _type = type.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_type);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "size")
    {
        unsigned int size = getTypeSize();
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, 1, &size);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an integer on the stack."));
        }

        return;
    }
    else if (lower == "nativetype")
    {
        std::string type = getNativeTypeName();
        const char * _type = type.c_str();
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, &_type);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a string on the stack."));
        }

        return;
    }
    else if (lower == "nativesize")
    {
        unsigned int size = getNativeTypeSize();
        err = createMatrixOfUnsignedInteger32(pvApiCtx, pos, 1, 1, &size);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an integer on the stack."));
        }

        return;
    }

    H5Object::getAccessibleAttribute(_name, pos, pvApiCtx);
}

std::string H5Type::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    std::string sorder, ssign;
    std::ostringstream os;
    H5T_sign_t sign;
    hsize_t * dims = 0;
    unsigned int ndims;
    hid_t strType;
    hid_t super;
    hid_t native = -1;
    size_t size;
    size_t dstSize;
    unsigned int nmembers;
    H5T_order_t order;
    H5T_str_t strpad;
    H5T_cset_t cset;
    htri_t isVariableLength;
    std::string indent;
    char * opaqueTag = 0;
    char * value = 0;

    os << H5Object::getIndentString(indentLevel);

    if (!name.empty())
    {
        os << "DATATYPE \"" << name << "\" ";
    }
    else
    {
        os << "DATATYPE ";
    }

    switch (H5Tget_class(type))
    {
        case H5T_INTEGER:
            if (H5Tequal(type, H5T_STD_I8BE) > 0)
            {
                os << "H5T_STD_I8BE";
            }
            else if (H5Tequal(type, H5T_STD_I8LE) > 0)
            {
                os << "H5T_STD_I8LE";
            }
            else if (H5Tequal(type, H5T_STD_I16BE) > 0)
            {
                os << "H5T_STD_I16BE";
            }
            else if (H5Tequal(type, H5T_STD_I16LE) > 0)
            {
                os << "H5T_STD_I16LE";
            }
            else if (H5Tequal(type, H5T_STD_I32BE) > 0)
            {
                os << "H5T_STD_I32BE";
            }
            else if (H5Tequal(type, H5T_STD_I32LE) > 0)
            {
                os << "H5T_STD_I32LE";
            }
            else if (H5Tequal(type, H5T_STD_I64BE) > 0)
            {
                os << "H5T_STD_I64BE";
            }
            else if (H5Tequal(type, H5T_STD_I64LE) > 0)
            {
                os << "H5T_STD_I64LE";
            }
            else if (H5Tequal(type, H5T_STD_U8BE) > 0)
            {
                os << "H5T_STD_U8BE";
            }
            else if (H5Tequal(type, H5T_STD_U8LE) > 0)
            {
                os << "H5T_STD_U8LE";
            }
            else if (H5Tequal(type, H5T_STD_U16BE) > 0)
            {
                os << "H5T_STD_U16BE";
            }
            else if (H5Tequal(type, H5T_STD_U16LE) > 0)
            {
                os << "H5T_STD_U16LE";
            }
            else if (H5Tequal(type, H5T_STD_U32BE) > 0)
            {
                os << "H5T_STD_U32BE";
            }
            else if (H5Tequal(type, H5T_STD_U32LE) > 0)
            {
                os << "H5T_STD_U32LE";
            }
            else if (H5Tequal(type, H5T_STD_U64BE) > 0)
            {
                os << "H5T_STD_U64BE";
            }
            else if (H5Tequal(type, H5T_STD_U64LE) > 0)
            {
                os << "H5T_STD_U64LE";
            }
            else if (H5Tequal(type, H5T_NATIVE_SCHAR) > 0)
            {
                os << "H5T_NATIVE_SCHAR";
            }
            else if (H5Tequal(type, H5T_NATIVE_UCHAR) > 0)
            {
                os << "H5T_NATIVE_UCHAR";
            }
            else if (H5Tequal(type, H5T_NATIVE_SHORT) > 0)
            {
                os << "H5T_NATIVE_SHORT";
            }
            else if (H5Tequal(type, H5T_NATIVE_USHORT) > 0)
            {
                os << "H5T_NATIVE_USHORT";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT) > 0)
            {
                os << "H5T_NATIVE_INT";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT) > 0)
            {
                os << "H5T_NATIVE_UINT";
            }
            else if (H5Tequal(type, H5T_NATIVE_LONG) > 0)
            {
                os << "H5T_NATIVE_LONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG) > 0)
            {
                os << "H5T_NATIVE_ULONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_LLONG) > 0)
            {
                os << "H5T_NATIVE_LLONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_ULLONG) > 0)
            {
                os << "H5T_NATIVE_ULLONG";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT8) > 0)
            {
                os << "H5T_NATIVE_INT8";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT8) > 0)
            {
                os << "H5T_NATIVE_UINT8";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST8) > 0)
            {
                os << "H5T_NATIVE_INT_LEAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST8) > 0)
            {
                os << "H5T_NATIVE_UINT_LEAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST8) > 0)
            {
                os << "H5T_NATIVE_INT_FAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST8) > 0)
            {
                os << "H5T_NATIVE_UINT_FAST8";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT16) > 0)
            {
                os << "H5T_NATIVE_INT16";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT16) > 0)
            {
                os << "H5T_NATIVE_UINT16";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST16) > 0)
            {
                os << "H5T_NATIVE_INT_LEAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST16) > 0)
            {
                os << "H5T_NATIVE_UINT_LEAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST16) > 0)
            {
                os << "H5T_NATIVE_INT_FAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST16) > 0)
            {
                os << "H5T_NATIVE_UINT_FAST16";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT32) > 0)
            {
                os << "H5T_NATIVE_INT32";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT32) > 0)
            {
                os << "H5T_NATIVE_UINT32";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST32) > 0)
            {
                os << "H5T_NATIVE_INT_LEAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST32) > 0)
            {
                os << "H5T_NATIVE_UINT_LEAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST32) > 0)
            {
                os << "H5T_NATIVE_INT_FAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST32) > 0)
            {
                os << "H5T_NATIVE_UINT_FAST32";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT64) > 0)
            {
                os << "H5T_NATIVE_INT64";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT64) > 0)
            {
                os << "H5T_NATIVE_UINT64";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_LEAST64) > 0)
            {
                os << "H5T_NATIVE_INT_LEAST64";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_LEAST64) > 0)
            {
                os << "H5T_NATIVE_UINT_LEAST64";
            }
            else if (H5Tequal(type, H5T_NATIVE_INT_FAST64) > 0)
            {
                os << "H5T_NATIVE_INT_FAST64";
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT_FAST64) > 0)
            {
                os << "H5T_NATIVE_UINT_FAST64";
            }
            else if (H5Tequal(type, H5T_INTEL_I8) > 0)
            {
                os << "H5T_INTEL_I8";
            }
            else if (H5Tequal(type, H5T_INTEL_I16) > 0)
            {
                os << "H5T_INTEL_I16";
            }
            else if (H5Tequal(type, H5T_INTEL_I32) > 0)
            {
                os << "H5T_INTEL_I32";
            }
            else if (H5Tequal(type, H5T_INTEL_I64) > 0)
            {
                os << "H5T_INTEL_I64";
            }
            else if (H5Tequal(type, H5T_INTEL_U8) > 0)
            {
                os << "H5T_INTEL_U8";
            }
            else if (H5Tequal(type, H5T_INTEL_U16) > 0)
            {
                os << "H5T_INTEL_U16";
            }
            else if (H5Tequal(type, H5T_INTEL_U32) > 0)
            {
                os << "H5T_INTEL_U32";
            }
            else if (H5Tequal(type, H5T_INTEL_U64) > 0)
            {
                os << "H5T_INTEL_U64";
            }
            else if (H5Tequal(type, H5T_ALPHA_I8) > 0)
            {
                os << "H5T_ALPHA_I8";
            }
            else if (H5Tequal(type, H5T_ALPHA_I16) > 0)
            {
                os << "H5T_ALPHA_I16";
            }
            else if (H5Tequal(type, H5T_ALPHA_I32) > 0)
            {
                os << "H5T_ALPHA_I32";
            }
            else if (H5Tequal(type, H5T_ALPHA_I64) > 0)
            {
                os << "H5T_ALPHA_I64";
            }
            else if (H5Tequal(type, H5T_ALPHA_U8) > 0)
            {
                os << "H5T_ALPHA_U8";
            }
            else if (H5Tequal(type, H5T_ALPHA_U16) > 0)
            {
                os << "H5T_ALPHA_U16";
            }
            else if (H5Tequal(type, H5T_ALPHA_U32) > 0)
            {
                os << "H5T_ALPHA_U32";
            }
            else if (H5Tequal(type, H5T_ALPHA_U64) > 0)
            {
                os << "H5T_ALPHA_U64";
            }
            else if (H5Tequal(type, H5T_MIPS_I8) > 0)
            {
                os << "H5T_MIPS_I8";
            }
            else if (H5Tequal(type, H5T_MIPS_I16) > 0)
            {
                os << "H5T_MIPS_I16";
            }
            else if (H5Tequal(type, H5T_MIPS_I32) > 0)
            {
                os << "H5T_MIPS_I32";
            }
            else if (H5Tequal(type, H5T_MIPS_I64) > 0)
            {
                os << "H5T_MIPS_I64";
            }
            else if (H5Tequal(type, H5T_MIPS_U8) > 0)
            {
                os << "H5T_MIPS_U8";
            }
            else if (H5Tequal(type, H5T_MIPS_U16) > 0)
            {
                os << "H5T_MIPS_U16";
            }
            else if (H5Tequal(type, H5T_MIPS_U32) > 0)
            {
                os << "H5T_MIPS_U32";
            }
            else if (H5Tequal(type, H5T_MIPS_U64) > 0)
            {
                os << "H5T_MIPS_U64";
            }
            else if (H5Tequal(type, H5T_NATIVE_HADDR) > 0)
            {
                os << "H5T_NATIVE_HADDR";
            }
            else if (H5Tequal(type, H5T_NATIVE_HSIZE) > 0)
            {
                os << "H5T_NATIVE_HSIZE";
            }
            else if (H5Tequal(type, H5T_NATIVE_HSSIZE) > 0)
            {
                os << "H5T_NATIVE_HSSIZE";
            }
            else if (H5Tequal(type, H5T_NATIVE_HERR) > 0)
            {
                os << "H5T_NATIVE_HERR";
            }
            else if (H5Tequal(type, H5T_NATIVE_HBOOL) > 0)
            {
                os << "H5T_NATIVE_HBOOL";
            }
            else
            {
                if (H5Tget_size(type) > 1)
                {
                    order = H5Tget_order(type);
                    if (H5T_ORDER_LE == order)
                    {
                        sorder = " little-endian";
                    }
                    else if (H5T_ORDER_BE == order)
                    {
                        sorder = " big-endian";
                    }
                    else if (H5T_ORDER_VAX == order)
                    {
                        sorder = " mixed-endian";
                    }
                    else
                    {
                        sorder = " unknown-byte-order";
                    }
                }
                else
                {
                    sorder = "";
                }

                sign = H5Tget_sign(type);
                if (sign >= 0)
                {
                    if (sign == H5T_SGN_NONE)
                    {
                        ssign = " unsigned";
                    }
                    else if (sign == H5T_SGN_2)
                    {
                        ssign = "";
                    }
                    else
                    {
                        ssign = " unknown-sign";
                    }
                }
                else
                {
                    ssign = " unknown-sign";
                }

                os << (unsigned long)(8 * H5Tget_size(type)) << "-bit"
                   << sorder
                   << ssign
                   << " integer";
            }
            break;
        case H5T_FLOAT:
            if (H5Tequal(type, H5T_IEEE_F32BE) > 0)
            {
                os << "H5T_IEEE_F32BE";
            }
            else if (H5Tequal(type, H5T_IEEE_F32LE) > 0)
            {
                os << "H5T_IEEE_F32LE";
            }
            else if (H5Tequal(type, H5T_IEEE_F64BE) > 0)
            {
                os << "H5T_IEEE_F64BE";
            }
            else if (H5Tequal(type, H5T_IEEE_F64LE) > 0)
            {
                os << "H5T_IEEE_F64LE";
            }
            else if (H5Tequal(type, H5T_VAX_F32) > 0)
            {
                os << "H5T_VAX_F32";
            }
            else if (H5Tequal(type, H5T_VAX_F64) > 0)
            {
                os << "H5T_VAX_F64";
            }
            else if (H5Tequal(type, H5T_NATIVE_FLOAT) > 0)
            {
                os << "H5T_NATIVE_FLOAT";
            }
            else if (H5Tequal(type, H5T_NATIVE_DOUBLE) > 0)
            {
                os << "H5T_NATIVE_DOUBLE";
            }
            else if (H5Tequal(type, H5T_NATIVE_LDOUBLE) > 0)
            {
                os << "H5T_NATIVE_LDOUBLE";
            }
            else if (H5Tequal(type, H5T_INTEL_F32) > 0)
            {
                os << "H5T_INTEL_F32";
            }
            else if (H5Tequal(type, H5T_INTEL_F64) > 0)
            {
                os << "H5T_INTEL_F64";
            }
            else if (H5Tequal(type, H5T_ALPHA_F32) > 0)
            {
                os << "H5T_ALPHA_F32";
            }
            else if (H5Tequal(type, H5T_ALPHA_F64) > 0)
            {
                os << "H5T_ALPHA_F64";
            }
            else if (H5Tequal(type, H5T_VAX_F32) > 0)
            {
                os << "H5T_VAX_F32";
            }
            else if (H5Tequal(type, H5T_VAX_F64) > 0)
            {
                os << "H5T_VAX_F64";
            }
            else if (H5Tequal(type, H5T_MIPS_F32) > 0)
            {
                os << "H5T_MIPS_F32";
            }
            else if (H5Tequal(type, H5T_MIPS_F64) > 0)
            {
                os << "H5T_MIPS_F64";
            }
            else
            {
                if (H5Tget_size(type) > 1)
                {
                    order = H5Tget_order(type);
                    if (order == H5T_ORDER_LE)
                    {
                        sorder = " little-endian";
                    }
                    else if (order == H5T_ORDER_BE)
                    {
                        sorder = " big-endian";
                    }
                    else if (order == H5T_ORDER_VAX)
                    {
                        sorder = " mixed-endian";
                    }
                    else
                    {
                        sorder = " unknown-byte-order";
                    }
                }
                else
                {
                    sorder = "";
                }

                os << (unsigned long)(8 * H5Tget_size(type)) << "-bit"
                   << sorder
                   << " floating-point";
            }
            break;
        case H5T_TIME:
            if (H5Tequal(type, H5T_UNIX_D32BE) > 0)
            {
                os << "H5T_UNIX_D32BE";
            }
            else if (H5Tequal(type, H5T_UNIX_D32LE) > 0)
            {
                os << "H5T_UNIX_D32LE";
            }
            else if (H5Tequal(type, H5T_UNIX_D64BE) > 0)
            {
                os << "H5T_UNIX_D64BE";
            }
            else if (H5Tequal(type, H5T_UNIX_D64LE) > 0)
            {
                os << "H5T_UNIX_D64LE";
            }
            else
            {
                os << "Unknown time format";
            }
            break;
        case H5T_STRING:
            size = H5Tget_size(type);
            strpad = H5Tget_strpad(type);
            cset = H5Tget_cset(type);
            isVariableLength = H5Tis_variable_str(type);
            indent = H5Object::getIndentString(indentLevel + 1);

            strType = H5Tcopy(H5T_C_S1);
            H5Tset_size(strType, isVariableLength ? H5T_VARIABLE : size);
            H5Tset_cset(strType, cset);
            H5Tset_strpad(strType, strpad);

            os << "H5T_STRING {" << std::endl;
            if (isVariableLength)
            {
                os << indent << "STRSIZE H5T_VARIABLE;" << std::endl;
            }
            else
            {
                os << indent << "STRSIZE " << (int)size << ";" << std::endl;
            }

            os << indent << "STRPAD ";
            switch (strpad)
            {
                case H5T_STR_NULLTERM:
                    os << "H5T_STR_NULLTERM;" << std::endl;
                    break;
                case H5T_STR_NULLPAD:
                    os << "H5T_STR_NULLPAD;" << std::endl;
                    break;
                case H5T_STR_SPACEPAD:
                    os << "H5T_STR_SPACEPAD;" << std::endl;
                    break;
                default:
                    os << "H5T_STR_ERROR;" << std::endl;
                    break;
            }

            os << indent << "CSET ";
            if (cset == H5T_CSET_ASCII)
            {
                os << "H5T_CSET_ASCII;" << std::endl;
            }
            else if (cset == H5T_CSET_UTF8)
            {
                os << "H5T_CSET_UTF8;" << std::endl;
            }
            else
            {
                os << "Unknown charset;" << std::endl;
            }

            // TODO: modif l'endianess (cf h5dump.c::1068)
            os << indent << "CTYPE ";
            if (H5Tequal(type, strType) > 0)
            {
                H5Tclose(strType);
                os << "H5T_C_S1;" << std::endl;
            }
            else
            {
                H5Tclose(strType);
                strType = H5Tcopy(H5T_FORTRAN_S1);
                H5Tset_size(strType, size);
                H5Tset_cset(strType, cset);
                H5Tset_strpad(strType, strpad);
                if (H5Tequal(type, H5T_FORTRAN_S1) > 0)
                {
                    os << "H5T_FORTRAN_S1;" << std::endl;
                }
                else
                {
                    os << "Unknown one character type;" << std::endl;
                }
            }

            os << H5Object::getIndentString(indentLevel) << "}";
            break;
        case H5T_BITFIELD:
            if (H5Tequal(type, H5T_STD_B8BE) > 0)
            {
                os << "H5T_STD_B8BE";
            }
            else if (H5Tequal(type, H5T_STD_B8LE) > 0)
            {
                os << "H5T_STD_B8LE";
            }
            else if (H5Tequal(type, H5T_STD_B16BE) > 0)
            {
                os << "H5T_STD_B16BE";
            }
            else if (H5Tequal(type, H5T_STD_B16LE) > 0)
            {
                os << "H5T_STD_B16LE";
            }
            else if (H5Tequal(type, H5T_STD_B32BE) > 0)
            {
                os << "H5T_STD_B32BE";
            }
            else if (H5Tequal(type, H5T_STD_B32LE) > 0)
            {
                os << "H5T_STD_B32LE";
            }
            else if (H5Tequal(type, H5T_STD_B64BE) > 0)
            {
                os << "H5T_STD_B64BE";
            }
            else if (H5Tequal(type, H5T_STD_B64LE) > 0)
            {
                os << "H5T_STD_B64LE";
            }
            else if (H5Tequal(type, H5T_INTEL_B8) > 0)
            {
                os << "H5T_INTEL_B8";
            }
            else if (H5Tequal(type, H5T_INTEL_B16) > 0)
            {
                os << "H5T_INTEL_B16";
            }
            else if (H5Tequal(type, H5T_INTEL_B32) > 0)
            {
                os << "H5T_INTEL_B32";
            }
            else if (H5Tequal(type, H5T_INTEL_B64) > 0)
            {
                os << "H5T_INTEL_B64";
            }
            else if (H5Tequal(type, H5T_ALPHA_B8) > 0)
            {
                os << "H5T_ALPHA_B8";
            }
            else if (H5Tequal(type, H5T_ALPHA_B16) > 0)
            {
                os << "H5T_ALPHA_B16";
            }
            else if (H5Tequal(type, H5T_ALPHA_B32) > 0)
            {
                os << "H5T_ALPHA_B32";
            }
            else if (H5Tequal(type, H5T_ALPHA_B64) > 0)
            {
                os << "H5T_ALPHA_B64";
            }
            else if (H5Tequal(type, H5T_MIPS_B8) > 0)
            {
                os << "H5T_MIPS_B8";
            }
            else if (H5Tequal(type, H5T_MIPS_B16) > 0)
            {
                os << "H5T_MIPS_B16";
            }
            else if (H5Tequal(type, H5T_MIPS_B32) > 0)
            {
                os << "H5T_MIPS_B32";
            }
            else if (H5Tequal(type, H5T_MIPS_B64) > 0)
            {
                os << "H5T_MIPS_B64";
            }
            else
            {
                os << "undefined bitfield";
            }
            break;
        case H5T_OPAQUE:
            opaqueTag = H5Tget_tag(type);
            os << "H5T_OPAQUE;" << std::endl
               << H5Object::getIndentString(indentLevel + 1)
               << "OPAQUE TAG \"" << opaqueTag << "\";";

            free(opaqueTag);
            break;
        case H5T_COMPOUND:
            nmembers = H5Tget_nmembers(type);
            indent = H5Object::getIndentString(indentLevel + 1);
            os << "H5T_COMPOUND {" << std::endl;

            for (unsigned int i = 0; i < nmembers; i++)
            {
                char * mname = H5Tget_member_name(type, i);
                hid_t mtype = H5Tget_member_type(type, i);

                os << indent;
                H5Type::printComplexNameFromType(os, indentLevel + 1, mtype);
                os << " \"" << mname << "\";" << std::endl;

                free(mname);
            }

            os << H5Object::getIndentString(indentLevel) << "}";
            break;
        case H5T_REFERENCE:
            os << "H5T_REFERENCE";
            if (H5Tequal(type, H5T_STD_REF_DSETREG) > 0)
            {
                os << " { H5T_STD_REF_DSETREG }";
            }
            else
            {
                os << " { H5T_STD_REF_OBJECT }";
            }
            break;
        case H5T_ENUM:
            nmembers = H5Tget_nmembers(type);
            indent = H5Object::getIndentString(indentLevel + 1);
            size = H5Tget_size(type);
            super = H5Tget_super(type);

            if (size <= sizeof(long long))
            {
                dstSize = sizeof(long long);
                if (H5Tget_sign(type) == H5T_SGN_NONE)
                {
                    native = H5T_NATIVE_ULLONG;
                }
                else
                {
                    native = H5T_NATIVE_LLONG;
                }
            }
            else
            {
                dstSize = size;
            }

            os << "H5T_ENUM { " << std::endl
               << indent << H5Type(*const_cast<H5Type *>(this), H5Tcopy(super)).dump(alreadyVisited, 0);

            value = new char[std::max(size, dstSize)]();

            for (unsigned int i = 0; i < nmembers; i++)
            {
                char * mname = H5Tget_member_name(type, i);
                std::string _mname = "\"" + std::string(mname) + "\"";
                free(mname);

                H5Tget_member_value(type, i, value);

                if (_mname.length() < 16)
                {
                    _mname.resize(16, ' ');
                }

                os << indent << _mname;

                if (native < 0)
                {
                    for (size_t j = 0; j < dstSize; j++)
                    {
                        os << "0x" << std::hex << std::setfill('0') << std::setw(2) << (int)value[j];
                    }
                }
                else
                {
                    H5Tconvert(super, native, 1, value, 0, H5P_DEFAULT);
                    if (H5Tget_sign(type) == H5T_SGN_NONE)
                    {
                        os << *reinterpret_cast<unsigned long long *>(value);
                    }
                    else
                    {
                        os << *reinterpret_cast<long long *>(value);
                    }
                }

                os << ";" << std::endl;
            }

            delete[] value;
            H5Tclose(super);
            os << H5Object::getIndentString(indentLevel) << "}";
            break;
        case H5T_VLEN:
            super = H5Tget_super(type);
            os << "H5T_VLEN { ";

            H5Type::printComplexNameFromType(os, indentLevel + 1, super);

            os << " }";
            break;
        case H5T_ARRAY:
            super = H5Tget_super(type);
            ndims = H5Tget_array_ndims(type);
            dims = new hsize_t[ndims];
            H5Tget_array_dims(type, dims);

            os << "H5T_ARRAY { ";

            for (unsigned int i = 0; i < ndims; i++)
            {
                os << "[" << (unsigned int)dims[i] << "]";
            }

            os << " ";
            H5Type::printComplexNameFromType(os, indentLevel + 1, super);
            os << " }";

            H5Tclose(super);
            delete[] dims;
            break;
        default:
            os << _("Unknown datatype");
            break;
    }

    os << std::endl;

    return os.str();
}

void H5Type::printLsInfo(std::ostringstream & os) const
{
    if (!getName().empty())
    {
        std::string str(getName());
        H5Object::getResizedString(str);

        os << str << "Type" << std::endl;
    }
}

std::string H5Type::ls() const
{
    std::ostringstream os;
    printLsInfo(os);

    return os.str();
}

std::string H5Type::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel);

    os << indentString << "Filename" << ": " << getFile().getFileName() << std::endl
       << indentString << "Name" << ": " << name << std::endl
       << indentString << "Class" << ": " << getClassName() << std::endl
       << indentString << "Type" << ": " << getTypeName() << std::endl
       << indentString << "Size" << ": " << getTypeSize() << std::endl
       << indentString << "Nativetype" << ": " << getNativeTypeName() << std::endl
       << indentString << "Nativesize" << ": " << getNativeTypeSize();

    return os.str();
}
}
