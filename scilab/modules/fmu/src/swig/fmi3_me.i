/* Inquire version numbers and setting logging status */
/* tag::GetVersion[] */
const char *fmi3GetVersion(void);
/* end::GetVersion[] */

/* tag::SetDebugLogging[] */
fmi3Status fmi3SetDebugLogging(fmi3Instance instance,
                               fmi3Boolean loggingOn,
                               size_t nCategories,
                               const fmi3String categories[]);
/* end::SetDebugLogging[] */

/* Creation and destruction of FMU instances and setting debug status */
/* tag::Instantiate[] */
fmi3Instance fmi3InstantiateModelExchange(
    fmi3String instanceName,
    fmi3String instantiationToken,
    fmi3String resourcePath,
    fmi3Boolean visible,
    fmi3Boolean loggingOn,
    fmi3InstanceEnvironment instanceEnvironment,
    fmi3LogMessageCallback logMessage);

/* tag::FreeInstance[] */
void fmi3FreeInstance(fmi3Instance instance);
/* end::FreeInstance[] */

/* Enter and exit initialization mode, enter event mode, terminate and reset */
/* tag::EnterInitializationMode[] */
fmi3Status fmi3EnterInitializationMode(fmi3Instance instance,
                                       fmi3Boolean toleranceDefined,
                                       fmi3Float64 tolerance,
                                       fmi3Float64 startTime,
                                       fmi3Boolean stopTimeDefined,
                                       fmi3Float64 stopTime);
/* end::EnterInitializationMode[] */

/* tag::ExitInitializationMode[] */
fmi3Status fmi3ExitInitializationMode(fmi3Instance instance);
/* end::ExitInitializationMode[] */

/* tag::EnterEventMode[] */
fmi3Status fmi3EnterEventMode(fmi3Instance instance);
/* end::EnterEventMode[] */

/* tag::Terminate[] */
fmi3Status fmi3Terminate(fmi3Instance instance);
/* end::Terminate[] */

/* tag::Reset[] */
fmi3Status fmi3Reset(fmi3Instance instance);
/* end::Reset[] */

/* Getting and setting variable values */
/* tag::Getters[] */
fmi3Status fmi3GetFloat32(fmi3Instance instance,
                          const fmi3ValueReference valueReferences[],
                          size_t nValueReferences,
                          fmi3Float32 values[],
                          size_t nValues);

fmi3Status fmi3GetFloat64(fmi3Instance instance,
                          const fmi3ValueReference valueReferences[],
                          size_t nValueReferences,
                          fmi3Float64 values[],
                          size_t nValues);

fmi3Status fmi3GetInt8(fmi3Instance instance,
                       const fmi3ValueReference valueReferences[],
                       size_t nValueReferences,
                       fmi3Int8 values[],
                       size_t nValues);

fmi3Status fmi3GetUInt8(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        fmi3UInt8 values[],
                        size_t nValues);

fmi3Status fmi3GetInt16(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        fmi3Int16 values[],
                        size_t nValues);

fmi3Status fmi3GetUInt16(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         fmi3UInt16 values[],
                         size_t nValues);

fmi3Status fmi3GetInt32(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        fmi3Int32 values[],
                        size_t nValues);

fmi3Status fmi3GetUInt32(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         fmi3UInt32 values[],
                         size_t nValues);

fmi3Status fmi3GetInt64(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        fmi3Int64 values[],
                        size_t nValues);

fmi3Status fmi3GetUInt64(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         fmi3UInt64 values[],
                         size_t nValues);

fmi3Status fmi3GetBoolean(fmi3Instance instance,
                          const fmi3ValueReference valueReferences[],
                          size_t nValueReferences,
                          fmi3Boolean values[],
                          size_t nValues);

fmi3Status fmi3GetString(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         fmi3String values[],
                         size_t nValues);

fmi3Status fmi3GetBinary(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         size_t valueSizes[],
                         fmi3Binary values[],
                         size_t nValues);
/* end::Getters[] */

