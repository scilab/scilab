/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5COMPOUNDDATA_HXX__
#define __H5COMPOUNDDATA_HXX__

#include "H5Data.hxx"
#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5CompoundData : public H5BasicData<char>
{
    const unsigned int nfields;
    const std::string * fieldsname;
    H5Data ** fieldsvalue;

public:

    H5CompoundData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, const unsigned int _nfields, std::string * _fieldsname, H5Data ** _fieldsvalue, char * _data, const bool _dataOwner) : H5BasicData(_parent, _totalSize, _dataSize, _ndims, _dims, _data, -1, 0, _dataOwner), nfields(_nfields), fieldsname(_fieldsname), fieldsvalue(_fieldsvalue)
    {

    }

    virtual ~H5CompoundData()
    {
        delete[] fieldsname;
        delete[] fieldsvalue;
    }

    H5Data & getData(const std::string fieldname) const
    {
        for (unsigned int i = 0; i < nfields; i++)
        {
            if (fieldname == fieldsname[i])
            {
                return *fieldsvalue[i];
            }
        }

        throw H5Exception(__LINE__, __FILE__, _("Invalid field name: %s"), fieldname.c_str());
    }

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
    {
        return H5DataConverter::dump(alreadyVisited, indentLevel, ndims, dims, *this, false);
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        os << "{" << std::endl;
        std::string indent = H5Object::getIndentString(indentLevel + 2);
        for (unsigned int i = 0; i < nfields - 1; i++)
        {
            os << indent;
            fieldsvalue[i]->printData(os, pos, indentLevel + 2);
            os << ", " << std::endl;
        }
        os << indent;
        fieldsvalue[nfields - 1]->printData(os, pos, indentLevel + 2);
        os << std::endl << H5Object::getIndentString(indentLevel + 1) << "}";
    }

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0) const
    {
        // TODO: gerer le cas ndims=0 (cas SCALAR)

        static int structdims[2] = { 1, 1 };
        const char ** _fieldsname = new const char *[nfields + 2];
        SciErr err;
        int * scilabStruct = 0;

        _fieldsname[0] = "st";
        _fieldsname[1] = "dims";
        for (int i = 0; i < nfields; i++)
        {
            _fieldsname[i + 2] = fieldsname[i].c_str();
        }

        if (parentList)
        {
            err = createMListInList(pvApiCtx, lhsPosition, parentList, listPosition, nfields + 1, &scilabStruct);
        }
        else
        {
            err = createMList(pvApiCtx, lhsPosition, nfields + 2, &scilabStruct);
        }

        if (err.iErr)
        {
            delete[] _fieldsname;
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a struct on the stack"));
        }

        err = createMatrixOfStringInList(pvApiCtx, lhsPosition, scilabStruct, 1, 1, nfields + 2, _fieldsname);
        delete[] _fieldsname;
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a struct on the stack"));
        }

        err = createMatrixOfInteger32InList(pvApiCtx, lhsPosition, scilabStruct, 2, 1, 2, structdims);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a struct on the stack"));
        }

        for (int i = 0; i < nfields; i++)
        {
            fieldsvalue[i]->toScilab(pvApiCtx, lhsPosition, scilabStruct, i + 3);
        }
    }
};
}

#endif // __H5COMPOUNDDATA_HXX__
