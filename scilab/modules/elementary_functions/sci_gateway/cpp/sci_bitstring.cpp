#include <bitset>
#include "double.hxx"
#include "function.hxx"
#include "string.hxx"
#include "int.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/* ==================================================================== */
types::Function::ReturnValue sci_bitstring(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pOut;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "bitstring", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "bitstring", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        if (in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s, %s or %s expected.\n"), "bitstring", 1, "integer", "boolean", "real double");
            return types::Function::Error;
        }
    }

    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble:
        case types::InternalType::ScilabUInt64:
        case types::InternalType::ScilabInt64:
        {
            types::UInt64* pdbl = in[0]->getAs<types::UInt64>();
            pOut = new types::String(pdbl->getDims(), pdbl->getDimsArray());
            for (int i = 0; i < pdbl->getSize(); ++i)
            {
                pOut->set(i, std::bitset<64>(pdbl->get(i)).to_string().c_str());
            }
            break;
        }
        case types::InternalType::ScilabUInt32:
        case types::InternalType::ScilabInt32:
        {
            types::UInt32* pdbl = in[0]->getAs<types::UInt32>();
            pOut = new types::String(pdbl->getDims(), pdbl->getDimsArray());
            for (int i = 0; i < pdbl->getSize(); ++i)
            {
                pOut->set(i, std::bitset<32>(pdbl->get(i)).to_string().c_str());
            }
            break;
        }
        case types::InternalType::ScilabUInt16:
        case types::InternalType::ScilabInt16:
        {
            types::UInt16* pdbl = in[0]->getAs<types::UInt16>();
            pOut = new types::String(pdbl->getDims(), pdbl->getDimsArray());
            for (int i = 0; i < pdbl->getSize(); ++i)
            {
                pOut->set(i, std::bitset<16>(pdbl->get(i)).to_string().c_str());
            }
            break;
        }
        case types::InternalType::ScilabUInt8:
        case types::InternalType::ScilabInt8:
        {
            types::UInt8* pdbl = in[0]->getAs<types::UInt8>();
            pOut = new types::String(pdbl->getDims(), pdbl->getDimsArray());
            for (int i = 0; i < pdbl->getSize(); ++i)
            {
                pOut->set(i, std::bitset<8>(pdbl->get(i)).to_string().c_str());
            }
            break;
        }
        case types::InternalType::ScilabBool:
        {
            types::Bool* pdbl = in[0]->getAs<types::Bool>();
            pOut = new types::String(pdbl->getDims(), pdbl->getDimsArray());
            for (int i = 0; i < pdbl->getSize(); ++i)
            {
                pOut->set(i, std::bitset<1>(pdbl->get(i)).to_string().c_str());
            }
            break;
        }
        default:
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s, %s or %s expected.\n"), "bitstring", 1, "integer", "boolean", "real double");
            return types::Function::Error;
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}

