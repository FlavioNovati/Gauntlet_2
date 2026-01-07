#include "Game/Subsystems/PoolerSubSystem/Projectile.h"
#include "Game/Subsystems/PoolerSubSystem/ObjectPoolerSubsystem.h"
#include "Game/Interfaces/Damageable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	// Collision
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision")); 
	Collision->InitSphereRadius(10.f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->SetGenerateOverlapEvents(false);
	RootComponent = Collision; 
	// Bind hit event 
	Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Collision);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Disable()
{
	ProjectileMovement->SetActive(false);

	Collision->SetActive(false);
	Mesh->SetActive(false);

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	SetActorTickEnabled(false);

	IPoollable::Disable();
}

void AProjectile::Enable(FPoolEnablingData enablingData)
{
	FVector newLocation = enablingData.ObjectPoolTransform.GetLocation();
	FQuat newRotation = enablingData.ObjectPoolTransform.GetRotation();

	SetActorLocation(newLocation);
	SetActorRotation(newRotation);

	ProjectileMovement->SetActive(true);
	Collision->SetActive(true);
	Mesh->SetActive(true);

	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);

	FVector newVelocity = newRotation.GetRotationAxis();
	ProjectileMovement->Velocity = (this->GetActorForwardVector() * ProjectileSpeed);

	SetActorTickEnabled(true);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Disable();

	if (!OtherActor || OtherActor == this)
	{
		return;
	}


	TScriptInterface<IDamageable> damageable = TScriptInterface<IDamageable>(OtherActor);
	if (damageable == nullptr)
	{
		return;
	}

	if (damageable.GetObject())
	{
		damageable.GetInterface()->Damage(ProjectileDamage);
	}
}

