#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KeyWidget.generated.h"

UCLASS()
class PROJETOFINAL_API UKeyWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(meta = (BindWidget))
    UImage* KeyIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KeyCountText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
    FName KeyID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
    UTexture2D* KeyIconTexture;

    UFUNCTION()
    void OnKeyCountChanged(FName UpdatedKeyID, int32 NewCount);

    UFUNCTION()
    void RefreshInventoryBinding();

private:
    FTimerHandle RefreshTimerHandle;
};