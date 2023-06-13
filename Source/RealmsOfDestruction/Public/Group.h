// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Group.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FGroupMemberData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 ID;

    UPROPERTY(BlueprintReadWrite)
    FString Name;
};

UCLASS()
class REALMSOFDESTRUCTION_API UGroup : public UObject
{
	GENERATED_BODY()
public:
    //Default Constructor
	UGroup();
	
    // Function for adding a group member
    UFUNCTION(BlueprintCallable, Category = "Group")
    void AddMember(int32 ID, const FString& Name);
    
    // Function for removing a group member
    UFUNCTION(BlueprintCallable, Category = "Group")
    void RemoveMember(int32 ID);

    // Function for set host group member
    UFUNCTION(BlueprintCallable, Category = "Group")
    void SetHost(int32 ID);

    // Function for get host ID
    UFUNCTION(BlueprintCallable, Category = "Group")
    int32 GetHostID();

    //Get number of members in group
    UFUNCTION()
    int GetGroupSize();

    //Check if player is in group
    UFUNCTION()
    bool IsMember(int32 ID);

    const int MAXGROUPSIZE = 4;

private:
    //List of members in group
    TArray<FGroupMemberData> Members;

    int GroupSize = 0;

    int32 HostID = NULL;

};
