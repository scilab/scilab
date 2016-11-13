/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include <string>

#include "struct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "scilabWrite.hxx"
#include "output/SLintScilabResult.hxx"
#include "checkers/SLintChecker.hxx"

namespace slint
{

SLintScilabResult::SLintScilabResult() { }
SLintScilabResult::~SLintScilabResult() { }

void SLintScilabResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::string & msg)
{
    auto & mmap = results[context.getFilename()];
    mmap.emplace(loc, checker.getId(sub) + ": " + msg);
}

void SLintScilabResult::finalize()
{
    for (const auto & p1 : results)
    {
        std::string str = "In " + p1.first + ":\n";
        scilabWrite(str.c_str());
        for (const auto & p2 : p1.second)
        {
            std::string str = "  At l. " + std::to_string(p2.first.first_line) + ", c. " +  std::to_string(p2.first.first_column) + ": " + p2.second + "\n";
            scilabWrite(str.c_str());
        }
    }
}

SLintScilabOut::SLintScilabOut() { }
SLintScilabOut::~SLintScilabOut() { }

void SLintScilabOut::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::string & msg)
{
    results[context.getFilename()][checker.getId(sub)].emplace_back(loc, msg);
}

void SLintScilabOut::finalize()
{

}

types::Struct * SLintScilabOut::getStruct() const
{
    /**
     * struct { file : string;
                info : struct { id1 : struct { loc : double[2,2];
    	                                   msg : string;
    					 };
    			    id2 : struct { loc : double[2,2];
    	                                   msg : string;
    					 };
    			    ...
    			   };
    	  }

     */

    types::Struct * st = new types::Struct((int)results.size(), 1);
    st->addField("file");
    st->addField("info");
    int index = 0;
    for (const auto & p1 : results)
    {
        types::SingleStruct * sst = st->get(index++);
        types::Struct * info = new types::Struct(1, 1);
        sst->set("file", new types::String(p1.first.c_str()));
        sst->set("info", info);
        types::SingleStruct * sst_info = info->get(0);

        for (const auto & p2 : p1.second)
        {
            // p2: { Id => { Location, Msg } }
            const std::string & id = p2.first;
            const std::vector<std::pair<Location, std::string>> & infos = p2.second;

            if (!infos.empty())
            {
                info->addField(id);
                types::Struct * loc_msg = new types::Struct((int)infos.size(), 1);
                sst_info->set(id, loc_msg);
                loc_msg->addField("loc");
                loc_msg->addField("msg");

                int index_info = 0;
                for (const auto & p3 : infos)
                {
                    double * loc = nullptr;
                    types::Double * pDbl = new types::Double(2, 2, &loc);
                    loc[0] = p3.first.first_line;
                    loc[1] = p3.first.last_line;
                    loc[2] = p3.first.first_column;
                    loc[3] = p3.first.last_column;
                    loc_msg->get(index_info)->set("loc", pDbl);
                    loc_msg->get(index_info++)->set("msg", new types::String(p3.second.c_str()));
                }
            }
        }
    }

    return st;
}

} // namespace slint
