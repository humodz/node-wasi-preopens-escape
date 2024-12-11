#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

int main() {
	char cwd[100] = { 0 };
   	getcwd(cwd, sizeof(cwd));

   	printf("cwd is %s\n", cwd);

	int fd = open("./inside.txt", O_RDONLY);

	if (fd < 0) {
		printf("Error %s\n", strerror(errno));
		return 0;
	}

	char buf[64] = { 0 };
	read(fd, buf, (sizeof buf) - 1);

	printf("Data: %s\n", buf);

	close(fd);
	return 0;
}