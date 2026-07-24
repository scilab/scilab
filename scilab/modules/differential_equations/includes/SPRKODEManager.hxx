//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021-2022 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

#ifndef _SPRKODEMANAGER_HXX_
#define _SPRKODEMANAGER_HXX_

#include "dynlib_differential_equations.h"

#include "ARKODEManager.hxx"

class DIFFERENTIAL_EQUATIONS_IMPEXP SPRKODEManager final : public ARKODEManager
{
public :

    SPRKODEManager()
    {
        m_strSolver = "sprkode";
        m_wstrSolver = L"sprkode";
    }

    void setPreviousManager(void *p)
    {
        m_prevManager = static_cast<SPRKODEManager *>(p);
        m_bHas[SRHS] = m_prevManager->m_bHas[SRHS];
        m_odeIsExtension = true;
    }

    std::wstring getDefaultNonLinSolver()
    {
        return L"NONE";
    }

    std::vector<std::wstring> getAvailableNonLinSolvers()
    {
        return {L"NONE"};
    }

    bool setSolverAndJacobian(char *errorMsg)
    {
        return false;
    }

    int getMaxMethodOrder(std::wstring wstrMethod)
    {
        return 10;
    }

    void parseMethodAndOrder(types::optional_list &opt);

    std::vector<std::wstring> getAvailableMethods()
    {
        std::vector<std::wstring> availableMethods;
  
        for (const auto &it : vectARKODEMethods)
        {
            if (it.second.sprkID >= ARKODE_MIN_SPRK_NUM)
            {
                availableMethods.push_back(it.first);
            }
        }
        return availableMethods;
    }

    bool create();
    bool initialize(char *);

private :

    SPRKODEManager* m_prevManager = NULL;
    ARKodeSPRKTable m_SPRKTable = NULL;
};

#endif
