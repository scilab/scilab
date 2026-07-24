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

#include "complexHelpers.hxx"
#include "OdeManager.hxx"
#include "SUNDIALSBridge.hxx"

void OdeManager::solve()
{
    solverReturnCode iFlag;
    int iStep = 0;
    int iMaxSteps = INT_MAX;
    int iNbSteps = 0;
    int iFirstStep = 0;
    solverTaskCode ODE_MODE;
    double dblTime = 0;
    double dblCurrTime;
    double dblFinalTime;
    bool bTerminalEvent = false;
    char errorMsg[256];
    OdeManager *prevManager = getPreviousManager();

    auto chrono_begin = std::chrono::steady_clock::now();

    if (m_bHas[INTCB])
    {
        intermediateCallback(dblTime, -1, m_N_VectorY, m_N_VectorYp);
    }

    if ((m_pDblTSpan->getSize() == 2 && std::isnan(m_dblOptT0)) || m_iRetCount == 1) 
    {
        // Case where we compute solution only at method steps guided only by precision (RTOL, ATOL) requirements.
        // This occurs when tspan is of the kind [t0 t1] or in the case of sol = method() syntax, i.e. when we yield
        // a solution structure with a pointer to the OdeManager instance for further custom evaluation by interpolation
        // or time extension.
        ODE_MODE = ODE_ONE_STEP;
        iMaxSteps = m_iMaxNumSteps == 0 ? INT_MAX : m_iMaxNumSteps;
    }
    else
    {
        // Case where we compute solution only at user steps given in tspan
        ODE_MODE = ODE_NORMAL;  
    }

    // Record (t0,y0) if applicable
    // Recording of initial valuea of additional states is done in initialize() solver method
    if (m_odeIsExtension == false)
    {
        m_indexInterpBasis.push_back(0);
        m_dblVecCurrTime.push_back(m_dblT0);
        if (m_dblT0 == m_pDblTSpan->get(0) || m_iRetCount == 1)
        {
            m_vecYOut.push_back(std::vector<double>(N_VGetArrayPointer(m_N_VectorY),N_VGetArrayPointer(m_N_VectorY) + m_iNbRealEq));
            m_dblVecTOut.push_back(m_dblT0);
            iStep = 1;
        }
    }
    else
    {
        // new values will be appended to previous vectors
        m_indexInterpBasis = prevManager->m_indexInterpBasis;
        m_vecYOut = prevManager->m_vecYOut;
        m_dblVecTOut = prevManager->m_dblVecTOut; // Beware, m_dblVecTOut has T0 hence its size is m_iVecOrder.size()+1
        m_dblVecCurrTime = prevManager->m_dblVecCurrTime;
        m_iVecOrder = prevManager->m_iVecOrder;
        m_iVecInterpBasisSize = prevManager->m_iVecInterpBasisSize;
        // events
        m_dblVecYEvent = prevManager->m_dblVecYEvent;
        m_dblVecIndexEvent = prevManager->m_dblVecIndexEvent;
        m_dblVecEventTime = prevManager->m_dblVecEventTime;
        // index of further steps takes into account previous values
        iFirstStep = prevManager->m_iVecOrder.size();
    }

    // prepare record vector(s)
    saveAdditionalStates();

    iNbSteps = iFirstStep;
    dblTime = m_dblT0; // will be the time reached by the solver
    dblFinalTime = m_pDblTSpan->get(m_pDblTSpan->getSize()-1);
    setStopTime(m_prob_mem,dblFinalTime);

    for (; (dblTime != dblFinalTime) && (iStep < iMaxSteps); iStep++)
    {
        // Solver internal step
        double dblPrevTime = dblTime;
        double dblNextTime = ODE_MODE == ODE_ONE_STEP ? dblFinalTime : m_pDblTSpan->get(iStep);
        if (m_bHas[PROJ] && ODE_MODE == ODE_NORMAL)
        {
            setStopTime(m_prob_mem, dblNextTime);
        }
        iFlag = doStep(dblNextTime, &dblTime, ODE_MODE);
        if (iFlag == ODE_SUCCESS || iFlag == ODE_TSTOP_RETURN || iFlag == ODE_ROOT_RETURN)
        {
            if (dblTime == dblPrevTime)
            {
                sprintf(errorMsg,"singularity likely at t = %g\n", dblTime);
                solverErrHandler(fromODEReturn[dblTime == m_dblT0 ? ODE_CONV_FAILURE : ODE_WARNING], errorMsg);
                break;
            }
            if (m_iRetCount > 0)
            {
                // Refine by interpolation if requested
                if (m_iNRefine>0)
                {
                    double dblStepRef = (dblTime - dblPrevTime)/((double)m_iNRefine+1.0);
                    double dblRefTime = m_iNRefine == 0 ? dblTime : dblPrevTime + dblStepRef;
                    for (int i=0; i<m_iNRefine+1; i++)
                    {
                        getDky(m_prob_mem, dblRefTime, 0, m_N_VectorYTemp);
                        m_vecYOut.push_back(std::vector<double>(N_VGetArrayPointer(m_N_VectorYTemp), N_VGetArrayPointer(m_N_VectorYTemp) + m_iNbRealEq));
                        m_dblVecTOut.push_back(dblRefTime);
                        saveAdditionalStates(dblRefTime);
                        dblRefTime += dblStepRef;
                    }                    
                }
                else
                {
                    m_vecYOut.push_back(std::vector<double>(N_VGetArrayPointer(m_N_VectorY), N_VGetArrayPointer(m_N_VectorY) + m_iNbRealEq));
                    m_dblVecTOut.push_back(dblTime);
                    saveAdditionalStates(dblTime);                    
                }
                // dblCurrTime is solver time (can be greater than user requested dblTime)
                getCurrentTime(m_prob_mem,&dblCurrTime);
                m_dblVecCurrTime.push_back(dblCurrTime);
                m_iVecOrder.push_back(m_iLastOrder);
                iNbSteps++;    
            }
            if (iFlag == ODE_ROOT_RETURN)
            {
                std::vector<int> newIndex(m_iNbEvents);
                getRootInfo(m_prob_mem, newIndex.data());
                // Check if one event is a terminal event
                if (m_iVecEventIsTerminal.size() > 0)
                {
                    for (int i=0; i < m_iNbEvents; i++)
                    {
                        bTerminalEvent |= (newIndex[i] !=0 && m_iVecEventIsTerminal[i] == 1);
                    }
                    // bTerminalEvent == true will stop integration
                }
                if (m_iRetCount > 0)
                {
                    m_dblVecIndexEvent.push_back(newIndex);                        
                    m_dblVecEventTime.push_back(dblTime);
                    m_dblVecYEvent.push_back(std::vector<double>(N_VGetArrayPointer(m_N_VectorY), N_VGetArrayPointer(m_N_VectorY) + m_iNbRealEq));
                    saveAdditionalEventStates(dblTime);
                }
            }
            if (m_iRetCount == 1)
            {
                m_iVecInterpBasisSize.push_back(getInterpBasisSize());
                saveInterpBasisVectors();
            }
            if (bTerminalEvent == true
                || (m_bHas[INTCB] && intermediateCallback(dblTime, iFlag == ODE_ROOT_RETURN ? 1 : 0, m_N_VectorY, m_N_VectorYp)))
            {
                break;
            }
        }
        else
        {
            // Errors/Warnings not trapped by SUNDIALS ErrorFunc
            solverErrHandler(fromODEReturn[iFlag], NULL);
            break;
        }
    }

    if (iStep == iMaxSteps && dblTime != dblFinalTime)
    {
      sprintf(errorMsg,"At t = %.15g, mxstep steps taken before reaching tout.", dblTime);
      solverErrHandler(fromODEReturn[ODE_TOO_MUCH_WORK], errorMsg);
    }

    if (m_iRetCount == 1) // Solution structure output
    {
        if (interpBasisVectorList.size() == 0)
        {
            // can occur when no special basis vectors are used other than solution itself (Lagrange interpolation)
            m_pDblInterpBasisVectors = new types::Double(m_iNbRealEq, m_vecYOut.size());
            double *pdblBasis = m_pDblInterpBasisVectors->get();
            for (auto i = 0; i < m_vecYOut.size(); i++)
            {
                 double *pdbl = m_vecYOut[i].data();   
                 std::copy(pdbl, pdbl+m_iNbRealEq, pdblBasis);
                 pdblBasis += m_iNbRealEq;
            }
        }
        else
        {
            double *pdblBasis;
            // Size may vary among solvers: m_iNbRealEq for CVODE and ARKODE and m_iNbRealEq+1 for IDA
            int iSize = interpBasisVectorList.front().size();

            m_pDblInterpBasisVectors = new types::Double(iSize, m_indexInterpBasis.back());
            // Check if we are extending a previous solution
            if (m_odeIsExtension)
            {
                types::Double *pDbl = prevManager->m_pDblInterpBasisVectors;

                // copy previous NS matrices
                std::copy(pDbl->get(), pDbl->get()+pDbl->getSize(), m_pDblInterpBasisVectors->get());
                pdblBasis = m_pDblInterpBasisVectors->get() + pDbl->getSize();
            }
            else
            {
                pdblBasis = m_pDblInterpBasisVectors->get();
            }

            for (int i = iFirstStep; i < iNbSteps; i++)
            {
                for (int j = 0; j < m_iVecInterpBasisSize[i]; j++)
                {
                    double *pdbl = interpBasisVectorList.front().data();
                    std::copy(pdbl, pdbl+iSize, pdblBasis);

                    interpBasisVectorList.pop_front();
                    pdblBasis += iSize;
                }
            }
        }
    }

    if (m_bHas[INTCB])
    {
        intermediateCallback(dblTime, 2, m_N_VectorY, m_N_VectorYp);
    }

    auto chrono_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = (chrono_end - chrono_begin);
    m_dblElapsedTime = diff.count();
    // save last current step for future extension
    getCurrentStep(m_prob_mem, &m_dblCurrentStep);
}

void OdeManager::solverErrHandler(int error_code, const char *msg)
{
    // as this function is called from the bridge, error_code is a native code (cvode, ida or arkode)
    char errorMsg[256];
    if (msg == NULL)
    {
        double dblTime = 0;
        getCurrentTime(m_prob_mem, &dblTime);
        sprintf(errorMsg, "%s: at t=%g, %s\n",getSolverName().c_str(), dblTime, getReturnFlagName(error_code));
    }
    else
    {
        sprintf(errorMsg, "%s: %s", getSolverName().c_str(), msg);        
    }
    if (toODEReturn[error_code] != ODE_WARNING)
    {
        throw ast::InternalError(errorMsg);
    }
    else
    {
        sciprint("\n%s",errorMsg);
    }
}
