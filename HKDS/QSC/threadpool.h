#ifndef QSC_CPUID_H
#define QSC_CPUID_H

#include "common.h"
#if defined(QSC_SYSTEM_OS_WINDOWS)
#	include <process.h>
#	include <Windows.h>
#elif defined(QSC_SYSTEM_OS_POSIX)
#	include <sys/types.h>
#	include <unistd.h>
#	include <pthread>
#else
#	error your operating system is not supported!
#endif

#define QSC_THREADPOOL_THREADS_MAX 1024

#if !defined(pthread_t)
#	define pthread_t int
#endif

typedef struct qsc_threadpool_state
{
	pthread_t tpool[QSC_THREADPOOL_THREADS_MAX];
	size_t tcount;
} qsc_threadpool_state;

QSC_EXPORT_API bool qsc_threadpool_add_task(qsc_threadpool_state* ctx, void (*thd_func)(void*), void* state)
{
	bool res;
	
	res = false;

	if (ctx->tcount <= QSC_THREADPOOL_THREADS_MAX)
	{
#if defined(QSC_SYSTEM_OS_WINDOWS)
		ctx->tpool[ctx->tcount] = _beginthread(thd_func, 0, state);
#elif defined(QSC_SYSTEM_OS_POSIX)
		p_thread_create(&ctx->tpool[ctx->tcount], NULL, thd_func, state);
#endif
		++ctx->tcount;
		res = true;
	}

	return res;
}

QSC_EXPORT_API void qsc_threadpool_clear(qsc_threadpool_state* ctx)
{
	size_t i;

	if (ctx->tcount != 0)
	{
		for (i = 0; i < ctx->tcount; ++i)
		{
#if defined(QSC_SYSTEM_OS_WINDOWS)
			_endthread(ctx->tpool[i]);
#elif defined(QSC_SYSTEM_OS_POSIX)
			pthread_cancel(ctx->tpool[i]);
#endif
		}
	}

	memset(ctx->tpool, 0x00, QSC_THREADPOOL_THREADS_MAX * sizeof(int));
	ctx->tcount = 0;
}

QSC_EXPORT_API void qsc_threadpool_initialize(qsc_threadpool_state* ctx)
{
	memset(ctx->tpool, 0x00, QSC_THREADPOOL_THREADS_MAX * sizeof(int));
}

QSC_EXPORT_API void qsc_threadpool_remove_task(qsc_threadpool_state* ctx, size_t index)
{
	if (ctx->tcount != 0 && index <= ctx->tcount)
	{
#if defined(QSC_SYSTEM_OS_WINDOWS)
		_endthread(ctx->tpool[index]);
#elif defined(QSC_SYSTEM_OS_POSIX)
		pthread_cancel(ctx->tpool[index]);
#endif
		--ctx->tcount;
	}
}

#endif