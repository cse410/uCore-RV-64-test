#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Seems that can't include <syscall.h>, so define SYSCALL id here
#define SYSCALL_WRITE 64
#define SYSCALL_EXIT 93
#define SYSCALL_YIELD 124
#define SYSCALL_GETTIMEOFDAY 169
#define SYSCALL_TASK_INFO 410

int main()
{
	int64 t1 = get_mtime();
	TaskInfo info;
	get_mtime();
	int64 t2 = get_mtime();
	// 注意本次 task info 调用也计入内
	assert(sys_task_info(&info) == 0);
	int64 t3 = get_mtime();
	assert(3 == info.syscall_times[SYSCALL_GETTIMEOFDAY]);
	assert(1 == info.syscall_times[SYSCALL_TASK_INFO]);
	assert(0 == info.syscall_times[SYSCALL_WRITE]);
	assert(0 == info.syscall_times[SYSCALL_YIELD]);
	assert(0 == info.syscall_times[SYSCALL_EXIT]);
	assert(t2 - t1 <= info.time);
	assert(info.time < t3 - t1 + 10);
	assert(Running == info.status);

	puts("hello world!");
	int64 t4 = get_mtime();
	// 想想为什么 write 调用是两次
	assert(sys_task_info(&info) == 0);
	int64 t5 = get_mtime();
	assert(5 == info.syscall_times[SYSCALL_GETTIMEOFDAY]);
	assert(2 == info.syscall_times[SYSCALL_TASK_INFO]);
	assert(2 == info.syscall_times[SYSCALL_WRITE]);
	assert(0 == info.syscall_times[SYSCALL_YIELD]);
	assert(0 == info.syscall_times[SYSCALL_EXIT]);
	assert(t4 - t1 <= info.time);
	assert(info.time < t5 - t1 + 10);
	assert(Running == info.status);
	return 0;
}