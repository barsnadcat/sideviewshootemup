// Fill out your copyright notice in the Description page of Project Settings.

#include "CalcNewPitch.h"
#include "Tests/TestHarnessAdapter.h"

TEST_CASE_NAMED(FEngineAxisTest, "Game::EngineAxis", "[Game][EngineAxis]")
{
    SECTION("NoDiff")
    {
        CHECK(FMath::IsNearlyEqual(CalcNewPitch(FVector::UnitZ(), FVector::UnitZ(), 3.0f), 90.f));
        CHECK(FMath::IsNearlyEqual(CalcNewPitch(-FVector::UnitZ(), -FVector::UnitZ(), 3.0f), -90.f));
        CHECK(FMath::IsNearlyEqual(CalcNewPitch(FVector::UnitX(), FVector::UnitX(), 3.0f), 0.0f));
        CHECK(FMath::IsNearlyEqual(FMath::Abs(CalcNewPitch(-FVector::UnitX(), -FVector::UnitX(), 3.0f)), 180.0f));
    }

    SECTION("LargeDiff")
    {
        CHECK(FMath::IsNearlyEqual(CalcNewPitch(FVector::UnitZ(), { 0.6f, 0.0f, 0.8f }, 3.0f), 87.0f));
        CHECK(FMath::IsNearlyEqual(CalcNewPitch(FVector::UnitX(), { 0.6f, 0.0f, -0.8f }, 3.0f), -3.0f));
    }

    SECTION("SmallDiff")
    {
        CHECK(FMath::IsNearlyEqual(CalcNewPitch({ 0.6f, 0.0f, 0.8f }, FVector::UnitX(), 90.0f), 0.0f));
    }
}
