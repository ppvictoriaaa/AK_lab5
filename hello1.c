#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Hello1 module");
MODULE_AUTHOR("IO-24 Victoria Pekur");

static LIST_HEAD(hello_list);

void print_hello(int hello_count)
{
    struct hello_entry *entry;
    int i;

    if (hello_count < 0 || hello_count > 10) {
        pr_err("Error! hello_count is greater than 10 or less than 0: %d\n", hello_count);
        return;
    }

    if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
        pr_warn("Warning! hello_count is %u: The number is 0 or > 5 and < 10\n");
    }

    for (i = 0; i < hello_count; i++) {
        entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry) {
            pr_err("Failed to allocate memory\n");
            return;
        }

        entry->start_time = ktime_get();
        pr_info("Hello, world!!!\n");
        entry->end_time = ktime_get();
        list_add_tail(&entry->list, &hello_list);
    }
}

EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
    struct hello_entry *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &hello_list, list) {
        pr_info("Print duration: %lld ns\n",
                ktime_to_ns(entry->end_time) - ktime_to_ns(entry->start_time));
        list_del(&entry->list);
        kfree(entry);
    }
}

module_exit(hello1_exit);
