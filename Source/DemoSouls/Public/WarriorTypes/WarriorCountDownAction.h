// Alex Goldberg rights reserved

#pragma once

#include "CoreMinimal.h"
#include "WarriorEnumTypes.h"

class FWarriorCountDownAction : public FPendingLatentAction
{
public:
	/// <summary>
	/// Constructor for the FWarriorCountDownAction class.
	/// Initializes the countdown action with the specified parameters.
	/// </summary>
	/// <param name="InTotalCountDownTime">The total time for the countdown.</param>
	/// <param name="InUpdateInterval">The interval at which to update the remaining time.</param>
	/// <param name="InOutRemainingTime">Reference to the remaining time in the countdown.</param>
	/// <param name="InCountDownOutput">Reference to the output action for the countdown.</param>
	/// <param name="LatentInfo">The latent action info for the countdown.</param>
	FWarriorCountDownAction(
		float InTotalCountDownTime,
		float InUpdateInterval,
		float& InOutRemainingTime,
		EWarriorCountDownActionOutput& InCountDownOutput,
		const FLatentActionInfo& LatentInfo) :
	bNeedToCancel(false),
	TotalCountDownTime(InTotalCountDownTime),
	UpdateInterval(InUpdateInterval),
	OutRemainingTime(InOutRemainingTime),
	CountDownOutput(InCountDownOutput),
	ExecutionFunction(LatentInfo.ExecutionFunction),
	OutputLink(LatentInfo.Linkage),
	CallbackTarget(LatentInfo.CallbackTarget),
	ElapsedInterval(0.f),
	ElapsedTimeSinceStart(0.f)
	{
	}

	/// <summary>
	/// Cancels the countdown action by setting the cancellation flag to true.
	/// </summary>
	void CancelAction();

	// ~ Begin FPendingLatentAction Interface
	virtual void UpdateOperation(FLatentResponse& Response) override;
	// ~ End FPendingLatentAction Interface

private:
	// Flag to indicate if the countdown action needs to be canceled.
	bool bNeedToCancel;
	// Total time for the countdown.
	float TotalCountDownTime;
	// Interval at which the countdown updates.
	float UpdateInterval;
	// Reference to the remaining time in the countdown.
	float& OutRemainingTime;
	// Reference to the output action for the countdown.
	EWarriorCountDownActionOutput& CountDownOutput;
	// Latent action info for the countdown.
	FName ExecutionFunction;
	// Linkage to the latent action.
	int32 OutputLink;
	// Object to which the latent action is bound.
	FWeakObjectPtr CallbackTarget;
	// Time elapsed since the last update interval.
	float ElapsedInterval;
	// Time elapsed since the start of the countdown.
	float ElapsedTimeSinceStart;
};
