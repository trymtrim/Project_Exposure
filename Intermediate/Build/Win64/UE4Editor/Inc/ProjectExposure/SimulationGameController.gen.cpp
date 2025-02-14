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
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
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
				{ "ToolTip", "For blueprints" },
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
				{ &Z_Construct_UFunction_ASimulationGameController_SpawnUnit, "SpawnUnit" }, // 1783992087
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Navigation" },
				{ "IncludePath", "SimulationGameController.h" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_collideMaterial_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_collideMaterial = { UE4CodeGen_Private::EPropertyClass::Object, "collideMaterial", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000000001, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, collideMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(NewProp_collideMaterial_MetaData, ARRAY_COUNT(NewProp_collideMaterial_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_normalMaterial_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
				{ "ToolTip", "Materials" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_normalMaterial = { UE4CodeGen_Private::EPropertyClass::Object, "normalMaterial", RF_Public|RF_Transient|RF_MarkAsNative, 0x0040000000000001, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, normalMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(NewProp_normalMaterial_MetaData, ARRAY_COUNT(NewProp_normalMaterial_MetaData)) };
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
				{ "ToolTip", "Prefabs" },
			};
#endif
			static const UE4CodeGen_Private::FClassPropertyParams NewProp__nuclearReactorPrefab = { UE4CodeGen_Private::EPropertyClass::Class, "_nuclearReactorPrefab", RF_Public|RF_Transient|RF_MarkAsNative, 0x0044000000000001, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, _nuclearReactorPrefab), Z_Construct_UClass_AActor_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(NewProp__nuclearReactorPrefab_MetaData, ARRAY_COUNT(NewProp__nuclearReactorPrefab_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_currentTurnText_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			static const UE4CodeGen_Private::FStrPropertyParams NewProp_currentTurnText = { UE4CodeGen_Private::EPropertyClass::Str, "currentTurnText", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000004, 1, nullptr, STRUCT_OFFSET(ASimulationGameController, currentTurnText), METADATA_PARAMS(NewProp_currentTurnText_MetaData, ARRAY_COUNT(NewProp_currentTurnText_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_showSimulationUI_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			auto NewProp_showSimulationUI_SetBit = [](void* Obj){ ((ASimulationGameController*)Obj)->showSimulationUI = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_showSimulationUI = { UE4CodeGen_Private::EPropertyClass::Bool, "showSimulationUI", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000004, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(ASimulationGameController), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_showSimulationUI_SetBit)>::SetBit, METADATA_PARAMS(NewProp_showSimulationUI_MetaData, ARRAY_COUNT(NewProp_showSimulationUI_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_showUI_MetaData[] = {
				{ "Category", "SimulationGameController" },
				{ "ModuleRelativePath", "SimulationGameController.h" },
			};
#endif
			auto NewProp_showUI_SetBit = [](void* Obj){ ((ASimulationGameController*)Obj)->showUI = 1; };
			static const UE4CodeGen_Private::FBoolPropertyParams NewProp_showUI = { UE4CodeGen_Private::EPropertyClass::Bool, "showUI", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000004, 1, nullptr, sizeof(bool), UE4CodeGen_Private::ENativeBool::Native, sizeof(ASimulationGameController), &UE4CodeGen_Private::TBoolSetBitWrapper<decltype(NewProp_showUI_SetBit)>::SetBit, METADATA_PARAMS(NewProp_showUI_MetaData, ARRAY_COUNT(NewProp_showUI_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_collideMaterial,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_normalMaterial,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp__oilRigPrefab,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp__windmillPrefab,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp__nuclearReactorPrefab,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_currentTurnText,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_showSimulationUI,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_showUI,
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
	IMPLEMENT_CLASS(ASimulationGameController, 2930935241);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASimulationGameController(Z_Construct_UClass_ASimulationGameController, &ASimulationGameController::StaticClass, TEXT("/Script/ProjectExposure"), TEXT("ASimulationGameController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASimulationGameController);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
