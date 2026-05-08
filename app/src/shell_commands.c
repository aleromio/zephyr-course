#include <zephyr/kernel.h>
#include "aleromio_ledsensor.h"
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

static int set_parameter_in_range(char* range){

    uint8_t range_byte  = (uint8_t)atoi(range);
    if((range_byte >= 0)&&(range_byte <= 100)){
        return true;
    }
    else{
        return false;
    }
}

// Subcommand sensor set
static int cmd_sensor_set(const struct shell *sh, size_t argc, char ** argv){

    ARG_UNUSED(argc);
	ARG_UNUSED(argv);

    
    if(argc == 2){
        if(set_parameter_in_range((char *)argv[1])){
            aleromio_set_parametro(ledsensor_driver, atoi(argv[1]));   // set ledsensor data with argument value
            shell_print(sh, "Data change to:%s",argv[1]); 
        }
        else{
            shell_error(sh, "set parameter out of range, must be 0-100");   
        }
    }
    else{
         shell_error(sh, "Bad argument count");
    }
    return 0;
}

// create sub command array
SHELL_STATIC_SUBCMD_SET_CREATE(sensor_commands,
    SHELL_CMD(fetch, NULL, "Use aleromio_ledsensor sample_fetch to turn led ON", cmd_sensor_fetch),
    SHELL_CMD(read,  NULL, "Use aleromio_ledsensor channel_get to turn led OFF", cmd_sensor_read),
    SHELL_CMD(info,  NULL, "Information of the ledsensor",  cmd_sensor_info),
    SHELL_CMD_ARG(set, NULL, "Change data value of the ledsensor. Range from 0 to 100.", cmd_sensor_set, 2, 3),
    SHELL_SUBCMD_SET_END
);

// Registro del comando raíz
SHELL_CMD_REGISTER(sensor, &sensor_commands, "Ledsensor commands. See subcomands help.", cmd_sensor);