/* tag::GetClock[] */
fmi3Status fmi3GetClock(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        fmi3Clock values[]);
/* end::GetClock[] */

/* tag::Setters[] */
fmi3Status fmi3SetFloat32(fmi3Instance instance,
                          const fmi3ValueReference valueReferences[],
                          size_t nValueReferences,
                          const fmi3Float32 values[],
                          size_t nValues);

fmi3Status fmi3SetFloat64(fmi3Instance instance,
                          const fmi3ValueReference valueReferences[],
                          size_t nValueReferences,
                          const fmi3Float64 values[],
                          size_t nValues);

fmi3Status fmi3SetInt8(fmi3Instance instance,
                       const fmi3ValueReference valueReferences[],
                       size_t nValueReferences,
                       const fmi3Int8 values[],
                       size_t nValues);

fmi3Status fmi3SetUInt8(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        const fmi3UInt8 values[],
                        size_t nValues);

fmi3Status fmi3SetInt16(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        const fmi3Int16 values[],
                        size_t nValues);

fmi3Status fmi3SetUInt16(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         const fmi3UInt16 values[],
                         size_t nValues);

fmi3Status fmi3SetInt32(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        const fmi3Int32 values[],
                        size_t nValues);

fmi3Status fmi3SetUInt32(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         const fmi3UInt32 values[],
                         size_t nValues);

fmi3Status fmi3SetInt64(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        const fmi3Int64 values[],
                        size_t nValues);

fmi3Status fmi3SetUInt64(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         const fmi3UInt64 values[],
                         size_t nValues);

fmi3Status fmi3SetBoolean(fmi3Instance instance,
                          const fmi3ValueReference valueReferences[],
                          size_t nValueReferences,
                          const fmi3Boolean values[],
                          size_t nValues);

fmi3Status fmi3SetString(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         const fmi3String values[],
                         size_t nValues);

fmi3Status fmi3SetBinary(fmi3Instance instance,
                         const fmi3ValueReference valueReferences[],
                         size_t nValueReferences,
                         const size_t valueSizes[],
                         const fmi3Binary values[],
                         size_t nValues);
/* end::Setters[] */
/* tag::SetClock[] */
fmi3Status fmi3SetClock(fmi3Instance instance,
                        const fmi3ValueReference valueReferences[],
                        size_t nValueReferences,
                        const fmi3Clock values[]);
/* end::SetClock[] */

/* Getting Variable Dependency Information */
/* tag::GetNumberOfVariableDependencies[] */
fmi3Status fmi3GetNumberOfVariableDependencies(fmi3Instance instance,
                                               fmi3ValueReference valueReference,
                                               size_t *nDependencies);
/* end::GetNumberOfVariableDependencies[] */

/* tag::GetVariableDependencies[] */
fmi3Status fmi3GetVariableDependencies(fmi3Instance instance,
                                       fmi3ValueReference dependent,
                                       size_t elementIndicesOfDependent[],
                                       fmi3ValueReference independents[],
                                       size_t elementIndicesOfIndependents[],
                                       fmi3DependencyKind dependencyKinds[],
                                       size_t nDependencies);
/* end::GetVariableDependencies[] */

/* Getting and setting the internal FMU state */
/* tag::GetFMUState[] */
fmi3Status fmi3GetFMUState(fmi3Instance instance, fmi3FMUState *FMUState);
/* end::GetFMUState[] */

/* tag::SetFMUState[] */
fmi3Status fmi3SetFMUState(fmi3Instance instance, fmi3FMUState FMUState);
/* end::SetFMUState[] */

/* tag::FreeFMUState[] */
fmi3Status fmi3FreeFMUState(fmi3Instance instance, fmi3FMUState *FMUState);
/* end::FreeFMUState[] */

/* tag::SerializedFMUStateSize[] */
fmi3Status fmi3SerializedFMUStateSize(fmi3Instance instance,
                                      fmi3FMUState FMUState,
                                      size_t *size);
/* end::SerializedFMUStateSize[] */

