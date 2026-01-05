#include "Game/Actors/Props/KillZone.h"
#include "Game/Interfaces/Damageable.h"

// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Body");
	RootComponent = Mesh;

	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_MAX);
	Mesh->OnComponentHit.AddDynamic(this, &AKillZone::ComponentHitted);
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillZone::ComponentHitted(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TScriptInterface<IDamageable> TInteractable = TScriptInterface<IDamageable>(OtherActor);
	if (TInteractable.GetInterface())
	{
		TInteractable.GetInterface()->Damage(std::numeric_limits<float>::max());
		return;
	}

	IDamageable* damageable = Cast<IDamageable>(OtherActor);
	if (damageable)
	{
		IDamageable::Execute_BP_OnDamage(OtherActor, std::numeric_limits<float>::max());
	}
}

