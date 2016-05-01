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

#ifndef __H5NAMEDOBJECTSLIST_HXX__
#define __H5NAMEDOBJECTSLIST_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Link.hxx"
#include "H5ListObject.hxx"

#include "os_string.h"

namespace org_modules_hdf5
{

class H5Group;

template <typename T>
class H5NamedObjectsList : public H5ListObject<T>
{

    typedef struct
    {
        union
        {
            unsigned int count;
            const char * name;
        } u;
        int linktype;
        int basetype;
    } OpData;


public :

    H5NamedObjectsList(H5Group & _parent, const int _baseType, const int _linkType, const std::string _baseTypeName) : H5ListObject<T>(_parent), baseType(_baseType), linkType(_linkType), baseTypeName(_baseTypeName), prevPos(0), idx(0) { }

    H5NamedObjectsList(H5Group & _parent, const unsigned int _size, const unsigned int * _index, const int _baseType, const int _linkType, const std::string _baseTypeName) : H5ListObject<T>(_parent, _size, _index), baseType(_baseType), linkType(_linkType), baseTypeName(_baseTypeName), prevPos(0), idx(0)
    {
        const unsigned int lsize = getSize(true);
        for (unsigned int i = 0; i < H5ListObject<T>::indexSize; i++)
        {
            if (H5ListObject<T>::indexList[i] >= lsize)
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid index at position %d"), i);
            }
        }
    }

    virtual ~H5NamedObjectsList()
    {

    }

    void setObject(const unsigned int pos, T & obj)
    {

    }

    T & getObject(const int pos)
    {
        return getObject(pos, true);
    }

    inline static herr_t count(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
    {
        OpData & opdata = *(OpData *)op_data;

        if ((opdata.linktype != -1 && info->type == opdata.linktype) || (opdata.linktype == -1 && info->type != H5L_TYPE_ERROR))
        {
            H5O_info_t oinfo;
            herr_t err;
            if (opdata.basetype != -1)
            {
                hid_t obj = H5Oopen(g_id, name, H5P_DEFAULT);

                if (obj < 0)
                {
                    return (herr_t) - 1;
                }

                err = H5Oget_info(obj, &oinfo);
                H5Oclose(obj);

                if (err < 0)
                {
                    return (herr_t) - 2;
                }

                if (oinfo.type == opdata.basetype)
                {
                    opdata.u.count++;
                }
            }
            else
            {
                opdata.u.count++;
            }
        }

        return (herr_t)0;
    }

    inline static herr_t getElement(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data)
    {
        OpData & opdata = *(OpData *)op_data;

        if ((opdata.linktype != -1 && info->type == opdata.linktype) || (opdata.linktype == -1 && info->type != H5L_TYPE_ERROR))
        {
            H5O_info_t oinfo;
            herr_t err;
            if (opdata.basetype != -1)
            {
                hid_t obj = H5Oopen(g_id, name, H5P_DEFAULT);

                if (obj < 0)
                {
                    return (herr_t) - 1;
                }

                err = H5Oget_info(obj, &oinfo);
                H5Oclose(obj);

                if (err < 0)
                {
                    return (herr_t) - 2;
                }

                if (oinfo.type == opdata.basetype)
                {
                    opdata.u.count--;
                }
            }
            else
            {
                opdata.u.count--;
            }

            if (opdata.u.count == 0)
            {
                opdata.u.name = os_strdup(name);

                return (herr_t)1;
            }
        }

        return (herr_t)0;
    }

    unsigned int getSize() const
    {
        return getSize(false);
    }

    unsigned int getSize(const bool indexChecking) const
    {
        if (H5ListObject<T>::indexList && !indexChecking)
        {
            return H5ListObject<T>::indexSize;
        }
        else
        {
            hsize_t idx = 0;
            OpData op_data;
            herr_t err;

            op_data.u.count = 0;
            op_data.linktype = linkType;
            op_data.basetype = baseType;

            err = H5Literate(H5Object::getParent().getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, count, &op_data);

            if (err < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of objects."));
            }

            return op_data.u.count;
        }
    }

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
    {
        std::ostringstream os;
        const unsigned int size = getSize();

        for (unsigned int i = 0; i < size; i++)
        {
            const T & obj = const_cast<H5NamedObjectsList *>(this)->getObject(i, false);
            os << obj.dump(alreadyVisited, indentLevel);

            delete &obj;
        }

        return os.str();
    }

    virtual void printLsInfo(std::ostringstream & os) const
    {
        const unsigned int size = getSize();

        for (unsigned int i = 0; i < size; i++)
        {
            const T & obj = const_cast<H5NamedObjectsList *>(this)->getObject(i, false);
            obj.printLsInfo(os);

            delete &obj;
        }
    }

    virtual std::string toString(const unsigned int indentLevel) const
    {
        std::ostringstream os;
        std::string indentString = H5Object::getIndentString(indentLevel);
        const unsigned int size = getSize();

        os << indentString << _("Filename") << ": " << H5Object::getParent().getFile().getFileName() << std::endl
           << indentString << _("Parent group name") << ": " << H5Object::getParent().getName() << std::endl
           << indentString << _("Parent group path") << ": " << H5Object::getParent().getCompletePath() << std::endl
           << indentString << _("Elements type") << ": " << baseTypeName << std::endl
           << indentString << _("Size") << ": " << size;

        return os.str();
    }

protected :

    const int baseType;
    const int linkType;
    const std::string baseTypeName;

private :

    int prevPos;
    hsize_t idx;


private:

    T & getObject(const int pos, const bool checkPos)
    {
        OpData op_data;
        herr_t err;
        int _pos = pos;

        if (H5ListObject<T>::indexList)
        {
            if (pos >= 0 && pos < (int)H5ListObject<T>::indexSize)
            {
                _pos = H5ListObject<T>::indexList[pos];
            }
            else
            {
                throw H5Exception(__LINE__, __FILE__, _("Invalid index: %d."), pos);
            }
        }

        op_data.linktype = linkType;
        op_data.basetype = baseType;

        if (_pos < prevPos)
        {
            idx = 0;
            op_data.u.count = _pos + 1;
        }
        else
        {
            op_data.u.count = _pos - prevPos + 1;
        }

        err = H5Literate(H5Object::getParent().getH5Id(), H5_INDEX_NAME, H5_ITER_INC, &idx, getElement, &op_data);

        if (err > 0)
        {
            prevPos = _pos + 1;
            return *new T(H5Object::getParent(), op_data.u.name);
        }
        else
        {
            idx = 0;
            prevPos = 0;
            throw H5Exception(__LINE__, __FILE__, _("Cannot get object at position %d."), pos);
        }
    }

    T & getObject(const std::string & name)
    {
        H5O_info_t info;
        herr_t err;
        H5Object * obj = 0;

        if (H5Lexists(H5Object::getParent().getH5Id(), name.c_str(), H5P_DEFAULT) <= 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }

        err = H5Oget_info_by_name(H5Object::getParent().getH5Id(), name.c_str(), &info, H5P_DEFAULT);
        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid name: %s."), name.c_str());
        }

        if (info.type == baseType)
        {
            return *new T(H5Object::getParent(), name);
        }

        throw H5Exception(__LINE__, __FILE__, _("Invalid HDF5 object"));
    }
};
}

#endif // __H5NAMEDOBJECTSLIST_HXX__
