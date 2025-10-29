#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), "@theengs/ble");
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        system("am startservice --user 0 -n com.theengs.bridge/.BleService");
        sleep(2);
        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            fprintf(stderr, "BLE service unavailable\n");
            return 1;
        }
    }
    FILE *f = fdopen(fd, "r");
    char line[2048];
    while (fgets(line, sizeof(line), f)) {
        fputs(line, stdout);
        fflush(stdout);
    }
    return 0;
}
