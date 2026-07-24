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

#include "ARKODEManager.hxx"
#include "SUNDIALSBridge.hxx"
#include "odeparameters.hxx"
#include "arkode/arkode_interp_impl.h"

extern "C"
{
    int ARKodeSetMaxOrd(void *m_prob_mem, int i)
    {
        return 1;
    }
#include "sciprint.h"
}

bool ARKODEManager::create()
{
    if (m_bHas[SRHS])
    {
        m_prob_mem = ARKStepCreate(rhsFunction, rhsFunctionStiff, m_dblT0, m_N_VectorY, m_sunctx);
    }
    else if (m_ERKButcherTable != NULL) // pure explicit
    {
        m_prob_mem = ARKStepCreate(rhsFunction, NULL, m_dblT0, m_N_VectorY, m_sunctx);
    }
    else if (m_DIRKButcherTable != NULL) // pure implicit
    {
        m_prob_mem = ARKStepCreate(NULL, rhsFunction, m_dblT0, m_N_VectorY, m_sunctx);
    }
    else
    {
        return true;
    }
    if (m_prob_mem == NULL)
    {
        return true;
    }
    return false;
}

std::wstring ARKODEManager::getDefaultNonLinSolver()
{
    // default is Newton when method is ImEx or fully implicit
    // and NONE when method is fully explicit.
    return m_DIRKButcherTable != NULL ? L"Newton" : L"NONE";
}

std::vector<std::wstring> ARKODEManager::getAvailableNonLinSolvers()
{
    // fixedPoint and Newton are available when method is ImEx or fully implicit
    if (m_DIRKButcherTable != NULL)
    {
        return {L"fixedPoint",L"Newton"};
    }
    // Only NONE is available when method is fully explicit
    return {L"NONE"};
}

