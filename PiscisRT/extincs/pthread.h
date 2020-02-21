/* static char sccsid[] = "@(#)15       1.32.2.32  src/bos/usr/ccs/lib/libpthreads/pthread.h,           libpth, bos43R 11/24/99 12:02:08"; */
/*
 * COMPONENT_NAME: libpth
 * 
 * FUNCTIONS:
 * 
 * ORIGINS:  27, 71, 83
 * 
 * LEVEL 1, 5 Years Bull Confidential Information
 *
 * (c) Copyright 1990, 1991, 1992, 1993 OPEN SOFTWARE FOUNDATION, INC. 
 * ALL RIGHTS RESERVED 
 *
 * OSF/1 1.2
 */

/*
 * file: pthread.h
 */

/* -----------------------------------------------------------------------------
 * Definitions for the Pthreads package.
 */


#ifndef	_PTHREAD_H_
#define	_PTHREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <standards.h>

#define _THREAD_SAFE	  1

/* includes 
 */
#include <sys/types.h>

#ifndef _AIX_PTHREADS_D7
#include <time.h>
#include <sched.h>
#else
#include <unistd.h>
#include <sys/timers.h>
#include <errno.h>
#include <sys/sched.h>
/* old definition. New definition in <sched.h> */
struct sched_param {
	int sched_priority;
	int sched_policy;
	int sched_reserved[6];
};
#endif /* _AIX_PTHREADS_D7 */

/* pthread class
 */
#define PTHREAD_PROCESS_SHARED		0
#define PTHREAD_PROCESS_PRIVATE		1

/* pthread mutex
 */
#ifndef _AIX_PTHREADS_D7
#define PTHREAD_MUTEX_ERRORCHECK	3 
#define PTHREAD_MUTEX_NORMAL		5 
#define PTHREAD_MUTEX_DEFAULT		PTHREAD_MUTEX_NORMAL
#endif /* _AIX_PTHREADS_D7 */

#define PTHREAD_MUTEX_RECURSIVE		4
	
/* scheduling
*/
#define PTHREAD_PRIO_INHERIT		0
#define PTHREAD_PRIO_NONE		1
#define PTHREAD_PRIO_PROTECT		2

#define PTHREAD_INHERIT_SCHED		0
#define PTHREAD_EXPLICIT_SCHED		1
#define PTHREAD_SCOPE_SYSTEM		0
#define PTHREAD_SCOPE_PROCESS		1

/* detach state
*/
#define PTHREAD_CREATE_DETACHED		1
#define PTHREAD_CREATE_JOINABLE		0

/* cancelability 
*/
#define PTHREAD_CANCELED			((void*) -1)
#define PTHREAD_CANCEL_DISABLE		0	/* general cancellation off */
#define PTHREAD_CANCEL_ENABLE		1	/* general cancellation on */
#define PTHREAD_CANCEL_DEFERRED		0	/* async cancellation off */
#define PTHREAD_CANCEL_ASYNCHRONOUS	1	/* async cancellation on */

/* initializers 
 */
#define PTHREAD_MUTEX_INITIALIZER \
{\
	0,	/* no waiters */\
	0,	/* no waiters */\
	0,	/* not locked */\
	2,	/* static mutex */\
	0,	/* no owner */\
	0,	/* not yet in mutex list */\
	0,	/* no acquisitions */\
	0,	/* no mutex type */\
	0,	/* not locked */\
	0,	/* no waits */\
	0,	/* no misses */\
	0,	/* not yet in mutex list */\
	0	/* not yet in mutex list */\
}

#define PTHREAD_COND_INITIALIZER \
{\
	0,	/* not yet in cond list */\
	0,	/* not yet in cond list */\
	0,	/* not locked */\
	2,	/* static cond */\
	0,	/* no waiters */\
	0,	/* no waiters */\
	0,	/* not yet in cond list */\
	0,	/* no mutex yet */\
	0,	/* no mutex yet */\
	0,	/* no waits */\
	0	/* reserved */\
}

