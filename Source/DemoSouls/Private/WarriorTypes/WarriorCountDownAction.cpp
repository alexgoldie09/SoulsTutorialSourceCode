// Alex Goldberg rights reserved


#include "WarriorTypes/WarriorCountDownAction.h"

/// <summary>
/// Cancels the countdown action by setting the cancellation flag to true.
/// </summary>
void FWarriorCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}

/// <summary>
/// Updates the countdown action based on the elapsed time and the specified update interval.
/// If the countdown is completed or canceled, it triggers the appropriate output and finishes the action.
/// </summary>
void FWarriorCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	// If the countdown action needs to be canceled, set the output to Cancelled and finish the action.
	if (bNeedToCancel)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// If the countdown has reached or exceeded the total countdown time, set the output to Completed and finish the action.
	if (ElapsedTimeSinceStart >= TotalCountDownTime)
	{
		CountDownOutput = EWarriorCountDownActionOutput::Completed;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// If the elapsed interval is less than the update interval, accumulate the elapsed time.
	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	// If the elapsed interval has reached or exceeded the update interval, update the remaining time and trigger the output.
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;
		
		CountDownOutput = EWarriorCountDownActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

		ElapsedInterval = 0.f;
	}
}
