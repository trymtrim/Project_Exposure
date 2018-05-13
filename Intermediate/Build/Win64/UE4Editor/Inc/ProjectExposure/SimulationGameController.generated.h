// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef PROJECTEXPOSURE_SimulationGameController_generated_h
#error "SimulationGameController.generated.h already included, missing '#pragma once' in SimulationGameController.h"
#endif
#define PROJECTEXPOSURE_SimulationGameController_generated_h

#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSpawnUnit) \
	{ \
		P_GET_PROPERTY(UIntProperty,Z_Param_index); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->SpawnUnit(Z_Param_index); \
		P_NATIVE_END; \
	}


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSpawnUnit) \
	{ \
		P_GET_PROPERTY(UIntProperty,Z_Param_index); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->SpawnUnit(Z_Param_index); \
		P_NATIVE_END; \
	}


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesASimulationGameController(); \
	friend PROJECTEXPOSURE_API class UClass* Z_Construct_UClass_ASimulationGameController(); \
public: \
	DECLARE_CLASS(ASimulationGameController, APawn, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/ProjectExposure"), NO_API) \
	DECLARE_SERIALIZER(ASimulationGameController) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_INCLASS \
private: \
	static void StaticRegisterNativesASimulationGameController(); \
	friend PROJECTEXPOSURE_API class UClass* Z_Construct_UClass_ASimulationGameController(); \
public: \
	DECLARE_CLASS(ASimulationGameController, APawn, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/ProjectExposure"), NO_API) \
	DECLARE_SERIALIZER(ASimulationGameController) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ASimulationGameController(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ASimulationGameController) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASimulationGameController); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASimulationGameController); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASimulationGameController(ASimulationGameController&&); \
	NO_API ASimulationGameController(const ASimulationGameController&); \
public:


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASimulationGameController(ASimulationGameController&&); \
	NO_API ASimulationGameController(const ASimulationGameController&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASimulationGameController); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASimulationGameController); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ASimulationGameController)


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO___nuclearReactorPrefab() { return STRUCT_OFFSET(ASimulationGameController, _nuclearReactorPrefab); } \
	FORCEINLINE static uint32 __PPO___windmillPrefab() { return STRUCT_OFFSET(ASimulationGameController, _windmillPrefab); } \
	FORCEINLINE static uint32 __PPO___oilRigPrefab() { return STRUCT_OFFSET(ASimulationGameController, _oilRigPrefab); }


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_10_PROLOG
#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_PRIVATE_PROPERTY_OFFSET \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_RPC_WRAPPERS \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_INCLASS \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_PRIVATE_PROPERTY_OFFSET \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_INCLASS_NO_PURE_DECLS \
	ProjectExposure_Source_ProjectExposure_SimulationGameController_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID ProjectExposure_Source_ProjectExposure_SimulationGameController_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