#ifndef _AIX_PTHREADS_D7
#define PTHREAD_RWLOCK_INITIALIZER \
{\
	0,	/* not yet in rwlock list */\
	0,	/* not yet in rwlock list */\
	PTHREAD_MUTEX_INITIALIZER,\
	PTHREAD_COND_INITIALIZER,\
	PTHREAD_COND_INITIALIZER,\
	2,	/* static rwlock */\
	0,	/* no owner */\
	0,	/* not yet in rwlock list */\
	0,	/* not locked */\
	0,	/* no write waiters */\
	0,	/* no write acquisitions */\
	0,	/* no read acquisitions */\
	0,	/* no write waits */\
	0	/* no read waits */\
}
#endif

#define	PTHREAD_ONCE_INIT \
{\
	0,	/* not locked */\
	0,	/* not initialized */\
	0,	/* not executing */\
	0	/* not completed */\
	/* other fields not initialized here */\
}

#ifdef _NO_PROTO
#define __(args)	()
#else /* _NO_PROTO */
#define __(args)	args
#endif /* _NO_PROTO */

/* -----------------------------------------------------------------------------
 * Pthread Attributes
 */

extern int
pthread_attr_init __((pthread_attr_t *));

extern int
pthread_attr_destroy __((pthread_attr_t *));

extern int
pthread_attr_setstacksize __((pthread_attr_t *, size_t));

extern int
pthread_attr_getstacksize __((const pthread_attr_t *, size_t *));

extern int
pthread_attr_setstackaddr __((pthread_attr_t *, void *));

extern int
pthread_attr_getstackaddr __((const pthread_attr_t *, void **));

extern int
pthread_attr_setschedpolicy __((pthread_attr_t *, int));

extern int
pthread_attr_getschedpolicy __((const pthread_attr_t *, int *));

extern int
pthread_attr_setschedparam __((pthread_attr_t *, const struct sched_param *));

extern int
pthread_attr_getschedparam __((const pthread_attr_t *, struct sched_param *));

extern int
pthread_attr_setinheritsched __((pthread_attr_t *, int));

extern int
pthread_attr_getinheritsched __((const pthread_attr_t *, int *));

extern int
pthread_attr_setdetachstate __((pthread_attr_t *, int));

extern int
pthread_attr_getdetachstate __((const pthread_attr_t *, int *));

#ifndef _AIX_PTHREADS_D7
extern int
pthread_attr_setguardsize __((pthread_attr_t *, size_t));

extern int
pthread_attr_getguardsize __((const pthread_attr_t *, size_t *));
#endif /* _AIX_PTHREADS_D7 */

/* -----------------------------------------------------------------------------
 * Pthreads
 */

extern int
pthread_setschedparam __((pthread_t, int , const struct sched_param *));

extern int
pthread_getschedparam __((pthread_t, int *, struct sched_param *));

extern pthread_t
pthread_self __((void));

extern int
pthread_create __((pthread_t *, const pthread_attr_t *, void *(*)(void *), void *));

extern int
pthread_detach __((pthread_t));

extern int
pthread_join __((pthread_t, void **));

extern void
pthread_exit __((void *));

#ifdef _AIX_PTHREADS_D7
extern void
pthread_yield __((void));
#endif

extern void
pthread_cleanup_push __((void (*)(void *), void *));

extern void
pthread_cleanup_pop __((int));

extern int
pthread_cancel __((pthread_t));

extern int
pthread_attr_setscope __((pthread_attr_t *, int));

extern int
pthread_attr_getscope __((const pthread_attr_t *, int *));

extern int
pthread_equal __((pthread_t, pthread_t));

extern int
pthread_kill __((pthread_t, int));

#define	pthread_equal(t1, t2)	((t1) == (t2))

/* -----------------------------------------------------------------------------
 * Mutex Attributes
 */

extern int
pthread_mutexattr_init __((pthread_mutexattr_t *));

extern int
pthread_mutexattr_destroy __((pthread_mutexattr_t *));

extern int
pthread_mutexattr_setprotocol __((pthread_mutexattr_t *, int));

extern int
pthread_mutexattr_getprotocol __((const pthread_mutexattr_t *, int *));

extern int
pthread_mutexattr_setprioceiling __((pthread_mutexattr_t *, int));

extern int
pthread_mutexattr_getprioceiling __((const pthread_mutexattr_t *, int *));

extern int
pthread_mutexattr_getpshared __((const pthread_mutexattr_t *, int *));

#ifdef _AIX_PTHREADS_D7
extern int
pthread_mutexattr_setpshared __((const pthread_mutexattr_t *, int));
#else
extern int
pthread_mutexattr_setpshared __((pthread_mutexattr_t *, int));
#endif

