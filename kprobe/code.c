#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>		// for basic filesystem
#include <linux/proc_fs.h>	// for the proc filesystem
#include <linux/seq_file.h>	// for sequence files
#include <linux/jiffies.h>	// for jiffies
#include <linux/slab.h>		// kmalloc and friends
#include <linux/list.h>		// link list
#include <linux/wait.h>		// Wait queue
#include <linux/kprobes.h>	// kprobe
#include <linux/ptrace.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kprobe - With hardcoded function probe");
MODULE_VERSION("110");
MODULE_AUTHOR("Arshad Hussain <arshad.super@gmail.com>");

/*
 * Three methods to get the address of the kernel fucntion.
 *
 * # cat /proc/kallsyms | grep -w submit_bio
 * ffffffff99b1b630 T submit_bio
 *
 * # nm /usr/lib/debug/lib/modules/3.10.0-862.9.1.el7_lustre.x86_64/vmlinux | grep -w submit_bio
 * ffffffff8131b630 T submit_bio
 *
 * # cat /usr/src/kernels/3.10.0-862.9.1.el7_lustre.x86_64/System.map | grep -w submit_bio
 * ffffffff8131b630 T submit_bio
 */

#define ADDRESS 0xffffffff8131b630 /* submit_bio - see above comment on how to get them. */

int pre_handler(struct kprobe *probe, struct pt_regs *regs)
{
	printk(KERN_INFO "Inside pre_handler.\n");
	return 0;
}

void post_handler(struct kprobe *probe, struct pt_regs *regs, unsigned long flags)
{
	printk(KERN_INFO "Inside POST_handler.\n");
}


static struct kprobe kp;


static int __init start(void)
{
	printk(KERN_INFO "Entering.....\n");
	kp.pre_handler = pre_handler;
	kp.post_handler = post_handler;
	/* provide address directly (hardcoded) */
	kp.addr = (kprobe_opcode_t *)ADDRESS;
	register_kprobe(&kp);
	return 0;
}

static void __exit stops(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "Exiting.....\n");
}

module_init(start);
module_exit(stops);
