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

#ifndef __H5NAMEDOBJECTSLIST_HXX__
#define __H5NAMEDOBJECTSLIST_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5ListObject.hxx"

namespace org_modules_hdf5
{

    class H5Group;

    template <typename T>
    class H5NamedObjectsList : public H5ListObject<T>
    {

    public :

        H5NamedObjectsList(H5Group & _parent, const int _baseType, const std::string _baseTypeName) : H5ListObject<T>(_parent), baseType(_baseType), baseTypeName(_baseTypeName) { }

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

        const unsigned int getSize() const
            {
                H5G_info_t info;
                hsize_t count = 0;
                hid_t parentId = H5Object::getParent().getH5Id();
                herr_t err = H5Gget_info(parentId, &info);

                if (err < 0)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of groups."));
                }

                for (hsize_t i = 0; i < info.nlinks; i++)
                {
                    int type = H5Gget_objtype_by_idx(parentId, i);
                    if (type == baseType)
                    {
                        count++;
                    }
                }

                return (unsigned int)count;
            }

        std::string dump(const unsigned int indentLevel) const
            {
                std::ostringstream os;
                const unsigned int size = getSize();
		
                for (unsigned int i = 0; i < size; i++)
                {
                    const T & obj = const_cast<H5NamedObjectsList *>(this)->getObject(i, false);
                    os << obj.toString(indentLevel) << std::endl;

                    delete &obj;
                }

                return os.str();
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
                   << indentString << _("Number of elements") << ": " << size;

                return os.str();
            }

    protected :

        const int baseType;
        const std::string baseTypeName;

    private:

        T & getObject(const int pos, const bool checkPos)
            {
                int type;
                herr_t err;
                hsize_t count = 0;
                hsize_t index = 0;
                hid_t parentId = H5Object::getParent().getH5Id();
		H5G_info_t info;
                ssize_t nameSize;
                char * name = 0;

                if (checkPos)
                {
                    unsigned int size = getSize();
                    if (pos < 0 || pos >= size)
                    {
                        throw H5Exception(__LINE__, __FILE__, _("Invalid index %u: must be between 0 and %u."), pos, size);
                    }
                }

                err = H5Gget_info(parentId, &info);
                if (err < 0)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Cannot get the number of groups."));
                }

                for (; index < info.nlinks; index++)
                {
                    int type = H5Gget_objtype_by_idx(parentId, index);
                    if (type == baseType)
                    {
                        count++;
                        if (count == pos)
                        {
                            break;
                        }
                    }
                }

                nameSize = H5Gget_objname_by_idx(parentId, index, 0, 0);
                name = (char *)MALLOC((nameSize + 1) * sizeof(char));
                H5Gget_objname_by_idx(parentId, index, name, nameSize + 1);

                return *new T(H5Object::getParent(), name);
            }
    };
}

#endif // __H5NAMEDOBJECTSLIST_HXX__
