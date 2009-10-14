/*	NAME:
		NUndoManager.cpp

	DESCRIPTION:
		Undo manager.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "Nano Build.h"
#include "NMenuItem.h"
#include "NUndoManager.h"





//============================================================================
//		NUndoManager::NUndoManager : Constructor.
//----------------------------------------------------------------------------
NUndoManager::NUndoManager(void)
{


	// Initialize ourselves
	mIsUndoing   = false;
	mIsRedoing   = false;
	mIsRecording = true;

	mUndoLimit = 0;
	mGroupOpen = false;
}





//============================================================================
//		NUndoManager::~NUndoManager : Destructor.
//----------------------------------------------------------------------------
NUndoManager::~NUndoManager(void)
{
}





//============================================================================
//		NUndoManager::Clear : Clear the undo stacks.
//----------------------------------------------------------------------------
void NUndoManager::Clear(void)
{


	// Validate our state
	NN_ASSERT(!mIsUndoing && !mIsRedoing);



	// Clear the undo stacks
	mStackUndo.clear();
	mStackRedo.clear();
	
	UpdatedStacks();
}





//============================================================================
//		NUndoManager::CanUndo : Can we undo?
//----------------------------------------------------------------------------
bool NUndoManager::CanUndo(void) const
{


	// Can we undo?
	return(!mStackUndo.empty());
}





//============================================================================
//		NUndoManager::CanRedo : Can we redo?
//----------------------------------------------------------------------------
bool NUndoManager::CanRedo(void) const
{


	// Can we redo?
	return(!mStackRedo.empty());
}





//============================================================================
//		NUndoManager::PerformUndo : Perform an undo.
//----------------------------------------------------------------------------
void NUndoManager::PerformUndo(void)
{	UndoGroup						theGroup;
	NFunctorListReverseIterator		theIter;



	// Validate our state
	NN_ASSERT(!mIsUndoing && !mIsRedoing);



	// Prepare to undo
	mIsUndoing = true;
	theGroup   = mStackUndo.back();

	mStackUndo.pop_back();



	// Peform the undo
	//
	// To undo we take the top group of the undo stack, open a group, and
	// replay those actions in reverse order.
	//
	// Those actions will capture their undo's to the new group, and when
	// we close the group the new group will be added to the redo stack.
	//
	// Although the group actions may also set an undo name, the name of
	// the group overrides them (so that the user sees a consistent name
	// as a group is moved between the undo/redo menu items).
	BeginGroup();
	
	for (theIter = theGroup.theActions.rbegin(); theIter != theGroup.theActions.rend(); theIter++)
		(*theIter)();
	
	SetActionName(theGroup.theName);
	EndGroup();



	// Update our state
	mIsUndoing = false;
	UpdatedStacks();
}





//============================================================================
//		NUndoManager::PerformRedo : Perform a redo.
//----------------------------------------------------------------------------
void NUndoManager::PerformRedo(void)
{	UndoGroup						theGroup;
	NFunctorListReverseIterator		theIter;



	// Validate our state
	NN_ASSERT(!mIsUndoing && !mIsRedoing);



	// Prepare to redo
	mIsRedoing = true;
	theGroup   = mStackRedo.back();
	
	mStackRedo.pop_back();



	// Peform the redo
	//
	// To redo we take the top group of the redo stack, open a group, and
	// replay those actions in reverse order.
	//
	// Those actions will capture their undo's to the new group, and when
	// we close the group the new group will be added to the undo stack.
	//
	// Although the group actions may also set an undo name, the name of
	// the group overrides them (so that the user sees a consistent name
	// as a group is moved between the undo/redo menu items).
	BeginGroup();
	
	for (theIter = theGroup.theActions.rbegin(); theIter != theGroup.theActions.rend(); theIter++)
		(*theIter)();
	
	SetActionName(theGroup.theName);
	EndGroup();



	// Update our state
	mIsRedoing = false;
	UpdatedStacks();
}





//============================================================================
//		NUndoManager::IsUndoing : Are we undoing?
//----------------------------------------------------------------------------
bool NUndoManager::IsUndoing(void) const
{


	// Are we undoing?
	return(mIsUndoing);
}





//============================================================================
//		NUndoManager::IsRedoing : Are we redoing?
//----------------------------------------------------------------------------
bool NUndoManager::IsRedoing(void) const
{


	// Are we redoing?
	return(mIsRedoing);
}





//============================================================================
//		NUndoManager::IsRecording : Are we recording?
//----------------------------------------------------------------------------
bool NUndoManager::IsRecording(void) const
{


	// Are we recording?
	return(mIsRecording);
}





//============================================================================
//		NUndoManager::SetRecording : Set the recording status.
//----------------------------------------------------------------------------
void NUndoManager::SetRecording(bool canRecord)
{


	// Set the recording status
	mIsRecording = canRecord;
}





//============================================================================
//		NUndoManager::GetUndoLimit : Get the undo limit.
//----------------------------------------------------------------------------
UInt32 NUndoManager::GetUndoLimit(void) const
{


	// Get the undo limit
	return(mUndoLimit);
}





//============================================================================
//		NUndoManager::SetUndoLimit : Set the undo limit.
//----------------------------------------------------------------------------
void NUndoManager::SetUndoLimit(UInt32 theLimit)
{


	// Set the undo limit
	mUndoLimit = theLimit;
}





//============================================================================
//		NUndoManager::BeginGroup : Begin an undo group.
//----------------------------------------------------------------------------
void NUndoManager::BeginGroup(void)
{


	// Validate our state
	NN_ASSERT(!mGroupOpen);



	// Open the group
	mGroupOpen = true;
	mCurrentGroup.theActions.clear();
}





//============================================================================
//		NUndoManager::EndGroup : End an undo group.
//----------------------------------------------------------------------------
void NUndoManager::EndGroup(bool flushGroup)
{


	// Validate our state
	NN_ASSERT(mGroupOpen);
	NN_ASSERT(!mCurrentGroup.theActions.empty());



	// Close the group
	//
	// A group is the basic primitive of the undo stack; each group contains
	// one or more actions, and each group corresponds to one user-visible
	// "undo" state change.
	//
	// Undo groups are typically recorded to the undo stack, building up a
	// history of groups that can revert to previous states.
	//
	// When a group is undone, a second "undo" group will be recorded (to
	// undo the undo). This group is pushed to the redo stack, allowing us
	// to rewind to previous states by applying the groups on that stack.
	//
	// This gives us three situations - we're undoing, we're redoing, or
	// we're just recording an action to undo.
	if (mIsUndoing)
		mStackRedo.push_back(mCurrentGroup);

	else if (mIsRedoing)
		mStackUndo.push_back(mCurrentGroup);

	else
		{
		// Flushing a group only captures the first action
		if (flushGroup)
			mCurrentGroup.theActions.resize(1);

		mStackUndo.push_back(mCurrentGroup);
		mStackRedo.clear();
		}



	// Update the stacks
	//
	// Each list is a FILO, so we pop from the front.
	if (mUndoLimit != 0 && mStackUndo.size() == mUndoLimit)
		mStackUndo.pop_front();

	if (mUndoLimit != 0 && mStackRedo.size() == mUndoLimit)
		mStackRedo.pop_front();

	UpdatedStacks();



	// Reset our state
	mGroupOpen = false;
	mCurrentGroup.theName.Clear();
	mCurrentGroup.theActions.clear();
}





//============================================================================
//		NUndoManager::SetActionName : Set the action name.
//----------------------------------------------------------------------------
void NUndoManager::SetActionName(const NString &theName)
{


	// Set the action name
	mCurrentGroup.theName = theName;
}





//============================================================================
//		NUndoManager::RecordAction : Record an undo action.
//----------------------------------------------------------------------------
void NUndoManager::RecordAction(const NFunctor &theAction)
{	bool	needGroup;



	// Validate our parameters
	NN_ASSERT(theAction != NULL);



	// Record the action
	//
	// If no group is currently open, we open a new one around the action.
	if (mIsRecording)
		{
		needGroup = !mGroupOpen;
		if (needGroup)
			BeginGroup();

		mCurrentGroup.theActions.push_back(theAction);

		if (needGroup)
			EndGroup();
		}
}





//============================================================================
//		NUndoManager::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NUndoManager::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd  = theEvent.GetHICommand();
	theErr = noErr;
	
	switch (hiCmd.commandID) {
		case kHICommandUndo:
			PerformUndo();
			break;
			
		case kHICommandRedo:
			PerformRedo();
			break;
			
		default:
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NUndoManager::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NUndoManager::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;
	NString			theValue;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandUndo:
			theStatus = EnableIf(CanUndo());
			theValue  = GetCommandString(hiCmd.commandID, theStatus);
			break;

		case kHICommandRedo:
			theStatus = EnableIf(CanRedo());
			theValue  = GetCommandString(hiCmd.commandID, theStatus);
			break;
		
		default:
			theStatus = NCarbonEventHandler::GetCommandStatus(hiCmd);
			break;
		}



	// Update the user interface
	//
	// NCarbonEventHandler will take care of enabling/disabling the menu
	// items for us, but we must update the text.
	if (theValue.IsNotEmpty())
		NMenuItem(hiCmd.commandID).SetText(theValue);

	return(theStatus);
}





//============================================================================
//		NUndoManager::UpdatedStacks : The undo stacks have been modified.
//----------------------------------------------------------------------------
#pragma mark -
void NUndoManager::UpdatedStacks(void)
{	OSStatus	theErr;



	// Invalidate the menu status
	theErr = InvalidateMenuEnabling(NULL);
	NN_ASSERT_NOERR(theErr);



	// Let everyone know
	BroadcastMessage(kMsgUndoManagerUpdated, this);
}





//============================================================================
//		NUndoManager::GetCommandString : Get the command string.
//----------------------------------------------------------------------------
NString NUndoManager::GetCommandString(UInt32 theCmd, CommandStatus theStatus)
{	NString		theValue, cmdName, keyNone, keyGeneric, keyCommand;



	// Validate our parameters
	NN_ASSERT(theCmd == kHICommandUndo || theCmd == kHICommandRedo);



	// Select the keys
	if (theCmd == kHICommandUndo)
		{
		cmdName    = GetUndoName();
		keyNone    = kStringUndoNone;
		keyGeneric = kStringUndoGeneric;
		keyCommand = kStringUndoCommand;
		}
	else
		{
		cmdName    = GetRedoName();
		keyNone    = kStringRedoNone;
		keyGeneric = kStringRedoGeneric;
		keyCommand = kStringRedoCommand;
		}



	// Prepare the command string
	if (theStatus == kCommandEnable)
		{
		if (cmdName.IsNotEmpty())
			{
			theValue = NBundleString(keyCommand, "", kNanoStrings);
			theValue.Format(theValue.GetUTF8(), cmdName.GetUTF8());
			}
		else
			theValue = NBundleString(keyGeneric, "", kNanoStrings);
		}
	else
		theValue = NBundleString(keyNone, "", kNanoStrings);

	return(theValue);
}





//============================================================================
//		NUndoManager::GetUndoName : Get the current undo name.
//----------------------------------------------------------------------------
NString NUndoManager::GetUndoName(void)
{	NString		theName;



	// Get the name
	if (!mStackUndo.empty())
		theName = mStackUndo.back().theName;
	
	return(theName);
}





//============================================================================
//		NUndoManager::GetRedoName : Get the current redo name.
//----------------------------------------------------------------------------
NString NUndoManager::GetRedoName(void)
{	NString		theName;



	// Get the name
	if (!mStackRedo.empty())
		theName = mStackRedo.back().theName;
	
	return(theName);
}


