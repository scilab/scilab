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

#include "OdeManager.hxx"
#include "odeparameters.hxx"
#include "complexHelpers.hxx"

extern "C"
{
#include "sciprint.h"
}

void OdeManager::parseFunctionFromOption(types::optional_list &opt, const wchar_t * _pwstLabel, functionKind what)
{
    types::InternalType *pIn = NULL;
    if (opt.find(_pwstLabel) == opt.end())
    {
        // we are possibly extending a solution with same Rhs, Jacobian, Events, ... function
        if (m_odeIsExtension)
        {
            m_functionAPI[what] =  getPreviousManager()->m_functionAPI[what];
            m_pCallFunction[what] = getPreviousManager()->m_pCallFunction[what]; // can be NULL
            m_pEntryPointFunction[what] = getPreviousManager()->m_pEntryPointFunction[what]; // can be NULL
            m_pEntryPointFunctionWithPar[what] = getPreviousManager()->m_pEntryPointFunctionWithPar[what]; // can be NULL
            m_pParameters[what] = getPreviousManager()->m_pParameters[what]; // can be NULL
            m_bHas[what] = getPreviousManager()->m_bHas[what]; // can be false
        }
        return;
    }
 
    pIn = opt[_pwstLabel];
    parseFunction(pIn, what);
 
    pIn->DecreaseRef();
    pIn->killMe();
    opt.erase(_pwstLabel);
}