void ARKODEManager::parseMethodAndOrder(types::optional_list &opt)
{
    char errorMsg[256];
    std::vector<double> defaultRAtolVect = {};
    std::vector<int> emptyVect = {};

    // specific ARKODe options

    // Parse Mass first (as it restricts the possible methods and other stuff)
    getIntVectorInPlist(getSolverName().c_str(), opt, L"massBand", m_iVecBand[MASS],
        m_odeIsExtension ? m_prevManager->m_iVecBand[MASS] : emptyVect, {0, m_iNbEq-1}, {2});
    if (m_iVecBand[MASS].size()>0)
    {
        // if band Mass is provided, Sundials packed style is supposed
        m_iSizeOfInput[MASS] = m_iNbEq*(m_iVecBand[MASS][0]+m_iVecBand[MASS][1]+1);
    }

    // MASS function or constant mass
    parseFunctionFromOption(opt, L"mass", MASS);
    
    // Detect Mass type by calling Scilab user function (if applicable)
    if (m_functionAPI[MASS] == SCILAB_CALLABLE)
    {
        // call will set m_typeOfOutput[MASS]
        types::typed_list in;
        in.push_back(new types::Double(m_dblT0));
        computeMatrix(in, MASS);
    }
    else if (m_functionAPI[MASS] == SUNDIALS_DLL)
    {
        // massNonZeros must be declared if SUNDIALS DLL returns a sparse Jacobian
        getIntInPlist(getSolverName().c_str(), opt, L"massNonZeros", &m_iNonZeros[MASS],
            m_odeIsExtension ? m_prevManager->m_iNonZeros[MASS] : -1, {0, m_iNbEq*m_iNbEq});
    }

    // Parse fixed step option. 0 falls back to adaptive stepsize
    // Fixed step allows to use RK method without embedded methods hence without error control
    getDoubleInPlist(getSolverName().c_str(), opt, L"fixedStep", &m_dblFixedStep,
        m_odeIsExtension ? m_prevManager->m_dblFixedStep : 0, {0, std::numeric_limits<double>::infinity()});

    // User Butcher tableau
    if (m_odeIsExtension)
    {
        m_ERKButcherTable = m_prevManager->m_ERKButcherTable; // can be NULL
        m_DIRKButcherTable = m_prevManager->m_DIRKButcherTable; // can be NULL
    }
    else
    {
        getButcherTabInPlist(opt, L"ERKButcherTab", m_ERKButcherTable);
        getButcherTabInPlist(opt, L"DIRKButcherTab", m_DIRKButcherTable);
    }

    if (m_ERKButcherTable != NULL || m_DIRKButcherTable != NULL)
    {
        if (m_bHas[SRHS])
        {
            if (m_ERKButcherTable == NULL || m_DIRKButcherTable == NULL)
            {
                sprintf(errorMsg, _("arkode: both ERKButcherTab and DIRKButcherTab must be set when two rhs are given.\n"));
                throw ast::InternalError(errorMsg);
            }
            ARKodeButcherTable_CheckARKOrder(m_DIRKButcherTable, m_ERKButcherTable, &m_iEmbeddedMethodOrder, &m_iMethodOrder, NULL);
            std::wstringstream wss;
            wss << L"USER_ARK_" << m_ERKButcherTable->stages;
            if (m_iEmbeddedMethodOrder > 0)
            {
                wss << L"_" << m_iEmbeddedMethodOrder;
            } 
            wss << L"_" << m_iMethodOrder;
            m_wstrMethod.assign(wss.str()); 
        }
        else if (m_ERKButcherTable != NULL && m_DIRKButcherTable != NULL)
        {
            sprintf(errorMsg, _("arkode: a secondary rhs is needed when both ERKButcherTab and DIRKButcherTab are given.\n"));
            throw ast::InternalError(errorMsg);
        }
        std::wstringstream wss;
        if (m_ERKButcherTable != NULL)
        {
            wss << L"USER_ERK_" << m_ERKButcherTable->stages;
            if (m_ERKButcherTable->p > 0)
            {
                wss << L"_" << m_ERKButcherTable->p;
            }
            wss  << L"_" << m_ERKButcherTable->q;
            m_iMethodOrder = m_ERKButcherTable->q;
            m_iEmbeddedMethodOrder = m_ERKButcherTable->p;
        }
        if (m_DIRKButcherTable != NULL)
        {
            wss << L"USER_DIRK_" << m_DIRKButcherTable->stages;
            if (m_DIRKButcherTable->p > 0)
            {
                wss << L"_" << m_DIRKButcherTable->p;
            }
            wss << L"_" << m_DIRKButcherTable->q;            
            m_iMethodOrder = m_DIRKButcherTable->q;
            m_iEmbeddedMethodOrder = m_DIRKButcherTable->p;
        }
        m_wstrMethod = wss.str();
    }
    else
    {
        // parse "method" option or set default method
        std::wstring wStrDefaultMethod = m_odeIsExtension ? m_prevManager->m_wstrMethod : (m_bHas[SRHS] ? L"ARK" : hasJacobian() ? L"DIRK" : L"ERK");
        getStringInPlist(getSolverName().c_str(), opt, L"method", m_wstrMethod, wStrDefaultMethod, getAvailableMethods());        
    
        m_iMethodOrder = ARKODEMethods[m_wstrMethod].order;
        m_iEmbeddedMethodOrder = ARKODEMethods[m_wstrMethod].embeddedOrder;
        // get standard SUNDIALS tableaux
        if (ARKODEMethods[m_wstrMethod].dirkID >= ARKODE_MIN_DIRK_NUM)
        {
            m_DIRKButcherTable = ARKodeButcherTable_LoadDIRK(ARKODEMethods[m_wstrMethod].dirkID);            
        }
        if (ARKODEMethods[m_wstrMethod].erkID >= ARKODE_MIN_ERK_NUM)
        {
            m_ERKButcherTable = ARKodeButcherTable_LoadERK(ARKODEMethods[m_wstrMethod].erkID);                
        }            
    }

    // linearity of implicit part
    if (m_DIRKButcherTable != NULL) // implicit or ImEx
    {
        std::wstring wStrDefaultLinear = m_odeIsExtension ? m_prevManager->m_wstrIsLinear : L"no";
        getStringInPlist(getSolverName().c_str(), opt, L"linear", m_wstrIsLinear, wStrDefaultLinear, {L"no",L"constant",L"timeDepend"});
    }

    // Absolute tolerance for the residual norm in nonlinear solver iterations
    getDoubleVectorInPlist(getSolverName().c_str(), opt, L"ratol", m_dblVecRAtol,
        m_odeIsExtension ? m_prevManager->m_dblVecRAtol : defaultRAtolVect, {1e-15, std::numeric_limits<double>::infinity()}, m_iNbEq);

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

void ARKODEManager::getButcherTabInPlist(types::optional_list &opt, const wchar_t * _pwstLabel, ARKodeButcherTable &ButcherTab)
{
    char errorMsg[1024];
    types::InternalType *pI = NULL;
    if (opt.find(_pwstLabel) != opt.end())
    {
        pI = opt[_pwstLabel];
        if (pI->isDouble() == false)
        {
            sprintf(errorMsg, _("%s: wrong value type for parameter \"%ls\": %s expected.\n"), getSolverName().c_str(), _pwstLabel, "double");
            throw ast::InternalError(errorMsg);
        }
        types::Double *pDbl = pI->getAs<types::Double>();

        int iStages = pDbl->getCols()-1;
        // Check if table size is these of a Butcher table with embedded method
        if (iStages < 1 || (pDbl->getRows() != iStages+2 && pDbl->getRows() != iStages+1))
        {
            sprintf(errorMsg, _("%s: wrong size for parameter \"%ls\": size should be (s+2,s+1) or (s+1,s+1), where s is the number of method stages.\n"), getSolverName().c_str(), _pwstLabel);
            throw ast::InternalError(errorMsg);
        }

        if (m_dblFixedStep == 0 && pDbl->getRows() == iStages+1)
        {
            sprintf(errorMsg, _("%s: wrong size (%d,%d) for parameter \"%ls\". Whitout an embedded method ""fixedStep"" option must be set with a positive value.\n"), getSolverName().c_str(), pDbl->getRows(), pDbl->getCols(), _pwstLabel);
            throw ast::InternalError(errorMsg);            
        }
        
        double *pdblA = new double[iStages*iStages];
        double *pdblb = new double[iStages];
        double *pdblc = new double[iStages];
        // pdbld is NULL if no embedded method
        double *pdbld = pDbl->getRows() == iStages+2 ? new double[iStages] : NULL;
        int q;
        int p;

        for (int i=0; i<iStages; i++)
        {
            pdblc[i] = pDbl->get(i,0);
            pdblb[i] = pDbl->get(iStages,i+1);
            if (pdbld != NULL)
            {
                pdbld[i] = pDbl->get(iStages+1,i+1);
            }
            for (int j=0; j<iStages; j++)
            {
                pdblA[j+i*iStages] = pDbl->get(i,j+1);
            }
        }
        q = pDbl->get(iStages,0);
        // p is 0 if no embedded method
        p = pdbld != NULL ? pDbl->get(iStages+1,0) : 0;
        ButcherTab = ARKodeButcherTable_Create(iStages, q, p, pdblc, pdblA, pdblb, pdbld);
        if (ButcherTab == NULL)
        {
            sprintf(errorMsg, _("%s: wrong value for parameter \"%s\": incoherent tableau.\n"), getSolverName().c_str(), scilab::UTF8::toUTF8(_pwstLabel).c_str());
            throw ast::InternalError(errorMsg);
        }
        int iRes = ARKodeButcherTable_CheckOrder(ButcherTab, &q, &p, NULL);
        if (iRes != 0)
        {
            if ((ButcherTab->q >= 6 && q==6) || (ButcherTab->p >= 6 && p==6))
            {
                sciprint(_("%s: parameter \"%s\": sufficient conditions not met for order > 6.\n"), getSolverName().c_str(), scilab::UTF8::toUTF8(_pwstLabel).c_str());
            }
            else
            {
                sprintf(errorMsg, _("%s: wrong value for parameter \"%ls\": claimed orders are (%d,%d) while computed orders are (%d,%d)\n"),
                 getSolverName().c_str(), _pwstLabel,ButcherTab->q,ButcherTab->p,q,p);            
                throw ast::InternalError(errorMsg);                
            }
        }
        delete[] pdblA;
        delete[] pdblb;
        delete[] pdblc;
        if (pdbld != NULL)
        {
            delete[] pdbld;
        }
    }
    else
    {
        ButcherTab = NULL;
        return;
    }

    pI->DecreaseRef();
    pI->killMe();
    opt.erase(_pwstLabel);
}

bool ARKODEManager::initialize(char *errorMsg)
{
    if (ARKodeSetFixedStep(m_prob_mem, m_dblFixedStep) != ARK_SUCCESS)
    {
        sprintf(errorMsg, "ARKStepSetFixedStep error");
        return true;                
    };

    if (ARKStepSetTables(m_prob_mem, m_iMethodOrder, m_iEmbeddedMethodOrder, m_DIRKButcherTable, m_ERKButcherTable) != ARK_SUCCESS)
    {
        sprintf(errorMsg, "ARKStepSetTables error");
        return true;                
    };

    // interpolant type and degree
    m_iInterpolationDegree = std::min(m_iMethodOrder-1,m_iInterpolationDegree);
    ARKodeSetInterpolantType(m_prob_mem, m_iInterpolationMethod);
    ARKodeSetInterpolantDegree(m_prob_mem, m_iInterpolationDegree);

    // Absolute residual tolerance (used by ARKODE only)
    // Only meaningful when a mass matrix is present.
    // Without a mass matrix the residual equals the RHS and shares units with the
    // solution, so rwt must equal ewt (SUNDIALS default when ARKodeResVtolerance is
    // never called). Calling it unconditionally sets rwt_is_ewt = SUNFALSE with
    // ratol=1e-6 while atol may be much tighter (e.g. 1e-9), making the
    // finite-difference Jacobian step 1000× too small at y=0 and causing NaN
    // columns -> Newton divergence -> random integration failure on Linux.
    if (m_dblFixedStep == 0 && m_dblVecRAtol.size() > 0 && m_bHas[MASS] == true)
    {
        m_N_VectorRAtol = N_VClone(m_N_VectorY);
        if (m_odeIsComplex)
        {
            m_dblVecRAtol.resize(m_iNbRealEq);
            for (int i=0; i<m_iNbEq; i++)
            {
                m_dblVecRAtol[i+m_iNbEq] = m_dblVecRAtol[i];
            }
        }
        std::copy(m_dblVecRAtol.begin(), m_dblVecRAtol.end(), N_VGetArrayPointer(m_N_VectorRAtol));
        if (ARKodeResVtolerance(m_prob_mem, m_N_VectorRAtol) < 0)
        {
            sprintf(errorMsg, "ARKStepResVtolerance error");
            return true;
        }
    }

    return false;
}

bool ARKODEManager::setSolverAndJacobian(char *errorMsg)
{
    // Mass matrix
    if (m_bHas[MASS])
    {
        bool bMassTimeDep = m_pIConstFunction[MASS] == NULL;

        setLinearSolver(MASS, m_N_VectorY, m_MASS, m_MASS_LS);
  
        if(ARKodeSetMassLinearSolver(m_prob_mem, m_MASS_LS, m_MASS, bMassTimeDep) != ARK_SUCCESS)
        {
            sprintf(errorMsg,"ARKStepSetMassLinearSolver error\n");
            return true;
        }
        if (ARKodeSetMassFn(m_prob_mem, massFunction)  != ARK_SUCCESS)
        {
            {
                sprintf(errorMsg,"ARKStepSetMassFn error\n");
                return true;
            }
        }
    }

    // if method is purely explicit, exit
    if (m_ERKButcherTable != NULL || m_DIRKButcherTable != NULL)
    {
        if (m_DIRKButcherTable == NULL)
        {
            return false;
        }
    }
    else if (ARKODEMethods[m_wstrMethod].dirkID == ARKODE_DIRK_NONE)
    {
        return false;
    }

    if (m_wstrNonLinSolver == L"Newton")
    {
        if (ARKodeSetLinearSolver(m_prob_mem, m_LS, m_A) != ARK_SUCCESS)
        {
            sprintf(errorMsg,"ARKStepSetLinearSolver error\n");
            return true;
        }

        m_NLS = SUNNonlinSol_Newton(m_N_VectorY, m_sunctx);

        if (m_bHas[JACY])
        {
            if (ARKodeSetJacFn(m_prob_mem, jacFunction) != ARK_SUCCESS)
            {
                sprintf(errorMsg,"ARKStepSetJacFn error\n");
                return true;
            }
        }
        else if (m_pIPattern[JACY] != NULL)
        {
            // Jacobian pattern has been provided
            if (ARKodeSetJacFn(m_prob_mem, colPackJac) != ARK_SUCCESS)
            {
                sprintf(errorMsg,"ARKStepSetJacFn error\n");
                throw ast::InternalError(errorMsg);
            }
        }
        else if (ARKodeSetJacFn(m_prob_mem, NULL) != ARK_SUCCESS)
        {
            sprintf(errorMsg,"ARKStepSetJacFn error\n");
            return true;
        }
        // Jacobian of implicit part is constant or only time dependent
        if (m_pIConstFunction[JACY] != NULL || m_wstrIsLinear == L"constant")
        {
            if (ARKodeSetLinear(m_prob_mem,0) != ARK_SUCCESS)
            {
                sprintf(errorMsg,"ARKStepSetLinear error\n");
                return true;
            }
        }
        else if (m_wstrIsLinear == L"timeDepend")
        {
            if (ARKodeSetLinear(m_prob_mem,1) != ARK_SUCCESS)
            {
                sprintf(errorMsg,"ARKStepSetLinear error\n");
                return true;
            }
        }
        if (m_wstrLinSolver != L"KLU" && m_wstrLinSolver != L"DENSE" && m_iPrecBand.size()>0)
        {
            sciprint("BANDPREC\n");
            ARKBandPrecInit(m_prob_mem, m_iNbRealEq, m_iPrecBand[0], m_iPrecBand[1]);
        }
    }
    else
    {
        /* create fixed point nonlinear solver object */
        m_NLS = SUNNonlinSol_FixedPoint(m_N_VectorY, m_iNonLinSolAccel, m_sunctx);
    }

    /* attach nonlinear solver object to ARKODE */
    if (ARKodeSetNonlinearSolver(m_prob_mem, m_NLS) != ARK_SUCCESS)
    {
        sprintf(errorMsg,"ARKStepSetNonlinearSolver error\n");
        return true;
    }
    if (m_iNonLinSolMaxIters > 0)
    {
        ARKodeSetMaxNonlinIters(m_prob_mem, m_iNonLinSolMaxIters);
    }

    return false;
}

bool ARKODEManager::setEventFunction()
{
    if (ARKodeRootInit(m_prob_mem, m_iNbEvents, eventFunction) != ARK_SUCCESS)
    {
        return true;
    }
    if (m_iVecEventDirection.size() > 0)
    {
        if (ARKodeSetRootDirection(m_prob_mem, m_iVecEventDirection.data()) != ARK_SUCCESS)
        {
            return true;
        }
    }
    return false;
}

int ARKODEManager::getInterpBasisSize()
{
    return std::max(2,m_iInterpolationDegree+1);
}

// Stepper function. Note: ARKODE does not return Yp, but prototype is imposed by Odemanager class
// to include IDA stepper prototype, which yields Yp.
OdeManager::solverReturnCode ARKODEManager::doStep(double dblFinalTime, double *pdblTime, solverTaskCode iKind)
{
    std::map<solverTaskCode, int> toARKODETask = {{ODE_ONE_STEP, ARK_ONE_STEP}, {ODE_NORMAL, ARK_NORMAL}};

    int iFlag = ARKodeEvolve(m_prob_mem, dblFinalTime, m_N_VectorY, pdblTime, toARKODETask[iKind]);
    m_iLastOrder = m_iMethodOrder;

    return toODEReturn[iFlag];
}

int ARKODEManager::getBasisDimensionAtIndex(int iIndex)
{
    if (m_iInterpolationMethod == ARK_INTERP_HERMITE)
    {
        return m_indexInterpBasis[iIndex] -  m_indexInterpBasis[iIndex - 1];                
    }
    else
    {
        // ARK_INTERP_LAGRANGE
        return std::min(iIndex+1,m_iInterpolationDegree+1);
    }
}

double *ARKODEManager::getBasisAtIndex(int iIndex)
{
    if (m_iInterpolationMethod == ARK_INTERP_HERMITE)
    {
        return m_pDblInterpBasisVectors->get() + m_indexInterpBasis[iIndex-1]*m_pDblInterpBasisVectors->getRows();
    }
    else
    {
        // ARK_INTERP_LAGRANGE
        int iDim = std::min(iIndex+1,m_iInterpolationDegree+1);
        return m_pDblInterpBasisVectors->get() + (iIndex-iDim+1)*m_pDblInterpBasisVectors->getRows();
    }
}

void ARKODEManager::saveInterpBasisVectors()
{
    ARKodeMem ark_mem = (ARKodeMem) m_prob_mem;
    ARKInterp interp = ark_mem->interp;
    m_indexInterpBasis.push_back(m_indexInterpBasis.back()+getInterpBasisSize());

    /* basis vectors are defined acording to arkInterpEvaluate_Hermite in arkode_interp.c */

    if (m_iInterpolationMethod == ARK_INTERP_HERMITE)
    {
        std::vector<double> basisVector (m_iNbRealEq);
        basisVector.assign(N_VGetArrayPointer(HINT_YOLD(interp)), N_VGetArrayPointer(HINT_YOLD(interp)) + m_iNbRealEq);
        interpBasisVectorList.push_back(basisVector);
        basisVector.assign(N_VGetArrayPointer(ark_mem->yn), N_VGetArrayPointer(ark_mem->yn) + m_iNbRealEq);
        interpBasisVectorList.push_back(basisVector);
        if (m_iInterpolationDegree > 1)
        {
            basisVector.assign(N_VGetArrayPointer(ark_mem->fn), N_VGetArrayPointer(ark_mem->fn) + m_iNbRealEq);
            interpBasisVectorList.push_back(basisVector);
        }
        if (m_iInterpolationDegree > 2)
        {
            basisVector.assign(N_VGetArrayPointer(HINT_FOLD(interp)), N_VGetArrayPointer(HINT_FOLD(interp)) + m_iNbRealEq);
            interpBasisVectorList.push_back(basisVector);
        }
        if (m_iInterpolationDegree > 3)
        {
            // the call below just aims ensure that FA and FB exist
            ark_mem->interp->ops->evaluate(ark_mem, ark_mem->interp, 0.0, 0, ARK_INTERP_MAX_DEGREE, m_N_VectorYTemp);

            basisVector.assign(N_VGetArrayPointer(HINT_FA(interp)), N_VGetArrayPointer(HINT_FA(interp)) + m_iNbRealEq);
            interpBasisVectorList.push_back(basisVector);
        }
        if (m_iInterpolationDegree > 4)
        {
            basisVector.assign(N_VGetArrayPointer(HINT_FB(interp)), N_VGetArrayPointer(HINT_FB(interp)) + m_iNbRealEq);
            interpBasisVectorList.push_back(basisVector);
        }
    }
}

void ARKODEManager::getInterpVectors(double *pdblNS, int iOrderPlusOne, int iIndex, double dblt0, double dblTUser, double h, double *pdblVect, double *pdblVectd)
{
    char errorMsg[256];
    double tau = (dblTUser-dblt0)/h;
    double tau2, tau3, tau4, tau5;
    
    /* The code comes from arkInterpEvaluate_Hermite in arkode_interp.c */
    tau2 = tau*tau;
    tau3 = tau*tau2;
    tau4 = tau*tau3;
    tau5 = tau*tau4;

    if (m_iInterpolationMethod == ARK_INTERP_HERMITE)
    {
        switch (m_iInterpolationDegree) {

            case(0):    /* constant interpolant, yout = 0.5*(yn+yp) */
            pdblVect[0] = 0.5;
            pdblVect[1] = 0.5;
            pdblVectd[0] = 0;
            pdblVectd[1] = 0;
            break;

            case(1):    /* linear interpolant */
            pdblVect[0] = -tau;
            pdblVect[1] = ONE+tau;
            pdblVectd[0] = -ONE/h;
            pdblVectd[1] =  ONE/h;
            break;

            case(2):    /* quadratic interpolant */
            pdblVect[0] = tau2;
            pdblVect[1] = ONE - tau2;
            pdblVect[2] = h*(tau2 + tau);
            pdblVectd[0] = TWO*tau/h;
            pdblVectd[1] = -TWO*tau/h;
            pdblVectd[2] = (ONE + TWO*tau);
            break;

            case(3):    /* cubic interpolant */
            // [2] and [3] are inverted since we store YOLD,YNEW,FNEW,FOLD (in that order)
            pdblVect[0] = THREE*tau2 + TWO*tau3;
            pdblVect[1] = ONE - THREE*tau2 - TWO*tau3;
            pdblVect[3] = h*(tau2 + tau3);
            pdblVect[2] = h*(tau + TWO*tau2 + tau3);
            pdblVectd[0] = SIX*(tau + tau2)/h;
            pdblVectd[1] = -SIX*(tau + tau2)/h;
            pdblVectd[3] = TWO*tau + THREE*tau2;
            pdblVectd[2] = ONE + FOUR*tau + THREE*tau2;
            break;

            case(4):    /* quartic interpolant */
            // [2] and [3] are inverted since we store YOLD,YNEW,FNEW,FOLD,FA (in that order)
            pdblVect[0] = -SIX*tau2 - SUN_RCONST(16.0)*tau3 - SUN_RCONST(9.0)*tau4;
            pdblVect[1] = ONE + SIX*tau2 + SUN_RCONST(16.0)*tau3 + SUN_RCONST(9.0)*tau4;
            pdblVect[3] = h*FOURTH*(-FIVE*tau2 - SUN_RCONST(14.0)*tau3 - SUN_RCONST(9.0)*tau4);
            pdblVect[2] = h*(tau + TWO*tau2 + tau3);
            pdblVect[4] = h*SUN_RCONST(27.0)*FOURTH*(-tau4 - TWO*tau3 - tau2);
            pdblVectd[0] = (-TWELVE*tau - SUN_RCONST(48.0)*tau2 - SUN_RCONST(36.0)*tau3)/h;
            pdblVectd[1] = (TWELVE*tau + SUN_RCONST(48.0)*tau2 + SUN_RCONST(36.0)*tau3)/h;
            pdblVectd[3] = HALF*(-FIVE*tau - SUN_RCONST(21.0)*tau2 - SUN_RCONST(18.0)*tau3);
            pdblVectd[2] = (ONE + FOUR*tau + THREE*tau2);
            pdblVectd[4] = -SUN_RCONST(27.0)*HALF*(TWO*tau3 + THREE*tau2 + tau);
            break;

            case(5):    /* quintic interpolant */
            // [2] and [3] are inverted since we store YOLD,YNEW,FNEW,FOLD,FA,FB (in that order)
            pdblVect[0] = SUN_RCONST(54.0)*tau5 + SUN_RCONST(135.0)*tau4 + SUN_RCONST(110.0)*tau3 + SUN_RCONST(30.0)*tau2;
            pdblVect[1] = ONE - pdblVect[0];
            pdblVect[3] = h/FOUR*(SUN_RCONST(27.0)*tau5 + SUN_RCONST(63.0)*tau4 + SUN_RCONST(49.0)*tau3 + SUN_RCONST(13.0)*tau2);
            pdblVect[2] = h/FOUR*(SUN_RCONST(27.0)*tau5 + SUN_RCONST(72.0)*tau4 + SUN_RCONST(67.0)*tau3 + SUN_RCONST(26.0)*tau2 + FOUR*tau);
            pdblVect[4] = h/FOUR*(SUN_RCONST(81.0)*tau5 + SUN_RCONST(189.0)*tau4 + SUN_RCONST(135.0)*tau3 + SUN_RCONST(27.0)*tau2);
            pdblVect[5] = h/FOUR*(SUN_RCONST(81.0)*tau5 + SUN_RCONST(216.0)*tau4 + SUN_RCONST(189.0)*tau3 + SUN_RCONST(54.0)*tau2);
            pdblVectd[0] = (SUN_RCONST(270.0)*tau4 + SUN_RCONST(540.0)*tau3 + SUN_RCONST(330.0)*tau2 + SUN_RCONST(60.0)*tau)/h;
            pdblVectd[1] = -pdblVectd[0];
            pdblVectd[3] = (SUN_RCONST(135.0)*tau4 + SUN_RCONST(252.0)*tau3 + SUN_RCONST(147.0)*tau2 + SUN_RCONST(26.0)*tau)/FOUR;
            pdblVectd[2] = (SUN_RCONST(135.0)*tau4 + SUN_RCONST(288.0)*tau3 + SUN_RCONST(201.0)*tau2 + SUN_RCONST(52.0)*tau + FOUR)/FOUR;
            pdblVectd[4] = (SUN_RCONST(405.0)*tau4 + SUN_RCONST(4.0)*189*tau3 + SUN_RCONST(405.0)*tau2 + SUN_RCONST(54.0)*tau)/FOUR;
            pdblVectd[5] = (SUN_RCONST(405.0)*tau4 + SUN_RCONST(864.0)*tau3 + SUN_RCONST(567.0)*tau2 + SUN_RCONST(108.0)*tau)/FOUR;
            break;

            default:
            sprintf(errorMsg, _("%s: %d is an invalid interpolation degree \n"), getSolverName().c_str(), m_iInterpolationDegree);
            throw ast::InternalError(errorMsg);
        }
    }
    else
    {
        int iDim = std::min(iIndex+1,m_iInterpolationDegree+1);
        double q = 0;
        for (int j=0; j<iDim; j++)
        {
            // compute the value of jth Lagrange polynomial at t=dblTUser
            pdblVect[j] = 1.0;
            for (int k=0; k<iDim; k++)
            {
                if (k==j) continue;
                pdblVect[j] *= (dblTUser - m_dblVecCurrTime[iIndex-iDim+1+k])/(m_dblVecCurrTime[iIndex-iDim+1+j] - m_dblVecCurrTime[iIndex-iDim+1+k]);                    
            }
            // compute the value of the derivative of jth Lagrange polynomial at t=dblTUser
            // formula is obtained by derivating the logarithm of jth Lagrange polynomial
            pdblVectd[j] = 0;
            for (int i=0; i<iDim; i++)
            {
                if (i == j) continue;
                q = 1.0;
                for (int k=0; k<iDim; k++)
                {
                  if (k == j) continue;
                  if (k == i) continue;
                  q *= (dblTUser-m_dblVecCurrTime[iIndex-iDim+1+k])/(m_dblVecCurrTime[iIndex-iDim+1+j]-m_dblVecCurrTime[iIndex-iDim+1+k]);
                }
                pdblVectd[i] += q/(m_dblVecCurrTime[iIndex-iDim+1+j]-m_dblVecCurrTime[iIndex-iDim+1+i]);
            }            
        }
    }
}

int ARKODEManager::DQJtimes(sunrealtype tt, N_Vector yy, N_Vector yp, N_Vector rr, N_Vector v, N_Vector Jv, sunrealtype c_j, N_Vector work2, N_Vector work3)
{
    ARKodeMem ark_mem = (ARKodeMem) m_prob_mem;
    ARKLsMem arkls_mem;
    void* ark_step_lmem;

    ark_step_lmem = ark_mem->step_getlinmem(ark_mem);
    arkls_mem = (ARKLsMem) ark_step_lmem;    

    return arkls_mem->jtimes(v, Jv, tt, yy, yp, m_prob_mem, work2);
}

types::Struct *ARKODEManager::getStats()
{
    double dblStat[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    std::wstring fieldNames[15] = {L"nSteps", L"nRhsEvals", L"nRhsEvalsFD", L"nJacEvals", L"nEventEvals",
    L"nLinSolve", L"nRejSteps", L"nNonLiniters", L"nNonLinCVFails", L"order",
    L"hIni", L"hLast", L"hCur", L"tCur", L"eTime"};

    ARKodeGetStepStats(m_prob_mem, m_incStat, dblStat, dblStat+1, dblStat+2, dblStat+3);

    dblStat[4] = m_dblElapsedTime;
    ARKodeGetNumStepAttempts(m_prob_mem, m_incStat+7);
    m_incStat[7] = m_incStat[7]-m_incStat[0];

    ARKodeGetNumRhsEvals(m_prob_mem, 0, m_incStat+1);
    ARKodeGetNumRhsEvals(m_prob_mem, 1, m_incStat+2);

    if (m_wstrNonLinSolver == L"Newton")
    {
        ARKodeGetNumLinSolvSetups(m_prob_mem, m_incStat+6);
        ARKodeGetNumJacEvals(m_prob_mem, m_incStat+4);
        ARKodeGetNumLinRhsEvals(m_prob_mem, m_incStat+3);
    }
    ARKodeGetNonlinSolvStats(m_prob_mem, m_incStat+8, m_incStat+9);

    if (m_iNbEvents > 0)
    {
        ARKodeGetNumGEvals(m_prob_mem, m_incStat+5);
    }

    // if extending a previous solution, update incremental stats only
    if (m_prevManager != NULL)
    {
        for (int i=0; i<10; i++)
        {
           m_incStat[i] += m_prevManager-> m_incStat[i];
        }
    }

    types::Struct *pSt = new types::Struct(1,1);

    // nSteps
    pSt->addField(fieldNames[0].c_str());
    pSt->get(0)->set(fieldNames[0].c_str(),new types::Double((double)m_incStat[0]));

    // nRhsEvals (first and second rhs)
    types::Double *pDblNRhsEvals = NULL;
    if (m_incStat[1] == 0 || m_incStat[2] == 0)
    {
        pDblNRhsEvals = new types::Double(m_incStat[1]+ m_incStat[2]);
    }
    else
    {
        pDblNRhsEvals = new types::Double(1, 2);
        pDblNRhsEvals->set(0,(double)m_incStat[1]);
        pDblNRhsEvals->set(1,(double)m_incStat[2]);
    }
    pSt->addField(fieldNames[1].c_str());
    pSt->get(0)->set(fieldNames[1].c_str(),pDblNRhsEvals);

    // nRhsEvalsFD, nJacEvals, nEventEvals, nLinSolve, nRejSteps, nNonLiniters, nNonLinCVFails
    for (int i=2; i<9; i++)
    {
        pSt->addField(fieldNames[i].c_str());
        pSt->get(0)->set(fieldNames[i].c_str(),new types::Double((double)m_incStat[i+1]));
    }

    // order of method for each step
    types::Double *pDblOrder = new types::Double(m_iMethodOrder);
    pSt->addField(fieldNames[9].c_str());
    pSt->get(0)->set(fieldNames[9].c_str(), pDblOrder);

    for (int i=10; i<15; i++)
    {
        pSt->addField(fieldNames[i].c_str());
        pSt->get(0)->set(fieldNames[i].c_str(), new types::Double(dblStat[i-10]));
    }
    return pSt;
}




















