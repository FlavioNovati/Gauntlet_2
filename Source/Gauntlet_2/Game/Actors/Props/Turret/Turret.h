#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Game/Interfaces/Switchable.h"
#include "Game/Interfaces/Targetable.h"
#include "Game/Subsystems/PoolerSubSystem/Projectile.h"
#include "Game/Subsystems/PoolerSubSystem/ObjectPoolerSubsystem.h"

#include "Turret.generated.h"

UCLASS()
class GAUNTLET_2_API ATurret : public AActor, public ISwitchable
{
	GENERATED_BODY()

private:

#pragma region Turret Creation

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* Root;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* PivotYaw;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* PivotPitch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* BaseBody;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* DefaultGun;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	USceneComponent* ShootingPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	UMaterialInterface* TurretBodyMaterialParent;
	UMaterialInstanceDynamic* TurretDynamicMaterial;
	
#pragma endregion

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor ActiveColor = FColor::Orange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Visual"))
	FColor DisabledColor = FColor::Blue;

	void AssignTurretDynamicMaterial(UMaterialInstanceDynamic* materialInstance);
	void SetTurretColor(FColor color) { TurretDynamicMaterial->SetVectorParameterValue("Base Color", color); };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Debug"))
	FColor TurretOverlapColor = FColor::Cyan;


#if WITH_EDITOR
	//Set Dynamic Material And Update color both in scene and in BP editor

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	void OnConstruction(const FTransform& Transform);
#endif


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret"))
	FVector OverlapOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret"))
	float TurretDetectingRadious;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret - Shooting"))
	float ShootDelay = 0.5f;
	float LastShotTime = .0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret - Shooting"))
	TSubclassOf<AProjectile> ProjectileToShoot;
	UObjectPoolerSubsystem* ObjectPoolerSusbsystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret - Shooting"))
	int ProjectileInitialPoolSize = 15;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret"))
	float PitchTurningSpeed = 5.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateaccess = true, Category = "Turret"))
	float YawTurningSpeed = 5.f;

	TScriptInterface<ITargetable> Target;
	AActor* TargetActor;

public:	
	// Sets default values for this actor's properties
	ATurret();
	
protected:
	virtual void BeginPlay() override;

	void CheckForTarget();
	void SetTarget(TScriptInterface<ITargetable> target);

	float GetDistanceToTarget();
	void ShootAtTarget(float deltaTime);
	void LookAtPosition(float deltaTime, FVector targetPosition);
	void Shoot(float deltaTime);

	void EnableTurret();
	void DisableTurret();

	// Inherited via ISwitchable
	//Switch got turned ON
	void NativeSwitchON() override { bIsEnabled ? DisableTurret() : EnableTurret(); };
	//Switch got turned OFF
	void NativeSwitchOFF() override { bIsEnabled ? DisableTurret() : EnableTurret(); };

	UFUNCTION(BlueprintImplementableEvent, Meta = (AllowPrivateAccess = true))
	void OnShoot();

	UFUNCTION(BlueprintImplementableEvent, Meta = (AllowPrivateAccess = true))
	void OnTargetRecieved(AActor* targetActor);

	UFUNCTION(BlueprintImplementableEvent, Meta = (AllowPrivateAccess = true))
	void OnLookingForTarget();

	bool bIsEnabled = true;

public:	
	
	virtual void Tick(float DeltaTime) override;
};
