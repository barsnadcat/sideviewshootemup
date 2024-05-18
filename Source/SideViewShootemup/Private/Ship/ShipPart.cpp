#include "Ship/ShipPart.h"

#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "HealthComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/ShipPawn.h"
#include "Ship/BridgeShipPart.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "MyGameMode.h"

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
    AShipPawn* pawn = GetOwner<AShipPawn>();
    if (!pawn)
    {
        return;
    }

    if (AShipPart* bridge = pawn->GetBridge())
    {
        static uint8 connectedToBridge = 0;
        connectedToBridge++;
        bridge->ConnectedToBridge = connectedToBridge;

        Disconnect();

        for (uint8 i = 0; i < CI_Size; i++)
        {
            if (AShipPart* part = Connections[i].Get())
            {
                TSet<AShipPart*> parts;
                if (!IsConnectedToBridge(part, bridge, parts))
                {
                    Reweld(parts, pawn);
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

bool AShipPart::IsConnectedToBridge(AShipPart* part, AShipPart* bridge, TSet<AShipPart*>& parts)
{
    SCOPED_NAMED_EVENT_TEXT("AShipPart::IsConnectedToWeldRoot", FColor::Red);
    std::multimap<int32, AShipPart*> openSet;
    static uint8 seen = 0;
    seen++;
    part->Seen = seen;
    openSet.insert(std::make_pair(Distance(part, bridge), part));
    parts.Reserve(500);

    while (!openSet.empty())
    {
        // Pop closest to the root
        AShipPart* current = nullptr;
        auto it = openSet.begin();
        current = it->second;
        openSet.erase(it);

        // Connected to the root check
        if (current->ConnectedToBridge == bridge->ConnectedToBridge)
        {
            // Caching path to the root
            for (auto& j : openSet)
            {
                j.second->ConnectedToBridge = bridge->ConnectedToBridge;
            }
            return true;
        }
        current->ConnectedToBridge = bridge->ConnectedToBridge;

        // Look around
        for (uint8 i = 0; i < CI_Size; i++)
        {
            if (AShipPart* n = current->Connections[i].Get())
            {
                if (n->Seen != seen)
                {
                    n->Seen = seen;
                    openSet.insert(std::make_pair(Distance(n, bridge), n));
                }
            }
        }

        parts.Add(current);
    }

    return false;
}

void AShipPart::Reweld(TSet<AShipPart*>& parts, AShipPawn* oldShip)
{
    

    if (parts.Num() < 2)
    {
        SCOPED_NAMED_EVENT_TEXT("AShipPart::Reweld::UnUnion", FColor::Red);
        for (AShipPart* part : parts)
        {
            oldShip->UnUnion(part);
        }
    }
    else
    {
        auto it = parts.begin();  
        FVector worldLocation = (*it)->GetActorLocation();
        SCOPED_NAMED_EVENT_TEXT("AShipPart::Reweld::Spawn", FColor::Red);
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnInfo.ObjectFlags |= RF_Transient;
        if (AShipPawn* newShip = GetWorld()->SpawnActor<AShipPawn>(AShipPawn::StaticClass(), worldLocation, FRotator::ZeroRotator, SpawnInfo))
        {
            newShip->ConstructShip(parts);
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
