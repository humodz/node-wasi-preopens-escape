#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

int main() {
	while (1) {
		int fd = open("/inside.txt", O_RDONLY);

		if (fd < 0) {
			printf("Error %s\n", strerror(errno));
			continue;
		}

		char buf[64] = { 0 };
		read(fd, buf, (sizeof buf) - 1);

		printf("Data: %s\n", buf);

		close(fd);
	}
	return 0;
}