#ifndef _AIX_PTHREADS_D7
extern int
pthread_mutexattr_gettype __(( const pthread_mutexattr_t *, int *));

extern int
pthread_mutexattr_settype __(( pthread_mutexattr_t *, int));
#endif /* _AIX_PTHREADS_D7 */

/* -----------------------------------------------------------------------------
 * Mutexes
 */

extern int
#ifndef _AIX_PTHREADS_D7
pthread_mutex_init __((pthread_mutex_t *, const pthread_mutexattr_t *));
#else
pthread_mutex_init __((pthread_mutex_t *, pthread_mutexattr_t *));
#endif 

extern int
pthread_mutex_destroy __((pthread_mutex_t *));

extern int
pthread_mutex_lock __((pthread_mutex_t *));

extern int
pthread_mutex_trylock __((pthread_mutex_t *));

extern int
pthread_mutex_unlock __((pthread_mutex_t *));

extern int
pthread_mutex_setprioceiling __((pthread_mutex_t *, int, int *));

extern int
#ifndef _AIX_PTHREADS_D7
pthread_mutex_getprioceiling __((const pthread_mutex_t *, int *));
#else
pthread_mutex_getprioceiling __((pthread_mutex_t *, int *));
#endif 

/* -----------------------------------------------------------------------------
 * Condition Variable Attributes
 */

extern int
pthread_condattr_init __((pthread_condattr_t *));

extern int
pthread_condattr_destroy __((pthread_condattr_t *));

extern int
pthread_condattr_getpshared __((const pthread_condattr_t *, int *));

#ifdef _AIX_PTHREADS_D7
extern int
pthread_condattr_setpshared __((const pthread_condattr_t *, int));
#else
extern int
pthread_condattr_setpshared __((pthread_condattr_t *, int));
#endif

/* -----------------------------------------------------------------------------
 * Condition Variables
 */

#ifdef _AIX_PTHREADS_D7
extern int
pthread_cond_init __((pthread_cond_t *, pthread_condattr_t *));
#else
extern int
pthread_cond_init __((pthread_cond_t *, const pthread_condattr_t *));
#endif

extern int
pthread_cond_destroy __((pthread_cond_t *));

extern int
pthread_cond_wait __((pthread_cond_t *, pthread_mutex_t *));

extern int
pthread_cond_timedwait __((pthread_cond_t *, pthread_mutex_t *,
			   const struct timespec *));

extern int
pthread_cond_signal __((pthread_cond_t *));

extern int
pthread_cond_broadcast __((pthread_cond_t *));

/* -----------------------------------------------------------------------------
 * Thread Specific Data
 */

extern int
pthread_key_create __((pthread_key_t *, void (*)(void *)));

extern int
pthread_key_delete __((pthread_key_t ));

extern void *
pthread_getspecific __((pthread_key_t));

extern int
pthread_setspecific __((pthread_key_t, const void *));

/* -----------------------------------------------------------------------------
 * Cancellation
 */

extern void
pthread_testcancel __((void));

extern int
pthread_setcancelstate __((int, int *));

extern int
pthread_setcanceltype __((int, int *));

/* -----------------------------------------------------------------------------
 * Pthread once
 */

extern int
pthread_once __((pthread_once_t *, void (*)(void)));

/* -----------------------------------------------------------------------------
 * Read / Write Lock
 */

#ifndef _AIX_PTHREADS_D7
#if (_XOPEN_SOURCE==500)
extern int 
pthread_rwlock_destroy __((pthread_rwlock_t *));

extern int 
pthread_rwlock_init __((pthread_rwlock_t *, const pthread_rwlockattr_t *));

extern int 
pthread_rwlock_rdlock __((pthread_rwlock_t *));

extern int 
pthread_rwlock_tryrdlock __((pthread_rwlock_t *));

extern int 
pthread_rwlock_trywrlock __((pthread_rwlock_t *));

extern int 
pthread_rwlock_unlock __((pthread_rwlock_t *));

extern int 
pthread_rwlock_wrlock __((pthread_rwlock_t *));

extern int 
pthread_rwlockattr_destroy __((pthread_rwlockattr_t *));

extern int 
pthread_rwlockattr_getpshared __((const pthread_rwlockattr_t *, int *));

