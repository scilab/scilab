/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __H5GROUP_HXX__
#define __H5GROUP_HXX__

#include "H5Object.hxx"
#include "H5Type.hxx"
#include "H5Dataset.hxx"
#include "H5SoftLink.hxx"
#include "H5ExternalLink.hxx"
#include "H5NamedObjectsList.hxx"
#include "H5AttributesList.hxx"

namespace org_modules_hdf5
{

class H5SoftLinksList;
class H5LinksList;
class H5GroupsList;
class H5DatasetsList;
class H5TypesList;
class H5File;

class H5Group : public H5Object
{
    typedef struct
    {
        H5Object * parent;
        std::ostringstream * os;
    } OpDataPrintLs;

    hid_t group;

public:

    H5Group(H5Object & _parent, const std::string & name);
    H5Group(H5Object & _parent, hid_t _group, const std::string & _name);

    virtual ~H5Group();

    virtual hid_t getH5Id() const
    {
        return group;
    }

    virtual bool isGroup() const
    {
        return true;
    }

    virtual H5NamedObjectsList<H5SoftLink> & getSoftLinks();
    virtual H5NamedObjectsList<H5ExternalLink> & getExternalLinks();
    virtual H5GroupsList & getGroups();
    virtual H5NamedObjectsList<H5Group> & getHardGroups();
    virtual H5NamedObjectsList<H5Type> & getHardTypes();
    virtual H5NamedObjectsList<H5Dataset> & getHardDatasets();
    virtual H5DatasetsList & getDatasets();
    virtual H5TypesList & getTypes();
    const unsigned int getLinksSize() const;
    virtual std::string getCompletePath() const;
    virtual std::string toString(const unsigned int indentLevel) const;
    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel = 0) const;
    virtual std::string ls() const;
    virtual void ls(std::vector<std::string> & name, std::vector<std::string> & type) const;
    virtual void printLsInfo(std::ostringstream & os) const;

    virtual void getAccessibleAttribute(const std::string & name, const int pos, void * pvApiCtx) const;

    static void createGroup(H5Object & parent, const std::string & name);
    static void createGroup(H5Object & parent, const int size, const char ** names);

private :

    void init();
    static herr_t printLsInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data);
    static herr_t getLsInfo(hid_t g_id, const char * name, const H5L_info_t * info, void * op_data);
};
}

#endif // __H5GROUP_HXX__
