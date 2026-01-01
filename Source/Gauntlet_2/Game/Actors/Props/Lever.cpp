#include "Game/Actors/Props/Lever.h"

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SwitchableTargets = TArray<TScriptInterface<ISwitchable>>();

	//Create Root
	Root = CreateDefaultSubobject<USceneComponent>("root");
	RootComponent = Root;
	//Create Body
	LeverBody = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	LeverBody->SetupAttachment(Root);
	//Create Handle
	HandlePivot = CreateDefaultSubobject<USceneComponent>("Handle Pivot");
	HandlePivot->SetupAttachment(Root);

	LeverHandle = CreateDefaultSubobject<UStaticMeshComponent>("Handle");
	LeverHandle->SetupAttachment(HandlePivot);

	//Create Dynamic material if missing
	if (LeverBodyMaterialParent && !LeverDynamicMaterial)
	{
		LeverDynamicMaterial = UMaterialInstanceDynamic::Create(LeverBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

		//Assign material
		if (LeverBody)
			LeverBody->SetMaterial(0, LeverDynamicMaterial);

		//Assign Material Color
		LeverDynamicMaterial->SetVectorParameterValue("Base Color", OffColor);
	}
}

void ALever::BeginPlay()
{	
	if (!GetWorld())
		return;

	//Check all Lever Target and cache the one who implements the interface
	for (AActor* targetActor : LeverTargets)
	{
		if(targetActor == nullptr)
			continue;

		ISwitchable* switchable = Cast<ISwitchable>(targetActor);
		if(!switchable)
			continue;

		//Create TScript Interface of type ISwitchable
		TScriptInterface<ISwitchable> switchableTScriptInterface = TScriptInterface<ISwitchable>();
		switchableTScriptInterface.SetObject(targetActor);
		switchableTScriptInterface.SetInterface(switchable);

		//Add Unique Switchable
		SwitchableTargets.AddUnique(switchableTScriptInterface);
	}

	if (LeverBodyMaterialParent && LeverDynamicMaterial == nullptr)
	{
		LeverDynamicMaterial = UMaterialInstanceDynamic::Create(LeverBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));
		LeverDynamicMaterial->SetFlags(RF_Transient);
		LeverDynamicMaterial->SetVectorParameterValue("Base Color", OffColor);
		LeverBody->SetMaterial(0, LeverDynamicMaterial);
	}

	Super::BeginPlay();
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALever::SwitchAllTargets()
{
	if (SwitchableTargets.Num() <= 0)
		return;

	//Invoke Switch on all lever targets
	for (TScriptInterface<ISwitchable> targetTInterface : SwitchableTargets)
	{
		targetTInterface.GetInterface()->Switch(Cast<AActor>(targetTInterface.GetObject()));
	}
}

void ALever::InvertLeverState()
{
	bLeverState = !bLeverState;

	if (bLeverState)
	{
		LeverDynamicMaterial->SetVectorParameterValue("Base Color", OnColor);
		OnLeverOn();
	}
	else
	{
		LeverDynamicMaterial->SetVectorParameterValue("Base Color", OffColor);
		OnLeverOff();
	}
}


#pragma region IInteractable Interface implementation

bool ALever::NativeCanInteract(AActor* contextActor)
{
	//Call default implementation of CanInteract
	return IInteractable::Execute_BP_CanInteract(this);
};

void ALever::NativeInteract(AActor* contextActor)
{
	//check if actor exists
	if (contextActor == nullptr)
		return;
	
	InvertLeverState();

	//Call On Interact function BP side
	IInteractable::Execute_BP_OnInteract(this, contextActor);
}
#pragma endregion

#pragma region Editor Side Color Change
#if WITH_EDITOR 
void ALever::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == NULL)
		return;

	//Check if the property changed is the material
	FName propertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None; 

	if (propertyName == GET_MEMBER_NAME_CHECKED(ALever, LeverBodyMaterialParent))
	{
		//Create Dynamic Material Starting from parent
		if (LeverBodyMaterialParent)
		{
			LeverDynamicMaterial = UMaterialInstanceDynamic::Create(LeverBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			if(LeverBody)
				LeverBody->SetMaterial(0, LeverDynamicMaterial);
		}

		//Assign Color
		LeverDynamicMaterial->SetVectorParameterValue("Base Color", OffColor);

		return;
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(ALever, OffColor))
	{
		if (!LeverBodyMaterialParent)
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: Assign LeverBodyMaterialParent first!"));
			return;
		}

		//Check if the material was created -> create it
		if (!LeverDynamicMaterial)
		{
			LeverDynamicMaterial = UMaterialInstanceDynamic::Create(LeverBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			if (LeverBody)
				LeverBody->SetMaterial(0, LeverDynamicMaterial);
		}

		//Update Color
		LeverDynamicMaterial->SetVectorParameterValue("Base Color", OffColor);
	}
}

//Update Material In Scene
void ALever::OnConstruction(const FTransform& Transform) 
{
	Super::OnConstruction(Transform);

	//Create Dynamic Material if Missing
	if (LeverBodyMaterialParent && !LeverDynamicMaterial)
	{
		LeverDynamicMaterial = UMaterialInstanceDynamic::Create(LeverBodyMaterialParent, this);
		if (LeverBody)
			LeverBody->SetMaterial(0, LeverDynamicMaterial);
	}

	//Assign material color
	LeverDynamicMaterial->SetVectorParameterValue("Base Color", OffColor); 
}
#endif
#pragma endregion