extern int 
pthread_rwlockattr_init __((pthread_rwlockattr_t *));

extern int 
pthread_rwlockattr_setpshared __((pthread_rwlockattr_t *, int));

#endif /* _XOPEN_SOURCE == 500 */

/* -----------------------------------------------------------------------------
 * Posix Concurrency
 */

extern int
pthread_getconcurrency __((void));

extern int
pthread_setconcurrency __((int));

#endif /* _AIX_PTHREADS_D7 */

#ifdef _AIX_PTHREADS_D7
extern int 
pthread_atfork __((void (*)(void), void (*)(void), void (*)(void)));
#endif

extern int __pthread_trace;		/* exported variable in libt */ 
extern int __n_pthreads;		/* Number of active pthreads */
 
#if defined(_ALL_SOURCE) || defined(_AIX_PTHREADS_D7)

/* ---------------------------------------------------------------------------- 
 * Decimal bit values for environment variable AIXTHREAD_DEBUG and exported 
 * program variable __pthread_trace.  Valid only for the DEBUG libraries.
 * export LIBPATH=/usr/ccs/lib/libt
 */
#define LOG_GENERAL			 1
#define LOG_LOCKS			 2
#define LOG_DATA			 4
#define LOG_CREATE			 8
#define LOG_USCHED			16
#define LOG_CANCEL			32
#define LOG_SIGNAL			64
#define LOG_SUSPEND		       128
#define LOG_HANG 		      1024 	/* causes program to hang */

/* ================ MACROS =====================================================
 */

/* mutex
 */
#define	MUTEX_FAST_NP			2 /* for DCE support */
#define	MUTEX_RECURSIVE_NP		1
#define	MUTEX_NONRECURSIVE_NP		0 /* Default */
#define DEFAULT_SCHED			SCHED_OTHER
#define	PTHREAD_PRIO_MAX		127
#define PTHREAD_PRIO_MIN		1
#define DEFAULT_PRIO			PTHREAD_PRIO_MIN
#define DEFAULT_INHERIT			PTHREAD_INHERIT_SCHED
#define PTHREAD_SCOPE_GLOBAL		PTHREAD_SCOPE_SYSTEM
#define PTHREAD_SCOPE_LOCAL		PTHREAD_SCOPE_PROCESS
#define DEFAULT_SCOPE			PTHREAD_SCOPE_LOCAL
#define PTHREAD_CREATE_UNDETACHED	0 /*same as PTHREAD_CREATE_JOINABLE */

/* pthread suspend state
*/
#define PTHREAD_CREATE_SUSPENDED_NP	1
#define PTHREAD_CREATE_UNSUSPENDED_NP	0

#ifdef _AIX_PTHREADS_D7
#define DEFAULT_DETACHSTATE		PTHREAD_CREATE_DETACHED
#else
#define DEFAULT_DETACHSTATE		PTHREAD_CREATE_JOINABLE
#endif

#define	pthread_mutex_getowner_np(mutex)	((mutex)->__ptmtx_owner)

/* --------------------------------------------------------
 * some limits - some of which also available in <limits.h>
 * --------------------------------------------------------
 */

#ifdef __64BIT__
#define	PTHREAD_SPECIFIC_DATA		2*PAGESIZE	/* per-thread data */
#else
#define	PTHREAD_SPECIFIC_DATA		PAGESIZE	/* per-thread data */
#endif /* __64_BIT__ */

#define	APTHREAD_DATAKEYS_MAX (PTHREAD_SPECIFIC_DATA / sizeof(specific_data_t))

/* PTHREAD_DATAKEYS_MAX has the same value as PTHREAD_KEYS_MAX in limits.h */
#ifndef PTHREAD_DATAKEYS_MAX 
#define PTHREAD_DATAKEYS_MAX		450
#endif /* PTHREAD_DATAKEYS_MAX */

/* PTHREAD_STACK_MIN has the same value as PTHREAD_STACK_MIN in limits.h */
#define PTHREAD_STACK_MIN_NP		( PAGESIZE * 2 )
#ifndef PTHREAD_STACK_MIN 
#ifdef _AIX_PTHREADS_D7
#define PTHREAD_STACK_MIN		( PAGESIZE * 24 )
#else	/* _AIX_PTHREADS_D7 */
#ifdef __64BIT__
#define PTHREAD_STACK_MIN		( 2 * PAGESIZE * 2)
#else	/* __64BIT__ */
#define PTHREAD_STACK_MIN		( PAGESIZE * 2)
#endif /* __64_BIT__ */
#endif /* _AIX_PTHREADS_D7 */
#endif /* PTHREAD_STACK_MIN */

