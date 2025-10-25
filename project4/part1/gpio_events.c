#include <gpiod.h> // All of the GPIO stuff...
#include <unistd.h> // usleep()
int main(int argc, char **argv)
{
    const char *chipname = "gpiochip0";
    struct gpiod_chip *chip;
    struct gpiod_line *led;
    chip = gpiod_chip_open_by_name(chipname);
    

    int ret;
    int state = 1;

    struct gpiod_line *input_line;
    struct gpiod_line_event event;

    unsigned int line_num = 4; /* GPIO Pin #4 */
    unsigned int line_num_out = 17; /* GPIO Pin #17 */

    led = gpiod_chip_get_line(chip, line_num_out);
    gpiod_line_request_in
    put(led, "example1", 0);




    input_line = gpiod_chip_get_line(chip, line_num);




    ret = gpiod_line_request_rising_edge_events(input_line, "gpio-test");


    while (1) {
        gpiod_line_event_wait(input_line, NULL); /* blocking */

        if (gpiod_line_event_read(input_line, &event) != 0)
        continue;

        /* should always be a rising event in our example */

        // if (event.event_type != GPIOD_LINE_EVENT_RISING_EDGE)
        // continue;

        state = !(state);

        gpiod_line_set_value(led, state);



    }


    gpiod_line_release(input_line);

    gpiod_chip_close(chip);

    return 0;
}