/*
 *  KERNEL/SCHED.C
 *
 *  Kernel scheduler and related syscalls
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *
 *  1996-12-23  Modified by Dave Grothe to fix bugs in semaphores and
 *		make semaphores SMP safe
 *  1998-11-19	Implemented schedule_timeout() and related stuff
 *		by Andrea Arcangeli
 *  2002-01-04	New ultra-scalable O(1) scheduler by Ingo Molnar:
 *		hybrid priority-list and round-robin design with
 *		an array-switch method of distributing timeslices
 *		and per-CPU runqueues.  Cleanups and useful suggestions
 *		by Davide Libenzi, preemptible kernel bits by Robert Love.
 *  2003-09-03	Interactivity tuning by Con Kolivas.
 *  2004-04-02	Scheduler domains code by Nick Piggin
 */

#include <linux/mm.h>
#include <linux/module.h>
#include <linux/nmi.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/highmem.h>
#include <linux/smp_lock.h>
#include <asm/mmu_context.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/completion.h>
#include <linux/kernel_stat.h>
#include <linux/security.h>
#include <linux/notifier.h>
#include <linux/profile.h>
#include <linux/suspend.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/timer.h>
#include <linux/rcupdate.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/percpu.h>
#include <linux/kthread.h>
#include <linux/seq_file.h>
#include <linux/syscalls.h>
#include <linux/times.h>
#include <linux/acct.h>
#include <asm/tlb.h>

#include <asm/unistd.h>


#ifdef CONFIG_SMP
#error "rgrrgr does not support SMP"
#endif

#ifdef CONFIG_SCHED_SMT
#error "rgrrgr does not support hyperthreading"
#endif

#ifdef CONFIG_PREEMPT
#error "rgrrgr does not support kernel-space process preemption"
#endif

#ifdef CONFIG_DEBUG_SPINLOCK_SLEEP
#error "rgrrgr does not support CONFIG_DEBUG_SPINLOCK_SLEEP"
#endif

#ifdef CONFIG_MAGIC_SYSRQ
#error "rgrrgr does not support magic SYSRQ's"
#endif

/*
 * These are the runqueue data structures:
 */

#define BITMAP_SIZE ((((MAX_PRIO+1+7)/8)+sizeof(long)-1)/sizeof(long))

typedef struct runqueue runqueue_t;

struct prio_array {
	unsigned int nr_active;
	unsigned long bitmap[BITMAP_SIZE];
	struct list_head queue[MAX_PRIO];
};

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
struct runqueue {
	spinlock_t lock;

	/*
	 * nr_running and cpu_load should be in the same cacheline because
	 * remote CPUs use both these fields when doing load calculation.
	 */
	unsigned long nr_running;
#ifdef CONFIG_SMP
	unsigned long cpu_load[3];
#endif
	unsigned long long nr_switches;

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long nr_uninterruptible;

	unsigned long expired_timestamp;
	unsigned long long timestamp_last_tick;
	task_t *curr, *idle;
	struct mm_struct *prev_mm;
	prio_array_t *active, *expired, arrays[2];
	int best_expired_prio;
	atomic_t nr_iowait;

#ifdef CONFIG_SMP
	struct sched_domain *sd;

	/* For active balancing */
	int active_balance;
	int push_cpu;

	task_t *migration_thread;
	struct list_head migration_queue;
	int cpu;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	struct sched_info rq_sched_info;

	/* sys_sched_yield() stats */
	unsigned long yld_exp_empty;
	unsigned long yld_act_empty;
	unsigned long yld_both_empty;
	unsigned long yld_cnt;

	/* schedule() stats */
	unsigned long sched_switch;
	unsigned long sched_cnt;
	unsigned long sched_goidle;

	/* try_to_wake_up() stats */
	unsigned long ttwu_cnt;
	unsigned long ttwu_local;
#endif
};




/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
 */
inline int task_curr(const task_t *p)
{
}

