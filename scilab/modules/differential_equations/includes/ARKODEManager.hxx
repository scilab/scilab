//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021-2022 - UTC - Stéphane MOTTELET
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
//
//--------------------------------------------------------------------------

#ifndef _ARKODEMANAGER_HXX_
#define _ARKODEMANAGER_HXX_

#include "dynlib_differential_equations.h"

#include "OdeManager.hxx"

#include <arkode/arkode_impl.h>
#include <arkode/arkode_ls_impl.h>
#include <arkode/arkode.h>            /* prototypes for ARKODES fcts. and consts. */
#include <arkode/arkode_butcher_erk.h>
#include <arkode/arkode_butcher_dirk.h>
#include <arkode/arkode_sprk.h>
#include <arkode/arkode_sprkstep.h>
#include <arkode/arkode_arkstep.h>
#include <arkode/arkode_bandpre.h>

extern "C"
{
    int ARKodeSetMaxOrd(void *, int);
}

typedef void(*dynlibFunPtr)();

class DIFFERENTIAL_EQUATIONS_IMPEXP ARKODEManager : public OdeManager
{
public :

    ARKODEManager()
    {
        m_strSolver = "arkode";
        m_wstrSolver = L"arkode";
        // construct map of methods
        for (const auto &it : vectARKODEMethods)
        {
            ARKODEMethods[it.first]=it.second;
        }
        setUserData = ARKodeSetUserData;
        setInitStep = ARKodeSetInitStep;
        setMinStep = ARKodeSetMinStep;
        setMaxStep = ARKodeSetMaxStep;
        setMaxNumSteps = ARKodeSetMaxNumSteps;
        setStopTime = ARKodeSetStopTime;
        setMaxOrd = ARKodeSetMaxOrd;
        getCurrentTime = ARKodeGetCurrentTime;
        getCurrentStep = ARKodeGetCurrentStep;
        getLastStep = ARKodeGetLastStep;
        getRootInfo = ARKodeGetRootInfo;
        setConstraints = ARKodeSetConstraints;
        setVTolerances = ARKodeSVtolerances;
        getReturnFlagName = ARKodeGetReturnFlagName;
        getDky = ARKodeGetDky;

        toODEReturn.emplace(ARK_SUCCESS, ODE_SUCCESS);
        toODEReturn.emplace(ARK_TSTOP_RETURN, ODE_TSTOP_RETURN);
        toODEReturn.emplace(ARK_ROOT_RETURN, ODE_ROOT_RETURN);
        toODEReturn.emplace(ARK_CONV_FAILURE, ODE_CONV_FAILURE);
        toODEReturn.emplace(ARK_TOO_MUCH_WORK, ODE_TOO_MUCH_WORK);
        toODEReturn.emplace(ARK_REPTD_RHSFUNC_ERR, ODE_REPTD_RHSFUNC_ERR);
        toODEReturn.emplace(ARK_WARNING, ODE_WARNING);

        fromODEReturn.emplace(ODE_SUCCESS, ARK_SUCCESS);
        fromODEReturn.emplace(ODE_TSTOP_RETURN, ARK_TSTOP_RETURN );
        fromODEReturn.emplace(ODE_ROOT_RETURN, ARK_ROOT_RETURN);
        fromODEReturn.emplace(ODE_CONV_FAILURE, ARK_CONV_FAILURE);
        fromODEReturn.emplace(ODE_TOO_MUCH_WORK, ARK_TOO_MUCH_WORK);
        fromODEReturn.emplace(ODE_REPTD_RHSFUNC_ERR, ARK_REPTD_RHSFUNC_ERR);
        fromODEReturn.emplace(ODE_WARNING, ARK_WARNING);
    }

    ~ARKODEManager()
    {
        if (m_prob_mem != NULL)
        {
            ARKodeFree(&m_prob_mem);
        }
        m_prob_mem = NULL;
    };

    void setPreviousManager(void *p)
    {
        m_prevManager = static_cast<ARKODEManager *>(p);
        m_bHas[SRHS] = m_prevManager->m_bHas[SRHS];
        m_odeIsExtension = true;
    }

    OdeManager *getPreviousManager()
    {
        return m_prevManager;
    }

