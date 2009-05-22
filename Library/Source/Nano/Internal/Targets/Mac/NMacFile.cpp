/*	NAME:
		NMacFile.cpp

	DESCRIPTION:
		Mac file support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <dirent.h>

#include "NCFString.h"

#include "NMacTarget.h"
#include "NTargetFile.h"





//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NTargetFile::IsFile(const NString &thePath)
{	struct stat		fileInfo;
	int				sysErr;
	bool			isFile;



	// Check the path
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	isFile = (sysErr == kNoErr && S_ISREG(fileInfo.st_mode));
	
	return(isFile);
}





//============================================================================
//      NTargetFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NTargetFile::IsDirectory(const NString &thePath)
{	struct stat		fileInfo;
	int				sysErr;
	bool			isFile;



	// Check the path
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	isFile = (sysErr == kNoErr && S_ISDIR(fileInfo.st_mode));
	
	return(isFile);
}





//============================================================================
//      NTargetFile::IsWriteable : Is a file writeable?
//----------------------------------------------------------------------------
bool NTargetFile::IsWriteable(const NString &thePath)
{	bool			isWriteable;
	struct stat		fileInfo;
	FILE			*tmpFile;
	DIR				*tmpDir;
	int				sysErr;



	// Check the path
	isWriteable = false;
	sysErr      = stat(thePath.GetUTF8(), &fileInfo);

	if (sysErr != kNoErr)
		return(isWriteable);



	// Check a file
	if (S_ISREG(fileInfo.st_mode))
		{
		tmpFile = fopen(thePath.GetUTF8(), "wb");
		if (tmpFile != NULL)
			{
			isWriteable = true;
			fclose(tmpFile);
			}
		}
	
	
	// Check a directory
	else if (S_ISDIR(fileInfo.st_mode))
		{
		tmpDir = opendir(thePath.GetUTF8());
		if (tmpDir != NULL)
			{
			isWriteable = true;
			closedir(tmpDir);
			}
		}


	// Handle failure
	else
		NN_LOG("Don't know how to open '%@'", thePath);
	
	return(isWriteable);
}





//============================================================================
//      NTargetFile::Exists : Does a file exist?
//----------------------------------------------------------------------------
bool NTargetFile::Exists(const NString &thePath)
{	struct stat		fileInfo;
	bool			doesExist;
	int				sysErr;



	// Check the path
	sysErr    = stat(thePath.GetUTF8(), &fileInfo);
	doesExist = (sysErr == kNoErr);
	
	return(doesExist);
}





//============================================================================
//      NTargetFile::GetName : Get a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::GetName(const NString &thePath, bool displayName)
{	CFStringRef		cfString;
	NRange			slashPos;
	NString			theName;
	NStatus			theErr;
	NCFObject		cfURL;



	// Get the display name
	if (displayName)
		{
		if (cfURL.Set(CFURLCreateWithFileSystemPath(NULL, NCFString(thePath), kCFURLPOSIXPathStyle, IsDirectory(thePath))))
			{
#if NN_TARGET_MAC
			theErr = LSCopyDisplayNameForURL(cfURL, &cfString);
			if (theErr == noErr)
				theName = NCFString(cfString, true);
#else
			(void) theErr;
			(void) cfString;
#endif
			}
		}



	// Get the file name
	if (theName.IsEmpty())
		{
		slashPos = thePath.Find("/", kNStringBackwards);
		if (slashPos.IsNotEmpty())
			theName = thePath.GetRight(thePath.GetSize() - slashPos.GetNext());
		}

	return(theName);
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile)
{	NString		newPath;
	int			sysErr;



	// Get the new path
	newPath = GetChild(GetParent(thePath), fileName);



	// Rename the file
	if (renameFile)
		{
		sysErr = rename(thePath.GetUTF8(), newPath.GetUTF8());
		NN_ASSERT_NOERR(sysErr);
		}
	
	return(newPath);
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetSize(const NString &thePath)
{	struct stat		fileInfo;
	UInt64			theSize;
	int				sysErr;



	// Validate our state
	NN_ASSERT(sizeof(fileInfo.st_size) >= sizeof(UInt64));



	// Get the file size
	theSize = 0;
	sysErr  = stat(thePath.GetUTF8(), &fileInfo);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr == kNoErr)
		theSize = fileInfo.st_size;

	return(theSize);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(NFileRef theFile, UInt64 theSize)
{	int		sysErr;



	// Validate our state
	NN_ASSERT(sizeof(off_t) >= sizeof(UInt64));



	// Set the position
	sysErr = fseeko((FILE *) theFile, theSize, SEEK_SET);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::GetChild : Get the child of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetChild(const NString &thePath, const NString &fileName)
{	NString		theChild;



	// Validate our parameters
	NN_ASSERT(!fileName.StartsWith("/"));



	// Get the child
	theChild = thePath;
	theChild.TrimRight("/");
	theChild += fileName;
	
	return(theChild);
}





//============================================================================
//      NTargetFile::GetParent : Get the parent of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetParent(const NString &thePath)
{	NString		theParent;
	NRange		slashPos;



	// Get the parent
	slashPos = thePath.Find("/", kNStringBackwards);

	if (slashPos.IsNotEmpty() && slashPos.GetLocation() >= 1)
		theParent = thePath.GetLeft(slashPos.GetLocation() - 1);
	else
		theParent = thePath;
	
	return(theParent);
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
void NTargetFile::Delete(const NString &thePath)
{	int		sysErr;



	// Delete the file
	sysErr = unlink(thePath.GetUTF8());
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//      NTargetFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateDirectory(const NString &thePath)
{	int		sysErr;



	// Create the directory
	sysErr = mkdir(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &dstPath)
{	int		sysErr;



	// Swap the files
	sysErr = exchangedata(srcPath.GetUTF8(), dstPath.GetUTF8(), 0);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::Open : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::Open(const NString &thePath, NFilePermission thePermission)
{	FILE	*theFile;
	int		sysErr;



	// Create the file
	if (!Exists(thePath))
		{
		sysErr = creat(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
		NN_ASSERT_NOERR(sysErr);
		}



	// Open the file
	theFile = fopen(thePath.GetUTF8(), NMacTarget::ConvertPermission(thePermission));
	
	return(theFile);
}





//============================================================================
//      NTargetFile::Close : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::Close(NFileRef theFile)
{	int		sysErr;



	// Close the file
	sysErr = fclose((FILE *) theFile);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//      NTargetFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetPosition(NFileRef theFile)
{	off_t		thePos;



	// Validate our state
	NN_ASSERT(sizeof(off_t) >= sizeof(UInt64));



	// Get the position
	thePos = ftello((FILE *) theFile);
	
	return(thePos);
}





//============================================================================
//      NTargetFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{	int		sysErr;



	// Validate our state
	NN_ASSERT(sizeof(off_t) >= sizeof(UInt64));



	// Set the position
	sysErr = fseeko((FILE *) theFile, theOffset, NMacTarget::ConvertPosition(thePosition));
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::Read : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Read(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition)
{	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theSize <= kUInt32Max);



	// Adjust the position
	if (thePosition != kFilePositionFromMark || theOffset != 0)
		{
		theErr = SetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the read
	numRead = fread(thePtr, 1, theSize, (FILE *) theFile);
	theErr  = kNoErr;

	if (feof((FILE *) theFile))
		theErr = kNErrExhaustedSrc;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::Write : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Write(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition)
{	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theSize <= kUInt32Max);



	// Adjust the position
	if (thePosition != kFilePositionFromMark || theOffset != 0)
		{
		theErr = SetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the write
	numWritten = fwrite(thePtr, 1, theSize, (FILE *) theFile);
	theErr     = noErr;
	
	 if (numWritten != theSize)
		theErr = kNErrDiskFull;
	
	return(theErr);
}




