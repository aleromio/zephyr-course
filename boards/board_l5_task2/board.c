#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(init, LOG_LEVEL_INF);

static int board_board_l5_task2_init(void){

    LOG_INF("Board Initialized"); 

    return 0;
}

SYS_INIT(board_board_l5_task2_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);