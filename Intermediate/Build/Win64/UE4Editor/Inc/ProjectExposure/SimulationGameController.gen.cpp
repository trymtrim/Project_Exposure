// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "SimulationGameController.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSimulationGameController() {}
// Cross Module References
	PROJECTEXPOSURE_API UClass* Z_Construct_UClass_ASimulationGameController_NoRegister();
	PROJECTEXPOSURE_API UClass* Z_Construct_UClass_ASimulationGameController();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UPackage* Z_Construct_UPackage__Script_ProjectExposure();
	PROJECTEXPOSURE_API UFunction* Z_Construct_UFunction_ASimulationGameController_SpawnUnit();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
// End Cross Module References
	void ASimulationGameController::StaticRegisterNativesASimulationGameController()
	{
		UClass* Class = ASimulationGameController::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SpawnUnit", &ASimulationGameController::execSpawnUnit },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_ASimulationGameController_SpawnUnit()
	{
		struct SimulationGameController_eventSpawnUnit_Parms
		{
			int32 index;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_index = { UE4CodeGen_Private::EPropertyClass::Int, "index", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(SimulationGameController_eventSpawnUnit_Parms, index), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_index,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "ButtonPress" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_ASimulationGameController, "SpawnUnit", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(SimulationGameController_eventSpawnUnit_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_ASimulationGameController_NoRegister()
	{
		return ASimulationGameController::StaticClass();
	}
	UClass* Z_Construct_UClass_ASimulationGameController()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_APawn,
				(UObject* (*)())Z_Construct_UPackage__Script_ProjectExposure,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_ASimulationGameController_SpawnUnit, "SpawnUnit" }, // 3697092190
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Navigation" },
				{ "IncludePath", "SimulationGameController.h" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp__oilRigPrefab_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			static const UE4CodeGen_Private::FClassPropertyParams NewProp__oilRigPrefab = { UE4CodeGen_Private::EPropertyClass::Class, "_oilRigPrefab", RF_Public|RF_Transient|RF_MarkAsNative, 0x0044000000000001, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, _oilRigPrefab), Z_Construct_UClass_AActor_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(NewProp__oilRigPrefab_MetaData, ARRAY_COUNT(NewProp__oilRigPrefab_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp__windmillPrefab_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			static const UE4CodeGen_Private::FClassPropertyParams NewProp__windmillPrefab = { UE4CodeGen_Private::EPropertyClass::Class, "_windmillPrefab", RF_Public|RF_Transient|RF_MarkAsNative, 0x0044000000000001, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, _windmillPrefab), Z_Construct_UClass_AActor_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(NewProp__windmillPrefab_MetaData, ARRAY_COUNT(NewProp__windmillPrefab_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp__nuclearReactorPrefab_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			static const UE4CodeGen_Private::FClassPropertyParams NewProp__nuclearReactorPrefab = { UE4CodeGen_Private::EPropertyClass::Class, "_nuclearReactorPrefab", RF_Public|RF_Transient|RF_MarkAsNative, 0x0044000000000001, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, _nuclearReactorPrefab), Z_Construct_UClass_AActor_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(NewProp__nuclearReactorPrefab_MetaData, ARRAY_COUNT(NewProp__nuclearReactorPrefab_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp__oilRigPrefab,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp__windmillPrefab,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp__nuclearReactorPrefab,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<ASimulationGameController>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&ASimulationGameController::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00900080u,
				FuncInfo, ARRAY_COUNT(FuncInfo),
				PropPointers, ARRAY_COUNT(PropPointers),
				nullptr,
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ASimulationGameController, 1095116398);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASimulationGameController(Z_Construct_UClass_ASimulationGameController, &ASimulationGameController::StaticClass, TEXT("/Script/ProjectExposure"), TEXT("ASimulationGameController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASimulationGameController);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
