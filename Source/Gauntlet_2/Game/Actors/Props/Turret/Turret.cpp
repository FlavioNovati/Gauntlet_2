#include "Game/Actors/Props/Turret/Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/Subsystems/PoolerSubSystem/ObjectPoolerSubsystem.h"
#include "Game/Subsystems/PoolerSubSystem/PoolEnablingData.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("root");
	RootComponent = Root;

	PivotYaw = CreateDefaultSubobject<USceneComponent>("Yaw Pivot");
	PivotYaw->SetupAttachment(Root);

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("Base Body");
	BaseBody->SetupAttachment(PivotYaw);

	PivotPitch = CreateDefaultSubobject<USceneComponent>("Pitch Pivot");
	PivotPitch->SetupAttachment(PivotYaw);

	DefaultGun = CreateDefaultSubobject<UStaticMeshComponent>("Gun Body");
	DefaultGun->SetupAttachment(PivotPitch);

	ShootingPoint = CreateDefaultSubobject<USceneComponent>("Shooting Point");
	ShootingPoint->SetupAttachment(DefaultGun);

	//Create Dynamic material if missing
	if (TurretBodyMaterialParent && !TurretDynamicMaterial)
	{
		TurretDynamicMaterial = UMaterialInstanceDynamic::Create(TurretBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

		//Assign material
		AssignTurretDynamicMaterial(TurretDynamicMaterial);

		//Set Turret Color
		SetTurretColor(ActiveColor);
	}
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	//Create Dynamic material if missing
	if (TurretBodyMaterialParent && !TurretDynamicMaterial)
	{
		TurretDynamicMaterial = UMaterialInstanceDynamic::Create(TurretBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));
		TurretDynamicMaterial->SetFlags(RF_Transient);

		//Assign material
		AssignTurretDynamicMaterial(TurretDynamicMaterial);

		//Set Turret Color
		SetTurretColor(ActiveColor);
	}

	ObjectPoolerSusbsystem = GetWorld()->GetSubsystem<UObjectPoolerSubsystem>();
	ObjectPoolerSusbsystem->CreateNewPool(ProjectileToShoot, ProjectileInitialPoolSize);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsEnabled)
		return;

	DrawDebugSphere(GetWorld(), GetActorLocation() + OverlapOffset, TurretDetectingRadious, 15, TurretOverlapColor, false);

	//No target -> Search For One
	if(TargetActor == nullptr)
	{
		CheckForTarget();
		return;
	}

	if (GetDistanceToTarget() > TurretDetectingRadious)
	{
		SetTarget(nullptr);
		return;
	}

	ShootAtTarget(DeltaTime);
}

#pragma region Check For Target "State"

void ATurret::CheckForTarget()
{
	TArray<FHitResult> outHits;
	FVector sweepLocation = GetActorLocation();
	sweepLocation += OverlapOffset;

	FCollisionShape interactionShape = FCollisionShape::MakeSphere(TurretDetectingRadious);

	//Set Hit variable for later check
	bool bIsHit = GetWorld()->SweepMultiByChannel(outHits, sweepLocation, sweepLocation, FQuat::Identity, ECC_WorldStatic, interactionShape);

	//Check if something got hit
	if (bIsHit)
	{
		//-> Cycle throught all hittet Objects
		for (FHitResult hit : outHits)
		{
			//Check Existance
			if (hit.GetActor() == nullptr)
				continue;

			if (hit.GetActor() == this)
				continue;

			TScriptInterface actorTInterface = TScriptInterface<ITargetable>(hit.GetActor());
			if (actorTInterface.GetObject())
			{
				SetTarget(TScriptInterface<ITargetable>(hit.GetActor()));
				return;
			}
		}
	}

	OnLookingForTarget();
}

void ATurret::SetTarget(TScriptInterface<ITargetable> target)
{
	if (target == nullptr)
	{
		Target = nullptr;
		TargetActor = nullptr;
		//UE_LOG(LogTemp, Warning, TEXT("Turret target Lost!"));
		return;
	}

	Target = target;
	TargetActor = Cast<AActor>(target.GetObject());
	OnTargetRecieved(TargetActor);
}

#pragma endregion

#pragma region Shoot at Target "State"

float ATurret::GetDistanceToTarget()
{
	if(!Target.GetObject())
		return std::numeric_limits<float>::max();

	return FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation());
}

void ATurret::ShootAtTarget(float deltaTime)
{
	FVector targetPosition = TargetActor->GetActorLocation();

	LookAtPosition(deltaTime, targetPosition);

	LastShotTime += deltaTime;

	//Check if can shoot
	if (LastShotTime >= ShootDelay)
	{
		//Reset Timer
		LastShotTime = .0f;
		//Shoot
		Shoot(deltaTime);
	}
}

