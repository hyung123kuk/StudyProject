// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	class USStatComponent* GetStatComponent() { return StatComponent; }

	virtual void SetWidget(class UStudyUserWidget* InStudyUserWidget) {}

	UFUNCTION()
	virtual void OnCharacterDeath();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASTPSCharacter", meta = (AllowPrivateAccess))
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASPlayerCharacter, Meta = (AllowPrivateAccess))
	TObjectPtr<class USStatComponent> StatComponent;

};