/* tag::SerializeFMUState[] */
fmi3Status fmi3SerializeFMUState(fmi3Instance instance,
                                 fmi3FMUState FMUState,
                                 fmi3Byte serializedState[],
                                 size_t size);
/* end::SerializeFMUState[] */

/* tag::DeserializeFMUState[] */
fmi3Status fmi3DeserializeFMUState(fmi3Instance instance,
                                   const fmi3Byte serializedState[],
                                   size_t size,
                                   fmi3FMUState *FMUState);
/* end::DeserializeFMUState[] */

/* Getting partial derivatives */
/* tag::GetDirectionalDerivative[] */
fmi3Status fmi3GetDirectionalDerivative(fmi3Instance instance,
                                        const fmi3ValueReference unknowns[],
                                        size_t nUnknowns,
                                        const fmi3ValueReference knowns[],
                                        size_t nKnowns,
                                        const fmi3Float64 seed[],
                                        size_t nSeed,
                                        fmi3Float64 sensitivity[],
                                        size_t nSensitivity);
/* end::GetDirectionalDerivative[] */

/* tag::GetAdjointDerivative[] */
fmi3Status fmi3GetAdjointDerivative(fmi3Instance instance,
                                    const fmi3ValueReference unknowns[],
                                    size_t nUnknowns,
                                    const fmi3ValueReference knowns[],
                                    size_t nKnowns,
                                    const fmi3Float64 seed[],
                                    size_t nSeed,
                                    fmi3Float64 sensitivity[],
                                    size_t nSensitivity);
/* end::GetAdjointDerivative[] */

/* Entering and exiting the Configuration or Reconfiguration Mode */

/* tag::EnterConfigurationMode[] */
fmi3Status fmi3EnterConfigurationMode(fmi3Instance instance);
/* end::EnterConfigurationMode[] */

/* tag::ExitConfigurationMode[] */
fmi3Status fmi3ExitConfigurationMode(fmi3Instance instance);
/* end::ExitConfigurationMode[] */

/* tag::GetIntervalDecimal[] */
fmi3Status fmi3GetIntervalDecimal(fmi3Instance instance,
                                  const fmi3ValueReference valueReferences[],
                                  size_t nValueReferences,
                                  fmi3Float64 intervals[],
                                  fmi3IntervalQualifier qualifiers[]);
/* end::GetIntervalDecimal[] */

/* tag::GetIntervalFraction[] */
fmi3Status fmi3GetIntervalFraction(fmi3Instance instance,
                                   const fmi3ValueReference valueReferences[],
                                   size_t nValueReferences,
                                   fmi3UInt64 counters[],
                                   fmi3UInt64 resolutions[],
                                   fmi3IntervalQualifier qualifiers[]);
/* end::GetIntervalFraction[] */

/* tag::GetShiftDecimal[] */
fmi3Status fmi3GetShiftDecimal(fmi3Instance instance,
                               const fmi3ValueReference valueReferences[],
                               size_t nValueReferences,
                               fmi3Float64 shifts[]);
/* end::GetShiftDecimal[] */

/* tag::GetShiftFraction[] */
fmi3Status fmi3GetShiftFraction(fmi3Instance instance,
                                const fmi3ValueReference valueReferences[],
                                size_t nValueReferences,
                                fmi3UInt64 counters[],
                                fmi3UInt64 resolutions[]);
/* end::GetShiftFraction[] */

/* tag::SetIntervalDecimal[] */
fmi3Status fmi3SetIntervalDecimal(fmi3Instance instance,
                                  const fmi3ValueReference valueReferences[],
                                  size_t nValueReferences,
                                  const fmi3Float64 intervals[]);
/* end::SetIntervalDecimal[] */

/* tag::SetIntervalFraction[] */
fmi3Status fmi3SetIntervalFraction(fmi3Instance instance,
                                   const fmi3ValueReference valueReferences[],
                                   size_t nValueReferences,
                                   const fmi3UInt64 counters[],
                                   const fmi3UInt64 resolutions[]);
/* end::SetIntervalFraction[] */

