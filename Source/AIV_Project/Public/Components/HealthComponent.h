// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveSystem/Savable.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRevive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, UObject*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHpChange,UObject*, Instigator, float, Damage, float, NewHp);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIV_PROJECT_API UHealthComponent : public UActorComponent, public ISavable
{
	GENERATED_BODY()

// Events
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnRevive OnRevive;

	UPROPERTY(BlueprintAssignable)
	FOnHpChange OnHpChange;
	
public:
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void LoadComponent_Implementation(const FComponentData& ComponentSaveData) override;

	UFUNCTION(BlueprintCallable)
	void Damage(UObject* Instigator, float Damage);

	UFUNCTION(BlueprintCallable)
	void Die(UObject* Instigator);

	UFUNCTION(BlueprintCallable)
	void Heal(UObject* Instigator, float HpAmount);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const {return bAlive;}

	UFUNCTION(BlueprintCallable)
	void Revive();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHp() const {return Hp;}

	UFUNCTION(BlueprintCallable)
	float GetLastDamageTime(UObject* Instigator) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanTakeDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization")
	float FlushOldDamagesTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization")
	float FlushOldDamagesInterval;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(SaveGame)
	float Hp;

	bool bAlive;

	TMap<UObject*, float> LastDamages;

	float LastFlushTime;
};
