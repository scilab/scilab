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

    SCICOS_BLOCKS_IMPEXP void fmu3_simulate_cs(scicos_block *block, scicos_flag flag);
}

SCICOS_BLOCKS_IMPEXP void fmu3_simulate_cs(scicos_block *block, scicos_flag flag)
{
    char *pathName = NULL, *resourceLocation = NULL, *identName = NULL, *instantiationToken = NULL;
    int *sciInputTypes = NULL, *sciOutputTypes = NULL;
    fmi3Status status;
    // main structure
    workFMU *structFmu;
    Fmu3CoSimulation *fmu;
    double time = 0, dt = 0;
    uint32_t *sciInputRefs = NULL, *sciOutputRefs = NULL;
    size_t outRefsSize, inpRefsSize;
    fmi3Boolean loggingOn = fmi3False;
    fmi3Boolean hasEventMode = fmi3False;
    fmi3Float64 tStart = 0, tStop = 0;
    fmiBoolean StopTimeDefined = fmiFalse;

    // return if an fmu has not been loaded
    if (block->nopar == 0)
    {
        return;
    }

    // settings
    pathName = Getint8OparPtrs(block, 1); // path to the lib
    resourceLocation = Getint8OparPtrs(block, 10); // fmuResourceLocation
    identName = Getint8OparPtrs(block, 11); // identifier
    instantiationToken = Getint8OparPtrs(block, 12); // instantiationToken
    char* configuration = Getint8OparPtrs(block, 13);
    loggingOn = configuration[0]; // logger configuration
    hasEventMode = configuration[1]; // hasEventMode

    // inputs
    sciInputRefs = Getuint32OparPtrs(block, 2);
    inpRefsSize = GetOparSize(block, 2, 1) * GetOparSize(block, 2, 2);
    sciInputTypes = Getint32OparPtrs(block, 3);
    // outputs
    sciOutputRefs = Getuint32OparPtrs(block, 4);
    outRefsSize = GetOparSize(block, 4, 1) * GetOparSize(block, 4, 2);
    sciOutputTypes = Getint32OparPtrs(block, 5);

    time = get_scicos_time();

    if (flag == Initialization)
    {
        // sciprint("\n");
        // sciprint("*****Initialization of block [%d] Time [%f] \n",get_block_number(), get_scicos_time());

        // memory allocation for the main structure
        if ((*(block->work) = (workFMU*)scicos_malloc(sizeof(workFMU))) == NULL)
        {
            set_block_error(-16); // memory allocation error
            *(block->work) = NULL;
            return;
        }
        structFmu = (workFMU*)(*block->work);
        // clear the structFmu->fmuCS3 and structFmu->data
        memset(structFmu, 0x0, sizeof(workFMU));

        // load library and create an instance
        structFmu->fmuCS3 = new Fmu3CoSimulation();
        fmu = structFmu->fmuCS3;
        if (fmu->create(pathName, identName) == false)
        {
            //sciprint(fmu_wrap_error(), "fmu_simulate_cs");
            free_instance(block, -3, "Fmu3CoSimulation"); // internal error
            return;
        }
        // set fmiType
        structFmu->type = workFMU::CoSimulation;
        structFmu->version = workFMU::fmiV3;

        // instantiate slave
        structFmu->instance = fmu->fmi3InstantiateCoSimulation(identName, instantiationToken, resourceLocation, fmi3True, loggingOn, hasEventMode, fmi3False, nullptr, 0, block, fmu3_callback_logger, nullptr);
        if (structFmu->instance == NULL)
        {
            free_instance(block, -3, "fmi3InstantiateCoSimulation");
            return;
        }
        structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.Instantiated;
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

        // setup experiment
        tStart = get_scicos_time();
        

        // set start values for variables with initial="exact" (as default or preset)
        status = (fmi3Status) fmiSet_parameters(block, flag);
        if(status > fmi3Warning)
        {
            free_instance(block, -3, "fmiSet_parameters");
            return;
        }

        // enter initialization mode
        status = fmu->fmi3EnterInitializationMode(structFmu->instance, fmiFalse, 0, tStart, StopTimeDefined, tStop);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3EnterInitializationMode");
            return;
        }
        structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.InitializationMode;
    }
    
    // synchronize inputs, outputs
    if (flag == Initialization || flag == ReInitialization)
    {
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS3;

        // TODO: use the start value from the xml file, this will fix the valid enumeration value problem

        // set inputs
        if (fmiSet_values(block, flag, 0, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            return;
        }

        fmi3Boolean discreteStatesNeedUpdate;
        fmi3Boolean terminateSimulation;
        fmi3Boolean nominalsOfContinuousStatesChanged;
        fmi3Boolean valuesOfContinuousStatesChanged;
        fmi3Boolean nextEventTimeDefined;
        fmi3Float64 nextEventTime;
        
        status = fmu->fmi3UpdateDiscreteStates(structFmu->instance, &discreteStatesNeedUpdate, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged, &nextEventTimeDefined, &nextEventTime);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3UpdateDiscreteStates");
            return;
        }

        // set outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
        {
            return;
        }
    }

    if (flag == OutputUpdate)
    {
        // sciprint("\n");
        // sciprint("*****Output update block [%d] Time [%f] \n",get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS3;

        // only at the start of the simulation, we exit initialization mode
        if (structFmu->data.coSimulationV3.state == structFmu->data.coSimulationV3.InitializationMode)
        {
            status = fmu->fmi3ExitInitializationMode(structFmu->instance);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3ExitInitializationMode");
                return;
            }
            FMU_SIM_PRINT("fmi3ExitInitializationMode\n");

            if (hasEventMode)
            {
                // switch to EventMode as fmi3InstantiateCoSimulation() was configure the FMU to use events
                structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.EventMode;
            } 
            else
            {
                structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.StepMode;
            }
        }

        // copy Scicos inputs to fmiSet variables
        if (fmiSet_values(block, flag, 0, sciInputTypes, &block->insz[2*block->nin], inpRefsSize, sciInputRefs, block->inptr) > fmi3Warning)
        {
            return;
        }

        if (structFmu->data.coSimulationV3.state != structFmu->data.coSimulationV3.StepMode)
        {
            status = fmu->fmi3EnterStepMode(structFmu->instance);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3EnterStepMode");
                return;
            }
            FMU_SIM_PRINT("fmi3EnterStepMode\n");
            
            structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.StepMode;
        }

        dt = time - structFmu->data.coSimulationV3.oldTime;

        if (dt > 0)
        {
            // do step
            fmi3Boolean eventHandlingNeeded;
            fmi3Boolean terminateSimulation;
            fmi3Boolean earlyReturn;
            fmi3Float64 lastSuccessfulTime;
            status = fmu->fmi3DoStep((structFmu->instance), structFmu->data.coSimulationV3.oldTime, dt, fmiTrue, &eventHandlingNeeded, &terminateSimulation, &earlyReturn, &lastSuccessfulTime);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3DoStep");
                return;
            }
            if (hasEventMode && eventHandlingNeeded)
            {
                if (structFmu->data.coSimulationV3.state != structFmu->data.coSimulationV3.EventMode)
                {
                    status = fmu->fmi3EnterEventMode(structFmu->instance);
                    if (status > fmi3Discard)
                    {
                        free_instance(block, -3, "fmi3EnterEventMode");
                        return;
                    }
                    FMU_SIM_PRINT("fmi3EnterEventMode\n");
                    
                    structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.EventMode;
                }

                fmi3Boolean discreteStatesNeedUpdate;
                fmi3Boolean terminateSimulation;
                fmi3Boolean nominalsOfContinuousStatesChanged;
                fmi3Boolean valuesOfContinuousStatesChanged;
                
                status = fmu->fmi3UpdateDiscreteStates(structFmu->instance, &discreteStatesNeedUpdate, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged, &structFmu->data.coSimulationV3.nextEventTimeDefined, &structFmu->data.coSimulationV3.nextEventTime);
                if (status > fmi3Warning)
                {
                    free_instance(block, -3, "fmi3UpdateDiscreteStates");
                    return;
                }
            }
            if (terminateSimulation)
            {
                sciprint("FMU requested to terminate simulation at time %lf\n", lastSuccessfulTime);
                set_block_error(-5);
                return;
            }
            if (earlyReturn)
            {
                sciprint("FMU requested to early return at time %lf\n", lastSuccessfulTime);
                set_block_error(-5);
                return;
            }
            structFmu->data.coSimulationV3.oldTime = time;
        }

        // copy fmiGet variables to Scicos outputs
        if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
        {
            return;
        }
    }

    if (hasEventMode && flag == StateUpdate)
    {
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS3;

        if (structFmu->data.coSimulationV3.state != structFmu->data.coSimulationV3.EventMode)
        {
            status = fmu->fmi3EnterEventMode(structFmu->instance);
            if (status > fmi3Discard)
            {
                free_instance(block, -3, "fmi3EnterEventMode");
                return;
            }
            FMU_SIM_PRINT("fmi3EnterEventMode\n");
            
            structFmu->data.coSimulationV3.state = structFmu->data.coSimulationV3.EventMode;
        }

        fmi3Boolean discreteStatesNeedUpdate;
        fmi3Boolean terminateSimulation;
        fmi3Boolean nominalsOfContinuousStatesChanged;
        fmi3Boolean valuesOfContinuousStatesChanged;
        
        status = fmu->fmi3UpdateDiscreteStates(structFmu->instance, &discreteStatesNeedUpdate, &terminateSimulation, &nominalsOfContinuousStatesChanged, &valuesOfContinuousStatesChanged, &structFmu->data.coSimulationV3.nextEventTimeDefined, &structFmu->data.coSimulationV3.nextEventTime);
        if (status > fmi3Warning)
        {
            free_instance(block, -3, "fmi3UpdateDiscreteStates");
            return;
        }
    }

    if (flag == Ending)
    {
        // sciprint("\n");
        // sciprint("*****Ending of block [%d] Time [%f] \n", get_block_number(), get_scicos_time());
        if ((structFmu = (workFMU*)(*block->work)) == NULL)
        {
            return;
        }
        fmu = structFmu->fmuCS3;

        if (structFmu->data.coSimulationV3.state > structFmu->data.coSimulationV3.InitializationMode)
        {
            // copy fmiGet variables to Scicos outputs
            if (fmiGet_values(block, sciOutputTypes, &block->outsz[2*block->nout], outRefsSize, sciOutputRefs, block->outptr) > fmi3Warning)
            {
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
        free_instance(block, 0, "fmu_simulate_cs");
    }
}
