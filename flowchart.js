// This is a flowchart using the plugin flowmaker.  It uses javascript to create a flowchart

function init() {
    block_time = 500;
    average_time = 2000;
    num_magnets = 1;
    num_blocks = average_time / block_length;

    time_blocks = Array(num_blocks);
    active_block = 0;

    diameter = 27;
    circumference = diameter * pi;

    sleep_time = 5 * 60;

    set_timer_interval(block_time);
}

function on_timer() {
    distance += time_blocks[active_block];
    odom = sum(time_blocks) / average_time;
    odom /= num_magnets;
    if (mode == speedometer) {
        speed = odom * circumference * rps_to_unit;
    } else if(mode == distance) {
        display_distance = distance * circumference;
        display_distance *= rps_to_unit;
    }

    active_block += 1;
    if (active_block > num_blocks) {
        active_block = 0;
    }
    time_blocks[active_block] = 0;

    update_display();

    sleep_if_not_moved(sleep_time);
}

function on_wheel_rotation() {
    wake_up_if_asleep();
    time_blocks[active_block] += 1;
}

function on_key_press(key) {
    wake_up_if_asleep();
    if (key == mode) {
        switch(mode) {
        case odom: mode = speed;
        case speed: mode = distance;
        case distance: mode = odom;
        }
    } else if (key == unit) {
        switch(unit) {
        case metric: unit = imperial;
        case imperial: unit = metric;
        }
    } else if (key == reset_distance) {
        distance = 0;
    }
}