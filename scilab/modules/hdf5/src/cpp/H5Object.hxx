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

#ifdef _MSC_VER
#pragma warning(disable: 4355) //disable Warning C4355: 'this' : used in base member initializer list
#endif

#ifndef __H5OBJECT_HXX__
#define __H5OBJECT_HXX__

#include "HDF5Objects.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include <set>
#include <string>

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "H5VariableScope.hxx"
#include "H5Exception.hxx"

#define __H5_INDENT_LENGTH__ 3
#define __H5_LS_LENGTH__ 25

namespace org_modules_hdf5
{
class H5AttributesList;
class H5File;

class H5Object
{
    static H5Object* root;

    H5Object & parent;
    std::set<H5Object *> children;
    bool locked;
    int scilabId;

    friend class H5AttributesList;
    friend class H5LinkList;
    friend class H5Dataset;

protected: // for error report only

    const std::string name;

public :

    enum FilterType {HARD, SOFT, EXTERNAL, DANGLING, GROUP, DATASET, TYPE, ATTRIBUTE};

    H5Object(H5Object & _parent);
    H5Object(H5Object & _parent, const std::string & _name);
    virtual ~H5Object();

    static void clearRoot()
    {
        delete root;
    }

    static void initRoot()
    {
        root = new H5Object();
    }

    virtual void cleanup();

    virtual hid_t getH5Id() const;
    virtual H5AttributesList & getAttributes();
    virtual hsize_t getAttributesNumber() const;

    virtual H5O_info_t getInfo() const
    {
        H5O_info_t info;
        H5Oget_info(getH5Id(), &info);

        return info;
    }

    virtual bool isFile() const
    {
        return false;
    }

    virtual bool isGroup() const
    {
        return false;
    }

    virtual bool isAttribute() const
    {
        return false;
    }

    virtual bool isReference() const
    {
        return false;
    }

    virtual bool isDataspace() const
    {
        return false;
    }

    virtual bool isDataset() const
    {
        return false;
    }

    virtual bool isType() const
    {
        return false;
    }

    virtual bool isList() const
    {
        return false;
    }

    virtual bool isCompound() const
    {
        return false;
    }

    virtual bool isArray() const
    {
        return false;
    }

    virtual bool isVlen() const
    {
        return false;
    }

    virtual bool mustDelete() const
    {
        return true;
    }

    virtual haddr_t getAddr() const
    {
        return getInfo().addr;
    }

    virtual const std::string & getName() const
    {
        return name;
    }

    virtual H5Object & getData(const unsigned int size, const unsigned int * index) const
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve numeric index."));
    }

    virtual H5Object & getData(const unsigned int size, const double * index) const
    {
        unsigned int * _index = new unsigned int[size];
        for (unsigned int i = 0; i < size; i++)
        {
            _index[i] = (unsigned int)(index[i] - 1);
        }

        try
        {
            return getData(size, _index);
        }
        catch (const H5Exception & /*e*/)
        {
            delete[] _index;
            throw;
        }
    }


    virtual const std::string getBaseName() const
    {
        std::string::size_type pos = name.find_last_of('/');
        if (pos == std::string::npos)
        {
            return name;
        }
        else
        {
            return name.substr(pos + 1);
        }
    }

    virtual std::string getCompletePath() const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const
    {
        return "";
    }

    virtual std::string ls() const
    {
        return "";
    }

    virtual void ls(std::vector<std::string> & name, std::vector<std::string> & type) const
    {

    }

    virtual void ls(std::vector<std::string> & name, FilterType type) const
    {
        getNames(*this, name, type);
    }

    virtual void printLsInfo(std::ostringstream & os) const
    {
        return;
    }