/* PTHREAD_THREADS_MAX has the same value as PTHREAD_THREADS_MAX in limits.h */
#ifdef PTHREAD_THREADS_MAX 
#undef PTHREAD_THREADS_MAX
#endif
#ifndef PTHREAD_THREADS_MAX
#ifdef _LARGE_THREADS
#define PTHREAD_THREADS_MAX		32767
#else
#define PTHREAD_THREADS_MAX		512
#endif
#endif

/* ================ STRUCTURES =================================================
 */
typedef struct specific_data specific_data_t;

typedef struct __pthread_cleanup_handler_t {
	struct __pthread_cleanup_handler_t	*__next_handler;
	void	(*__handler_function)();
	void	*__handler_arg;
} __pthread_cleanup_handler_t;

struct specific_data {
	int flags;
#ifdef __64_BIT__
	int pad;
#endif    
	const void *value;
};

/*
 * Structures to retrieve data on suspended pthreads.  Filled in by
 * pthread_getthrds_np().
 */
/* This must be identical to pthdb_context_t in pthdebug.h */
struct __pthrdscontext
{
	unsigned long long	__pc_gpr[_NGPRS]; /* general purpose */
	unsigned long long	__pc_msr;	  /* machine state */
	unsigned long long	__pc_iar;	  /* instruction address */
	unsigned long long	__pc_lr;	  /* link */
	unsigned long long	__pc_ctr;	  /* count */
	unsigned int		__pc_cr;	  /* condition */
	unsigned int		__pc_xer;	  /* exception */
	unsigned int		__pc_fpscr;	 /* float status and control */
	unsigned int		__pc_fpscrx;	 /* fpscr software extension */
        unsigned long long      __pc_except[1];	  /* exception address */
	double  		__pc_fpr[_NFPRS]; /* floating point */
	char			__pc_fpeu;	  /* float ever used */
	char			__pc_fpinfo;	  /* float info */
	unsigned int		__pc_mq;	  /* multiply quotient */
        int                     __pc_excp_type;	  /* exception type */
};

struct __pthrdsinfo
{
	unsigned long long	__pi_handle;		/* dbx pthread id */
	pthread_t		__pi_ptid;		/* pthread_t */
	tid_t			__pi_tid;		/* null or tid_t */
	int			__pi_state;		/* state */
	int			__pi_suspendstate;	/* suspend status */
	void *			__pi_exit;		/* return status */
	unsigned long		__pi_ustk;		/* stack pointer */
	void *			__pi_func;		/* initial function */
	void *			__pi_arg;		/* initial argument */
	unsigned int		__pi_siglevel;		/* # nested sigs */
	unsigned int		__pi_frame_count;	/* # stack frames */
	char *			__pi_stackaddr;		/* lowest word */
	char *			__pi_stackend;		/* highest word */
	size_t			__pi_stacksize;		/* bytes in stack */
#ifdef __64BIT__
	ulong			__pi_pad[14];
#else
	ulong			__pi_pad[13];
#endif /* __64BIT__ */
	struct __pthrdscontext	__pi_context;		/* thread context */
};

/*
 * alternate __pthrdsinfo field names.
 */
#define __pi_suspended	__pi_suspendstate
#define __pi_returned	__pi_exit

/*
 * __pi_state values.
 */
#define PTHRDSINFO_STATE_NOTSUP		0x00000100
#define PTHRDSINFO_STATE_IDLE		0x00000004
#define PTHRDSINFO_STATE_RUN		0x00000001
#define PTHRDSINFO_STATE_SLEEP		0x00000008
#define PTHRDSINFO_STATE_READY		0x00000002
#define PTHRDSINFO_STATE_TERM		0x00000010

/*
 * __pi_suspendstate values.
 */
#define PTHRDSINFO_SUSPENDSTATE_NOTSUP		0x00000100
#define PTHRDSINFO_SUSPENDSTATE_SUSPENDED	0x00000001
#define PTHRDSINFO_SUSPENDSTATE_UNSUSPENDED	0x00000000

