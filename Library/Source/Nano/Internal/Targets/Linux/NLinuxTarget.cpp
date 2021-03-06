/*	NAME:
		NLinuxTarget.cpp

	DESCRIPTION:
		Linux target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <errno.h>

#include "NLinuxTarget.h"





//============================================================================
//		NLinuxTarget::ConvertSysErr : Convert an errno code.
//----------------------------------------------------------------------------
NStatus NLinuxTarget::ConvertSysErr(int sysErr)
{	NStatus		theErr;



	// Convert the value
	switch (sysErr) {
		case 0:						theErr = kNoErr;					break;
		case EPERM:					theErr = kNErrParam;				break;		// Operation not permitted
		case ENOENT:				theErr = kNErrNotFound;				break;		// No such file or directory
		case ESRCH:					theErr = kNErrNotFound;				break;		// No such process
		case EINTR:					theErr = kNErrParam;				break;		// Interrupted system call
		case EIO:					theErr = kNErrInternal;				break;		// Input/output error
		case ENXIO:					theErr = kNErrParam;				break;		// Device not configured
		case E2BIG:					theErr = kNErrParam;				break;		// Argument list too long
		case ENOEXEC:				theErr = kNErrParam;				break;		// Exec format error
		case EBADF:					theErr = kNErrNotFound;				break;		// Bad file descriptor
		case ECHILD:				theErr = kNErrNotFound;				break;		// No child processes
		case EDEADLK:				theErr = kNErrBusy;					break;		// Resource deadlock avoided
		case ENOMEM:				theErr = kNErrMemory;				break;		// Cannot allocate memory
		case EACCES:				theErr = kNErrPermission;			break;		// Permission denied
		case EFAULT:				theErr = kNErrParam;				break;		// Bad address
		case ENOTBLK:				theErr = kNErrParam;				break;		// Block device required
		case EBUSY:					theErr = kNErrBusy;					break;		// Device / Resource busy
		case EEXIST:				theErr = kNErrPermission;			break;		// File exists
		case EXDEV:					theErr = kNErrPermission;			break;		// Cross-device link
		case ENODEV:				theErr = kNErrNotSupported;			break;		// Operation not supported by device
		case ENOTDIR:				theErr = kNErrParam;				break;		// Not a directory
		case EISDIR:				theErr = kNErrParam;				break;		// Is a directory
		case EINVAL:				theErr = kNErrParam;				break;		// Invalid argument
		case ENFILE:				theErr = kNErrParam;				break;		// Too many open files in system
		case EMFILE:				theErr = kNErrParam;				break;		// Too many open files
		case ENOTTY:				theErr = kNErrParam;				break;		// Inappropriate ioctl for device
		case ETXTBSY:				theErr = kNErrBusy;					break;		// Text file busy
		case EFBIG:					theErr = kNErrExhaustedDst;			break;		// File too large
		case ENOSPC:				theErr = kNErrDiskFull;				break;		// No space left on device
		case ESPIPE:				theErr = kNErrParam;				break;		// Illegal seek
		case EROFS:					theErr = kNErrPermission;			break;		// Read-only file system
		case EMLINK:				theErr = kNErrParam;				break;		// Too many links
		case EPIPE:					theErr = kNErrParam;				break;		// Broken pipe
		case EDOM:					theErr = kNErrParam;				break;		// Numerical argument out of domain
		case ERANGE:				theErr = kNErrParam;				break;		// Result too large
		case EAGAIN:				theErr = kNErrBusy;					break;		// Resource temporarily unavailable
		case EINPROGRESS:			theErr = kNErrBusy;					break;		// Operation now in progress
		case EALREADY:				theErr = kNErrBusy;					break;		// Operation already in progress
		case ENOTSOCK:				theErr = kNErrParam;				break;		// Socket operation on non-socket
		case EDESTADDRREQ:			theErr = kNErrParam;				break;		// Destination address required
		case EMSGSIZE:				theErr = kNErrParam;				break;		// Message too long
		case EPROTOTYPE:			theErr = kNErrParam;				break;		// Protocol wrong type for socket
		case ENOPROTOOPT:			theErr = kNErrParam;				break;		// Protocol not available
		case EPROTONOSUPPORT:		theErr = kNErrNotSupported;			break;		// Protocol not supported
		case ESOCKTNOSUPPORT:		theErr = kNErrNotSupported;			break;		// Socket type not supported
		case ENOTSUP:				theErr = kNErrNotSupported;			break;		// Operation not supported
		case EPFNOSUPPORT:			theErr = kNErrNotSupported;			break;		// Protocol family not supported
		case EAFNOSUPPORT:			theErr = kNErrNotSupported;			break;		// Address family not supported by protocol family
		case EADDRINUSE:			theErr = kNErrBusy;					break;		// Address already in use
		case EADDRNOTAVAIL:			theErr = kNErrParam;				break;		// Can't assign requested address
		case ENETDOWN:				theErr = kNErrParam;				break;		// Network is down
		case ENETUNREACH:			theErr = kNErrParam;				break;		// Network is unreachable
		case ENETRESET:				theErr = kNErrParam;				break;		// Network dropped connection on reset
		case ECONNABORTED:			theErr = kNErrParam;				break;		// Software caused connection abort
		case ECONNRESET:			theErr = kNErrParam;				break;		// Connection reset by peer
		case ENOBUFS:				theErr = kNErrParam;				break;		// No buffer space available
		case EISCONN:				theErr = kNErrParam;				break;		// Socket is already connected
		case ENOTCONN:				theErr = kNErrParam;				break;		// Socket is not connected
		case ESHUTDOWN:				theErr = kNErrParam;				break;		// Can't send after socket shutdown
		case ETOOMANYREFS:			theErr = kNErrParam;				break;		// Too many references: can't splice
		case ETIMEDOUT:				theErr = kNErrTimeout;				break;		// Operation timed out
		case ECONNREFUSED:			theErr = kNErrParam;				break;		// Connection refused
		case ELOOP:					theErr = kNErrParam;				break;		// Too many levels of symbolic links
		case ENAMETOOLONG:			theErr = kNErrParam;				break;		// File name too long
		case EHOSTDOWN:				theErr = kNErrParam;				break;		// Host is down
		case EHOSTUNREACH:			theErr = kNErrParam;				break;		// No route to host
		case ENOTEMPTY:				theErr = kNErrDuplicate;			break;		// Directory not empty
//		case EPROCLIM:				theErr = kNErrParam;				break;		// Too many processes
		case EUSERS:				theErr = kNErrParam;				break;		// Too many users
		case EDQUOT:				theErr = kNErrDiskFull;				break;		// Disc quota exceeded
		case ESTALE:				theErr = kNErrParam;				break;		// Stale NFS file handle
		case EREMOTE:				theErr = kNErrParam;				break;		// Too many levels of remote in path
//		case EBADRPC:				theErr = kNErrParam;				break;		// RPC struct is bad
//		case ERPCMISMATCH:			theErr = kNErrParam;				break;		// RPC version wrong
//		case EPROGUNAVAIL:			theErr = kNErrParam;				break;		// RPC prog. not avail
//		case EPROGMISMATCH:			theErr = kNErrParam;				break;		// Program version wrong
//		case EPROCUNAVAIL:			theErr = kNErrParam;				break;		// Bad procedure for program
		case ENOLCK:				theErr = kNErrParam;				break;		// No locks available
		case ENOSYS:				theErr = kNErrParam;				break;		// Function not implemented
//		case EFTYPE:				theErr = kNErrParam;				break;		// Inappropriate file type or format
//		case EAUTH:					theErr = kNErrPermission;			break;		// Authentication error
//		case ENEEDAUTH:				theErr = kNErrPermission;			break;		// Need authenticator
//		case EPWROFF:				theErr = kNErrParam;				break;		// Device power is off
//		case EDEVERR:				theErr = kNErrParam;				break;		// Device error, e.g. paper out
		case EOVERFLOW:				theErr = kNErrParam;				break;		// Value too large to be stored in data type
//		case EBADEXEC:				theErr = kNErrMalformed;			break;		// Bad executable
//		case EBADARCH:				theErr = kNErrMalformed;			break;		// Bad CPU type in executable
//		case ESHLIBVERS:			theErr = kNErrMalformed;			break;		// Shared library version mismatch
//		case EBADMACHO:				theErr = kNErrMalformed;			break;		// Malformed Macho file
		case ECANCELED:				theErr = kNErrCancelled;			break;		// Operation canceled
		case EIDRM:					theErr = kNErrParam;				break;		// Identifier removed
		case ENOMSG:				theErr = kNErrParam;				break;		// No message of desired type   
		case EILSEQ:				theErr = kNErrMalformed;			break;		// Illegal byte sequence
//		case ENOATTR:				theErr = kNErrNotFound;				break;		// Attribute not found
		case EBADMSG:				theErr = kNErrParam;				break;		// Bad message
		case EMULTIHOP:				theErr = kNErrParam;				break;		// Reserved
		case ENODATA:				theErr = kNErrParam;				break;		// No message available on STREAM
		case ENOLINK:				theErr = kNErrParam;				break;		// Reserved
		case ENOSR:					theErr = kNErrParam;				break;		// No STREAM resources
		case ENOSTR:				theErr = kNErrParam;				break;		// Not a STREAM
		case EPROTO:				theErr = kNErrParam;				break;		// Protocol error
		case ETIME:					theErr = kNErrTimeout;				break;		// STREAM ioctl timeout
//		case ENOPOLICY:				theErr = kNErrParam;				break;		// No such policy registered

		default:
			NN_LOG("Unable to convert %ld", sysErr);
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NLinuxTarget::ConvertFilePermission : Convert file permissions.
//----------------------------------------------------------------------------
const char *NLinuxTarget::ConvertFilePermission(NFilePermission thePermission)
{	const char	*theResult;



	// Convert the value
	switch (thePermission) {
		case kNPermissionRead:			theResult = "r";		break;
		case kNPermissionWrite:			theResult = "a";		break;
		case kNPermissionUpdate:		theResult = "r+";		break;

		default:
			NN_LOG("Unable to convert %ld", thePermission);
			theResult = "r+";
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NLinuxTarget::ConvertFilePosition : Convert file positions.
//----------------------------------------------------------------------------
int NLinuxTarget::ConvertFilePosition(NFilePosition thePosition)
{	int		theResult;



	// Convert the value
	switch (thePosition) {
		case kNPositionFromStart:		theResult = SEEK_SET;	break;
		case kNPositionFromMark:		theResult = SEEK_CUR;	break;
		case kNPositionFromEnd:			theResult = SEEK_END;	break;

		default:
			NN_LOG("Unable to convert %ld", thePosition);
			theResult = SEEK_SET;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NLinuxTarget::ConvertTimeSpec : Convert a time.
//----------------------------------------------------------------------------
struct timespec NLinuxTarget::ConvertTimeSpec(NTime theTime)
{	NTime				timeSecs, timeFrac;
	struct timespec		timeSpec;



	// Convert the time
	timeSecs = floor(theTime);
	timeFrac = theTime - timeSecs;
		
	timeSpec.tv_sec  = (time_t) timeSecs;
	timeSpec.tv_nsec = (long) (timeFrac / kNTimeNanosecond);
	
	return(timeSpec);
}







