#include "fmu_wrapper.hxx"

extern "C" {
#include "dynlib_fmucpp_wrapper.h"

#include "fmiPlatformTypes.h"
#include "fmiFunctionTypes.h"
#include "fmiFunctions.h"

#include "fmi2TypesPlatform.h"
#include "fmi2FunctionTypes.h"
#include "fmi2Functions.h"

#include "scicos.h"
#include "scicos_block4.h"
#include "Scierror.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "lasterror.h"
#include "sciprint.h"
#include "fmu_sim_utility.h"
}

#include "UTF8.hxx"

#include <cstring>
#include <cinttypes>

// mask for negatedAlias or discrete variables
#define NEGATED_ALIAS   (1 << 0)
#define DISCRETE        (1 << 1)

// set start values for variables with initial="exact" (as default or preset)
int fmiSet_parameters(scicos_block *block, scicos_flag flag)
{
    uint32_t *sciRealRefs = NULL, *sciIntRefs = NULL, *sciBoolRefs = NULL;
    fmiReal *rpar = NULL;
    fmiInteger *ipar = NULL;
    int *sciBoolValues = NULL;
    size_t realRefsSize, intRefsSize, boolRefsSize;
    fmi2Status status2 = fmi2OK;
    fmi3Status status3 = fmi3OK;
    fmi2Status globalStatus2 = fmi2OK;
    fmi3Status globalStatus3 = fmi3OK;

    size_t i, j;
    workFMU *structFmu = *(workFMU **)(block->work);

    // real values
    sciRealRefs = Getuint32OparPtrs(block, 6); // real refs
    realRefsSize = GetOparSize(block, 6, 1) * GetOparSize(block, 6, 2); // real refs size
    rpar = GetRparPtrs(block); // real values
    // integer values
    sciIntRefs = Getuint32OparPtrs(block, 7);
    intRefsSize = GetOparSize(block, 7, 1) * GetOparSize(block, 7, 2);
    ipar = GetIparPtrs(block);
    // boolean values
    sciBoolRefs = Getuint32OparPtrs(block, 8);
    boolRefsSize = GetOparSize(block, 8, 1) * GetOparSize(block, 8, 2);
    sciBoolValues = Getint32OparPtrs(block, 9); // boolean values
    
    // real
    if (realRefsSize > 0)
    {
        fmi2Real* realValues = (fmi2Real*) scicos_malloc(realRefsSize/2 * sizeof(fmi2Real));

        // check negated alias and store referenceValues
        for (i = 0, j = realRefsSize/2; j < realRefsSize; i++, j++)
        {
            if (sciRealRefs[j] > 0)
            {
                // negated alias set to negative parameter value
                realValues[i] = -rpar[i];
            }
            else
            {
                realValues[i] = rpar[i];
            }

            FMU_SIM_PRINT("fmiSet_parameters real valueReference %d set to %lf\n", sciRealRefs[i], realValues[i]);
        }
        // set real values
        if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
        {
            status2 = structFmu->fmuME2->fmi2SetReal(structFmu->instance, sciRealRefs, i, realValues);
        }
        else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
        {
            status2 = structFmu->fmuCS2->fmi2SetReal(structFmu->instance, sciRealRefs, i, realValues);
        }
        else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
        {
            status3 = structFmu->fmuME3->fmi3SetFloat64(structFmu->instance, sciRealRefs, i, realValues, i);
        }
        else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
        {
            status3 = structFmu->fmuCS3->fmi3SetFloat64(structFmu->instance, sciRealRefs, i, realValues, i);
        }
        else
        {
            // programming error
            assert(!"fmiSet_parameters: invalid version or type");
        }

        // store the highest possible error
        if (status2 > globalStatus2)
        {
            globalStatus2 = status2;
        }
        if (status3 > globalStatus3)
        {
            globalStatus3 = status3;
        }

        scicos_free(realValues);
    }
    // integers
    if (intRefsSize > 0)
    {
        fmi2Integer* intValues = (fmi2Integer*) scicos_malloc(intRefsSize/2 * sizeof(fmi2Integer));

        // check negated alias and store referenceValues
        for (i = 0, j = intRefsSize/2; j < intRefsSize; i++, j++)
        {
            if (sciIntRefs[j] > 0)
            {
                // negated alias set to negative parameter value
                intValues[i] = -ipar[i];
            }
            else
            {
                intValues[i] = ipar[i];
            }

            FMU_SIM_PRINT("fmiSet_parameters integer valueReference %d set to %lf\n", sciIntRefs[i], intValues[i]);
        }
        // set integer values
        if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
        {
            status2 = structFmu->fmuME2->fmi2SetInteger(structFmu->instance, sciIntRefs, i, intValues);
        }
        else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
        {
            status2 = structFmu->fmuCS2->fmi2SetInteger(structFmu->instance, sciIntRefs, i, intValues);
        }
        else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
        {
            status3 = structFmu->fmuME3->fmi3SetInt32(structFmu->instance, sciIntRefs, i, intValues, i);
        }
        else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
        {
            status3 = structFmu->fmuCS3->fmi3SetInt32(structFmu->instance, sciIntRefs, i, intValues, i);
        }
        else
        {
            // programming error
            assert(!"fmiSet_parameters: invalid version or type");
        }

        // store the highest possible error
        if (status2 > globalStatus2)
        {
            globalStatus2 = status2;
        }
        if (status3 > globalStatus3)
        {
            globalStatus3 = status3;
        }

        scicos_free(intValues);
    }
    // boolean
    if (boolRefsSize > 0)
    {
        // pick the largest boolean type
        union sized_boolean_t
        {
            fmiBoolean b1;
            fmi2Boolean b2;
            fmi3Boolean b3;
        };
        
        fmi2Boolean* boolValues2 = (fmi2Boolean*) scicos_malloc(boolRefsSize/2 * sizeof(sized_boolean_t));
        fmi3Boolean* boolValues3 = (fmi3Boolean*) boolValues2;

        // check negated alias and store referenceValues
        for (i = 0, j = boolRefsSize/2; j < boolRefsSize; i++, j++)
        {
            if (sciBoolRefs[j] > 0)
            {
                // negated alias set to negative parameter value
                if (sciBoolValues[i] == 0)
                {
                    if (structFmu->version == workFMU::fmiV2)   
                    {
                        boolValues2[i] = fmi2True;
                    }
                    else if (structFmu->version == workFMU::fmiV3)   
                    {
                        boolValues3[i] = fmi3True;
                    }
                }
                else
                {
                    if (structFmu->version == workFMU::fmiV2)   
                    {
                        boolValues2[i] = fmi2False;
                    }
                    else if (structFmu->version == workFMU::fmiV3)   
                    {
                        boolValues3[i] = fmi3False;
                    }
                }
            }
            else
            {
                if (sciBoolValues[i] != 0)
                {
                    if (structFmu->version == workFMU::fmiV2)   
                    {
                        boolValues2[i] = fmi2True;
                    }
                    else if (structFmu->version == workFMU::fmiV3)   
                    {
                        boolValues3[i] = fmi3True;
                    }
                }
                else
                {
                    if (structFmu->version == workFMU::fmiV2)   
                    {
                        boolValues2[i] = fmi2False;
                    }
                    else if (structFmu->version == workFMU::fmiV3)   
                    {
                        boolValues3[i] = fmi3False;
                    }
                }
            }
            
            FMU_SIM_PRINT("fmiSet_parameters boolean valueReference %d set to %lf\n", sciBoolRefs[i], boolValues3[i]);
        }
        // set boolean values
        if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
        {
            status2 = structFmu->fmuME2->fmi2SetBoolean(structFmu->instance, sciBoolRefs, i, boolValues2);
        }
        else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
        {
            status2 = structFmu->fmuCS2->fmi2SetBoolean(structFmu->instance, sciBoolRefs, i, boolValues2);
        }
        else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
        {
            status3 = structFmu->fmuME3->fmi3SetBoolean(structFmu->instance, sciBoolRefs, i, boolValues3, i);
        }
        else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
        {
            status3 = structFmu->fmuCS3->fmi3SetBoolean(structFmu->instance, sciBoolRefs, i, boolValues3, i);
        }
        else
        {
            // programming error
            assert(!"fmiSet_parameters: invalid version or type");
        }

        // store the highest possible error
        if (status2 > globalStatus2)
        {
            globalStatus2 = status2;
        }
        if (status3 > globalStatus3)
        {
            globalStatus3 = status3;
        }

        scicos_free(boolValues2);
    }

    // report previous errors
    if (globalStatus2 > fmi2OK)
    {
        return globalStatus2;
    }
    if (globalStatus3 > fmi3OK)
    {
        return globalStatus3;
    }
    return globalStatus3;
}