void OdeManager::parseMatrices(types::typed_list &in)
{
    char errorMsg[256];
    types::Double *pDblTSpan = NULL;
    int iTspan = m_odeIsExtension ? 1 :  in.size() - (isDAE() ? 3 : 2);

    // Parse second (or third) argument "tspan"
    if (in[iTspan]->isDouble() && !in[iTspan]->getAs<types::Double>()->isComplex())
    {
        pDblTSpan = in[iTspan]->getAs<types::Double>();

        // Check if we are extending a previously computed solution
        if (m_odeIsExtension)
        {
            OdeManager *prevManager = getPreviousManager();
            if (pDblTSpan->getSize() > 1)
            {
                sprintf(errorMsg, _("%s: Wrong size for input argument #%d: expecting %d.\n"), getSolverName().c_str(),iTspan+1, 1);
                throw ast::InternalError(errorMsg);
            }
            double dblTFinal = prevManager->m_dblVecTOut.back();
            if ( ((pDblTSpan->get(0)-dblTFinal)*(dblTFinal-prevManager->m_dblT0)) < 0)
            {
                sprintf(errorMsg, _("%s: parameter #%d must be greater than %g.\n"), getSolverName().c_str(),iTspan+1, dblTFinal);
                throw ast::InternalError(errorMsg);
            }
            // Simulate option t0 = tFinal
            m_dblOptT0 = dblTFinal;
        }

        m_pDblTSpan = pDblTSpan->clone();
        if (m_pDblTSpan->getSize() > 2)
        {
            for (int i=2; i<m_pDblTSpan->getSize(); i++)
            {
                if ( (m_pDblTSpan->get(i)-m_pDblTSpan->get(i-1)) * (m_pDblTSpan->get(i-1)-m_pDblTSpan->get(i-2)) < 0)
                {
                    sprintf(errorMsg, _("%s: values in parameter #%d should be monotone.\n"), getSolverName().c_str(),iTspan+1);
                    throw ast::InternalError(errorMsg);
                }
            }
        }
    }
    else
    {
        sprintf(errorMsg, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), getSolverName().c_str(),iTspan+1);
        throw ast::InternalError(errorMsg);
    }

    // parse initial conditions. Use final conditions of previous solution if extending
    if (m_odeIsExtension == false)
    {
        m_pDblY0 = parseInitialCondition(in, iTspan+1);
        if (isDAE())
        {
            m_pDblYp0 = parseInitialCondition(in, iTspan+2);
            if (m_pDblYp0->getSize() != m_pDblY0->getSize())
            {
                sprintf(errorMsg, _("%s: y0 and yp0 must have the same size.\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);
            }
        }        
    }
    else
    {
        OdeManager *prevManager = getPreviousManager();
        // by default, extend solution with Y0/Yp0 = final value of previous Y/Yp

        m_pDblY0 = prevManager->m_pDblY0->clone(); // Just for the size and dimensions
        copyComplexVectorToDouble(N_VGetArrayPointer(prevManager->m_N_VectorY), m_pDblY0->get(), m_pDblY0->getImg(), prevManager->m_iNbEq, prevManager->m_odeIsComplex);
        if (isDAE())
        {
            m_pDblYp0 = prevManager->m_pDblY0->clone(); // Just for the size and dimensions
            copyComplexVectorToDouble(N_VGetArrayPointer(prevManager->m_N_VectorYp), m_pDblYp0->get(), m_pDblYp0->getImg(), prevManager->m_iNbEq, prevManager->m_odeIsComplex);
        }

        m_odeIsComplex = prevManager->m_odeIsComplex;
    }

    m_iNbEq = m_pDblY0->getSize();

    m_iSizeOfInput[RHS] = m_iNbEq;
    m_iSizeOfInput[SRHS] = m_iNbEq;
    m_iSizeOfInput[RES] = m_iNbEq;
    m_iSizeOfInput[PROJ] = m_iNbEq;
 
    m_iSizeOfInput[JACY] = m_iNbEq*m_iNbEq;
    m_iSizeOfInput[JACYYP] = m_iNbEq*m_iNbEq;
    m_iSizeOfInput[MASS] = m_iNbEq*m_iNbEq;
}

types::Double *OdeManager::parseInitialCondition(types::typed_list &in, int iPos)
{
    char errorMsg[256];
    types::Double *pDblY = NULL;

    // Parse argument "y0" or "yp0"
    // They can be parsed as options when extending solution (otherwise final values are used)
    if (in[iPos]->isDouble())
    {
        types::Double *pDbl =  in[iPos]->getAs<types::Double>();
        if (pDbl->isVector() && pDbl->getRows() == 1)
        {
            types::InternalType* pIT = pDblY;
            pDbl->transpose(pIT);
            pDblY = static_cast<decltype(pDblY)>(pIT);
        }
        else
        {
            pDblY = pDbl->clone();
        }
    }
    else
    {
        sprintf(errorMsg, _("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"), getSolverName().c_str(),iPos+1);
        throw ast::InternalError(errorMsg);
    }
    m_odeIsComplex |= pDblY->isComplex();

    return pDblY;
}

void OdeManager::parseOptions(types::optional_list &opt)
{
    char errorMsg[256];
    std::vector<int> emptyVect = {};
    std::vector<double> defaultAtolVect = {m_dblDefaultAtol};
    OdeManager *prevManager = getPreviousManager();
    std::wstring wstrDefaultMethod;

    // first process a struct of options
    if (opt.find(L"options") != opt.end())
    {
        if (opt[L"options"]->isStruct() == false || opt[L"options"]->getAs<types::Struct>()->getSize() != 1)
        {
            sprintf(errorMsg, _("%s: Wrong type for options parameter: expecting a scalar Struct.\n"), getSolverName().c_str());
            throw ast::InternalError(errorMsg);
        }
        types::Struct* pSt = opt[L"options"]->getAs<types::Struct>();
        types::String* pFields = pSt->getFieldNames();
         for (int i=0; i<pFields->getSize(); i++)
         {
             auto pWStr = pFields->get(i);
             types::InternalType *pIT = pSt->get(0)->get(pWStr)->clone();;
             pIT->IncreaseRef();
             if (opt.find(pWStr) == opt.end())
             {
                 opt[pWStr]=pIT;                 
             }
         }
         opt.erase(L"options");
    }
    
    if (m_odeIsExtension == false)
    {
        // Initial time
        if (m_pDblTSpan->getSize() > 1 )
        {
            if (m_pDblTSpan->get(0) < m_pDblTSpan->get(1))
            {
                getDoubleInPlist(getSolverName().c_str(),opt, L"t0", &m_dblOptT0,
                    m_dblOptT0, {-std::numeric_limits<double>::infinity(), m_pDblTSpan->get(0)});
            }
            else
            {
                getDoubleInPlist(getSolverName().c_str(),opt, L"t0", &m_dblOptT0,
                    m_dblOptT0, {m_pDblTSpan->get(0), std::numeric_limits<double>::infinity()});
            }
        }
        else // "t0" is mandatory if  m_TSpan->getSize() == 1
        {
            getDoubleInPlist(getSolverName().c_str(),opt, L"t0", &m_dblOptT0, m_dblOptT0, {});
        }
        // final definition of m_dblT0
        if (std::isnan(m_dblOptT0))
        {
            if (m_pDblTSpan->getSize() > 1)
            {
                m_dblT0 = m_pDblTSpan->get(0);
            }
            else
            {
                sprintf(errorMsg, _("%s: parameter \"%ls\" must be defined by user.\n"), getSolverName().c_str(),L"t0");
                throw ast::InternalError(errorMsg);
            }
         }
        else
        {
            m_dblT0 = m_dblOptT0;
        }
    }
    else
    {
        m_dblT0 = m_dblOptT0;
    }

    // parse eventual sensitivity parameter as it has to be added early in the list of parameters
    // if callables have to be tested
    if (hasSensFeature() && opt.find(L"sensPar") != opt.end())
    {
        // parameter w.r.t. which sensitivity is to be computed
        if (opt[L"sensPar"]->isDouble())
        {
            types::Double *pDbl =  opt[L"sensPar"]->getAs<types::Double>();
            if (pDbl->isComplex() == false)
            {
                m_pDblSensPar = pDbl;
                m_pDblSensPar->IncreaseRef();
                // add the parameter to parameters list for RHS, SENSRHS, JACY, JACYTIMES before eventual other parameters
                if (isDAE() == true)
                {
                    for (auto& what : {RES, SENSRES, JACYYP, JACYYPTIMES})
                    {
                        m_pParameters[what].insert(m_pParameters[what].begin(),m_pDblSensPar);
                    }
                }
                else
                {
                    for (auto& what : {RHS, SENSRHS, JACY, JACYTIMES})
                    {
                        m_pParameters[what].insert(m_pParameters[what].begin(),m_pDblSensPar);
                    }
                }
            }
        }
        if (m_pDblSensPar == NULL)
        {
            sprintf(errorMsg, _("%s: Wrong type for option \"sensPar\": a real double vector is expected.\n"), getSolverName().c_str());
            throw ast::InternalError(errorMsg);
        }
        opt.erase(L"sensPar");
        
        // parameter components w.r.t. which sensitivity is to be computed (default is 1:m_pDblSensPar->getSize())
        getIntVectorInPlist(getSolverName().c_str(),opt, L"sensParIndex", m_iVecSensParIndex,
            m_odeIsExtension ? prevManager->m_iVecSensParIndex : emptyVect, {1,m_pDblSensPar->getSize()}, {1,m_pDblSensPar->getSize()});

        // Scaling vector (default is ones(getNbSensPar(),1))
        getDoubleVectorInPlist(getSolverName().c_str(),opt, L"typicalPar", m_dblVecTypicalPar,
            m_odeIsExtension ? prevManager->m_dblVecTypicalPar : std::vector<double>(getNbSensPar(),1.0), 
            {0, std::numeric_limits<double>::infinity()}, getNbSensPar());

        // initial condition of sensitivity (default is zeros(m_iNbEq,getNbSensPar()))
        if (opt.find(L"yS0") != opt.end())
        {
            if (m_pDblSensPar == NULL)
            {
                sprintf(errorMsg, _("%s: sensitivity parameter \"sensPar\" has not been set.\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);         
            }
            if (opt[L"yS0"]->isDouble())
            {
                types::Double *pDbl = opt[L"yS0"]->getAs<types::Double>();
                if (pDbl->isComplex() == false && pDbl->getDims()==2 && pDbl->getRows() == m_iNbEq && pDbl->getCols() == getNbSensPar())
                {
                    m_pDblYS0 = pDbl;
                    m_pDblYS0->IncreaseRef();
                    opt.erase(L"yS0");
                }
            }
            if (m_pDblYS0 == NULL)
            {
                sprintf(errorMsg, _("%s: Wrong type and/or size for option \"yS0\": a real double matrix of size %d x %d is expected.\n"), 
                    getSolverName().c_str(), m_iNbEq, getNbSensPar());
                throw ast::InternalError(errorMsg);                
            }
        }
        // default zero matrix
        if (m_pDblSensPar != NULL && m_pDblYS0 == NULL)
        {
            m_pDblYS0 = new types::Double(m_iNbEq,getNbSensPar());
            m_pDblYS0->setZeros();
            m_pDblYS0->IncreaseRef();
        }

        // correction method 
        wstrDefaultMethod = m_odeIsExtension ? prevManager->m_wstrMethod : getAvailableMethods()[0];
        getStringInPlist(getSolverName().c_str(),opt, L"sensCorrStep", m_wstrSensCorrStep, 
             m_odeIsExtension ? prevManager->m_wstrSensCorrStep : L"simultaneous", {L"simultaneous",L"staggered"});

        // Sensitivity error control (include sensitivity variables in error test, default is false)
        getBooleanInPlist(getSolverName().c_str(),opt, L"sensErrCon", &m_bSensErrCon, m_odeIsExtension ? prevManager->m_bSensErrCon : false);
        
        // sensitivity rhs or residual, if user knows how to derive rhs w.r.t. parameter
        functionKind whatSENSEQ = isDAE() ? SENSRES : SENSRHS;
        parseFunctionFromOption(opt, isDAE() ? L"sensRes" : L"sensRhs", whatSENSEQ);
        m_iSizeOfInput[whatSENSEQ] = getNbSensPar()*m_iNbEq;            
    }

    // pure quadrature variables
    if (hasQuadFeature() && opt.find(L"quadRhs") != opt.end())
    {
        // parse quadrature variables rhs function
        parseFunctionFromOption(opt, L"quadRhs", QRHS);
        if (opt.find(L"yQ0") != opt.end())
        {
            if (opt[L"yQ0"]->isDouble())
            {
                types::Double *pDbl = opt[L"yQ0"]->getAs<types::Double>();
                m_pDblYQ0 = pDbl;
                m_pDblYQ0->IncreaseRef();
                m_iSizeOfInput[QRHS] = m_pDblYQ0->getSize();
                opt.erase(L"yQ0");
                m_odeIsComplex |= m_pDblYQ0->isComplex();
            }
            else
            {
                sprintf(errorMsg, _("%s: Wrong type for option \"yQ0\": a double matrix is expected.\n"), 
                    getSolverName().c_str());
                throw ast::InternalError(errorMsg);                
            }
        }
        if (m_functionAPI[QRHS] == SCILAB_CALLABLE)
        {
            types::typed_list in;
            if (m_pDblYQ0 == NULL)
            {
                // call will set m_iSizeOfInput[QRHS] from scilab function output
                callOpening(QRHS, in, m_dblT0);
                computeFunction(in, QRHS, NULL);
                m_pDblYQ0 = new types::Double(m_iSizeOfInput[QRHS],1);
                m_pDblYQ0->setZeros();
                m_pDblYQ0->IncreaseRef();
            }
            else
            {
                // call will check that scilab function output has size m_iSizeOfInput[QRHS]
                callOpening(QRHS, in, m_dblT0);                
                computeFunction(in, QRHS, NULL);
            }            
        }
        if (m_pDblYQ0 == NULL)
        {
            sprintf(errorMsg, _("%s: option \"yQ0\" has not been set.\n"), 
                getSolverName().c_str());
            throw ast::InternalError(errorMsg);                                
        }
        // security setting. ode can turn to be complex due to quadrature variable only
        // in that case we promote m_pDblY0.
        m_pDblY0->setComplex(m_odeIsComplex);
        
        // Quadrature states error control (include thems in error test, default is false)
        getBooleanInPlist(getSolverName().c_str(),opt, L"quadErrCon", &m_bQuadErrCon, m_odeIsExtension ? prevManager->m_bQuadErrCon : false);
        
        // Common options
        getDoubleInPlist(getSolverName().c_str(),opt, L"rtolQ", &m_dblQuadRtol,
            m_odeIsExtension ? prevManager->m_dblQuadRtol : m_dblDefaultRtol, {1e-15, 1});
        getDoubleVectorInPlist(getSolverName().c_str(),opt, L"atolQ", m_dblVecQuadAtol,
            m_odeIsExtension ? prevManager->m_dblVecQuadAtol : defaultAtolVect, {1e-15, std::numeric_limits<double>::infinity()}, m_pDblYQ0->getSize());
        
    }

    // Parse Jacobian first (as it restricts the possible methods and other stuff)
    functionKind whatJAC = isDAE() ? JACYYP : JACY;

    parseMatrixPattern(opt, L"jacPattern", whatJAC);
    //TODO: implement jacTimes option !
    if (m_pIPattern[JACY] == NULL)
    {
        getIntVectorInPlist(getSolverName().c_str(),opt, L"jacBand", m_iVecBand[whatJAC],
        m_odeIsExtension ? prevManager->m_iVecBand[whatJAC] : emptyVect, {0, m_iNbEq-1}, {2});
        if (m_iVecBand[whatJAC].size()>0)
        {
            // if band Jacobian is provided, Sundials packed style is supposed
            m_iSizeOfInput[whatJAC] = m_iNbEq*(m_iVecBand[whatJAC][0]+m_iVecBand[whatJAC][1]+1);
        }

        // parse Jacobian. If constant Jacobian then m_typeOfOutput[JACY] wil be set
        parseFunctionFromOption(opt, L"jacobian", whatJAC);

        // Detect Jacobian type by calling Scilab user function (if applicable)
        if (m_functionAPI[whatJAC] == SCILAB_CALLABLE)
        {
            // call will set m_typeOfOutput[JACY] or m_typeOfOutput[JACYYP]
            types::typed_list in;
            callOpening(whatJAC, in, m_dblT0);
            if (whatJAC == JACYYP)
            {
                in.push_back(new types::Double(1.0));
            }
            computeMatrix(in, whatJAC, NULL);
        }
        else if (m_functionAPI[whatJAC] == SUNDIALS_DLL)
        {
            // jacNonZeros must be declared if SUNDIALS DLL returns a sparse Jacobian
            getIntInPlist(getSolverName().c_str(),opt, L"jacNonZeros", &m_iNonZeros[whatJAC],
                m_odeIsExtension ? prevManager->m_iNonZeros[whatJAC] : -1, {0, m_iNbEq*m_iNbEq});
            if (m_iNonZeros[whatJAC] >= 0)
            {
                getStringInPlist(getSolverName().c_str(),opt, L"jacFormat", m_wstrSparseFormat[whatJAC],
                    m_odeIsExtension ? prevManager->m_wstrSparseFormat[whatJAC] : L"CSC", {L"CSC",L"CSR"});            
            }
        }
    }
    
    // Parse specific stuff (the method below is overriden for each solver)
    parseMethodAndOrder(opt);

#ifdef _OPENMP
    // Thread or no threads
    getIntInPlist(getSolverName().c_str(),opt, L"nbThreads", &m_iNbThreads,
        m_odeIsExtension ? prevManager->m_iNbThreads : 0, {0, INT_MAX});
#endif
    // Common options
    getDoubleInPlist(getSolverName().c_str(),opt, L"rtol", &m_dblRtol,
        m_odeIsExtension ? prevManager->m_dblRtol : m_dblDefaultRtol, {1e-15, 1});
    getDoubleVectorInPlist(getSolverName().c_str(),opt, L"atol", m_dblVecAtol,
        m_odeIsExtension ? prevManager->m_dblVecAtol : defaultAtolVect, {1e-15, std::numeric_limits<double>::infinity()}, m_iNbEq);
    getIntInPlist(getSolverName().c_str(),opt, L"refine", &m_iNRefine,
        m_odeIsExtension ? prevManager->m_iNRefine : 0, {0, INT_MAX});

    getDoubleInPlist(getSolverName().c_str(),opt, L"h0", &m_dblInitialStep,
        m_odeIsExtension ? prevManager->m_dblInitialStep : 0, {0, std::numeric_limits<double>::infinity()});
    getIntInPlist(getSolverName().c_str(),opt, L"maxSteps", &m_iMaxNumSteps,
        m_odeIsExtension ? prevManager->m_iMaxNumSteps : 500, {0, INT_MAX});
    getDoubleInPlist(getSolverName().c_str(),opt, L"hMax", &m_dblMaxStep,
        m_odeIsExtension ? prevManager->m_dblMaxStep : 0, {0, std::numeric_limits<double>::infinity()});
    getDoubleInPlist(getSolverName().c_str(),opt, L"hMin", &m_dblMinStep,
        m_odeIsExtension ? prevManager->m_dblMinStep : 0, {0, std::numeric_limits<double>::infinity()});

    getIntVectorInPlist(getSolverName().c_str(),opt, L"positive", m_iVecPositive,
        m_odeIsExtension ? prevManager->m_iVecPositive : emptyVect, {1,m_iNbEq}, {1,m_iNbEq});
    getIntVectorInPlist(getSolverName().c_str(),opt, L"negative", m_iVecNegative,
        m_odeIsExtension ? prevManager->m_iVecNegative : emptyVect, {1,m_iNbEq}, {1,m_iNbEq});
    getIntVectorInPlist(getSolverName().c_str(),opt, L"nonNegative", m_iVecNonNegative,
        m_odeIsExtension ? prevManager->m_iVecNonNegative : emptyVect, {1,m_iNbEq}, {1,m_iNbEq});
    getIntVectorInPlist(getSolverName().c_str(),opt, L"nonPositive", m_iVecNonPositive,
        m_odeIsExtension ? prevManager->m_iVecNonPositive : emptyVect, {1,m_iNbEq}, {1,m_iNbEq});

    // non linear solver
    getStringInPlist(getSolverName().c_str(),opt, L"nonLinSol", m_wstrNonLinSolver,
        m_odeIsExtension ? prevManager->m_wstrNonLinSolver : getDefaultNonLinSolver(), getAvailableNonLinSolvers());
    getIntInPlist(getSolverName().c_str(),opt, L"nonLinSolMaxIters", &m_iNonLinSolMaxIters,
        m_odeIsExtension ? prevManager->m_iNonLinSolMaxIters : 0, {0, INT_MAX});
    getIntInPlist(getSolverName().c_str(),opt, L"nonLinSolAccel", &m_iNonLinSolAccel,
        m_odeIsExtension ? prevManager->m_iNonLinSolAccel : 0, {0, INT_MAX});

    // parse linear solver
    getStringInPlist(getSolverName().c_str(),opt, L"linearSolver", m_wstrLinSolver, 
        m_odeIsExtension ? prevManager->m_wstrLinSolver : getDefaultLinSolver(), getAvailableLinSolvers());

    // iterative solvers
    if (m_wstrLinSolver == L"CG" || 
        m_wstrLinSolver == L"BCGS" || 
        m_wstrLinSolver == L"FGMR" || 
        m_wstrLinSolver == L"GMR" || 
        m_wstrLinSolver == L"TFQMR")
    {
        getIntInPlist(getSolverName().c_str(),opt, L"linSolMaxIters", &m_iLinSolMaxIters,
            m_odeIsExtension ? prevManager->m_iLinSolMaxIters : 0, {0, INT_MAX});
        if (hasBandPrec())
        {
            getIntVectorInPlist(getSolverName().c_str(),opt, L"precBand", m_iPrecBand,
                m_odeIsExtension ? prevManager->m_iPrecBand : emptyVect, {0, m_iNbEq-1}, {2});                
        }
        getStringInPlist(getSolverName().c_str(),opt, L"precType", m_wstrPrecondType,
            m_odeIsExtension ? prevManager->m_wstrPrecondType : getDefaultPrecondType(), getAvailablePrecondType());
    } 

    // options with user functions
    parseFunctionFromOption(opt, L"callback", INTCB);
    parseFunctionFromOption(opt, L"events", EVENTS);
    setupEvents(opt);

    if (m_odeIsExtension)
    {
        // y0 option
        if (opt.find(L"y0") != opt.end())
        {
            if (opt[L"y0"]->isDouble())
            {
                types::Double *pDbl = opt[L"y0"]->getAs<types::Double>();
                bool bSameDims = true;
                if (pDbl->getDims() == m_pDblY0->getDims())
                {
                    for (int i=0; i<pDbl->getDims(); i++)
                    {
                        bSameDims &= (pDbl->getDimsArray()[i] == m_pDblY0->getDimsArray()[i]);
                    }
                }
                if (bSameDims == false)
                {
                    sprintf(errorMsg, _("%s: Wrong size for option \"y0\": expecting %d x %d.\n"), getSolverName().c_str(),m_pDblY0->getRows(), m_pDblY0->getCols());
                    throw ast::InternalError(errorMsg);
                }
                if (prevManager->m_odeIsComplex == false && pDbl->isComplex())
                {
                    sprintf(errorMsg, _("%s: Wrong type (complex) for option \"y0\": extended ode is real.\n"), getSolverName().c_str());
                    throw ast::InternalError(errorMsg);
                }
                m_pDblY0->killMe();
                m_pDblY0 = pDbl->clone();
                m_pDblY0->setComplex(m_odeIsComplex);
                opt.erase(L"y0");
            }
            else
            {
                sprintf(errorMsg, _("%s: Wrong type for option \"y0\": a double vector is expected.\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);
            }
        }

        if (isDAE() && opt.find(L"yp0") != opt.end())
        {
            if (opt[L"yp0"]->isDouble())
            {
                types::Double *pDbl = opt[L"yp0"]->getAs<types::Double>();
                bool bSameDims = true;
                if (pDbl->getDims() == m_pDblYp0->getDims())
                {
                    for (int i=0; i<pDbl->getDims(); i++)
                    {
                        bSameDims &= (pDbl->getDimsArray()[i] == m_pDblYp0->getDimsArray()[i]);
                    }
                }
                if (bSameDims == false)
                {
                    sprintf(errorMsg, _("%s: Wrong size for option \"y0\": expecting %d x %d.\n"), getSolverName().c_str(),m_pDblYp0->getRows(), m_pDblYp0->getCols());
                    throw ast::InternalError(errorMsg);
                }
                if (prevManager->m_odeIsComplex == false && pDbl->isComplex())
                {
                    sprintf(errorMsg, _("%s: Wrong type (complex) for option \"y0\": extended ode is real.\n"), getSolverName().c_str());
                    throw ast::InternalError(errorMsg);
                }
                m_pDblYp0->killMe();
                m_pDblYp0 = pDbl->clone();
                m_pDblYp0->setComplex(m_odeIsComplex);
                opt.erase(L"yp0");
            }
            else
            {
                sprintf(errorMsg, _("%s: Wrong type for option \"yp0\": a double vector is expected.\n"), getSolverName().c_str());
                throw ast::InternalError(errorMsg);
            }
        }

        // By default we use rhs of previous solution.
        // rhs or residual function can be overriden by "rhs" and "res" options
        functionKind what = isDAE() ? RES : RHS;
        parseFunctionFromOption(opt, isDAE() ? L"res" : L"rhs", what);
        parseFunctionFromOption(opt, L"secRhs", SRHS);

        // below call is made to detect complex extended ode
        if (m_pCallFunction[what] != NULL)
        {
            types::typed_list in;
            callOpening(what, in, m_dblT0);
            computeFunction(in, what);            
        }
    }
    else
    {
        if (isDAE())
        {
            getStringInPlist(getSolverName().c_str(),opt, L"calcIc", m_wstrCalcIc, L"", {L"y0",L"y0yp0"});
            getIntVectorInPlist(getSolverName().c_str(),opt, L"yIsAlgebraic", m_iVecIsAlgebraic, {}, {1,m_iNbEq}, {1,m_iNbEq});
            if (m_iVecIsAlgebraic.size() > 0)
            {
                getBooleanInPlist(getSolverName().c_str(),opt, L"suppressAlg", &m_bSuppressAlg, false);                
            }
        }
    }

    // check unparsed/invalid options
    if (opt.size() > 0)
    {
        std::wstringstream wstr;
        for (auto it = opt.begin(); it != opt.end(); it++)
        {
            wstr << L"\"" << it->first << L"\"";
            if (std::next(it) != opt.end())
            {
                wstr << L", ";
            }
        }
        if (opt.size() == 1)
        {
            sprintf(errorMsg, _("%s: %ls is an unknown or invalid (in this context) solver option.\n"), getSolverName().c_str(), wstr.str().c_str());
        }
        else
        {
            sprintf(errorMsg, _("%s: %ls are unknown or invalid (in this context) solver options.\n"), getSolverName().c_str(), wstr.str().c_str());
        }
        throw ast::InternalError(errorMsg);
    }

    if (m_odeIsExtension == false)
    {
        // Determine if ode is complex (can be already detected if Y0 is complex)
        // in the case where RHS is a Scilab function
        functionKind what = isDAE() ? RES : RHS;
    
        if (m_odeIsComplex == false && m_pCallFunction[what] != NULL)
        {
            types::typed_list in;
            callOpening(what, in, m_dblT0);
            computeFunction(in, what);
        }    
        if (m_odeIsComplex) // complexify Y0 and also Yp0 if solving a DAE (clones of user Y0 and Yp0)
        {
            m_pDblY0->setComplex(true);
            if (isDAE())
            {
                m_pDblYp0->setComplex(true);                        
            }
        }
    }

    if (m_odeIsExtension && m_odeIsComplex && prevManager->m_odeIsComplex == false)
    {
        sprintf(errorMsg, _("%s: Wrong type (complex) for option \"f\" or \"r\": extended ode is real.\n"), getSolverName().c_str());
        throw ast::InternalError(errorMsg);
    }

}

