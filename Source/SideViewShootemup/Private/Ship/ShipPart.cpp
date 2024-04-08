#include "Ship/ShipPart.h"

#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include <map>

AShipPart::AShipPart()
{
    Connections.SetNum(CI_Size);

    PrimaryActorTick.bCanEverTick = false;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    RootComponent = MainBody;
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

    BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxCollisionComponent->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    BoxCollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    BoxCollisionComponent->SetupAttachment(RootComponent);
}

void AShipPart::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);
}

void AShipPart::BreakAndReweldShip()
{
    if (AShipPart* weldRoot = GetWeldRoot())
    {
        weldRoot->ConnectedToWeldRoot++;

        Disconnect();

        for (uint8 i = 0; i < CI_Size; i++)
        {
            if (AShipPart* part = Connections[i].Get())
            {
                TSet<AShipPart*> parts;
                if (!IsConnectedToWeldRoot(part, weldRoot, parts))
                {
                    ReWeld(parts);
                }
            }
            Connections[i] = nullptr;
        }
    }
}

int32 AShipPart::Distance(AShipPart* a, AShipPart* b)
{
    int32 rowDistance = a->Row - b->Row;
    int32 colDistance = a->Column - b->Column;
    return rowDistance * rowDistance + colDistance * colDistance;
}

bool AShipPart::IsConnectedToWeldRoot(AShipPart* part, AShipPart* root, TSet<AShipPart*>& parts)
{
    std::multimap<int32, AShipPart*> openSet;
    openSet.insert(std::make_pair(Distance(part, root), part));

    while (!openSet.empty())
    {
        // Pop closest to the root
        auto it = openSet.begin();
        AShipPart* current = it->second;
        openSet.erase(it);

        // Connected to the root check
        if (current->ConnectedToWeldRoot == root->ConnectedToWeldRoot)
        {
            // Caching path to the root
            for (AShipPart* part : parts)
            {
                part->ConnectedToWeldRoot = root->ConnectedToWeldRoot;
            }
            for (auto& j : openSet)
            {
                j.second->ConnectedToWeldRoot = root->ConnectedToWeldRoot;
            }
            return true;
        }
        
        // Look around
        for (uint8 i = 0; i < CI_Size; i++)
        {
            if (AShipPart* n = current->Connections[i].Get())
            {
                if (!parts.Contains(n))
                {
                    openSet.insert(std::make_pair(Distance(n, root), n));
                }
            }
        }

        parts.Add(current);
    }

    return false;
}

AShipPart* AShipPart::GetWeldRoot()
{
    FBodyInstance* bodyInstance = MainBody->GetBodyInstance();
    check(bodyInstance);
    UPrimitiveComponent* ownerComponent = bodyInstance->OwnerComponent.Get();
    check(ownerComponent);
    return ownerComponent->GetOwner<AShipPart>();
}

void AShipPart::ReWeld(TSet<AShipPart*>& parts)
{
    AShipPart* newRoot = nullptr;
    for (AShipPart* part : parts)
    {
        if (!newRoot)
        {
            newRoot = part;
            newRoot->MainBody->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }
        else
        {
            part->MainBody->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            part->MainBody->AttachToComponent(newRoot->MainBody, {EAttachmentRule::KeepWorld, true});
        }
    }
}

void AShipPart::Disconnect()
{
    if (AShipPart* top = Connections[CI_Top].Get())
    {
        top->Connections[CI_Bottom] = nullptr;
    }
    if (AShipPart* right = Connections[CI_Right].Get())
    {
        right->Connections[CI_Left] = nullptr;
    }
    if (AShipPart* bottom = Connections[CI_Bottom].Get())
    {
        bottom->Connections[CI_Top] = nullptr;
    }
    if (AShipPart* left = Connections[CI_Left].Get())
    {
        left->Connections[CI_Right] = nullptr;
    }
}

void AShipPart::ConnectVertically(AShipPart* top, AShipPart* bottom)
{
    if (top && bottom)
    {
        top->Connections[CI_Bottom] = bottom;
        bottom->Connections[CI_Top] = top;
    }
}

void AShipPart::ConnectHorizontally(AShipPart* left, AShipPart* right)
{
    if (left && right)
    {
        left->Connections[CI_Right] = right;
        right->Connections[CI_Left] = left;
    }
}