template<typename T> inline constexpr
void assign_from(void* from, int type, uint32_t ref, T& value)
{
    if (type == SCSREAL_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSREAL_COP* negated = (SCSREAL_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSREAL_COP* src = (SCSREAL_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSCOMPLEX_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSCOMPLEX_COP* negated = (SCSCOMPLEX_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSCOMPLEX_COP* src = (SCSCOMPLEX_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSINT_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT_COP* negated = (SCSINT_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSINT_COP* src = (SCSINT_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSINT8_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT8_COP* negated = (SCSINT8_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSINT8_COP* src = (SCSINT8_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSINT16_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT16_COP* negated = (SCSINT16_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSINT16_COP* src = (SCSINT16_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSINT32_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT32_COP* negated = (SCSINT32_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSINT32_COP* src = (SCSINT32_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSUINT_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT_COP* negated = (SCSUINT_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSUINT_COP* src = (SCSUINT_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSUINT8_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT8_COP* negated = (SCSUINT8_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSUINT8_COP* src = (SCSUINT8_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSUINT16_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT16_COP* negated = (SCSUINT16_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSUINT16_COP* src = (SCSUINT16_COP*) from;
            value = (T) *src;
        }
    }
    else if (type == SCSUINT32_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT32_COP* negated = (SCSUINT32_COP*) from;
            value = - (T) *negated;
        }
        else
        {
            SCSUINT32_COP* src = (SCSUINT32_COP*) from;
            value = (T) *src;
        }
    }
    else
    {
        // do nothing on unknown "into" type
        value = T();
    }
};

template<> inline constexpr
void assign_from(void* from, int type, uint32_t ref, bool& value)
{
    if (type == SCSREAL_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSREAL_COP* negated = (SCSREAL_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSREAL_COP* src = (SCSREAL_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSCOMPLEX_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSCOMPLEX_COP* negated = (SCSCOMPLEX_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSCOMPLEX_COP* src = (SCSCOMPLEX_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSINT_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT_COP* negated = (SCSINT_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSINT_COP* src = (SCSINT_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSINT8_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT8_COP* negated = (SCSINT8_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSINT8_COP* src = (SCSINT8_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSINT16_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT16_COP* negated = (SCSINT16_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSINT16_COP* src = (SCSINT16_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSINT32_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT32_COP* negated = (SCSINT32_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSINT32_COP* src = (SCSINT32_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSUINT_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT_COP* negated = (SCSUINT_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSUINT_COP* src = (SCSUINT_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSUINT8_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT8_COP* negated = (SCSUINT8_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSUINT8_COP* src = (SCSUINT8_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSUINT16_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT16_COP* negated = (SCSUINT16_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSUINT16_COP* src = (SCSUINT16_COP*) from;
            value = *src != 0;
        }
    }
    else if (type == SCSUINT32_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT32_COP* negated = (SCSUINT32_COP*) from;
            value = *negated == 0;
        }
        else
        {
            SCSUINT32_COP* src = (SCSUINT32_COP*) from;
            value = *src != 0;
        }
    }
    else
    {
        // do nothing on unknown "into" type
        value = false;
    }
};


// set input Values
int fmiSet_values(scicos_block *block, scicos_flag flag, int continuousMode, int *declaredTypes, int* blockTypes, size_t inpSize, uint32_t *sciInputRefs, void** from)
{
    fmiStatus status = fmiOK;
    fmi2Status status2 = fmi2OK;
    fmi3Status status3 = fmi3OK;
    fmiStatus globalStatus = fmiOK;
    fmi2Status globalStatus2 = fmi2OK;
    fmi3Status globalStatus3 = fmi3OK;

    size_t i, j, k;
    workFMU *structFmu = *(workFMU **)(block->work);
    if (structFmu == nullptr)
    {
        return fmiFatal;
    }

    for (i = 0, k = inpSize / 2, j = 0; k < inpSize; i++, j++, k++)
    {
        if (declaredTypes[i] == 108) // Real
        {
            double value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }

            FMU_SIM_PRINT("fmiSet_values real valueReference %d set to %lf\n", sciInputRefs[j], value);

            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiSetReal((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiSetReal((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2SetReal((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2SetReal((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetFloat64((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetFloat64((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        if (declaredTypes[i] == 104) // Float32
        {
            float value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }

            FMU_SIM_PRINT("fmiSet_values real valueReference %d set to %lf\n", sciInputRefs[j], value);

            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetFloat32((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetFloat32((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 808) // Int64
        {
            int64_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetInt64((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetInt64((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 804) // Int32
        {
            int value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            FMU_SIM_PRINT("fmiSet_values integer valueReference %d set to %d\n", sciInputRefs[j], value);
            
            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiSetInteger((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiSetInteger((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2SetInteger((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2SetInteger((structFmu->instance), &sciInputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetInt32((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetInt32((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 802) // Int16
        {
            int16_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetInt16((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetInt16((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 801) // Int8
        {
            int8_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetInt8((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetInt8((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 818) // UInt64
        {
            uint64_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetUInt64((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetUInt64((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 814) // UInt32
        {
            uint32_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetUInt32((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetUInt32((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 812) // UInt16
        {
            uint16_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetUInt16((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetUInt16((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 811) // UInt8
        {
            uint8_t value;
            assign_from(from[i], blockTypes[i], sciInputRefs[k], value);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetUInt8((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetUInt8((structFmu->instance), &sciInputRefs[j], 1, &value, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 4) // Boolean
        {
            fmiBoolean v1Bool;
            if (structFmu->version == workFMU::fmiV1) assign_from(from[i], blockTypes[i], sciInputRefs[k], v1Bool);
            fmi2Boolean v2Bool;
            if (structFmu->version == workFMU::fmiV2) assign_from(from[i], blockTypes[i], sciInputRefs[k], v2Bool);
            fmi3Boolean v3Bool;
            if (structFmu->version == workFMU::fmiV3) assign_from(from[i], blockTypes[i], sciInputRefs[k], v3Bool);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            if (structFmu->version == workFMU::fmiV1) FMU_SIM_PRINT("fmiSet_values boolean valueReference %d set to %d\n", sciInputRefs[j], v1Bool);
            if (structFmu->version == workFMU::fmiV2) FMU_SIM_PRINT("fmiSet_values boolean valueReference %d set to %d\n", sciInputRefs[j], v2Bool);
            if (structFmu->version == workFMU::fmiV3) FMU_SIM_PRINT("fmiSet_values boolean valueReference %d set to %d\n", sciInputRefs[j], v3Bool);
            
            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiSetBoolean((structFmu->instance), &sciInputRefs[j], 1, &v1Bool);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiSetBoolean((structFmu->instance), &sciInputRefs[j], 1, &v1Bool);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2SetBoolean((structFmu->instance), &sciInputRefs[j], 1, &v2Bool);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2SetBoolean((structFmu->instance), &sciInputRefs[j], 1, &v2Bool);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetBoolean((structFmu->instance), &sciInputRefs[j], 1, &v3Bool, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetBoolean((structFmu->instance), &sciInputRefs[j], 1, &v3Bool, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 10) // String
        {
            int maxStringLen = GetInPortSize(block, i+1, 2);
            char* asCharPtr = (char *) from[i];
            asCharPtr[maxStringLen - 1] = '\0'; // ensure string termination

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            FMU_SIM_PRINT("fmiSet_values string valueReference %d set to \"%s\"\n", sciInputRefs[j], asCharPtr);

            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiSetString((structFmu->instance), &sciInputRefs[j], 1, &asCharPtr);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiSetString((structFmu->instance), &sciInputRefs[j], 1, &asCharPtr);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2SetString((structFmu->instance), &sciInputRefs[j], 1, &asCharPtr);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2SetString((structFmu->instance), &sciInputRefs[j], 1, &asCharPtr);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetString((structFmu->instance), &sciInputRefs[j], 1, &asCharPtr, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetString((structFmu->instance), &sciInputRefs[j], 1, &asCharPtr, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 0) // Binary
        {
            size_t len = (size_t) GetInPortSize(block, i+1, 2);
            fmi3Binary ptr = (fmi3Binary) from[i];

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }
            
            FMU_SIM_PRINT("fmiSet_values binary valueReference %d set to \"%x\"\n", sciInputRefs[j], ptr);

            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetBinary((structFmu->instance), &sciInputRefs[j], 1, &len, &ptr, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetBinary((structFmu->instance), &sciInputRefs[j], 1,  &len, &ptr, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
        else if (declaredTypes[i] == 800) // Enumeration
        {
            int value2;
            if (structFmu->version == workFMU::fmiV1 || structFmu->version == workFMU::fmiV2) assign_from(from[i], blockTypes[i], sciInputRefs[k], value2);
            int64_t value3;
            if (structFmu->version == workFMU::fmiV3) assign_from(from[i], blockTypes[i], sciInputRefs[k], value3);

            if ((sciInputRefs[k] & DISCRETE) == DISCRETE) // discrete variable
            {
                if (structFmu->type == workFMU::ModelExchange && continuousMode)
                {
                    // do not set in continuous mode
                    continue;
                }
            }

            // 0 is not a valid enumeration value, do not set at re-initialization where everything is set to 0 first
            if (flag == Initialization || flag == ReInitialization)
            {
                FMU_SIM_PRINT("fmiSet_values enumeration set to 0, ignored\n");
                continue;
            }
            
            if (structFmu->version == workFMU::fmiV2) FMU_SIM_PRINT("fmiSet_values enum valueReference %d set to %d\n", sciInputRefs[j], value2);
            if (structFmu->version == workFMU::fmiV3) FMU_SIM_PRINT("fmiSet_values enum valueReference %d set to %ld\n", sciInputRefs[j], value3);
            
            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiSetInteger((structFmu->instance), &sciInputRefs[j], 1, &value2);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiSetInteger((structFmu->instance), &sciInputRefs[j], 1, &value2);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2SetInteger((structFmu->instance), &sciInputRefs[j], 1, &value2);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2SetInteger((structFmu->instance), &sciInputRefs[j], 1, &value2);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3SetInt64((structFmu->instance), &sciInputRefs[j], 1, &value3, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3SetInt64((structFmu->instance), &sciInputRefs[j], 1, &value3, 1);
            }

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
        }
    }

    // report the highest possible error
    if (globalStatus != fmiOK)
    {
        return globalStatus;
    }
    if (globalStatus2 != fmi2OK)
    {
        return globalStatus2;
    }
    if (globalStatus3 != fmi3OK)
    {
        return globalStatus3;
    }
    return globalStatus3;
}

template<typename T> inline constexpr
void assign_into(T value, int type, uint32_t ref, void* into)
{
    if (type == SCSREAL_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSREAL_COP* negated = (SCSREAL_COP*) into;
            *negated = - (SCSREAL_COP) value;
        }
        else
        {
            SCSREAL_COP* dest = (SCSREAL_COP*) into;
            *dest = (SCSREAL_COP) value;
        }
    }
    else if (type == SCSCOMPLEX_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSCOMPLEX_COP* negated = (SCSCOMPLEX_COP*) into;
            *negated = - (SCSCOMPLEX_COP) value;
        }
        else
        {
            SCSCOMPLEX_COP* dest = (SCSCOMPLEX_COP*) into;
            *dest = (SCSCOMPLEX_COP) value;
        }
    }
    else if (type == SCSINT_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT_COP* negated = (SCSINT_COP*) into;
            *negated = - (SCSINT_COP) value;
        }
        else
        {
            SCSINT_COP* dest = (SCSINT_COP*) into;
            *dest = (SCSINT_COP) value;
        }
    }
    else if (type == SCSINT8_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT8_COP* negated = (SCSINT8_COP*) into;
            *negated = - (SCSINT8_COP) value;
        }
        else
        {
            SCSINT8_COP* dest = (SCSINT8_COP*) into;
            *dest = (SCSINT8_COP) value;
        }
    }
    else if (type == SCSINT16_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT16_COP* negated = (SCSINT16_COP*) into;
            *negated = - (SCSINT16_COP) value;
        }
        else
        {
            SCSINT16_COP* dest = (SCSINT16_COP*) into;
            *dest = (SCSINT16_COP) value;
        }
    }
    else if (type == SCSINT32_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSINT32_COP* negated = (SCSINT32_COP*) into;
            *negated = - (SCSINT32_COP) value;
        }
        else
        {
            SCSINT32_COP* dest = (SCSINT32_COP*) into;
            *dest = (SCSINT32_COP) value;
        }
    }
    else if (type == SCSUINT_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT_COP* negated = (SCSUINT_COP*) into;
            *negated = ! (SCSUINT_COP) value;
        }
        else
        {
            SCSUINT_COP* dest = (SCSUINT_COP*) into;
            *dest = (SCSUINT_COP) value;
        }
    }
    else if (type == SCSUINT8_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT8_COP* negated = (SCSUINT8_COP*) into;
            *negated = ! (SCSUINT8_COP) value;
        }
        else
        {
            SCSUINT8_COP* dest = (SCSUINT8_COP*) into;
            *dest = (SCSUINT8_COP) value;
        }
    }
    else if (type == SCSUINT16_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT16_COP* negated = (SCSUINT16_COP*) into;
            *negated = ! (SCSUINT16_COP) value;
        }
        else
        {
            SCSUINT16_COP* dest = (SCSUINT16_COP*) into;
            *dest = (SCSUINT16_COP) value;
        }
    }
    else if (type == SCSUINT32_N)
    {
        if ((ref & NEGATED_ALIAS) == NEGATED_ALIAS)
        {
            SCSUINT32_COP* negated = (SCSUINT32_COP*) into;
            *negated = ! (SCSUINT32_COP) value;
        }
        else
        {
            SCSUINT32_COP* dest = (SCSUINT32_COP*) into;
            *dest = (SCSUINT32_COP) value;
        }
    }
    else
    {
        // do nothing on unknown "into" type
    }
};

// get output Values
int fmiGet_values(scicos_block *block, int* declaredTypes, int* blockTypes, size_t outSize, uint32_t *sciOutputRefs, void** into)
{
    fmiStatus status = fmiOK;
    fmi2Status status2 = fmi2OK;
    fmi3Status status3 = fmi3OK;
    fmiStatus globalStatus = fmiOK;
    fmi2Status globalStatus2 = fmi2OK;
    fmi3Status globalStatus3 = fmi3OK;

    size_t i, j, k;
    workFMU *structFmu = *(workFMU **)(block->work);
    if (structFmu == nullptr)
    {
        return fmi2Fatal;
    }

    for (i = 0, k = outSize / 2, j = 0;
        k < outSize && globalStatus == fmiOK && globalStatus2 == fmi2OK && globalStatus3 == fmi3OK;
        i++, j++, k++)
    {
        if (declaredTypes[i] == 108) // Real
        {
            double value;
            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiGetReal((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiGetReal((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2GetReal((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2GetReal((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetFloat64((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetFloat64((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %lf\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 104) // Float32
        {
            float value;

            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetFloat32((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetFloat32((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %lf\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 808) // Int64
        {
            int64_t value;
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetInt64((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetInt64((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values int64 valueReference %d set to %" PRId64 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
            
            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 804) // Int32
        {
            int value;
            
            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiGetInteger((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiGetInteger((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2GetInteger((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2GetInteger((structFmu->instance), &sciOutputRefs[j], 1, &value);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetInt32((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetInt32((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values integer valueReference %d set to %d\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 802) // Int16
        {
            int16_t value;
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetInt16((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetInt16((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %" PRId16 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 801) // Int8
        {
            int8_t value;
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetInt8((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetInt8((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %" PRId8 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 818) // UInt64
        {
            uint64_t value;
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetUInt64((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetUInt64((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %" PRIu64 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }
            
            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 814) // UInt32
        {
            uint32_t value;
            
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetUInt32((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetUInt32((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values integer valueReference %d set to %" PRIu32 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 812) // UInt16
        {
            uint16_t value;

            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetUInt16((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetUInt16((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %" PRIu16 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 811) // UInt8
        {
            uint8_t value;

            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetUInt8((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetUInt8((structFmu->instance), &sciOutputRefs[j], 1, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values real valueReference %d set to %" PRIu8 "\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(value, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 4) // Boolean
        {
            fmiBoolean v1Bool = 0;
            fmi2Boolean v2Bool = 0;
            fmi3Boolean v3Bool = 0;
            int wideBool = 0;

            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiGetBoolean((structFmu->instance), &sciOutputRefs[j], 1, &v1Bool);
                wideBool = v1Bool;
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiGetBoolean((structFmu->instance), &sciOutputRefs[j], 1, &v1Bool);
                wideBool = v1Bool;
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2GetBoolean((structFmu->instance), &sciOutputRefs[j], 1, &v2Bool);
                wideBool = v2Bool;
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2GetBoolean((structFmu->instance), &sciOutputRefs[j], 1, &v2Bool);
                wideBool = v2Bool;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetBoolean((structFmu->instance), &sciOutputRefs[j], 1, &v3Bool, 1);
                wideBool = v3Bool;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetBoolean((structFmu->instance), &sciOutputRefs[j], 1, &v3Bool, 1);
                wideBool = v3Bool;
            }

            FMU_SIM_PRINT("fmiGet_values boolean valueReference %d set to %d\n", sciOutputRefs[j], wideBool);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            assign_into(wideBool, blockTypes[i], sciOutputRefs[k], into[i]);
        }
        else if (declaredTypes[i] == 10) // String
        {
            // \0 terminated string, utf-8 encoded
            char* str;

            int maxStringLen = GetOutPortSize(block, i+1, 2);
            if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
            {
                status = structFmu->fmuME1->fmiGetString((structFmu->instance), &sciOutputRefs[j], 1, &str);
            }
            else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
            {
                status = structFmu->fmuCS1->fmiGetString((structFmu->instance), &sciOutputRefs[j], 1, &str);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2GetString((structFmu->instance), &sciOutputRefs[j], 1, &str);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2GetString((structFmu->instance), &sciOutputRefs[j], 1, &str);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetString((structFmu->instance), &sciOutputRefs[j], 1, &str, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetString((structFmu->instance), &sciOutputRefs[j], 1, &str, 1);
            }

            FMU_SIM_PRINT("fmiGet_values string valueReference %d set to \"%s\"\n", sciOutputRefs[j], str);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            char* asCharPtr = (char *) into[i];
            std::strncpy(asCharPtr, str, maxStringLen);
            asCharPtr[maxStringLen - 1] = '\0'; // ensure string termination
        }
        else if (declaredTypes[i] == 0) // Binary
        {
            size_t len = (size_t) GetOutPortSize(block, i+1, 2);
            fmi3Binary value;

            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2)
            {
                status2 = fmi2Fatal;
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetBinary((structFmu->instance), &sciOutputRefs[j], 1, &len, &value, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetBinary((structFmu->instance), &sciOutputRefs[j], 1, &len, &value, 1);
            }

            FMU_SIM_PRINT("fmiGet_values binary valueReference %d set to \"%x\"\n", sciOutputRefs[j], value);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            // must be copied by the importer
            fmi3Binary asPtr = (fmi3Binary) into[i];
            memcpy(asPtr, value, len);
        }
        else if (declaredTypes[i] == 800) // Enumeration
        {
            int value2;
            int64_t value3;
            if (structFmu->version == workFMU::fmiV1)
            {
                status = fmiFatal;
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
            {
                status2 = structFmu->fmuME2->fmi2GetInteger((structFmu->instance), &sciOutputRefs[j], 1, &value2);
            }
            else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
            {
                status2 = structFmu->fmuCS2->fmi2GetInteger((structFmu->instance), &sciOutputRefs[j], 1, &value2);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
            {
                status3 = structFmu->fmuME3->fmi3GetInt64((structFmu->instance), &sciOutputRefs[j], 1, &value3, 1);
            }
            else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
            {
                status3 = structFmu->fmuCS3->fmi3GetInt64((structFmu->instance), &sciOutputRefs[j], 1, &value3, 1);
            }

            if (structFmu->version == workFMU::fmiV2) FMU_SIM_PRINT("fmiGet_values integer valueReference %d set to %d \n", sciOutputRefs[j], value2);
            if (structFmu->version == workFMU::fmiV3) FMU_SIM_PRINT("fmiGet_values integer valueReference %d set to %" PRId64 " \n", sciOutputRefs[j], value3);

            // store the highest possible error
            if (status > globalStatus)
            {
                globalStatus = status;
            }
            if (status2 > globalStatus2)
            {
                globalStatus2 = status2;
            }
            if (status3 > globalStatus3)
            {
                globalStatus3 = status3;
            }

            if (structFmu->version == workFMU::fmiV2) assign_into(value2, blockTypes[i], sciOutputRefs[k], into[i]);
            if (structFmu->version == workFMU::fmiV3) assign_into(value3, blockTypes[i], sciOutputRefs[k], into[i]);
        }
    }

    // report the highest possible error
    if (globalStatus != fmiOK)
    {
        return globalStatus;
    }
    if (globalStatus2 != fmi2OK)
    {
        return globalStatus2;
    }
    if (globalStatus3 != fmi3OK)
    {
        return globalStatus3;
    }
    return globalStatus3;
}
void free_instance(scicos_block *block, int errorNumber, const char *functionName)
{
    workFMU *structFmu = *(workFMU **)(block->work);
    if (errorNumber)
    {
        Coserror("Error in simulation, returned by %s\n", functionName);
        set_block_error(errorNumber);
        
        std::wstring err = L"Error in FMU, " + scilab::UTF8::toWide(functionName) + L" function returned " + std::to_wstring(errorNumber) + L"\n";
        setLastErrorMessage(err.c_str());
    }

    if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::ModelExchange)
    {
        delete structFmu->fmuME1;
    }
    else if (structFmu->version == workFMU::fmiV1 && structFmu->type == workFMU::CoSimulation)
    {
        delete structFmu->fmuCS1;
    }
    else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::ModelExchange)
    {
        delete structFmu->fmuME2;
    }
    else if (structFmu->version == workFMU::fmiV2 && structFmu->type == workFMU::CoSimulation)
    {
        delete structFmu->fmuCS2;
    }
    else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::ModelExchange)
    {
        delete structFmu->fmuME3;
    }
    else if (structFmu->version == workFMU::fmiV3 && structFmu->type == workFMU::CoSimulation)
    {
        delete structFmu->fmuCS3;
    }

    scicos_free(structFmu);
    *(block->work) = NULL;
}
