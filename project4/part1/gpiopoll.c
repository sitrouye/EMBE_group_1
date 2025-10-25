#include <gpiod.h> // All of the GPIO stuff...
#include <unistd.h> // usleep()
#include <stdio.h>
int main(int argc, char **argv)
{
    const char *chipname = "gpiochip0";
    struct gpiod_chip *chip;
    struct gpiod_line *led;
    chip = gpiod_chip_open_by_name(chipname);
    int state = 0;
    struct gpiod_line *input_line;
    unsigned int line_num = 4; /* GPIO Pin #4 */
    unsigned int line_num_out = 17; /* GPIO Pin #17 */

    led = gpiod_chip_get_line(chip, line_num_out);
    gpiod_line_request_output(led, "example1", 0);

    input_line = gpiod_chip_get_line(chip, line_num);
    // gpiod_line_request_input(input_line, "example1");
    gpiod_line_request_output(input_line, "change",0);

    while (1) {
        usleep(1000000);
        state = !state;
        gpiod_line_set_value(input_line, state);
        state = gpiod_line_get_value(input_line); 
        gpiod_line_set_value(led, state);
    }

    gpiod_line_release(input_line);
    gpiod_line_set_value(led, 0);
    gpiod_line_release(led);
    gpiod_chip_close(chip);
    return 0;
}