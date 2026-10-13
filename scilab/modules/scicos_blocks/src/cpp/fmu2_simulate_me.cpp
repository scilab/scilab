//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
// Copyright (C) - 2017 - ESI Group - Clement DAVID
// Copyright (C) - 2024 - Dassault Systèmes - Clement DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

#include "fmu_wrapper.hxx"

#include <algorithm>
#include <vector>

extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "scicos.h"
#include "dynlib_scicos_blocks.h"
#include "scicos_block4.h"
#include "scicos_malloc.h"
#include "scicos_free.h"

#include "fmu_sim_utility.h"

    SCICOS_BLOCKS_IMPEXP void fmu2_simulate_me(scicos_block *block, scicos_flag flag);
}

// FMI 2.0.3, chapter 3.2.4: in Event Mode, the FMU is iterated until the
// discrete states are stable (newDiscreteStatesNeeded == fmi2False):
//  - set inputs
//  - fmi2NewDiscreteStates
//  - get outputs
// The iteration is capped by the block setting to fail loudly on an FMU that
// never converges. Returns the number of iterations, or a negative error code.
static int fmi2EventIteration(scicos_block *block, scicos_flag flag, Fmu2ModelExchange *fmu, workFMU *structFmu, int iterationLimit)
{
    uint32_t *sciInputRefs = Getuint32OparPtrs(block, 2);
    size_t inpRefsSize = GetOparSize(block, 2, 1) * GetOparSize(block, 2, 2);
    int *sciInputTypes = Getint32OparPtrs(block, 3);
    uint32_t *sciOutputRefs = Getuint32OparPtrs(block, 4);
    size_t outRefsSize = GetOparSize(block, 4, 1) * GetOparSize(block, 4, 2);
    int *sciOutputTypes = Getint32OparPtrs(block, 5);
    fmi2Status status;
    fmi2Boolean needed = fmi2True;
    int iteration = 0;

    while (needed)
    {
        if (iteration >= iterationLimit)
        {
            return -1;
        }
        // Set continuous-time and discrete-time inputs
        if (fmiSet_values(block, flag, fmi2False, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi2Warning)
        {
            return -2;
        }
        status = fmu->fmi2NewDiscreteStates(structFmu->instance, &structFmu->data.modelExchangeV2.eventInfo);
        if (status > fmi2Discard)
        {
            return -3;
        }
        // Get outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
        {
            return -4;
        }
        needed = structFmu->data.modelExchangeV2.eventInfo.newDiscreteStatesNeeded;
        iteration++;
    }
    return iteration;
}

SCICOS_BLOCKS_IMPEXP void fmu2_simulate_me(scicos_block *block, scicos_flag flag)
{
    char *pathName = NULL, *resourceLocation = NULL, *identName = NULL, *guid = NULL;
    double *sciTolerance = NULL;
    int *sciInputTypes = NULL, *sciOutputTypes = NULL;
    fmi2Status status;
    workFMU *structFmu = (workFMU *)(*block->work);
    Fmu2ModelExchange *fmu = nullptr;
    fmi2CallbackFunctions functions{fmu2_callback_logger, calloc, free, NULL, block};
    double time = 0;
    int eventIterationLimit;
    uint32_t *sciInputRefs = NULL, *sciOutputRefs = NULL;
    fmi2Boolean loggingOn = fmi2False;
    fmiReal tStart = 0;
    size_t outRefsSize, inpRefsSize;

    // return if an fmu has not been loaded
    if (block->nopar == 0)
    {
        return;
    }

    // settings
    pathName = Getint8OparPtrs(block, 1);          // path to the lib
    resourceLocation = Getint8OparPtrs(block, 10); // fmuResourceLocation
    identName = Getint8OparPtrs(block, 11);        // identifier
    guid = Getint8OparPtrs(block, 12);             // guid
    loggingOn = *Getint8OparPtrs(block, 13);       // logger
    sciTolerance = GetRealOparPtrs(block, 16);     // relative tolerance
    // event iteration limit ; diagrams saved with an older block have no such setting
    eventIterationLimit = 100;
    if (block->nopar >= 17)
    {
        eventIterationLimit = *Getint32OparPtrs(block, 17);
    }
    if (eventIterationLimit < 1)
    {
        eventIterationLimit = 1;
    }

    // inputs
    sciInputRefs = Getuint32OparPtrs(block, 2);
    inpRefsSize = GetOparSize(block, 2, 1) * GetOparSize(block, 2, 2);
    sciInputTypes = Getint32OparPtrs(block, 3);
    // outputs
    sciOutputRefs = Getuint32OparPtrs(block, 4);
    outRefsSize = GetOparSize(block, 4, 1) * GetOparSize(block, 4, 2);
    sciOutputTypes = Getint32OparPtrs(block, 5);
    // states and events
    assert(*Getuint32OparPtrs(block, 14) == (unsigned int)block->nx);

    if (flag == Initialization)
    {
        // sciprint("\n");
        // sciprint("*****Initialization block [%d] Time [%f] \n",get_block_number(), get_scicos_time());

        // memory allocation for the main structure
        if ((*(block->work) = (workFMU *)scicos_malloc(sizeof(workFMU))) == NULL)
        {
            set_block_error(-16); // memory allocation error
            *(block->work) = NULL;
            return;
        }
        structFmu = (workFMU *)(*block->work);
        memset(structFmu, 0x0, sizeof(workFMU));

        // load library and create an instance
        structFmu->fmuME2 = new Fmu2ModelExchange();
        fmu = structFmu->fmuME2;
        if (fmu->create(pathName, identName) == false)
        {
            free_instance(block, -3, "Fmu2ModelExchange"); // internal error
            return;
        }
        // set fmi2Type
        structFmu->type = workFMU::ModelExchange;
        structFmu->version = workFMU::fmiV2;

        // instantiate model
        structFmu->instance = fmu->fmi2Instantiate(identName, fmi2ModelExchange, guid, resourceLocation, &functions, fmi2True, loggingOn);
        if (structFmu->instance == NULL)
        {
            free_instance(block, -3, "fmi2Instantiate");
            return;
        }
        FMU_SIM_PRINT("fmi2Instantiate\n");
        structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.Instantiated;
        // set debug logging
        if (loggingOn)
        {
            status = fmu->fmi2SetDebugLogging(structFmu->instance, loggingOn, 0, NULL);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2SetDebugLogging");
                return;
            }
        }

        // initialize model
        if (sciTolerance[0])
        {

            status = fmu->fmi2SetupExperiment(structFmu->instance, fmi2True, sciTolerance[0], tStart, fmi2False, 0.0);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2SetupExperiment");
                return;
            }
        }
        else
        {
            status = fmu->fmi2SetupExperiment(structFmu->instance, fmi2False, 0.0, tStart, fmi2False, 0.0);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2SetupExperiment");
                return;
            }
        }

        // set start values
        status = (fmi2Status) fmiSet_parameters(block, flag);
        if (status > 1)
        {
            free_instance(block, -3, "fmiSet_parameters");
            return;
        }

        // enter initialization mode
        status = fmu->fmi2EnterInitializationMode(structFmu->instance);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2EnterInitializationMode");
            return;
        }
        FMU_SIM_PRINT("fmi2EnterInitializationMode\n");
        structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.InitializationMode;

        status = fmu->fmi2GetContinuousStates(structFmu->instance, block->x, block->nx);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2GetContinuousStates");
            return;
        }

        // retrieve solution at time = 0.00
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }
    }
    else if (flag == ReInitialization)
    {
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME2;

        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }

        status = fmu->fmi2GetContinuousStates(structFmu->instance, block->x, block->nx);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2GetContinuousStates");
            return;
        }

        // Get event indicators to detect the zero-crossing
        status = fmu->fmi2GetEventIndicators(structFmu->instance, block->g, block->ng);
        if (status > fmi2Discard)
        {
            free_instance(block, -3, "fmi2GetEventIndicators");
            return;
        }
    }
    else if (flag == DerivativeState)
    {
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME2;

        // Derivative computation is always in Continuous mode
        if (structFmu->data.modelExchangeV2.state != structFmu->data.modelExchangeV2.ContinuousTimeMode)
        {
            status = fmu->fmi2EnterContinuousTimeMode(structFmu->instance);
            if (status > fmi2Discard)
            {
                free_instance(block, -3, "fmi2EnterContinuousTimeMode");
                return;
            }
            FMU_SIM_PRINT("fmi2EnterContinuousTimeMode\n");
            structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.ContinuousTimeMode;
        }
        structFmu->data.modelExchangeV2.enterEventMode = fmi2False;

        // Set independent variable time
        time = get_scicos_time();
        status = fmu->fmi2SetTime(structFmu->instance, time);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2SetTime");
            return;
        }

        // Set continuous-time inputs
        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV2.state == structFmu->data.modelExchangeV2.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // Set continuous-time states
        status = fmu->fmi2SetContinuousStates(structFmu->instance,  block->x, block->nx);
        if (status > fmi2Discard)
        {
            free_instance(block, -3, "fmi2SetContinuousStates");
            return;
        }

        // Evaluate and get derivatives
        status = fmu->fmi2GetDerivatives(structFmu->instance, block->xd, block->nx);
        if (status > fmi2Discard)
        {
            free_instance(block, -3, "fmi2GetDerivatives");
            return;
        }

        // Complete integrator step and return enterEventMode
        fmi2Boolean terminateSimulation = fmi2False;
        status = fmu->fmi2CompletedIntegratorStep(structFmu->instance, fmi2True, &structFmu->data.modelExchangeV2.enterEventMode, &terminateSimulation);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2CompletedIntegratorStep");
            return;
        }
        if (terminateSimulation)
        {
            free_instance(block, -3, "fmi2CompletedIntegratorStep");
            return;
        }
    }
    else if (flag == OutputUpdate)
    {
        // sciprint("\n");
        // sciprint("*****Output update block [%d] at time: %f\n", get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME2;

        // From Initialization and ReInitialization scicos flags, we switch to regular simulation Event mode
        if (structFmu->data.modelExchangeV2.state == structFmu->data.modelExchangeV2.InitializationMode)
        {
            // exit initialization mode on the first call
            status = fmu->fmi2ExitInitializationMode(structFmu->instance);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2ExitInitializationMode");
                return;
            }
            FMU_SIM_PRINT("fmi2ExitInitializationMode\n");
            structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.EventMode;
        }

        // Set independent variable time
        time = get_scicos_time();
        status = fmu->fmi2SetTime(structFmu->instance, time);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2SetTime");
            return;
        }

        // From the previous fmi2NewDiscreteStates call, the FMU requested to enter into EventMode at a specific time
        structFmu->data.modelExchangeV2.timeEvent = (structFmu->data.modelExchangeV2.eventInfo.nextEventTimeDefined) && (structFmu->data.modelExchangeV2.eventInfo.nextEventTime <= time);

        // From Continuous-Time Mode, the FMU requested to enter into EventMode
        //  - with the enterEventMode output of fmi2CompletedIntegratorStep coming from the previous DerivativeState
        //  - with the eventInfo.nextEventTimeDefined output of fmi2NewDiscreteStates
        if (structFmu->data.modelExchangeV2.state != structFmu->data.modelExchangeV2.EventMode)
        {
            if (structFmu->data.modelExchangeV2.enterEventMode || structFmu->data.modelExchangeV2.timeEvent)
            {
                status = fmu->fmi2EnterEventMode(structFmu->instance);
                if (status > fmi2Warning)
                {
                    free_instance(block, -3, "fmi2EnterEventMode");
                    return;
                }
                FMU_SIM_PRINT("fmi2EnterEventMode\n");
                structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.EventMode;
            }
        }

        // Set continuous-time and discrete-time inputs
        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV2.state == structFmu->data.modelExchangeV2.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // From the previous fmi2NewDiscreteStates call, the FMU requested to
        // enter into EventMode at a specific time, or with the enterEventMode
        // output of fmi2CompletedIntegratorStep coming from the previous DerivativeState
        if (structFmu->data.modelExchangeV2.enterEventMode || structFmu->data.modelExchangeV2.timeEvent)
        {
            // FMI 2.0.3, chapter 3.2.4: event iteration on the FMU
            int iterations = fmi2EventIteration(block, flag, fmu, structFmu, eventIterationLimit);
            if (iterations < 0)
            {
                free_instance(block, -3, "fmi2NewDiscreteStates");
                return;
            }
            if (structFmu->data.modelExchangeV2.eventInfo.valuesOfContinuousStatesChanged)
            {
                status = fmu->fmi2GetContinuousStates(structFmu->instance, block->x, block->nx);
                if (status > fmi2Discard)
                {
                    free_instance(block, -3, "fmi2GetContinuousStates");
                    return;
                }
            }
        }

        if (structFmu->data.modelExchangeV2.state == structFmu->data.modelExchangeV2.ContinuousTimeMode)
        {
            // Set continuous-time states
            status = fmu->fmi2SetContinuousStates(structFmu->instance, block->x, block->nx);
            if (status > fmi2Discard)
            {
                free_instance(block, -3, "fmi2SetContinuousStates");
                return;
            }
        }

        // get outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }
    }
    else if (flag == StateUpdate)
    {
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME2;

        // after a scicos "root found" event ; increment super dense time: done by the solver
        if (structFmu->data.modelExchangeV2.state != structFmu->data.modelExchangeV2.EventMode)
        {
            status = fmu->fmi2EnterEventMode(structFmu->instance);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2EnterEventMode");
                return;
            }
            FMU_SIM_PRINT("fmi2EnterEventMode\n");
            structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.EventMode;
        }

        // in Event-mode, there is a point-fix iteration on the FMU
        // (FMI 2.0.3, chapter 3.2.4): until newDiscreteStatesNeeded == fmi2False
        int iterations = fmi2EventIteration(block, flag, fmu, structFmu, eventIterationLimit);
        if (iterations < 0)
        {
            free_instance(block, -3, "fmi2NewDiscreteStates");
            return;
        }
        if (structFmu->data.modelExchangeV2.eventInfo.terminateSimulation)
        {
            free_instance(block, -3, "eventInfo.terminateSimulation");
            return;
        }
        if (iterations > 1)
        {
            // discrete states changed during the event iteration
            do_cold_restart();
        }
        if (structFmu->data.modelExchangeV2.eventInfo.nominalsOfContinuousStatesChanged)
        {
            do_cold_restart();
        }
        if (structFmu->data.modelExchangeV2.eventInfo.valuesOfContinuousStatesChanged)
        {
            status = fmu->fmi2GetContinuousStates(structFmu->instance, block->x, block->nx);
            if (status > fmi2Discard)
            {
                free_instance(block, -3, "fmi2GetContinuousStates");
                return;
            }
        }
    }
    else if (flag == ZeroCrossing)
    {
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME2;

        // Zero-crossing detection is always on continuous mode, we have to set the states values
        if (structFmu->data.modelExchangeV2.state != structFmu->data.modelExchangeV2.ContinuousTimeMode)
        {
            status = fmu->fmi2EnterContinuousTimeMode(structFmu->instance);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2EnterContinuousTimeMode");
                return;
            }
            FMU_SIM_PRINT("fmi2EnterContinuousTimeMode\n");
            structFmu->data.modelExchangeV2.state = structFmu->data.modelExchangeV2.ContinuousTimeMode;
        }

        // Set independent variable time
        time = get_scicos_time();
        status = fmu->fmi2SetTime(structFmu->instance, time);
        if (status > fmi2Warning)
        {
            free_instance(block, -3, "fmi2SetTime");
            return;
        }

        // Set continuous-time and discrete-time inputs
        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV2.state == structFmu->data.modelExchangeV2.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi2Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // Set continuous states
        status = fmu->fmi2SetContinuousStates(structFmu->instance, block->x, block->nx);
        if (status > fmi2Discard)
        {
            free_instance(block, -3, "fmi2SetContinuousStates");
            return;
        }

        // Get event indicators to detect the zero-crossing
        status = fmu->fmi2GetEventIndicators(structFmu->instance, block->g, block->ng);
        if (status > fmi2Discard)
        {
            free_instance(block, -3, "fmi2GetEventIndicators");
            return;
        }
    }
    else if (flag == Ending)
    {
        // sciprint("\n");
        // sciprint("*****Ending block [%d] \n", get_block_number());
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME2;

        if (structFmu->data.modelExchangeV2.state > structFmu->data.modelExchangeV2.InitializationMode)
        {
            time = get_scicos_time();
            status = fmu->fmi2SetTime(structFmu->instance, time);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2SetTime");
                return;
            }

            // retrieve outputs at final time; the FMU is unusable after fmi2Terminate
            if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi2Warning)
            {
                free_instance(block, -3, "fmiGet_values");
                return;
            }

            // terminate simulation
            status = fmu->fmi2Terminate(structFmu->instance);
            if (status > fmi2Warning)
            {
                free_instance(block, -3, "fmi2Terminate");
                return;
            }
        }

        // freeing of instance
        fmu->fmi2FreeInstance(structFmu->instance);
        free_instance(block, 0, "fmu_simulate_me");
        return;
    }
}