#ifdef CONFIG_SMP
/*
* wait_task_inactive - wait for a thread to unschedule.
*
* The caller must ensure that the task *will* unschedule sometime soon,
* else this function might spin for a *long* time. This function can't
* be called with interrupts off, or it may introduce deadlock with
* smp_call_function() if an IPI is sent by the same process we are
* waiting to become inactive.
*/
void wait_task_inactive(struct task_struct *p)
{
}
#endif

#ifdef CONFIG_SMP
/***
* kick_process - kick a running thread to enter/exit the kernel
* @p: the to-be-kicked thread
*
* Cause a process which is running on another CPU to enter
* kernel-mode, without any delay. (to get signals handled.)
*
* NOTE: this function doesnt have to take the runqueue lock,
* because all it wants to ensure is that the remote task enters
* the kernel. If the IPI races and the task has been migrated
* to another CPU then no harm is done and the purpose has been
* achieved as well.
*/
void kick_process(struct task_struct *p)
{
}
#endif


int fastcall wake_up_process(task_t *p)
{
}

EXPORT_SYMBOL(wake_up_process);

int fastcall wake_up_state(task_t *p, unsigned int state)
{
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 */
void fastcall sched_fork(task_t *p, int clone_flags)
{
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
void fastcall wake_up_new_task(task_t *p, unsigned long clone_flags)
{
}

/*
 * Potentially available exiting-child timeslices are
 * retrieved here - this way the parent does not get
 * penalized for creating too many threads.
 *
 * (this cannot be used to 'generate' timeslices
 * artificially, because any timeslice recovered here
 * was given away by the parent in the first place.)
 */
void fastcall sched_exit(task_t *p)
{
}

/**
* schedule_tail - first thing a freshly forked thread must call.
* @prev: the thread we just switched away from.
*/
asmlinkage void schedule_tail(struct task_struct *prev)
        __releases(rq->lock)
{
}


/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
/*static void double_lock_balance(struct rq *this_rq, struct rq *busiest)
        __releases(this_rq->lock)
        __acquires(busiest->lock)
        __acquires(this_rq->lock)
{
}*/

#ifdef CONFIG_SMP
/*
* sched_exec - execve() is a valuable balancing opportunity, because at
* this point the task has the smallest effective memory and cache footprint.
*/
void sched_exec(void)
{
}
#endif


/*
 * nr_running, nr_uninterruptible and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, current number of uninterruptible-sleeping threads, total
 * number of context switches performed since bootup.
 */
unsigned long nr_running(void)
{
}

unsigned long nr_uninterruptible(void)
{
}

unsigned long long nr_context_switches(void)
{
}

unsigned long nr_iowait(void)
{
}

#ifdef CONFIG_SMP
/* blablabla */
#else
/*
 * on UP we do not need to balance between CPUs:
 */
static inline void rebalance_tick(int cpu, runqueue_t *rq, enum idle_type idle)
{
}
static inline void idle_balance(int cpu, runqueue_t *rq)
{
}
#endif



DEFINE_PER_CPU(struct kernel_stat, kstat);

EXPORT_PER_CPU_SYMBOL(kstat);

/*
 * Return current->sched_time plus any more ns on the sched_clock
 * that have not yet been banked.
 */
unsigned long long current_sched_time(const task_t *tsk)
{
}

/*
* Account user cpu time to a process.
* @p: the process that the cpu time gets accounted to
* @hardirq_offset: the offset to subtract from hardirq_count()
* @cputime: the cpu time spent in user space since the last update
*/
void account_user_time(struct task_struct *p, cputime_t cputime)
{
}

/*
* Account system cpu time to a process.
* @p: the process that the cpu time gets accounted to
* @hardirq_offset: the offset to subtract from hardirq_count()
* @cputime: the cpu time spent in kernel space since the last update
*/
void account_system_time(struct task_struct *p, int hardirq_offset,
                         cputime_t cputime)
{
}

/*
* Account for involuntary wait time.
* @p: the process from which the cpu time has been stolen
* @steal: the cpu time spent in involuntary wait
*/
void account_steal_time(struct task_struct *p, cputime_t steal)
{
}


/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 *
 * It also gets called by the fork code, when changing the parent's
 * timeslices.
 */
void scheduler_tick(void)
{
}

#ifdef CONFIG_SCHED_SMT
/* blablabla */
#else
static inline void wake_sleeping_dependent(int this_cpu, runqueue_t *this_rq)
{
}

static inline int dependent_sleeper(int this_cpu, runqueue_t *this_rq)
{
	return 0;
}
#endif

/*
 * schedule() is the main scheduler function.
 */
asmlinkage void __sched schedule(void)
{
}

#ifdef CONFIG_PREEMPT
/*
 * this is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable. Kernel preemptions off return from interrupt
 * occur there and call schedule directly.
 */
asmlinkage void __sched preempt_schedule(void)
{
}
EXPORT_SYMBOL(preempt_schedule);

/*
* this is the entry point to schedule() from kernel preemption
* off of irq context.
* Note, that this is called and return with irqs disabled. This will
* protect us against recursive calling from irq.
*/
asmlinkage void __sched preempt_schedule_irq(void)
{
}

#endif /* CONFIG_PREEMPT */

int default_wake_function(wait_queue_t *curr, unsigned mode, int sync, void *key)
{
}
EXPORT_SYMBOL(default_wake_function);

/**
 * __wake_up - wake up threads blocked on a waitqueue.
 * @q: the waitqueue
 * @mode: which threads
 * @nr_exclusive: how many wake-one or wake-many threads to wake up
 * @key: is directly passed to the wakeup function
 */
void fastcall __wake_up(wait_queue_head_t *q, unsigned int mode,
                        int nr_exclusive, void *key)
{
}
EXPORT_SYMBOL(__wake_up);

/*
* Same as __wake_up but called with the spinlock in wait_queue_head_t held.
*/
void fastcall __wake_up_locked(wait_queue_head_t *q, unsigned int mode)
{
}

void fastcall
__wake_up_sync(wait_queue_head_t *q, unsigned int mode, int nr_exclusive)
{
}
EXPORT_SYMBOL_GPL(__wake_up_sync);      /* For internal use only */

void fastcall complete(struct completion *x)
{
}
EXPORT_SYMBOL(complete);

void fastcall complete_all(struct completion *x)
{
}
EXPORT_SYMBOL(complete_all);

void fastcall __sched wait_for_completion(struct completion *x)
{
}
EXPORT_SYMBOL(wait_for_completion);

unsigned long fastcall __sched wait_for_completion_timeout(struct completion *x, unsigned long timeout)
{
}
EXPORT_SYMBOL(wait_for_completion_timeout);

int fastcall __sched wait_for_completion_interruptible(struct completion *x)
{
}
EXPORT_SYMBOL(wait_for_completion_interruptible);

unsigned long fastcall __sched wait_for_completion_interruptible_timeout(struct completion *x, unsigned long timeout)
{
}
EXPORT_SYMBOL(wait_for_completion_interruptible_timeout);

void fastcall __sched interruptible_sleep_on(wait_queue_head_t *q)
{
}
EXPORT_SYMBOL(interruptible_sleep_on);

long fastcall __sched interruptible_sleep_on_timeout(wait_queue_head_t *q, long timeout)
{
}
EXPORT_SYMBOL(interruptible_sleep_on_timeout);

void fastcall __sched sleep_on(wait_queue_head_t *q)
{
}
EXPORT_SYMBOL(sleep_on);

long fastcall __sched sleep_on_timeout(wait_queue_head_t *q, long timeout)
{
}
EXPORT_SYMBOL(sleep_on_timeout);

void set_user_nice(task_t *p, long nice)
{
}

EXPORT_SYMBOL(set_user_nice);

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
int can_nice(const task_t *p, const int nice)
{
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
asmlinkage long sys_nice(int increment)
{
}

#endif

/**
 * task_prio - return the priority value of a given task.
 * @p: the task in question.
 *
 * This is the priority value as seen by users in /proc.
 * RT tasks are offset by -200. Normal tasks are centered
 * around 0, value goes from -16 to +15.
 */
int task_prio(const task_t *p)
{
}

/**
 * task_nice - return the nice value of a given task.
 * @p: the task in question.
 */
int task_nice(const task_t *p)
{
}
EXPORT_SYMBOL_GPL(task_nice);

/**
 * idle_cpu - is a given cpu idle currently?
 * @cpu: the processor in question.
 */
int idle_cpu(int cpu)
{
}

/**
 * idle_task - return the idle task for a given cpu.
 * @cpu: the processor in question.
 */
task_t *idle_task(int cpu)
{
}



/**
 * sched_setscheduler - change the scheduling policy and/or RT priority of
 * a thread.
 * @p: the task in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 */
int sched_setscheduler(struct task_struct *p, int policy,
		       struct sched_param *param)
{
}
EXPORT_SYMBOL_GPL(sched_setscheduler);

/*static int
do_sched_setscheduler(pid_t pid, int policy, struct sched_param __user *param)
{
}*/

/**
 * sys_sched_setscheduler - set/change the scheduler policy and RT priority
 * @pid: the pid in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 */
asmlinkage long sys_sched_setscheduler(pid_t pid, int policy,
				       struct sched_param __user *param)
{
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 */
asmlinkage long sys_sched_setparam(pid_t pid, struct sched_param __user *param)
{
}

/**
 * sys_sched_getscheduler - get the policy (scheduling class) of a thread
 * @pid: the pid in question.
 */
asmlinkage long sys_sched_getscheduler(pid_t pid)
{
}

/**
 * sys_sched_getscheduler - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 */
asmlinkage long sys_sched_getparam(pid_t pid, struct sched_param __user *param)
{
}

long sched_setaffinity(pid_t pid, cpumask_t new_mask)
{
}

/*static int get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     cpumask_t *new_mask)
{
}*/

/**
 * sys_sched_setaffinity - set the cpu affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new cpu mask
 */
asmlinkage long sys_sched_setaffinity(pid_t pid, unsigned int len,
				      unsigned long __user *user_mask_ptr)
{
}

/*
 * Represents all cpu's present in the system
 * In systems capable of hotplug, this map could dynamically grow
 * as new cpu's are detected in the system via any platform specific
 * method, such as ACPI for e.g.
 */

cpumask_t cpu_present_map __read_mostly;
EXPORT_SYMBOL(cpu_present_map);

#ifndef CONFIG_SMP
cpumask_t cpu_online_map __read_mostly = CPU_MASK_ALL;
EXPORT_SYMBOL(cpu_online_map);

cpumask_t cpu_possible_map __read_mostly = CPU_MASK_ALL;
EXPORT_SYMBOL(cpu_possible_map);
#endif


long sched_getaffinity(pid_t pid, cpumask_t *mask)
{
}

/**
 * sys_sched_getaffinity - get the cpu affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to hold the current cpu mask
 */
asmlinkage long sys_sched_getaffinity(pid_t pid, unsigned int len,
				      unsigned long __user *user_mask_ptr)
{
}

/**
 * sys_sched_yield - yield the current processor to other threads.
 *
 * this function yields the current CPU by moving the calling thread
 * to the expired array. If there are no other threads running on this
 * CPU then this function will return.
 */
asmlinkage long sys_sched_yield(void)
{
}


int __sched cond_resched(void)
{
}

EXPORT_SYMBOL(cond_resched);

/*
 * cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPT.  We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
int cond_resched_lock(spinlock_t *lock)
{
}

EXPORT_SYMBOL(cond_resched_lock);

int __sched cond_resched_softirq(void)
{
}

EXPORT_SYMBOL(cond_resched_softirq);


/**
 * yield - yield the current processor to other threads.
 *
 * this is a shortcut for kernel-space yielding - it marks the
 * thread runnable and calls sys_sched_yield().
 */
void __sched yield(void)
{
}

EXPORT_SYMBOL(yield);

/*
 * This task is about to go to sleep on IO.  Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 *
 * But don't do that if it is a deliberate, throttling IO wait (this task
 * has set its backing_dev_info: the queue against which it should throttle)
 */
void __sched io_schedule(void)
{
}

EXPORT_SYMBOL(io_schedule);

long __sched io_schedule_timeout(long timeout)
{
}

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * this syscall returns the maximum rt_priority that can be used
 * by a given scheduling class.
 */
asmlinkage long sys_sched_get_priority_max(int policy)
{
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * this syscall returns the minimum rt_priority that can be used
 * by a given scheduling class.
 */
asmlinkage long sys_sched_get_priority_min(int policy)
{
}

/**
 * sys_sched_rr_get_interval - return the default timeslice of a process.
 * @pid: pid of the process.
 * @interval: userspace pointer to the timeslice value.
 *
 * this syscall writes the default timeslice value of a given process
 * into the user-space timespec buffer. A value of '0' means infinity.
 */
asmlinkage
long sys_sched_rr_get_interval(pid_t pid, struct timespec __user *interval)
{
}

/*
* In a system that switches off the HZ timer nohz_cpu_mask
* indicates which cpus entered this state. This is used
* in the rcu update to wait only for active cpus. For system
* which do not switch off the HZ timer nohz_cpu_mask should
* always be CPU_MASK_NONE.
*/
cpumask_t nohz_cpu_mask = CPU_MASK_NONE;

#ifdef CONFIG_SMP
/*
* This is how migration works:
*
* 1) we queue a struct migration_req structure in the source CPU's
*    runqueue and wake up that CPU's migration thread.
* 2) we down() the locked semaphore => thread blocks.
* 3) migration thread wakes up (implicitly it forces the migrated
*    thread off the CPU)
* 4) it gets the migration request and checks whether the migrated
*    task is still in the wrong runqueue.
* 5) if it's in the wrong runqueue then the migration thread removes
*    it and puts it into the right queue.
* 6) migration thread up()s the semaphore.
* 7) we wake up and the migration is done.
*/

/*
* Change a given task's CPU affinity. Migrate the thread to a
* proper CPU and schedule it away if the CPU it's executing on
* is removed from the allowed bitmask.
*
* NOTE: the caller must have a valid reference to the task, the
* task must not exit() & deallocate itself prematurely.  The
* call is not atomic; no spinlocks may be held.
*/
int set_cpus_allowed(struct task_struct *p, cpumask_t new_mask)
{
}
EXPORT_SYMBOL_GPL(set_cpus_allowed);

int __init migration_init(void)
{
}
#endif

unsigned int max_cache_size;

void show_state_filter(unsigned long state_filter)
{
}

void show_state(void)
{
}

/**
 * init_idle - set up an idle thread for a given CPU
 * @idle: task in question
 * @cpu: cpu the idle task belongs to
 *
 * NOTE: this function does not set the idle thread's NEED_RESCHED
 * flag, to make booting more robust.
 */
void __devinit init_idle(task_t *idle, int cpu)
{
}

#ifdef CONFIG_SMP
void __init sched_init_smp(void)
{
/* blablabla */
}
#else
/* empty */
void __init sched_init_smp(void)
{
}
#endif /* CONFIG_SMP */

int in_sched_functions(unsigned long addr)
{
}

void __init sched_init(void)
{
}

#ifdef CONFIG_DEBUG_SPINLOCK_SLEEP
void __might_sleep(char * file, int line)
{
}
EXPORT_SYMBOL(__might_sleep);
#endif

#ifdef CONFIG_MAGIC_SYSRQ
void normalize_rt_tasks(void)
{
}
#endif /* CONFIG_MAGIC_SYSRQ */


