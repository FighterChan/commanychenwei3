/* 
 * list_jhash.c
 * Original Author: chenwei3@ruijie.com.cn, 2018-11-16
 *
 * 
 * 
 *
 * History 
 * 
 *  
 */
#include <linux/init.h>

#include <linux/module.h>

#include <linux/moduleparam.h>

#include <linux/list.h>

#include <linux/jhash.h>

#include <linux/ctype.h>

#include <linux/proc_fs.h>

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static unsigned int ip = 123456789;

module_param(ip, uint, 0);

static unsigned int port = 1;

module_param(port, uint, 0);

#define TCCOUNT_SIZE 255

struct tccount

{

	struct list_head list;

	unsigned int ip;

	unsigned short port;

	unsigned short counter;

};

struct tccount tclist[TCCOUNT_SIZE];

unsigned int gethash(unsigned int ip, unsigned int port)

{

	return jhash_2words(ip, port, 4) % TCCOUNT_SIZE;

}

int tccount_init(void)

{

	int i;

	for (i = 0; i < TCCOUNT_SIZE; i++)

	{

		INIT_LIST_HEAD(&(tclist[i].list));

		tclist[i].ip = 0;

		tclist[i].port = 0;

		tclist[i].counter = 0;

	}

	return 0;

}

int addNewStream(unsigned int sip, unsigned int sport)

{

	unsigned int hash;

	struct tccount *stream;

	hash = gethash(sip, sport);

	list_for_each_entry(stream, &(tclist[hash].list), list)

	{

		if (stream && stream->ip == sip && stream->port == sport)

		{

			stream->counter++;

			return 0;

		}

	}

	stream = kmalloc(sizeof(struct tccount), GFP_KERNEL);

	stream->ip = sip;

	stream->port = sport;

	stream->counter = 1;

	list_add_rcu(&(stream->list), &(tclist[hash].list));

	return 0;

}

int delStream(unsigned int sip, unsigned int sport)

{

	unsigned int hash;

	struct tccount *stream;

	hash = gethash(sip, sport);

	list_for_each_entry(stream, &(tclist[hash].list), list)

	{

		if (stream->ip == sip && stream->port == sport)

		{

			stream->counter--;

			if (stream->counter == 0 && stream != &tclist[hash])

			{

				list_del_rcu(&(stream->list));

				kfree(&stream);

			}

		}

	}

	return 0;

}

unsigned short getStreamCount(unsigned int sip, unsigned int sport)

{

	unsigned int hash;

	struct tccount *stream;

	hash = gethash(sip, sport);

	list_for_each_entry(stream, &(tclist[hash].list), list)

	{

		if (stream->ip == sip && stream->port == sport)

			return stream->counter;

	}

	return 0;

}

/*debug proc*/

static int proc_show_tccount(struct file *f, const char *buf, unsigned long cnt,
		void *data)

{

	char input[32];

	int r;

	unsigned int ip;

	unsigned int port;

	unsigned short count;

	if (copy_from_user(input, buf, cnt) != 0)

		return -EFAULT;

	r = cnt;

	sscanf(input, "%u %u", &ip, &port);

	count = getStreamCount(ip, port);

	printk(KERN_ALERT "(%d, %d) counter:%u/n",ip, port, count);

	return cnt;

}

static int proc_set_tccount(struct file *f, const char *buf, unsigned long cnt,
		void *data)

{

	char input[32];

	int r;

	unsigned int ip;

	unsigned int port;

	unsigned int count;

	if (copy_from_user(input, buf, cnt) != 0)

		return -EFAULT;

	r = cnt;

	sscanf(input, "%u %u", &ip, &port);

	count = addNewStream(ip, port);

	printk(KERN_ALERT "(%d, %d) has been added/n", ip, port);

	return cnt;

}

static int proc_del_tccount(struct file *f, const char *buf, unsigned long cnt,
		void *data)

{

	char input[32];

	int r;

	unsigned int ip;

	unsigned int port;

	unsigned int count;

	if (copy_from_user(input, buf, cnt) != 0)

		return -EFAULT;

	r = cnt;

	sscanf(input, "%u %u", &ip, &port);

	count = delStream(ip, port);

	printk(KERN_ALERT "a stream of (%d, %d) has been deleted/n", ip, port);

	return cnt;

}

static int proc_add(void)

{

	char tmp[32];

	struct proc_dir_entry *proc_tcctl, *pentry;

	sprintf(tmp, "tccount");

	proc_tcctl = proc_mkdir(tmp, NULL);

	sprintf(tmp, "show");

	pentry = create_proc_entry(tmp, 0644, proc_tcctl);

	pentry->data = NULL;

	pentry->read_proc = NULL;

	pentry->write_proc = proc_show_tccount;

	pentry->owner = THIS_MODULE;

	sprintf(tmp, "del");

	pentry = create_proc_entry(tmp, 0644, proc_tcctl);

	pentry->data = NULL;

	pentry->read_proc = NULL;

	pentry->write_proc = proc_del_tccount;

	pentry->owner = THIS_MODULE;

	sprintf(tmp, "add");

	pentry = create_proc_entry(tmp, 0644, proc_tcctl);

	pentry->data = NULL;

	pentry->read_proc = NULL;

	pentry->write_proc = proc_set_tccount;

	pentry->owner = THIS_MODULE;

	return 0;

}

static int hello_init(void)

{

	tccount_init();

	proc_add();

	return 0;

}

static void hello_exit(void)

{

	printk("Goodbye!/n");

}

module_init( hello_init);

module_exit( hello_exit);

