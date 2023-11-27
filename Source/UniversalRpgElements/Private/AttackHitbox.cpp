// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values
#include "AttackHitbox.h"

AAttackHitbox::AAttackHitbox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HitboxCollider = CreateDefaultSubobject<UBoxComponent>("Hitbox");
	HitboxCollider->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AAttackHitbox::BeginPlay()
{
	Super::BeginPlay();

	HitboxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAttackHitbox::OnHitboxCollision);
}

void AAttackHitbox::OnHitboxCollision(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep, 
	const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(7, 2, FColor::Emerald, TEXT("Hit Object!"));
	if (!TargetsHit.Contains(OtherActor))
	{
		TargetsHit.Add(OtherActor);
		OnHitDelegate.Broadcast(OtherActor, OtherComp, Hit);
	}
}

// Called every frame
void AAttackHitbox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(7, 2, FColor::Emerald, TEXT("Hit Object!"));
}

