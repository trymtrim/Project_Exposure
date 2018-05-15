// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPrimitiveComponent;
class AActor;
struct FHitResult;
#ifdef PROJECTEXPOSURE_PlaceableUnit_generated_h
#error "PlaceableUnit.generated.h already included, missing '#pragma once' in PlaceableUnit.h"
#endif
#define PROJECTEXPOSURE_PlaceableUnit_generated_h

#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execBeginOverlap) \
	{ \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OverlappedComponent); \
		P_GET_OBJECT(AActor,Z_Param_OtherActor); \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp); \
		P_GET_PROPERTY(UIntProperty,Z_Param_OtherBodyIndex); \
		P_GET_UBOOL(Z_Param_bFromSweep); \
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_SweepResult); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->BeginOverlap(Z_Param_OverlappedComponent,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_OtherBodyIndex,Z_Param_bFromSweep,Z_Param_Out_SweepResult); \
		P_NATIVE_END; \
	}


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execBeginOverlap) \
	{ \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OverlappedComponent); \
		P_GET_OBJECT(AActor,Z_Param_OtherActor); \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp); \
		P_GET_PROPERTY(UIntProperty,Z_Param_OtherBodyIndex); \
		P_GET_UBOOL(Z_Param_bFromSweep); \
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_SweepResult); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->BeginOverlap(Z_Param_OverlappedComponent,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_OtherBodyIndex,Z_Param_bFromSweep,Z_Param_Out_SweepResult); \
		P_NATIVE_END; \
	}


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAPlaceableUnit(); \
	friend PROJECTEXPOSURE_API class UClass* Z_Construct_UClass_APlaceableUnit(); \
public: \
	DECLARE_CLASS(APlaceableUnit, APawn, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/ProjectExposure"), NO_API) \
	DECLARE_SERIALIZER(APlaceableUnit) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_INCLASS \
private: \
	static void StaticRegisterNativesAPlaceableUnit(); \
	friend PROJECTEXPOSURE_API class UClass* Z_Construct_UClass_APlaceableUnit(); \
public: \
	DECLARE_CLASS(APlaceableUnit, APawn, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/ProjectExposure"), NO_API) \
	DECLARE_SERIALIZER(APlaceableUnit) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API APlaceableUnit(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(APlaceableUnit) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APlaceableUnit); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APlaceableUnit); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APlaceableUnit(APlaceableUnit&&); \
	NO_API APlaceableUnit(const APlaceableUnit&); \
public:


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APlaceableUnit(APlaceableUnit&&); \
	NO_API APlaceableUnit(const APlaceableUnit&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APlaceableUnit); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APlaceableUnit); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(APlaceableUnit)


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_PRIVATE_PROPERTY_OFFSET
#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_10_PROLOG
#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_PRIVATE_PROPERTY_OFFSET \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_RPC_WRAPPERS \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_INCLASS \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_PRIVATE_PROPERTY_OFFSET \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_INCLASS_NO_PURE_DECLS \
	Project_Exposure_Source_ProjectExposure_PlaceableUnit_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Project_Exposure_Source_ProjectExposure_PlaceableUnit_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
