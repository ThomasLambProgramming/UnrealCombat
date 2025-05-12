// Fill out your copyright notice in the Description page of Project Settings.
#include "Projectile.h"

#include "BaseAi.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);		// set up a notification for when this component hits something blocking

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	//add damage. here other actor is the ai if it hits. do a cast check.
	ABaseAi* enemyHit = Cast<ABaseAi>(OtherActor);
	if (enemyHit == nullptr)
	{
		if (BounceAmount > 0 && bounceCount < BounceAmount)
		{
			bounceCount++;
		}
		else
			Destroy();
		return;
	}

	if (AoeRange > 0)
	{
		AIManager->DamageEnemiesInRadius(GetActorLocation(), AoeRange, Damage, this);	
	}
	else
	{
		//Get the actor that fired/setup this projectile.
		enemyHit->TakeDamage(Damage, OwningActor);
	}
	
	Destroy();
}
void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(101, 5, FColor::Blue, TEXT("Hit something"));
	if (BounceAmount > 0 && bounceCount < BounceAmount)
	{
		bounceCount++;
		return;
	}
	Destroy();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (Multishot <= 0)
		Multishot = 1;

	closestDistanceForTrackingSquared = closestDistanceForTracking * closestDistanceForTracking;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);		// set up a notification for when this component hits something blocking
	ProjectileMovement->Activate();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EntityTrackingState == TrackEnemy && AIManager != nullptr)
	{
		closestEnemyForTracking = AIManager->FindNearestEnemy(GetActorLocation(), closestDistanceForTrackingSquared);
		if (closestEnemyForTracking == nullptr)
			return;

		FVector velocityToApply = closestEnemyForTracking->GetActorLocation() - GetActorLocation();
		velocityToApply.Normalize();
		velocityToApply *= trackingForce;
		velocityToApply += ProjectileMovement->Velocity;
		if (velocityToApply.SquaredLength() > MaximumProjectileSpeed)
			velocityToApply = velocityToApply.GetSafeNormal() * MaximumProjectileSpeed;

		ProjectileMovement->Velocity = velocityToApply;
	}
}

void AProjectile::SetupProjectile(AAIManager* aiManager, AActor* owningActor)
{
	AIManager = aiManager;
	OwningActor = owningActor;
}

