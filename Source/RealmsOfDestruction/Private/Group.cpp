// Fill out your copyright notice in the Description page of Project Settings.


#include "Group.h"

//Default Constructor
UGroup::UGroup()
{

}

// Function for adding a group member
void UGroup::AddMember(int32 ID, const FString& Name)
{
	if (GroupSize >= MAXGROUPSIZE || IsMember(ID))
		return;

	FGroupMemberData NewMember;
	NewMember.ID = ID;
	NewMember.Name = Name;

	Members.Add(NewMember);
	
	GroupSize++;
}

// Function for removing a group member
void UGroup::RemoveMember(int32 ID)
{
	for (int i = 0; i < GroupSize; i++)
	{
		if (Members[i].ID == ID)
		{
			Members.RemoveAt(i);
			GroupSize--;
			return;
		}
	}
}

// Function for set host group member
void UGroup::SetHost(int32 ID)
{
	if (IsMember(ID))
	{
		HostID = ID;
	}
}

// Function for get host ID
int32 UGroup::GetHostID()
{
	return HostID;
}

//Get number of members in group
int UGroup::GetGroupSize()
{
	return GroupSize;
}

//Check if player is in group
bool UGroup::IsMember(int32 ID)
{
	for (FGroupMemberData data : Members)
	{
		if (data.ID == ID)
			return true;
	}
	return false;
}

