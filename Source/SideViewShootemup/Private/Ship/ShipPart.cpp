#include "Ship/ShipPart.h"

#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "HealthComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"

#include <map>

AShipPart::AShipPart()
{
    Connections.SetNum(CI_Size);

    PrimaryActorTick.bCanEverTick = false;

    GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
    GeometryCollection->SetComponentTickEnabled(false);
    GeometryCollection->SetGenerateOverlapEvents(false);
    GeometryCollection->SetSimulatePhysics(false);
    GeometryCollection->SetCollisionProfileName(TEXT("ShipPartMesh"));
    GeometryCollection->BodyInstance.bAutoWeld = false;
    RootComponent = GeometryCollection;
    Primitive = GeometryCollection;

    Overlap = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap"));
    Overlap->SetComponentTickEnabled(false);
    Overlap->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    Overlap->SetupAttachment(RootComponent);
    Overlap->SetGenerateOverlapEvents(true);
    Overlap->SetSimulatePhysics(false);
    Overlap->SetCollisionProfileName(TEXT("ShipPartOverlap"));
    Overlap->BodyInstance.bAutoWeld = false;

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
    FBodyInstance* bodyInstance = Primitive->GetBodyInstance();
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
            newRoot->Primitive->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }
        else
        {
            part->Primitive->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
            part->Primitive->AttachToComponent(newRoot->Primitive, {EAttachmentRule::KeepWorld, true});
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
