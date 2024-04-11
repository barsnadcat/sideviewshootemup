#include "Ship/ShipPart.h"

#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/ShipPawn.h"
#include "Ship/ShipStaticMeshComponent.h"
#include "SideViewShootemup/SideViewShootemup.h"

#include <map>

AShipPart::AShipPart()
{
    Connections.SetNum(CI_Size);

    PrimaryActorTick.bCanEverTick = false;

    MainBody2 = CreateDefaultSubobject<UBoxComponent>(TEXT("MainBody2"));
    MainBody2->InitBoxExtent(FVector(110.0f, 110.0f, 110.0f));
    RootComponent = MainBody2;

    MainBody = CreateDefaultSubobject<UShipStaticMeshComponent>(TEXT("MainBody"));
    MainBody->SetupAttachment(RootComponent);

    BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxCollisionComponent->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    BoxCollisionComponent->SetupAttachment(RootComponent);

    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AShipPart::BreakAndReweldShip()
{
    SCOPED_NAMED_EVENT_TEXT("AShipPart::BreakAndReweldShip", FColor::Red);

    if (AShipPart* weldRoot = GetWeldRoot())
    {
        static uint8 connectedToWeldRoot = 0;
        connectedToWeldRoot++;
        weldRoot->ConnectedToWeldRoot = connectedToWeldRoot;

        Disconnect();

        for (uint8 i = 0; i < CI_Size; i++)
        {
            if (AShipPart* part = Connections[i].Get())
            {
                TSet<AShipPart*> parts;
                if (!IsConnectedToWeldRoot(part, weldRoot, parts))
                {
                    Reweld(parts);
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
    SCOPED_NAMED_EVENT_TEXT("AShipPart::IsConnectedToWeldRoot", FColor::Red);
    std::multimap<int32, AShipPart*> openSet;
    static uint8 seen = 0;
    seen++;
    part->Seen = seen;
    openSet.insert(std::make_pair(Distance(part, root), part));
    parts.Reserve(500);

    while (!openSet.empty())
    {
        // Pop closest to the root
        AShipPart* current = nullptr;
        auto it = openSet.begin();
        current = it->second;
        openSet.erase(it);

        // Connected to the root check
        if (current->ConnectedToWeldRoot == root->ConnectedToWeldRoot)
        {
            // Caching path to the root
            for (auto& j : openSet)
            {
                j.second->ConnectedToWeldRoot = root->ConnectedToWeldRoot;
            }
            return true;
        }
        current->ConnectedToWeldRoot = root->ConnectedToWeldRoot;

        // Look around
        for (uint8 i = 0; i < CI_Size; i++)
        {
            if (AShipPart* n = current->Connections[i].Get())
            {
                if (n->Seen != seen)
                {
                    n->Seen = seen;
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
    FBodyInstance* bodyInstance = MainBody2->GetBodyInstance();
    check(bodyInstance);
    UPrimitiveComponent* ownerComponent = bodyInstance->OwnerComponent.Get();
    check(ownerComponent);
    return ownerComponent->GetOwner<AShipPart>();
}

void AShipPart::Reweld(TSet<AShipPart*>& parts)
{
    SCOPED_NAMED_EVENT_TEXT("AShipPart::Reweld", FColor::Red);
    AShipPart* newRoot = nullptr;
    for (AShipPart* part : parts)
    {
        if (!newRoot)
        {
            newRoot = part;
            {
                SCOPED_NAMED_EVENT_TEXT("DetachFromComponent newRoot", FColor::Red);
                newRoot->MainBody2->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            }
        }
        else
        {
            {
                SCOPED_NAMED_EVENT_TEXT("DetachFromComponent part", FColor::Red);
                part->MainBody2->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            }
            {
                SCOPED_NAMED_EVENT_TEXT("AttachToComponent part", FColor::Red);
                part->MainBody2->AttachToComponent(newRoot->MainBody2, {EAttachmentRule::KeepWorld, true});
            }
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
