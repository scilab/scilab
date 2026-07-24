/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021 - 2023 - UTC - Stéphane MOTTELET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "UTF8.hxx"

#include "SPRKODEManager.hxx"
#include "odeparameters.hxx"


bool SPRKODEManager::create()
{
    m_prob_mem = SPRKStepCreate(rhsFunction, rhsFunctionStiff, m_dblT0, m_N_VectorY, m_sunctx);        
    if (m_prob_mem == NULL)
    {
        return true;
    }
    return false;
}

void SPRKODEManager::parseMethodAndOrder(types::optional_list &opt)
{
    char errorMsg[256];
    std::vector<double> defaultRAtolVect = {m_dblDefaultAtol};
    std::vector<int> emptyVect = {};

    // specific SPRKODe options
    
    if (m_bHas[SRHS] == false)
    {
        // throw catched gateway error message when SRHS has been parsed
        throw ast::InternalError(m_wstrLastErrorMessage);
    }

    // Parse fixed step option. 0 falls back to adaptive stepsize but is forbidden for SPRK
    getDoubleInPlist(getSolverName().c_str(), opt, L"fixedStep", &m_dblFixedStep,
        m_odeIsExtension ? m_prevManager->m_dblFixedStep : 0, {0, std::numeric_limits<double>::infinity()});

    if (m_dblFixedStep == 0)
    {
        sprintf(errorMsg, _("sprkode: ""fixedStep"" option must be set with a nonzero value.\n"));
        throw ast::InternalError(errorMsg);
    } 

    // parse "method" option or set default method
    std::wstring wStrDefaultMethod = m_odeIsExtension ? m_prevManager->m_wstrMethod : L"SPRK";
    getStringInPlist(getSolverName().c_str(), opt, L"method", m_wstrMethod, wStrDefaultMethod, getAvailableMethods());        
    m_SPRKTable = ARKodeSPRKTable_Load(ARKODEMethods[m_wstrMethod].sprkID);  

    // interpolation (cannot be changed when extending solution)
    if (m_odeIsExtension)
    {
        m_wstrInterpolationMethod = m_prevManager->m_wstrInterpolationMethod;
        m_iInterpolationMethod = m_prevManager->m_iInterpolationMethod;
        m_iInterpolationDegree = m_prevManager->m_iInterpolationDegree;
    }
    else
    {
        getStringInPlist(getSolverName().c_str(), opt, L"interpolation", m_wstrInterpolationMethod, L"Hermite", {L"Hermite",L"Lagrange"});
        int iMaxInterpolationDegree = m_wstrInterpolationMethod == L"Hermite" ? 5:3;
        getIntInPlist(getSolverName().c_str(), opt, L"degree", &m_iInterpolationDegree, iMaxInterpolationDegree, {0,iMaxInterpolationDegree});
        m_iInterpolationMethod = m_wstrInterpolationMethod == L"Hermite" ? ARK_INTERP_HERMITE : ARK_INTERP_LAGRANGE;
    }
}

bool SPRKODEManager::initialize(char *errorMsg)
{
    if (ARKodeSetFixedStep(m_prob_mem, m_dblFixedStep) != ARK_SUCCESS)
    {
        sprintf(errorMsg, "ARKStepSetFixedStep error");
        return true;                
    };
    if (SPRKStepSetMethod(m_prob_mem, m_SPRKTable) != ARK_SUCCESS)
    {
        sprintf(errorMsg, "SPRKStepSetMethod error");
        return true; 
    }

    // interpolant type and degree
    m_iInterpolationDegree = std::min(m_iMethodOrder-1,m_iInterpolationDegree);
    ARKodeSetInterpolantType(m_prob_mem, m_iInterpolationMethod);
    ARKodeSetInterpolantDegree(m_prob_mem, m_iInterpolationDegree);

    return false;
}




















