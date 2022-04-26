#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char *program, char **args) {
	if(fork() == 0) { // child exec
		exec(program, args);
		exit(0);
	}
	return; // parent return
}

int main(int argc, char *argv[]){
	char buf[2048];
	char *p = buf;

	char *last_p = p;
	
	char *argsbuf[32];
	char **argsbuf_index = argsbuf;

	for(int i=1;i<argc;i++) {
		*argsbuf_index = argv[i];
		argsbuf_index++;
	}
	
	char **argsbuf_index_start = argsbuf_index;
	
	int is_end = 0;
	while(read(0, p, 1)) {
		if(*p == '\0') is_end = 1;
		
		if(*p == '\n' || (*p == '\0' && argsbuf_index_start != argsbuf_index)) {
			*p = '\0';
			*(argsbuf_index_start++) = last_p;
			
			*argsbuf_index_start = 0; // 参数列表末尾用 null 标识列表结束
			run(argv[1], argsbuf); // 执行最后一行指令
			argsbuf_index_start = argsbuf_index; // 重置读入参数指针，准备读入下一行
		}

		if(is_end) break;

		if(*p == ' ') {
			*p = '\0';
			*(argsbuf_index_start++) = last_p;
			last_p = p+1;
		}
		p++;
	}
	
	while(wait(0) != -1) {}; // 循环等待所有子进程完成，每一次 wait(0) 等待一个
	
	exit(0);
}