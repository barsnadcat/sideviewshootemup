#include "CalcNewPitch.h"

double CalcNewPitch(const FVector& current, const FVector& requested, double maxDelta)
{
    double currentPitch = FMath::RadiansToDegrees(FMath::Atan2(current.Z, current.X));
    double requestedPitch = FMath::RadiansToDegrees(FMath::Atan2(requested.Z, requested.X));
    double delta = FMath::FindDeltaAngleDegrees(currentPitch, requestedPitch);
    return FMath::Abs(delta) > maxDelta ? currentPitch + copysign(maxDelta, delta) : requestedPitch;
}
