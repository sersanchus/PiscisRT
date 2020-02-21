/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* bos430 src/bos/usr/include/sched.h 1.1                                 */
/*                                                                        */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* Restricted Materials of IBM                                            */
/*                                                                        */
/* (C) COPYRIGHT International Business Machines Corp. 1997               */
/* All Rights Reserved                                                    */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/* @(#)56       1.1  src/bos/usr/include/sched.h, sysproc, bos430, 9737A_430 2/12/97 14:07:45 */

/*
 * COMPONENT_NAME: (SYSPROC) 
 *
 * FUNCTIONS:
 *
 * ORIGINS: 27
 *
 * (C) COPYRIGHT International Business Machines Corp. 1996
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 */

/* THIS FILE is for conformance to IEEE POSIX 1003.1c.
 * It describes required symbols for the Realtime option
 * which is NOT SUPPORTED.
 */

#ifndef _H_SCHED
#define _H_SCHED

#ifndef _H_STANDARDS
#include <standards.h>
#endif

#ifndef _H_TYPES
#include <sys/types.h>
#endif


#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sched_param
{
	int	 sched_priority;
	int	 sched_policy;
	int	 sched_reserved[6];
};


extern int 	sched_get_priority_max(int);
extern int 	sched_get_priority_min(int);
extern int 	sched_getparam(pid_t, struct sched_param *);
extern int 	sched_getscheduler(pid_t);
extern int 	sched_rr_get_interval(pid_t, struct timespec *);
extern int 	sched_setparam(pid_t, const struct sched_param *);
extern int 	sched_setscheduler(pid_t, int, const struct sched_param *);
extern int 	sched_yield(void);


#ifdef __cplusplus
}
#endif

#endif /* _H_SCHED */
