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

    SCICOS_BLOCKS_IMPEXP void fmu3_simulate_me(scicos_block *block, scicos_flag flag);
}

// FMI 3.0, Event Mode: the FMU is iterated until the discrete states are
// stable (discreteStatesNeedUpdate == fmi3False):
//  - set inputs
//  - fmi3UpdateDiscreteStates
//  - get outputs
// The iteration is capped by the block setting to fail loudly on an FMU that
// never converges. Returns the number of iterations, or a negative error code.
static int fmi3EventIteration(scicos_block *block, scicos_flag flag, Fmu3ModelExchange *fmu, workFMU *structFmu, int iterationLimit,
                              fmi3Boolean *terminateSimulation, fmi3Boolean *nominalsOfContinuousStatesChanged, fmi3Boolean *valuesOfContinuousStatesChanged)
{
    uint32_t *sciInputRefs = Getuint32OparPtrs(block, 2);
    size_t inpRefsSize = GetOparSize(block, 2, 1) * GetOparSize(block, 2, 2);
    int *sciInputTypes = Getint32OparPtrs(block, 3);
    uint32_t *sciOutputRefs = Getuint32OparPtrs(block, 4);
    size_t outRefsSize = GetOparSize(block, 4, 1) * GetOparSize(block, 4, 2);
    int *sciOutputTypes = Getint32OparPtrs(block, 5);
    fmi3Status status;
    fmi3Boolean discreteStatesNeedUpdate = fmi3True;
    int iteration = 0;

    while (discreteStatesNeedUpdate)
    {
        if (iteration >= iterationLimit)
        {
            return -1;
        }
        // Set continuous-time and discrete-time inputs
        if (fmiSet_values(block, flag, fmi3False, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            return -2;
        }
        status = fmu->fmi3UpdateDiscreteStates(structFmu->instance, &discreteStatesNeedUpdate, terminateSimulation, nominalsOfContinuousStatesChanged, valuesOfContinuousStatesChanged, &structFmu->data.modelExchangeV3.nextEventTimeDefined, &structFmu->data.modelExchangeV3.nextEventTime);
        if (status > fmi3Discard)
        {
            return -3;
        }
        // Get outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
        {
            return -4;
        }
        iteration++;
    }
    return iteration;
}

SCICOS_BLOCKS_IMPEXP void fmu3_simulate_me(scicos_block *block, scicos_flag flag)
{
    char *pathName = NULL, *resourceLocation = NULL, *identName = NULL, *instantiationToken = NULL;
    double *sciTolerance = NULL;
    int *sciInputTypes = NULL, *sciOutputTypes = NULL;
    fmi3Status status;
    workFMU *structFmu = (workFMU *)(*block->work);
    Fmu3ModelExchange *fmu = nullptr;
    double time = 0;
    int eventIterationLimit;
    uint32_t *sciInputRefs = NULL, *sciOutputRefs = NULL;
    fmi3Boolean loggingOn = fmi3False;
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
    instantiationToken = Getint8OparPtrs(block, 12); // instantiationToken
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
        structFmu->fmuME3 = new Fmu3ModelExchange();
        fmu = structFmu->fmuME3;
        if (fmu->create(pathName, identName) == false)
        {
            free_instance(block, -3, "Fmu3ModelExchange"); // internal error
            return;
        }
        // set fmi3Type
        structFmu->type = workFMU::ModelExchange;
        structFmu->version = workFMU::fmiV3;

        // instantiate model
        structFmu->instance = fmu->fmi3InstantiateModelExchange(identName, instantiationToken, resourceLocation, fmi3True, loggingOn, block, fmu3_callback_logger);
        if (structFmu->instance == NULL)
        {
            free_instance(block, -3, "fmi3InstantiateModelExchange");
            return;
        }
        FMU_SIM_PRINT("fmi3InstantiateModelExchange\n");
        structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.Instantiated;
        // set debug logging
        if (loggingOn)
        {
            status = fmu->fmi3SetDebugLogging(structFmu->instance, loggingOn, 0, NULL);
            if (status > fmi3Warning)
            {
                free_instance(block, -3, "fmi3SetDebugLogging");
                return;
            }
        }

        // set start values
        status = (fmi3Status) fmiSet_parameters(block, flag);
        if (status > 1)
        {
            free_instance(block, -3, "fmiSet_parameters");
            return;
        }

        // enter initialization mode
        if (sciTolerance[0])
        {
            status = fmu->fmi3EnterInitializationMode(structFmu->instance, fmi3True, sciTolerance[0], tStart, fmi3False, 0.0);
        }
        else
        {
            status = fmu->fmi3EnterInitializationMode(structFmu->instance, fmi3False, 0.0, tStart, fmi3False, 0.0);
        }
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3EnterInitializationMode");
            return;
        }
        FMU_SIM_PRINT("fmi3EnterInitializationMode\n");
        structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.InitializationMode;

        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // retrieve solution at time = 0.00
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
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
        fmu = structFmu->fmuME3;

        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
        {
            free_instance(block, -3, "fmiGet_values");
            return;
        }

        if (block->nx > 0)
        {
            status = fmu->fmi3GetContinuousStates(structFmu->instance, block->x, block->nx);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3GetContinuousStates");
                return;
            }
        }
    }
    else if (flag == DerivativeState)
    {
        if ((structFmu = (workFMU *)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuME3;

        // Derivative computation is always in Continuous mode
        if (structFmu->data.modelExchangeV3.state != structFmu->data.modelExchangeV3.ContinuousTimeMode)
        {
            status = fmu->fmi3EnterContinuousTimeMode(structFmu->instance);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3EnterContinuousTimeMode");
                return;
            }
            FMU_SIM_PRINT("fmi3EnterContinuousTimeMode\n");
            structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.ContinuousTimeMode;
        }
        structFmu->data.modelExchangeV3.enterEventMode = fmi3False;

        // Set independent variable time
        time = get_scicos_time();
        status = fmu->fmi3SetTime(structFmu->instance, time);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3SetTime");
            return;
        }

        // Set continuous-time inputs
        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // Set continuous-time states
        status = fmu->fmi3SetContinuousStates(structFmu->instance,  block->x, block->nx);
        if (status > fmi3Discard)
        {
            free_instance(block, -3, "fmi3SetContinuousStates");
            return;
        }

        // Evaluate and get derivatives
        status = fmu->fmi3GetContinuousStateDerivatives(structFmu->instance, block->xd, block->nx);
        if (status > fmi3Discard)
        {
            free_instance(block, -3, "fmi3GetContinuousStateDerivatives");
            return;
        }

        // Complete integrator step and return enterEventMode
        fmi3Boolean terminateSimulation = fmi3False;
        status = fmu->fmi3CompletedIntegratorStep(structFmu->instance, fmi3True, &structFmu->data.modelExchangeV3.enterEventMode, &terminateSimulation);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3CompletedIntegratorStep");
            return;
        }
        if (terminateSimulation)
        {
            free_instance(block, -3, "fmi3CompletedIntegratorStep");
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
        fmu = structFmu->fmuME3;

        if (structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.InitializationMode)
            {
            // exit initialization mode on the first call
            status = fmu->fmi3ExitInitializationMode(structFmu->instance);
            if (status > fmi3Warning)
            {
                free_instance(block, -3, "fmi3ExitInitializationMode");
                return;
            }
            FMU_SIM_PRINT("fmi3ExitInitializationMode\n");
            structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.EventMode;
        }

        // Set independent variable time
        time = get_scicos_time();
        status = fmu->fmi3SetTime(structFmu->instance, time);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3SetTime");
            return;
        }

        // From the previous fmi3NewDiscreteStates call, the FMU requested to enter into EventMode at a specific time
        structFmu->data.modelExchangeV3.timeEvent = (structFmu->data.modelExchangeV3.nextEventTimeDefined) && (structFmu->data.modelExchangeV3.nextEventTime <= time);

        // From Continuous-Time Mode, the FMU requested to enter into EventMode
        //  - with the enterEventMode output of fmi3CompletedIntegratorStep coming from the previous DerivativeState
        //  - with the eventInfo.nextEventTimeDefined output of fmi3NewDiscreteStates
        if (structFmu->data.modelExchangeV3.state != structFmu->data.modelExchangeV3.EventMode)
        {
            if (structFmu->data.modelExchangeV3.enterEventMode || structFmu->data.modelExchangeV3.timeEvent)
            {
                status = fmu->fmi3EnterEventMode(structFmu->instance);
                if (status > fmi3Warning)
                {
                    free_instance(block, -3, "fmi3EnterEventMode");
                    return;
                }
                FMU_SIM_PRINT("fmi3EnterEventMode\n");
                structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.EventMode;
            }
        }

        // Set continuous-time and discrete-time inputs
        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // From the previous fmi3UpdateDiscreteStates call, the FMU requested to
        // enter into EventMode at a specific time, or with the enterEventMode
        // output of fmi3CompletedIntegratorStep coming from the previous DerivativeState
        if (structFmu->data.modelExchangeV3.enterEventMode || structFmu->data.modelExchangeV3.timeEvent)
        {
            // FMI 3.0, Event Mode: event iteration on the FMU
            fmi3Boolean terminateSimulation;
            fmi3Boolean nominalsOfContinuousStatesChanged;
            fmi3Boolean valuesOfContinuousStatesChanged;
            int iterations = fmi3EventIteration(block, flag, fmu, structFmu, eventIterationLimit, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged);
            if (iterations < 0)
            {
                free_instance(block, -3, "fmi3UpdateDiscreteStates");
                return;
            }
            if (valuesOfContinuousStatesChanged)
            {
                status = fmu->fmi3GetContinuousStates(structFmu->instance, block->x, block->nx);
                if (status > fmi3Discard)
                {
                    free_instance(block, -3, "fmi3GetContinuousStates");
                    return;
                }
            }
        }

        if (structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.ContinuousTimeMode)
        {
            // Set continuous-time states
            status = fmu->fmi3SetContinuousStates(structFmu->instance, block->x, block->nx);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3SetContinuousStates");
                return;
            }
        }

        // get outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
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
        fmu = structFmu->fmuME3;

        // after a scicos "root found" event ; increment super dense time: done by the solver
        if (structFmu->data.modelExchangeV3.state != structFmu->data.modelExchangeV3.EventMode)
        {
            status = fmu->fmi3EnterEventMode(structFmu->instance);
            if (status > fmi3Warning)
            {
                free_instance(block, -3, "fmi3EnterEventMode");
                return;
            }
            FMU_SIM_PRINT("fmi3EnterEventMode\n");
            structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.EventMode;
        }

        // in Event-mode, there is a point-fix iteration on the FMU
        // (FMI 3.0, Event Mode): until discreteStatesNeedUpdate == fmi3False
        fmi3Boolean terminateSimulation;
        fmi3Boolean nominalsOfContinuousStatesChanged;
        fmi3Boolean valuesOfContinuousStatesChanged;
        int iterations = fmi3EventIteration(block, flag, fmu, structFmu, eventIterationLimit, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged);
        if (iterations < 0)
        {
            free_instance(block, -3, "fmi3UpdateDiscreteStates");
            return;
        }
        if (terminateSimulation)
        {
            free_instance(block, -3, "terminateSimulation");
            return;
        }
        if (iterations > 1)
        {
            // discrete states changed during the event iteration
            do_cold_restart();
        }
        if (nominalsOfContinuousStatesChanged)
        {
            do_cold_restart();
        }
        if (valuesOfContinuousStatesChanged)
        {
            status = fmu->fmi3GetContinuousStates(structFmu->instance, block->x, block->nx);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3GetContinuousStates");
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
        fmu = structFmu->fmuME3;

        // Zero-crossing detection is always on continuous mode, we have to set the states values
        if (structFmu->data.modelExchangeV3.state != structFmu->data.modelExchangeV3.ContinuousTimeMode)
        {
            status = fmu->fmi3EnterContinuousTimeMode(structFmu->instance);
            if (status > fmi3Warning)
            {
                free_instance(block, -3, "fmi3EnterContinuousTimeMode");
                return;
            }
            FMU_SIM_PRINT("fmi3EnterContinuousTimeMode\n");
            structFmu->data.modelExchangeV3.state = structFmu->data.modelExchangeV3.ContinuousTimeMode;
        }

        // Set independent variable time
        time = get_scicos_time();
        status = fmu->fmi3SetTime(structFmu->instance, time);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3SetTime");
            return;
        }

        // Set continuous-time and discrete-time inputs
        if (fmiSet_values(block, flag, structFmu->data.modelExchangeV3.state == structFmu->data.modelExchangeV3.ContinuousTimeMode, sciInputTypes, &block->insz[2 * block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            free_instance(block, -3, "fmiSet_values");
            return;
        }

        // Set continuous states
        status = fmu->fmi3SetContinuousStates(structFmu->instance, block->x, block->nx);
        if (status > fmi3Discard)
        {
            free_instance(block, -3, "fmi3SetContinuousStates");
            return;
        }

        // Get event indicators to detect the zero-crossing
        status = fmu->fmi3GetEventIndicators(structFmu->instance, block->g, block->ng);
        if (status > fmi3Discard)
        {
            free_instance(block, -3, "fmi3GetEventIndicators");
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
        fmu = structFmu->fmuME3;

        if (structFmu->data.modelExchangeV3.state > structFmu->data.modelExchangeV3.InitializationMode)
        {
            time = get_scicos_time();
            status = fmu->fmi3SetTime(structFmu->instance, time);
            if (status > fmi3Warning)
            {
                free_instance(block, -3, "fmi3SetTime");
                return;
            }

            // retrieve outputs at final time; the FMU is unusable after fmi3Terminate
            if (fmiGet_values(block, sciOutputTypes, &block->outsz[2 * block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
            {
                free_instance(block, -3, "fmiGet_values");
                return;
            }

            // terminate simulation
            status = fmu->fmi3Terminate(structFmu->instance);
            if (status > fmi3Warning)
            {
                free_instance(block, -3, "fmi3Terminate");
                return;
            }
        }

        // freeing of instance
        fmu->fmi3FreeInstance(structFmu->instance);
        free_instance(block, 0, "fmu_simulate_me");
        return;
    }
}