    virtual bool isODE()
    {
        return true;
    }

    std::wstring getInterpolationMethod()
    {
        std::wstringstream wstr;
        wstr << m_wstrInterpolationMethod << L"(" << m_iInterpolationDegree << L")";
        return wstr.str();
    }

    int getBasisDimensionAtIndex(int iIndex);
    double *getBasisAtIndex(int iIndex);

    void parseMethodAndOrder(types::optional_list &opt);
    int getMaxNargin()
    {
        return 4;
    }
    bool create();
    std::wstring getDefaultNonLinSolver();
    std::vector<std::wstring> getAvailableNonLinSolvers();
    bool initialize(char *);
    // void errHandler(int error_code, const char *module, const char *function, char *msg);
    bool setEventFunction();
    bool setSolverAndJacobian(char *errorMsg);
    int getInterpBasisSize();
    solverReturnCode doStep(double dblFinalTime, double *pdblTime, solverTaskCode iKind);
    void saveInterpBasisVectors();
    void getInterpVectors(double *pdblNS, int iOrderPlusOne, int iIndex, double dblt0, double dblTUser, double dblStep, double *pdblVect, double *pdblVectd);
    void getButcherTabInPlist(types::optional_list &opt, const wchar_t * _pwstLabel, ARKodeButcherTable &ButcherTab);

    int DQJtimes(sunrealtype tt, N_Vector yy, N_Vector yp, N_Vector rr,
                      N_Vector v, N_Vector Jv, sunrealtype c_j,
                      N_Vector work2, N_Vector work3) final;

    types::Struct *getStats();

    typedef struct {
        ARKODE_ERKTableID erkID;
        ARKODE_DIRKTableID dirkID;
        ARKODE_SPRKMethodID sprkID;
        int order;
        int embeddedOrder;
    } methodInfo;

    std::map<std::wstring,methodInfo> ARKODEMethods;

