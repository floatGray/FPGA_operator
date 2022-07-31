#define _INIT_GLOBLE

#include "include.h"

/*
inotify_add_watch使用注意
1.inotify_add_watch返回并不是一个fd，而只是一个标识
2.对于同一个PATH，inotify_add_watch将返回相同的标识。不需要调用close关闭，但需要调用inotify_rm_watch来删除。

在对文件进行读、写、关闭监控时需要注意这个特性。但inotify_init返回的是一个真正的fd，因此需要调用close关闭它。
但是如果相同PATH，但是是add和rm交替进行的，则不会重复，而且是从1递增。
在未rm上一个之前对同一个PATH进行add_watch，实际只是进行修改，返回值是不会变的。
*/

void pic_monitor_init(void){
	int result;

	/* 初始化一个inotify的实例，获得一个该实例的文件描述符 */
	inotify_fd = inotify_init();
	assert(inotify_fd >= 0);

#ifdef INOTIFY_ENABLE
	/* 添加一个用于监视的目录:主要监视该目录中文件的添加 */
	result = inotify_add_watch(inotify_fd, INOTIFY_PATH, IN_ALL_EVENTS);
	assert(result != -1);
#endif
}

#undef _INIT_GLOBLE
