/*	NAME:
		NTargetFile.h

	DESCRIPTION:
		File support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_FILE_HDR
#define NTARGET_FILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetFile {
public:
	// Is this a file?
	static bool							IsFile(const NString &thePath);


	// Is this a directory?
	static bool							IsDirectory(const NString &thePath);


	// Is a file writeable?
	static bool							IsWriteable(const NString &thePath);


	// Does a file exist?
	static bool							Exists(const NString &thePath);


	// Get/set a file's name
	static NString						GetName(const NString &thePath, bool displayName);
	static NString						SetName(const NString &thePath, const NString &theName, bool renameFile);


	// Get/set a file's size
	static SInt64						GetSize(const NString &thePath);
	static NStatus						SetSize(const NString &thePath, SInt64 theSize);


	// Get the child/parent of a path
	static NString						GetChild( const NString &thePath, const NString &theChild);
	static NString						GetParent(const NString &thePath);


	// Delete the file
	static void							Delete(const NString &thePath);


	// Create a directory
	static NStatus						CreateDirectory(const NString &thePath);


	// Exchange two files
	static NStatus						ExchangeWith(const NString &srcPath, const NString &targetPath);


	// Open/close a file
	static NFileRef						Open(const NString &thePath, NFilePermission thePermissions);
	static void							Close(NFileRef theFile);


	// Get/set the read/write position
	static SInt64						GetPosition(NFileRef theFile);
	static NStatus						SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition filePos);


	// Read/write from a file
	static NStatus						Read( NFileRef theFile, SInt64 theSize,       void *thePtr, UInt64 &numRead,    SInt64 theOffset, NFilePosition filePos);
	static NStatus						Write(NFileRef theFile, SInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition filePos);
};





#endif // NTARGET_FILE_HDR