/* tag::SetShiftDecimal[] */
fmi3Status fmi3SetShiftDecimal(fmi3Instance instance,
                               const fmi3ValueReference valueReferences[],
                               size_t nValueReferences,
                               const fmi3Float64 shifts[]);
/* end::SetShiftDecimal[] */

/* tag::SetShiftFraction[] */
fmi3Status fmi3SetShiftFraction(fmi3Instance instance,
                                const fmi3ValueReference valueReferences[],
                                size_t nValueReferences,
                                const fmi3UInt64 counters[],
                                const fmi3UInt64 resolutions[]);
/* end::SetShiftFraction[] */

/* tag::EvaluateDiscreteStates[] */
fmi3Status fmi3EvaluateDiscreteStates(fmi3Instance instance);
/* end::EvaluateDiscreteStates[] */

/* tag::UpdateDiscreteStates[] */
fmi3Status fmi3UpdateDiscreteStates(fmi3Instance instance,
                                    fmi3Boolean *discreteStatesNeedUpdate,
                                    fmi3Boolean *terminateSimulation,
                                    fmi3Boolean *nominalsOfContinuousStatesChanged,
                                    fmi3Boolean *valuesOfContinuousStatesChanged,
                                    fmi3Boolean *nextEventTimeDefined,
                                    fmi3Float64 *nextEventTime);
/* end::UpdateDiscreteStates[] */

/***************************************************
Types for Functions for Model Exchange
****************************************************/

/* tag::EnterContinuousTimeMode[] */
fmi3Status fmi3EnterContinuousTimeMode(fmi3Instance instance);
/* end::EnterContinuousTimeMode[] */

/* tag::CompletedIntegratorStep[] */
fmi3Status fmi3CompletedIntegratorStep(fmi3Instance instance,
                                       fmi3Boolean noSetFMUStatePriorToCurrentPoint,
                                       fmi3Boolean *enterEventMode,
                                       fmi3Boolean *terminateSimulation);
/* end::CompletedIntegratorStep[] */

/* Providing independent variables and re-initialization of caching */
/* tag::SetTime[] */
fmi3Status fmi3SetTime(fmi3Instance instance, fmi3Float64 time);
/* end::SetTime[] */

/* tag::SetContinuousStates[] */
fmi3Status fmi3SetContinuousStates(fmi3Instance instance,
                                   const fmi3Float64 continuousStates[],
                                   size_t nContinuousStates);
/* end::SetContinuousStates[] */

/* Evaluation of the model equations */
/* tag::GetDerivatives[] */
fmi3Status fmi3GetContinuousStateDerivatives(fmi3Instance instance,
                                             fmi3Float64 derivatives[],
                                             size_t nContinuousStates);
/* end::GetDerivatives[] */

/* tag::GetEventIndicators[] */
fmi3Status fmi3GetEventIndicators(fmi3Instance instance,
                                  fmi3Float64 eventIndicators[],
                                  size_t nEventIndicators);
/* end::GetEventIndicators[] */

/* tag::GetContinuousStates[] */
fmi3Status fmi3GetContinuousStates(fmi3Instance instance,
                                   fmi3Float64 continuousStates[],
                                   size_t nContinuousStates);
/* end::GetContinuousStates[] */

/* tag::GetNominalsOfContinuousStates[] */
fmi3Status fmi3GetNominalsOfContinuousStates(fmi3Instance instance,
                                             fmi3Float64 nominals[],
                                             size_t nContinuousStates);
/* end::GetNominalsOfContinuousStates[] */

/* tag::GetNumberOfEventIndicators[] */
fmi3Status fmi3GetNumberOfEventIndicators(fmi3Instance instance,
                                          size_t *nEventIndicators);
/* end::GetNumberOfEventIndicators[] */

/* tag::GetNumberOfContinuousStates[] */
fmi3Status fmi3GetNumberOfContinuousStates(fmi3Instance instance,
                                           size_t *nContinuousStates);
/* end::GetNumberOfContinuousStates[] */