/*
 * __pi_siglevel values
 */
#define PTHRDSINFO_SIGCTX_BASE		((unsigned int)-1)
#define PTHRDSINFO_SIGCTX_INVALID	((unsigned int)-1)

/*
 * pthread_getthrds_np mode flags :
 */
#define PTHRDSINFO_QUERY_GPRS	0x00000001		/* query GPRs */
#define PTHRDSINFO_QUERY_SPRS	0x00000002		/* query SPRs */
#define PTHRDSINFO_QUERY_FPRS	0x00000004		/* query FPRs */
#define PTHRDSINFO_QUERY_REGS	0x0000000F		/* query registers */
#define PTHRDSINFO_QUERY_TID	0x00000010		/* query kernel th id */
#define PTHRDSINFO_QUERY_ALL	0x0000001F		/* full query */

/*
 * pthread_getthrds_np special mode flags:
 */
#define PTHRDSINFO_QUERY_SIGCTX	0x00000100		/* query sig level */


/*
 * atfork unregister flags
 */
#define PTHREAD_ATFORK_ARGUMENT 0x00000001
#define PTHREAD_ATFORK_ALL      0x00000002

extern int
pthread_atfork_unregister_np __((void *,
                                 void (*)(),
                                 void (*)(),
                                 void (*)(),
                                 int));


/* =============== EXTERNAL VARIABLES =========================================
 */
#ifdef _AIX_PTHREADS_D7
extern pthread_attr_t pthread_attr_default;
extern pthread_mutexattr_t pthread_mutexattr_default;
extern pthread_condattr_t pthread_condattr_default;
extern pthread_key_t __pthread_cleanup_handlerqueue;
#endif
/* -----------------------------------------------------------------------------
 * DBX interface
 */
extern void __funcblock_np();

/* -----------------------------------------------------------------------------
 * DCE Compatibility
 */
#ifndef _AIX32_THREADS
extern int
pthread_getunique_np __((pthread_t *, int *));

extern int
pthread_mutexattr_getkind_np __((pthread_mutexattr_t *, int *));
#endif /* _AIX32_THREADS */

/* -----------------------------------------------------------------------------
 * Non portable interfaces
 */
extern int
pthread_mutexattr_setkind_np __((pthread_mutexattr_t *, int ));

extern int
pthread_set_mutexattr_default_np __(( int ));

extern int
pthread_signal_to_cancel_np __((sigset_t *, pthread_t *));

extern int
pthread_delay_np __((struct timespec *));

extern int
pthread_get_expiration_np __((struct timespec *, struct timespec *));

extern void
pthread_lock_global_np __((void));

extern void
pthread_unlock_global_np __((void));

extern int
pthread_atfork_np __((void *, void (*)(void *), void (*)(void *), void (*)(void *)));

extern int
pthread_test_exit_np __((int *));

extern void
pthread_clear_exit_np __((pthread_t));

extern int
pthread_setcancelstate_np __((int, int *));

extern int
pthread_join_np __((pthread_t, void **));

extern void
pthread_cleanup_push_np __((void (*)(void *), void *, pthread_t *));

extern void
pthread_cleanup_pop_np __((int, pthread_t));

extern int
pthread_cleanup_information_np __((pthread_t, void *, tid_t *, void *, pthread_t *)); 

#ifdef _AIX_PTHREADS_D7
extern int
forkall __((pid_t *));
#endif

extern int
sigthreadmask __((int, const sigset_t *, sigset_t *));

extern int
pthread_attr_setsuspendstate_np __((pthread_attr_t *, int));

extern int
pthread_attr_getsuspendstate_np __((const pthread_attr_t *, int *));

extern int 
pthread_suspend_np __((pthread_t));

extern int 
pthread_suspend_others_np __((void));

extern int 
pthread_suspend __((pthread_t));

extern int 
pthread_continue_np __((pthread_t));

extern int 
pthread_continue_others_np __((void));

extern int 
pthread_continue __((pthread_t));

extern int 
pthread_attr_setstacksize_np __((pthread_attr_t *, size_t));

extern int
pthread_getthrds_np __((pthread_t *, int, struct __pthrdsinfo *, int,
			void *, int *));

#endif /* _ALL_SOURCE */

#undef __

#ifdef __cplusplus
}
#endif

#endif	/*_PTHREAD_H_*/
