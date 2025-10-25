#!/bin/bash
# Short script to toggle a GPIO pin at the highest
# frequency possible from Bash using gpiod
COUNTER=0
while [ $COUNTER -lt 10000 ]; do
    gpioset 0 4=1
    state=$(gpioget 0 4)
    echo $state
    gpioset 0 17=$state
    let COUNTER=COUNTER+1

    gpioset 0 4=0
    state=$(gpioget 0 4)
    echo $state
    gpioset 0 17=$state
    let COUNTER=COUNTER+1
done