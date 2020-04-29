#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage long sys_my_gettime(void) {
    struct timespec t;
    getnstimeofday(&t);
    return t.tv_sec * (long)1e9 + t.tv_nsec;
}