    virtual std::string toString() const
    {
        return toString(0);
    }
    virtual std::string toString(const unsigned int indentLevel) const
    {
        return "";
    }
    virtual void getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const;
    virtual void getAccessibleAttribute(const double index, const int pos, void * pvApiCtx) const
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid operation"));
    }

    virtual void setAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid operation"));
    }

    virtual void setAccessibleAttribute(const double index, const int pos, void * pvApiCtx) const
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid operation"));
    }

    void setScilabId(const int id)
    {
        scilabId = id;
    }

    int getScilabId() const
    {
        return scilabId;
    }

    H5Object & getParent() const
    {
        return parent;
    }
    H5File & getFile() const;

    virtual void getNames(const H5Object & obj, std::vector<std::string> & names, FilterType type) const;
    virtual void createOnScilabStack(int pos, void * pvApiCtx) const;
    virtual void createInScilabList(int * list, int stackPos, int pos, void * pvApiCtx) const;

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const
    {
        if (parentList)
        {
            createInScilabList(parentList, lhsPosition, listPosition, pvApiCtx);
        }
        else
        {
            createOnScilabStack(lhsPosition, pvApiCtx);
        }
    }

    bool isRoot() const
    {
        return this == root;
    }

    void unregisterChild(H5Object * child)
    {
        if (!locked)
        {
            children.erase(child);
        }
    }

    static std::string getIndentString(const unsigned int indentLevel)
    {
        return std::string((size_t)(__H5_INDENT_LENGTH__ * indentLevel), ' ');
    }

    static H5Object & getRoot()
    {
        return *root;
    }

    static void cleanAll()
    {
        root->locked = true;
        for (std::set<H5Object *>::iterator it = root->children.begin(); it != root->children.end(); it++)
        {
            delete *it;
        }
        root->children.clear();
        root->locked = false;
        H5VariableScope::clearScope();
    }

    static void getResizedString(std::string & str)
    {
        if (str.length() < __H5_LS_LENGTH__)
        {
            str.resize(__H5_LS_LENGTH__, ' ');
        }
    }

    static H5Object & getObject(H5Object & parent, hid_t obj);
    static H5Object & getObject(H5Object & parent, const std::string & name);
    static H5Object & getObject(H5Object & parent, const std::string & name, const bool isAttr);
    static void getLinksInfo(const H5Object & obj, std::vector<std::string> & linksName, std::vector<std::string> & types, std::vector<std::string> & linksType);

    inline static hsize_t * getCumProd(const hsize_t ndims, const hsize_t * dims)
    {
        hsize_t * ret = new hsize_t[ndims];
        ret[0] = 1;
        for (unsigned int i = 1; i < ndims; i++)
        {
            ret[i] *= ret[i - 1];
        }

        return ret;
    }

    inline static bool isEmptyPath(const std::string & path)
    {
        return path.empty() || path == ".";
    }

    inline static bool isEmptyPath(const char * path)
    {
        return path[0] == '\0' || (path[0] == '.' && path[1] == '\0');
    }

protected :

    class OpDataGetLs
    {
    public:
        H5Object * parent;
        std::vector<std::string> * name;
        std::vector<std::string> * type;

        OpDataGetLs(H5Object * _parent, std::vector<std::string> * _name, std::vector<std::string> * _type) : parent(_parent), name(_name), type(_type) { }
    };

    class OpDataCount
    {
    public:
        unsigned int soft;
        unsigned int external;
        unsigned int hard;
        unsigned int dangling;
        unsigned int group;
        unsigned int dataset;
        unsigned int type;
        const bool followLink;

        OpDataCount(const bool _followLink) : soft(0), external(0), hard(0), dangling(0), group(0), dataset(0), type(0), followLink(_followLink) { }
    };

    class OpDataFilter
    {
    public:
        std::vector<std::string> * name;
        FilterType type;
        const bool followLink;

        OpDataFilter(std::vector<std::string> * _name, FilterType _type, const bool _followLink) : name(_name), type(_type), followLink(_followLink) { }
    };

    class OpDataSoftLinkFilter
    {
    public:
        std::vector<std::string> * name;
        std::vector<std::string> * value;
        FilterType type;

        OpDataSoftLinkFilter(std::vector<std::string> * _name, std::vector<std::string> * _value, FilterType _type) : name(_name), value(_value), type(_type) { }
    };

    void registerChild(H5Object * child)
    {
        if (!locked)
        {
            children.insert(child);
        }
    }

    static void count(const H5Object & obj, OpDataCount & opdata);
    static herr_t countIterator(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data);
    static herr_t filterAttributesIterator(hid_t location_id, const char * attr_name, const H5A_info_t * ainfo, void * op_data);
    static herr_t filterIterator(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data);
    static herr_t filterSoftLinkIterator(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data);
    static herr_t getLsAttributes(hid_t location_id, const char * attr_name, const H5A_info_t * ainfo, void * op_data);

private :

    H5Object() : parent(*this), locked(false), scilabId(-1) { }

    class LinksInfo
    {
    public:
        std::vector<std::string> * name;
        std::vector<std::string> * type;
        std::vector<std::string> * linkType;

        LinksInfo(std::vector<std::string> * _name, std::vector<std::string> * _type, std::vector<std::string> * _linkType) : name(_name), type(_type), linkType(_linkType) { }
    };

    static herr_t iterateGetInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data);
};
}

#undef __H5_INDENT_LENGTH__
#undef __H5_LS_LENGTH__

#endif // __H5OBJECT_HXX__

