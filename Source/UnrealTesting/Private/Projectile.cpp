// Fill out your copyright notice in the Description page of Project Settings.
#include "Projectile.h"

#include "StandardAi.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnitConversion.h"


// Sets default values
AProjectile::AProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Activate();

	PrimaryActorTick.bCanEverTick = true;
}

//void AProjectile::ResetProjectile()
//{
//	ProjectileMovement->Velocity.X = 0;
//	ProjectileMovement->Velocity.Y = 0;
//	ProjectileMovement->Velocity.Z = 0;
//	SetActorLocation(FVector(0,0,-400));
//}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//add damage. here other actor is the ai if it hits. do a cast check.
	AStandardAi* enemyHit = Cast<AStandardAi>(OtherActor);
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
		AIManager->DamageEnemiesInRadius(GetActorLocation(), AoeRange, Damage);	
	}
	else
		enemyHit->DamageAi(Damage);
	
	Destroy();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (Multishot <= 0)
		Multishot = 1;

	closestDistanceForTrackingSquared = closestDistanceForTracking * closestDistanceForTracking;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EntityTrackingState == TrackEnemy && AIManager != nullptr)
	{
		closestEnemyForTracking = AIManager->FindNearestEnemy(GetActorLocation());
		if (closestEnemyForTracking == nullptr)
			return;

		const auto quaternion = FQuat::FindBetweenNormals((closestEnemyForTracking->GetActorLocation() - GetActorLocation()).GetSafeNormal(), ProjectileMovement->Velocity.GetSafeNormal());
		float angleDeg = 0.0f;
		FVector axisT;
		quaternion.ToAxisAndAngle(axisT, angleDeg);
		angleDeg = FMath::RadiansToDegrees(angleDeg);
		angleDeg = -angleDeg;

		if (firstLog)
		{
			firstLog = false;
			GEngine->AddOnScreenDebugMessage(25, 5, FColor::Emerald, FString::FromInt(angleDeg) + TEXT(" Angle"));
		}
		
		ProjectileMovement->Velocity = ProjectileMovement->Velocity.RotateAngleAxis(angleDeg * DeltaTime * 20 , axisT);
	}
}

void AProjectile::SetupProjectile(AAIManager* aiManager)
{
	AIManager = aiManager;	
}

