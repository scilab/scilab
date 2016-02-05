/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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

#ifndef ADAPTERS_HXX_
#define ADAPTERS_HXX_

#include <utility>
#include <vector>
#include <string>

#include "internal.hxx"
#include "utilities.hxx"
#include "model/BaseObject.hxx"

extern "C"
{
#include "dynlib_scicos.h"
}
namespace org_scilab_modules_scicos
{
namespace view_scilab
{

/*
 * Shared data between adapters
 */
class SCICOS_IMPEXP Adapters
{
public:
    typedef enum
    {
        BLOCK_ADAPTER = 0,  //!< \see view_scilab::BlockAdapter
        CPR_ADAPTER,        //!< \see view_scilab::CprAdapter
        DIAGRAM_ADAPTER,    //!< \see view_scilab::DiagramAdapter
        GRAPHIC_ADAPTER,    //!< \see view_scilab::GraphicAdapter
        LINK_ADAPTER,       //!< \see view_scilab::LinkAdapter
        MODEL_ADAPTER,      //!< \see view_scilab::ModelAdapter
        PARAMS_ADAPTER,    //!< \see view_scilab::ParamsAdapter
        SCS_ADAPTER,        //!< \see view_scilab::ScsAdapter
        STATE_ADAPTER,      //!< \see view_scilab::StateAdapter
        TEXT_ADAPTER,       //!< \see view_scilab::TextAdapter

        INVALID_ADAPTER     //!< length of the enum is used to report error
    } adapters_index_t;

    static Adapters& instance();

    adapters_index_t lookup_by_typename(const std::wstring& name);
    std::wstring get_typename(adapters_index_t index);
    const model::BaseObject* descriptor(types::InternalType* v);
    types::InternalType* allocate_view(ScicosID id, kind_t kind);

private:

    struct adapter_t
    {
        adapter_t(const std::wstring& n, adapters_index_t k) : name(n), kind(k) {};

        inline bool operator<(const adapter_t& v) const
        {
            return name < v.name;
        }

        inline bool operator<(const std::wstring& v) const
        {
            return name < v;
        }

        std::wstring name;
        adapters_index_t kind;
    };
    typedef std::vector<adapter_t> adapters_t;

    Adapters();
    ~Adapters();

    static Adapters m_instance;
    adapters_t adapters;
};

} /* namespace view_scilab */
} /* namespace org_scilab_modules_scicos */

#endif /* ADAPTERS_HXX_ */
