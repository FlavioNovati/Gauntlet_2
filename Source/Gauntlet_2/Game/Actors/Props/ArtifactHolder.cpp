#include "Game/Actors/Props/ArtifactHolder.h"
#include "Game/Actors/Characters/GauntletCharacter.h"
#include "Game/Actors/Props/Artefact.h"

// Sets default values
AArtifactHolder::AArtifactHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Body
	ArtefactHoldeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = ArtefactHoldeMesh;
	
	ArtefactHolder = CreateDefaultSubobject<USceneComponent>("root");
	ArtefactHolder->SetupAttachment(ArtefactHoldeMesh);
	
}

// Called when the game starts or when spawned
void AArtifactHolder::BeginPlay()
{
	Super::BeginPlay();
	
	//Create Dynamic Material Starting from parent
	if(ArtefactBodyMaterialParent)
	{
		ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

		//Assign material
		if (ArtefactHoldeMesh)
			ArtefactHoldeMesh->SetMaterial(0, ArtefactDynamicMaterial);

		ArtefactDynamicMaterial->SetFlags(RF_Transient);

		//Assign Color
		ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", EmptyColor);
	}
}

// Called every frame
void AArtifactHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AArtifactHolder::NativeCanInteract(AActor* contextActor)
{
	return Artifact == nullptr;
}

void AArtifactHolder::NativeInteract(AActor* contextActor)
{
	AGauntletCharacter* player = Cast<AGauntletCharacter>(contextActor);
	if (player)
	{
		AActor* pickedActor;
		pickedActor = player->GetPickedActor();
		
		if (pickedActor)
		{
			AArtefact* artefact = Cast<AArtefact>(pickedActor);

			if (artefact == nullptr)
				return;

			Artifact = artefact;
			Artifact->AttachToComponent(ArtefactHolder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			//Assign Color
			ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", FullColor);


			//TODO: EXECUTE VICTORY
		}
	}
}

void AArtifactHolder::PickupPickable(AActor* actorToPlace)
{

}

FTransform AArtifactHolder::GetPickableTargetTransform()
{
	return FTransform();
}

#pragma region Editor Side Color Change
#if WITH_EDITOR 
void AArtifactHolder::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == NULL)
		return;

	//Check if the property changed is the material
	FName propertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(AArtifactHolder, ArtefactBodyMaterialParent))
	{
		//Create Dynamic Material Starting from parent
		if (ArtefactBodyMaterialParent)
		{
			ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			if (ArtefactHoldeMesh)
				ArtefactHoldeMesh->SetMaterial(0, ArtefactDynamicMaterial);
		}

		//Assign Color
		ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", EmptyColor);

		return;
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(AArtifactHolder, EmptyColor))
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
			if (ArtefactHoldeMesh)
				ArtefactHoldeMesh->SetMaterial(0, ArtefactDynamicMaterial);
		}

		//Update Color
		ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", EmptyColor);
	}
}

//Update Material In Scene
void AArtifactHolder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Create Dynamic Material if Missing
	if (ArtefactBodyMaterialParent && !ArtefactDynamicMaterial)
	{
		ArtefactDynamicMaterial = UMaterialInstanceDynamic::Create(ArtefactBodyMaterialParent, this);
		if (ArtefactHoldeMesh)
			ArtefactHoldeMesh->SetMaterial(0, ArtefactDynamicMaterial);
	}

	//Assign material color
	ArtefactDynamicMaterial->SetVectorParameterValue("Base Color", EmptyColor);
}
#endif
#pragma endregion