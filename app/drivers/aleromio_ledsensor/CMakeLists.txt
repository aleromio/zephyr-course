if(CONFIG_ALEROMIO_LEDSENSOR)
    target_sources(app PRIVATE aleromio_ledsensor.c)
endif()

zephyr_include_directories(include)