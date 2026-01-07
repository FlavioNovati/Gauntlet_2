#include "Game/Actors/Props/Artefact.h"
#include "Game/Actors/Characters/GauntletCharacter.h"

// Sets default values
AArtefact::AArtefact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LeverBody = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = LeverBody;
}

// Called when the game starts or when spawned
void AArtefact::BeginPlay()
{
	Super::BeginPlay();

	//Create Dynamic material if missing
	if (ArtefactBodyMaterialParent && !ArtefactDynamicMaterial)
	{
		ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

		//Assign material
		if (LeverBody)
			LeverBody->SetMaterial(0, ArtefactDynamicMaterial);

		//Assign Material Color
		ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", RelasedColor);
	}

	StartingTransform = GetActorTransform();
}

// Called every frame
void AArtefact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AArtefact::NativeCanInteract(AActor* contextActor)
{
	return !bIsPickedUp;
}

void AArtefact::NativeInteract(AActor* contextActor)
{
	IPickableTarget* pickableTarget = Cast<IPickableTarget>(contextActor);
	
	if(pickableTarget != nullptr)
	{
		pickableTarget->PickupPickable(this);
		Pickup(nullptr);
	}
}

void AArtefact::Pickup(TScriptInterface<IPickableTarget> PickableTarget)
{
	bIsPickedUp = true;
	ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", PickedColor);
	SetActorEnableCollision(false);
	LeverBody->SetCollisionProfileName(TEXT("NoCollision"));
	//TODO: EVENT
}

void AArtefact::RelasePickable(TScriptInterface<IPickableTarget> PickableTarget)
{
	SetActorEnableCollision(true);
	LeverBody->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	PickableTarget.GetInterface()->PickupPickable(this);
	ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", RelasedColor);
	bIsPickedUp = false;
}

void AArtefact::ResetPickable()
{
	SetActorEnableCollision(true);
	LeverBody->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", RelasedColor);

	SetActorTransform(StartingTransform);
	bIsPickedUp = false;

	this->Reset();
}

#pragma region Editor Side Color Change
#if WITH_EDITOR 
void AArtefact::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == NULL)
		return;

	//Check if the property changed is the material
	FName propertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(AArtefact, ArtefactBodyMaterialParent))
	{
		//Create Dynamic Material Starting from parent
		if (ArtefactBodyMaterialParent)
		{
			ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			if (LeverBody)
				LeverBody->SetMaterial(0, ArtefactDynamicMaterial);
		}

		//Assign Color
		ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", RelasedColor);

		return;
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(AArtefact, RelasedColor))
	{
		if (!ArtefactBodyMaterialParent)
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: Assign LeverBodyMaterialParent first!"));
			return;
		}

		//Check if the material was created -> create it
		if (!ArtefactDynamicMaterial)
		{
			ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			if (LeverBody)
				LeverBody->SetMaterial(0, ArtefactDynamicMaterial);
		}

		//Update Color
		ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", RelasedColor);
	}
}

//Update Material In Scene
void AArtefact::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Create Dynamic Material if Missing
	if (ArtefactBodyMaterialParent && !ArtefactDynamicMaterial)
	{
		ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this);
		if (LeverBody)
			LeverBody->SetMaterial(0, ArtefactDynamicMaterial);
	}

	//Assign material color
	ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", RelasedColor);
}
#endif
#pragma endregion
