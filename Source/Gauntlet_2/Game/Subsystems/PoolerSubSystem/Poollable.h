// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolEnablingData.h"
#include "Poollable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoollable : public UInterface
{
	GENERATED_BODY()
};

class GAUNTLET_2_API IPoollable
{
	GENERATED_BODY()
	
public:

	//Disable version to implement in blueprint
	UFUNCTION(BlueprintNativeEvent)
	void BP_Disable();

	//Enable version to implement in blueprint
	UFUNCTION(BlueprintNativeEvent)
	void BP_Enable(FPoolEnablingData enablingData);
	
	//Disable to call in C++
	void NativeDisable() { Disable(); }
	//Enable to call in C++
	void NativeEnable(FPoolEnablingData enablingData) { Enable(enablingData); }

	//Disable version to Implement in c++
	virtual void Disable()
	{
		UObject* Object = Cast<UObject>(this);

		if (IsValid(Object))
		{
			Execute_BP_Disable(Object);
		}
	}

	//Enable version to Implement in c++
	virtual void Enable(FPoolEnablingData enablingData)
	{
		AActor* actor = Cast<AActor>(this);

		if (IsValid(actor))
		{
			actor->SetActorTransform(enablingData.ObjectPoolTransform);
			Execute_BP_Enable(actor, enablingData);
		}
	}
};
