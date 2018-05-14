#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#ifdef __WINDOWS__
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;

	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;

	typedef __int16 int16_t;
	typedef unsigned __int16 uint16_t;

	typedef signed char int8_t;
	typedef unsigned char uint8_t;
	# ifndef socklen_t
	typedef int socklen_t;
	# endif
#else
	#ifndef __KERNEL__
		#include <inttypes.h>
	#else
		#include <linux/types.h>
		typedef int16_t wchar_t;
	#endif
	typedef int SOCKET;
	#define INVALID_SOCKET (-1)
	#define SOCKET_ERROR (-1)
#endif

typedef uint64_t sd_u64_t;
typedef int64_t sd_s64_t;

typedef uint32_t sd_u32_t;
typedef int32_t sd_s32_t;

typedef uint16_t sd_u16_t;
typedef int16_t sd_s16_t;

typedef unsigned char sd_u8_t;
typedef char sd_s8_t;
typedef sd_s64_t sd_time_t;

#endif //__TYPE_DEF_H__

