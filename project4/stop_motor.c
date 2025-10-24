#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <gpiod.h>

int main(void) {
    // ----- Stop PWM (GPIO12) -----
    const char *duty = "/sys/class/pwm/pwmchip0/pwm0/duty_cycle";
    const char *enable = "/sys/class/pwm/pwmchip0/pwm0/enable";

    int fd = open(duty, O_WRONLY);
    if (fd >= 0) { write(fd, "0", 1); close(fd); }

    fd = open(enable, O_WRONLY);
    if (fd >= 0) { write(fd, "0", 1); close(fd); }

    // ----- Set direction pin (GPIO13) LOW -----
    struct gpiod_chip *chip = gpiod_chip_open_by_name("gpiochip0");
    if (chip) {
        struct gpiod_line *dir = gpiod_chip_get_line(chip, 13);
        if (dir) {
            if (gpiod_line_request_output(dir, "stop-motor", 0) == 0)
                gpiod_line_set_value(dir, 0);
            gpiod_line_release(dir);
        }
        gpiod_chip_close(chip);
    }

    printf("Motor stopped: PWM duty=0, disabled, direction=LOW\n");
    return 0;
}
