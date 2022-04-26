// primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int pleft[2]) {
	int p;
	read(pleft[0], &p, sizeof(p));
	if(p == 0) {
		exit(0);
	}
	printf("prime %d\n", p);

	int pright[2];
	pipe(pright);

	if(fork() == 0) {
		// 子进程
		close(pright[1]);
		close(pleft[0]);
		sieve(pright);
	} else {
		// 父进程
		close(pright[0]);
		int buf;
		while(read(pleft[0], &buf, sizeof(buf)) && buf != 0) {
			if(buf % p != 0) {
				write(pright[1], &buf, sizeof(buf));
			}
		}
		buf = 0;
		write(pright[1], &buf, sizeof(buf));
		wait(0);
		exit(0);
	}
}

int main(int argc, char **argv) {
	int p[2];
	pipe(p);

	int i;
	for(i = 2; i <= 35; i++ ){
		write(p[1], &i, sizeof(i));
	}
	i = 0;
	write(p[1], &i, sizeof(i)); // 0用于标识输入完成

	sieve(p);

	exit(0);
}