    inline static std::vector<std::pair<std::wstring, methodInfo>> vectARKODEMethods =
    {
        // explicit methods
        {L"ERK",{ARKODE_SOFRONIOU_SPALETTA_5_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"ERK_2",{ARKODE_RALSTON_3_1_2,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,2,1}},
        {L"ERK_3",{ARKODE_BOGACKI_SHAMPINE_4_2_3,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,3,2}},
        {L"ERK_4",{ARKODE_SOFRONIOU_SPALETTA_5_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"ERK_5",{ARKODE_TSITOURAS_7_4_5,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,5,4}},
        {L"ERK_6",{ARKODE_VERNER_9_5_6,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,6,5}},
        {L"ERK_7",{ARKODE_VERNER_10_6_7,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,7,6}},
        {L"ERK_8",{ARKODE_VERNER_13_7_8,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,8,7}},
        {L"ERK_9",{ARKODE_VERNER_16_8_9,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,9,8}},
        {L"FORWARD_EULER_1_1",{ARKODE_FORWARD_EULER_1_1,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,1,1}},
        {L"RALSTON_EULER_2_1_2",{ARKODE_RALSTON_EULER_2_1_2,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,2,1}},
        {L"RALSTON_3_1_2",{ARKODE_RALSTON_3_1_2,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,2,1}},
        {L"EXPLICIT_MIDPOINT_EULER_2_1_2",{ARKODE_EXPLICIT_MIDPOINT_EULER_2_1_2,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,2,1}},
        {L"HEUN_EULER_2_1_2",{ARKODE_HEUN_EULER_2_1_2,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,2,1}},
        {L"SHU_OSHER_3_2_3",{ARKODE_SHU_OSHER_3_2_3,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,3,2}},
        {L"BOGACKI_SHAMPINE_4_2_3",{ARKODE_BOGACKI_SHAMPINE_4_2_3,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,3,2}},
        {L"ARK324L2SA_ERK_4_2_3",{ARKODE_ARK324L2SA_ERK_4_2_3,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,3,2}},
        {L"ZONNEVELD_5_3_4",{ARKODE_ZONNEVELD_5_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"SOFRONIOU_SPALETTA_5_3_4",{ARKODE_SOFRONIOU_SPALETTA_5_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"ARK436L2SA_ERK_6_3_4",{ARKODE_ARK436L2SA_ERK_6_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"SAYFY_ABURUB_6_3_4",{ARKODE_SAYFY_ABURUB_6_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"CASH_KARP_6_4_5",{ARKODE_CASH_KARP_6_4_5,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,5,4}},
        {L"FEHLBERG_6_4_5",{ARKODE_FEHLBERG_6_4_5,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,5,4}},
        {L"TSITOURAS_7_4_5",{ARKODE_TSITOURAS_7_4_5,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,5,4}},
        {L"ARK548L2SA_ERK_8_4_5",{ARKODE_ARK548L2SA_ERK_8_4_5,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,5,4}},
        {L"VERNER_8_5_6",{ARKODE_VERNER_8_5_6,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,6,5}},
        {L"VERNER_9_5_6",{ARKODE_VERNER_9_5_6,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,6,5}},
        {L"VERNER_10_6_7",{ARKODE_VERNER_10_6_7,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,7,6}},
        {L"FEHLBERG_13_7_8",{ARKODE_FEHLBERG_13_7_8,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,8,7}},
        {L"VERNER_13_7_8",{ARKODE_VERNER_13_7_8,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,8,7}},
        {L"VERNER_16_8_9",{ARKODE_VERNER_16_8_9,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,9,8}},
        {L"ARK437L2SA_ERK_7_3_4",{ARKODE_ARK437L2SA_ERK_7_3_4,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,4,3}},
        {L"ARK548L2SAb_ERK_8_4_5",{ARKODE_ARK548L2SAb_ERK_8_4_5,ARKODE_DIRK_NONE,ARKODE_SPRK_NONE,5,4}},

        // implicit methods
        {L"DIRK",{ARKODE_ERK_NONE,ARKODE_SDIRK_5_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"DIRK_2",{ARKODE_ERK_NONE,ARKODE_ARK2_DIRK_3_1_2,ARKODE_SPRK_NONE,2,1}},
        {L"DIRK_3",{ARKODE_ERK_NONE,ARKODE_ESDIRK325L2SA_5_2_3,ARKODE_SPRK_NONE,3,3}},
        {L"DIRK_4",{ARKODE_ERK_NONE,ARKODE_ESDIRK436L2SA_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"DIRK_5",{ARKODE_ERK_NONE,ARKODE_ESDIRK547L2SA2_7_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"BACKWARD_EULER_1_1",{ARKODE_ERK_NONE,ARKODE_BACKWARD_EULER_1_1,ARKODE_SPRK_NONE,1,1}},
        {L"IMPLICIT_MIDPOINT_1_2",{ARKODE_ERK_NONE,ARKODE_IMPLICIT_MIDPOINT_1_2,ARKODE_SPRK_NONE,2,1}},
        {L"IMPLICIT_TRAPEZOIDAL_2_2",{ARKODE_ERK_NONE,ARKODE_IMPLICIT_TRAPEZOIDAL_2_2,ARKODE_SPRK_NONE,2,2}},
        {L"ARK2_DIRK_3_1_2",{ARKODE_ERK_NONE,ARKODE_ARK2_DIRK_3_1_2,ARKODE_SPRK_NONE,2,2}},
        {L"SDIRK_2_1_2",{ARKODE_ERK_NONE,ARKODE_SDIRK_2_1_2,ARKODE_SPRK_NONE,2,1}},
        {L"BILLINGTON_3_3_2",{ARKODE_ERK_NONE,ARKODE_BILLINGTON_3_3_2,ARKODE_SPRK_NONE,2,3}},
        {L"TRBDF2_3_3_2",{ARKODE_ERK_NONE,ARKODE_TRBDF2_3_3_2,ARKODE_SPRK_NONE,2,3}},
        {L"KVAERNO_4_2_3",{ARKODE_ERK_NONE,ARKODE_KVAERNO_4_2_3,ARKODE_SPRK_NONE,3,2}},
        {L"ARK324L2SA_DIRK_4_2_3",{ARKODE_ERK_NONE,ARKODE_ARK324L2SA_DIRK_4_2_3,ARKODE_SPRK_NONE,3,2}},
        {L"CASH_5_2_4",{ARKODE_ERK_NONE,ARKODE_CASH_5_2_4,ARKODE_SPRK_NONE,4,2}},
        {L"CASH_5_3_4",{ARKODE_ERK_NONE,ARKODE_CASH_5_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"SDIRK_5_3_4",{ARKODE_ERK_NONE,ARKODE_SDIRK_5_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"KVAERNO_5_3_4",{ARKODE_ERK_NONE,ARKODE_KVAERNO_5_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ARK436L2SA_DIRK_6_3_4",{ARKODE_ERK_NONE,ARKODE_ARK436L2SA_DIRK_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"KVAERNO_7_4_5",{ARKODE_ERK_NONE,ARKODE_KVAERNO_7_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ARK548L2SA_DIRK_8_4_5",{ARKODE_ERK_NONE,ARKODE_ARK548L2SA_DIRK_8_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ARK437L2SA_DIRK_7_3_4",{ARKODE_ERK_NONE,ARKODE_ARK437L2SA_DIRK_7_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ARK548L2SAb_DIRK_8_4_5",{ARKODE_ERK_NONE,ARKODE_ARK548L2SAb_DIRK_8_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ESDIRK324L2SA_4_2_3",{ARKODE_ERK_NONE,ARKODE_ESDIRK324L2SA_4_2_3,ARKODE_SPRK_NONE,3,2}},
        {L"ESDIRK325L2SA_5_2_3",{ARKODE_ERK_NONE,ARKODE_ESDIRK325L2SA_5_2_3,ARKODE_SPRK_NONE,3,2}},
        {L"ESDIRK32I5L2SA_5_2_3",{ARKODE_ERK_NONE,ARKODE_ESDIRK32I5L2SA_5_2_3,ARKODE_SPRK_NONE,3,2}},
        {L"ESDIRK436L2SA_6_3_4",{ARKODE_ERK_NONE,ARKODE_ESDIRK436L2SA_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ESDIRK43I6L2SA_6_3_4",{ARKODE_ERK_NONE,ARKODE_ESDIRK43I6L2SA_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"QESDIRK436L2SA_6_3_4",{ARKODE_ERK_NONE,ARKODE_QESDIRK436L2SA_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ESDIRK437L2SA_7_3_4",{ARKODE_ERK_NONE,ARKODE_ESDIRK437L2SA_7_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ESDIRK547L2SA_7_4_5",{ARKODE_ERK_NONE,ARKODE_ESDIRK547L2SA_7_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ESDIRK547L2SA2_7_4_5",{ARKODE_ERK_NONE,ARKODE_ESDIRK547L2SA2_7_4_5,ARKODE_SPRK_NONE,5,4}},

        // implicit/explicit (ImEx) methods
        {L"ARK",{ARKODE_ARK437L2SA_ERK_7_3_4,ARKODE_ARK437L2SA_DIRK_7_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ARK_3",{ARKODE_ARK324L2SA_ERK_4_2_3,ARKODE_ARK324L2SA_DIRK_4_2_3,ARKODE_SPRK_NONE,3,3}},
        {L"ARK_4",{ARKODE_ARK436L2SA_ERK_6_3_4,ARKODE_ARK436L2SA_DIRK_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ARK_5",{ARKODE_ARK548L2SA_ERK_8_4_5,ARKODE_ARK548L2SA_DIRK_8_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ARK324",{ARKODE_ARK324L2SA_ERK_4_2_3,ARKODE_ARK324L2SA_DIRK_4_2_3,ARKODE_SPRK_NONE,3,3}},
        {L"ARK436",{ARKODE_ARK436L2SA_ERK_6_3_4,ARKODE_ARK436L2SA_DIRK_6_3_4,ARKODE_SPRK_NONE,4,3}},
        {L"ARK548",{ARKODE_ARK548L2SA_ERK_8_4_5,ARKODE_ARK548L2SA_DIRK_8_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ARK548b",{ARKODE_ARK548L2SAb_ERK_8_4_5,ARKODE_ARK548L2SAb_DIRK_8_4_5,ARKODE_SPRK_NONE,5,4}},
        {L"ARK437",{ARKODE_ARK437L2SA_ERK_7_3_4,ARKODE_ARK437L2SA_DIRK_7_3_4,ARKODE_SPRK_NONE,4,3}},      
        
        // symplectic methods
        {L"SPRK",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_4_4,4,4}},
        {L"SPRK_1",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_EULER_1_1,1,1}},
        {L"SPRK_2",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_LEAPFROG_2_2,2,2}},
        {L"SPRK_3",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_3_3,3,3}},
        {L"SPRK_4",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_4_4,4,4}},
        {L"SPRK_5",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_5_6,5,6}},
        {L"SPRK_6",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_YOSHIDA_6_8,6,8}},
        {L"SPRK_8",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_SUZUKI_UMENO_8_16,8,16}},
        {L"SPRK_10",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_SOFRONIOU_10_36,10,36}},
        {L"EULER_1_1",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_EULER_1_1,1,1}},
        {L"LEAPFROG_2_2",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_LEAPFROG_2_2,2,2}},
        {L"PSEUDO_LEAPFROG_2_2",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_PSEUDO_LEAPFROG_2_2,2,2}},
        {L"RUTH_3_3",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_RUTH_3_3,3,3}},
        {L"MCLACHLAN_2_2",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_2_2,2,2}},
        {L"MCLACHLAN_3_3",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_3_3,3,3}},
        {L"CANDY_ROZMUS_4_4",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_CANDY_ROZMUS_4_4,4,4}},
        {L"MCLACHLAN_4_4",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_4_4,4,4}},
        {L"MCLACHLAN_5_6",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_MCLACHLAN_5_6,5,6}},
        {L"YOSHIDA_6_8",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_YOSHIDA_6_8,6,8}},
        {L"SUZUKI_UMENO_8_16",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_SUZUKI_UMENO_8_16,8,16}},
        {L"SOFRONIOU_10_36",{ARKODE_ERK_NONE,ARKODE_DIRK_NONE,ARKODE_SPRK_SOFRONIOU_10_36,10,36}}      
    };

    virtual std::vector<std::wstring> getAvailableMethods()
    {
        std::vector<std::wstring> availableMethods;
  
        for (const auto &it : vectARKODEMethods)
        {
            if (m_bHas[SRHS])
            {
                if (it.second.erkID >= ARKODE_MIN_ERK_NUM && it.second.dirkID >= ARKODE_MIN_DIRK_NUM)
                {
                    availableMethods.push_back(it.first);
                }
            }
            else if (hasJacobian() && it.second.erkID == ARKODE_ERK_NONE && it.second.dirkID >= ARKODE_MIN_DIRK_NUM)
            {
                availableMethods.push_back(it.first);
            }
            else if ((it.second.erkID == ARKODE_ERK_NONE || it.second.dirkID == ARKODE_DIRK_NONE) && it.second.sprkID == ARKODE_SPRK_NONE)
            {
                availableMethods.push_back(it.first);
            }
        }
        return availableMethods;
    }

    virtual std::wstring getMethodName()
    {
        methodInfo info = ARKODEMethods[m_wstrMethod];
        // init with m_wstrMethod for the user tableaux case ("info" above will be uninitialized)
        std::wstring wstrMethodFullName = m_wstrMethod;
        for (const auto &it : vectARKODEMethods)
        {
            // last matching has the full name
            if (it.second.erkID == info.erkID && it.second.dirkID == info.dirkID && it.second.sprkID == info.sprkID)
            {
               wstrMethodFullName = it.first;
            }
        }
        return wstrMethodFullName;
    }

    int getMaxMethodOrder(std::wstring wstrMethod)
    {
        return m_bHas[SRHS] ? 5 : 9;
    }

    bool hasBandPrec()
    {
        return true;
    }

    std::wstring m_wstrInterpolationMethod;
    std::wstring m_wstrIsLinear;
    int m_iInterpolationDegree = 0;
    int m_iInterpolationMethod = 0;
    int m_iMethodOrder = 0;
    long int m_incStat[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    double m_dblFixedStep = 0;

private :

    ARKODEManager* m_prevManager = NULL;

    int m_iEmbeddedMethodOrder = 0;
    ARKodeButcherTable m_ERKButcherTable = NULL;
    ARKodeButcherTable m_DIRKButcherTable = NULL;
};

#endif
