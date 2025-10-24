#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    // reset to zero at start (optional but nice for demos)
    int w = open("/dev/encoder0", O_WRONLY);
    if (w >= 0) { write(w, "reset", 5); close(w); }

    int fd = open("/dev/encoder0", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    while (1) {
        int64_t cnt = 0;                         // <-- signed
        ssize_t n = read(fd, &cnt, sizeof(cnt)); // read 8 bytes
        if (n != sizeof(cnt)) { perror("read"); break; }

        printf("count=%" PRId64 "\n", cnt);      // <-- print as signed
        usleep(100000);
    }
}