void ATurret::LookAtPosition(float deltaTime, FVector targetPosition)
{
	FVector yawLocation = PivotYaw->GetComponentLocation();

	FRotator lookAtYaw = UKismetMathLibrary::FindLookAtRotation(
		yawLocation,
		targetPosition
	);

	// Only yaw is needed
	FRotator currentYaw = PivotYaw->GetComponentRotation();
	FRotator desiredYaw(0.f, lookAtYaw.Yaw, 0.f);

	FRotator newYaw = FMath::RInterpTo(
		currentYaw,
		desiredYaw,
		deltaTime,
		YawTurningSpeed
	);

	PivotYaw->SetWorldRotation(newYaw);
	
	FVector pitchLocation = PivotPitch->GetComponentLocation();

	// Recalculate after yaw has moved
	FRotator lookAtPitch = UKismetMathLibrary::FindLookAtRotation(
		pitchLocation,
		targetPosition
	);

	// Only pitch is needed
	FRotator currentPitch = PivotPitch->GetRelativeRotation();
	FRotator desiredPitch = currentPitch;
	desiredPitch.Pitch = lookAtPitch.Pitch;

	FRotator newPitch = FMath::RInterpTo(
		currentPitch,
		desiredPitch,
		deltaTime,
		PitchTurningSpeed
	);

	PivotPitch->SetRelativeRotation(newPitch);

	DrawDebugLine(GetWorld(), PivotYaw->GetComponentLocation(), targetPosition, FColor::Red, false, 0, 0, 2);
	DrawDebugLine(GetWorld(), PivotPitch->GetComponentLocation(), targetPosition, FColor::Green, false, 0, 0, 2);
}

void ATurret::Shoot(float deltaTime)
{
	FPoolEnablingData enablingData = FPoolEnablingData();
	enablingData.ObjectPoolTransform = ShootingPoint->GetComponentTransform();

	IPoollable* projectile = ObjectPoolerSusbsystem->GetObjectFromPool(ProjectileToShoot).GetInterface();
	projectile->Enable(enablingData);
}

#pragma endregion

void ATurret::EnableTurret()
{
	SetTurretColor(ActiveColor);
	bIsEnabled = true;

	ISwitchable::Execute_BP_OnSwitchON(this);
}

void ATurret::DisableTurret()
{
	SetTurretColor(DisabledColor);
	bIsEnabled = false;

	ISwitchable::Execute_BP_OnSwitchOFF(this);
}

#pragma region Material Change

void ATurret::AssignTurretDynamicMaterial(UMaterialInstanceDynamic* materialInstance)
{
	if (!materialInstance)
		return;

	BaseBody->SetMaterial(0, materialInstance);
}

#pragma endregion


#pragma region Editor Side Color Change
#if WITH_EDITOR 
void ATurret::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == NULL)
		return;

	//Check if the property changed is the material
	FName propertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(ATurret, TurretBodyMaterialParent))
	{
		//Create Dynamic Material Starting from parent
		if (TurretBodyMaterialParent)
		{
			TurretDynamicMaterial = UMaterialInstanceDynamic::Create(TurretBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			AssignTurretDynamicMaterial(TurretDynamicMaterial);
		}

		//Set Turret Color
		SetTurretColor(ActiveColor);

		return;
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(ATurret, ActiveColor))
	{
		if (!TurretBodyMaterialParent)
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: Assign TurretBodyMaterialParent first!"));
			return;
		}

		//Check if the material was created -> create it
		if (!TurretDynamicMaterial)
		{
			TurretDynamicMaterial = UMaterialInstanceDynamic::Create(TurretBodyMaterialParent, this, FName(TEXT("Base Material Dynamic")));

			//Assign material
			AssignTurretDynamicMaterial(TurretDynamicMaterial);
		}

		//Update Color
		SetTurretColor(ActiveColor);

		return;
	}
}

//Update Material In Scene
void ATurret::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!TurretBodyMaterialParent)
		return;

	//Create Dynamic Material if Missing
	if (TurretBodyMaterialParent && !TurretDynamicMaterial)
	{
		TurretDynamicMaterial = UMaterialInstanceDynamic::Create(TurretBodyMaterialParent, this);
		//Assign material
		AssignTurretDynamicMaterial(TurretDynamicMaterial);;
	}

	//Update Color
	SetTurretColor(ActiveColor);
}
#endif
#pragma endregion
