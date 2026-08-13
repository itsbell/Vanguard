// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Attack.h"
#include "Characters/Enemy.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner()))
    {
        Enemy->OnAttackHit();
    }
}
