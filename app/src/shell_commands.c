#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h> 

static const struct  device* ledsensor_driver = DEVICE_DT_GET(DT_NODELABEL(ledsensor0));   // get my driver

// shell commands

// Root command sensor
static int cmd_sensor(const struct shell *sh, size_t argc, char ** argv){
    shell_print(sh,"Use Subcommands !!");
    return 0;
}

// Subcomand sensor fetch
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char ** argv){
    sensor_sample_fetch(ledsensor_driver);
    shell_print(sh,"LED_ON from Shell !!");
    return 0;
}

// Subcommand sensor read
static int cmd_sensor_read(const struct shell *sh, size_t argc, char ** argv){
    sensor_channel_get(ledsensor_driver,SENSOR_CHAN_ALL, NULL);
    shell_print(sh,"LED_OFF from Shell !!");
    return 0;
}

// Subcomand info
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Device name:  %s", ledsensor_driver->name);
    if(ledsensor_driver->state->init_res == 0){
        shell_print(sh, "Ready state: YES");   
    }
    else{
        shell_print(sh, "Ready state: NO");   
    }
    return 0;
}

// create sub command array
SHELL_STATIC_SUBCMD_SET_CREATE(sensor_commands,
    SHELL_CMD(fetch, NULL, "Use aleromio_ledsensor sample_fetch to turn led ON", cmd_sensor_fetch),
    SHELL_CMD(read,  NULL, "Use aleromio_ledsensor channel_get to turn led OFF", cmd_sensor_read),
    SHELL_CMD(info,  NULL, "Information of the ledsensor",  cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

// Registro del comando raíz
SHELL_CMD_REGISTER(sensor, &sensor_commands, "Ledsensor commands. See subcomands help.", cmd_sensor);
