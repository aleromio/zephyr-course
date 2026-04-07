#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>

//#define SLEEP_TIME_MS 200

#ifdef CONFIG_ADVANCED_LED_SETTINGS  // Advanced settings for PWM LED.
static const struct pwm_dt_spec led_pwm = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

/** @brief States for led_pwm FSM. */
typedef enum {
    LED_PWM_CHECK_CUSTOM_BLINK, 
    LED_PWM_NORMAL_BLINK_INCREASE,
    LED_PWM_NORMAL_BLINK_DECREASE,
    LED_PWM_CUSTOM_BLINK_INCREASE,
    LED_PWM_CUSTOM_BLINK_DECREASE,
    LED_PWM_CUSTOM_BLINK_DELAY   // El prefijo común (LED_STATE_) ayuda a evitar choques
} led_pwm_state_t;         // Nombre del tipo en snake_case y suele terminar en _t

led_pwm_state_t led_pwm_state = LED_PWM_CHECK_CUSTOM_BLINK;
uint32_t step_time_ms = 20; 
uint32_t total_steps = CONFIG_LED_FADE_DURATION / step_time_ms;
uint32_t period = led_pwm.period;
uint32_t configured_brightness = (period * CONFIG_LED_BRIGHTNESS) / 100;
uint32_t step = (total_steps > 0) ? configured_brightness / total_steps : 0;
uint32_t actual_brightness = configured_brightness;

#else
/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

#endif // CONFIG_ADVANCED_LED_SETTINGS


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

void led_pwm_tick(void);

int main(void)
{
#ifdef CONFIG_ADVANCED_LED_SETTINGS  // Advanced settings for PWM LED.
    if (!pwm_is_ready_dt(&led_pwm)) return 0;
    
    while(1){
        led_pwm_tick();
        k_msleep(step_time_ms);
    }
#else    // No advanced settings so normal led blinking.
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_LED_SLEEP_TIME);
    }
#endif

    return 0;
}

#ifdef CONFIG_ADVANCED_LED_SETTINGS  // Advanced settings for PWM LED.
/**
 * @brief FSM for led PWM. Normal of cutsom blink deppending on CONFIG_ADVANCED_LED_SETTINGS
 */
void led_pwm_tick(void){

    static uint8_t counter = 0;

    pwm_set_dt(&led_pwm, period, actual_brightness);
    
    switch (led_pwm_state){    
        
    case LED_PWM_CHECK_CUSTOM_BLINK:
        if(IS_ENABLED(CONFIG_CUSTOM_BLINK_PATTERN)){
            led_pwm_state = LED_PWM_CUSTOM_BLINK_INCREASE;
        }
        else{
            led_pwm_state = LED_PWM_NORMAL_BLINK_INCREASE;
        }
        if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING)) {
            LOG_INF("DEBUG: Brillo al %d%%, Fade Duration: %u , Total de Pasos: %u, Paso %u" , CONFIG_LED_BRIGHTNESS, CONFIG_LED_FADE_DURATION, total_steps, step);
        }
        break;

    case LED_PWM_NORMAL_BLINK_INCREASE:
        actual_brightness += step;
        if (actual_brightness >= configured_brightness) {
            actual_brightness = configured_brightness;
            led_pwm_state = LED_PWM_NORMAL_BLINK_DECREASE; // Start to decrease  duty cicle.
            if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING)) {
                LOG_INF("LED state: MAX");
            }
        }
        break;

    case LED_PWM_NORMAL_BLINK_DECREASE:
        if (actual_brightness <= step) {
            actual_brightness = 0;
            led_pwm_state = LED_PWM_NORMAL_BLINK_INCREASE;  // Start to increase duty cicle.
            if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING)) {
                LOG_INF("LED state: OFF");
            }
        } else {
            actual_brightness -= step;
        }
        break;

    case LED_PWM_CUSTOM_BLINK_INCREASE:
        actual_brightness += 2*step;
        if (actual_brightness >= configured_brightness) {
            actual_brightness = configured_brightness;
            led_pwm_state = LED_PWM_CUSTOM_BLINK_DECREASE; // Start to decrease  duty cicle.
            if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING)) {
                LOG_INF("LED state: MAX");
            }
        }
        break;

    case LED_PWM_CUSTOM_BLINK_DECREASE:
        if (actual_brightness <= step) {
            actual_brightness = 0;
            counter++;
            if(counter < 2){
                led_pwm_state = LED_PWM_CUSTOM_BLINK_INCREASE;  // Start to increase duty cicle.
            }
            else{
                led_pwm_state = LED_PWM_CUSTOM_BLINK_DELAY;  // Delay CONFIG_LED_FADE_DURATION.
                counter = 0;
            }
        if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING)) {
            LOG_INF("LED state: OFF");
        }
        } else {
            actual_brightness -= 2*step;
        }
        break;

    case LED_PWM_CUSTOM_BLINK_DELAY:
        if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING)) {
            LOG_INF("Custom Blink Delay");
        }
        k_msleep(CONFIG_LED_FADE_DURATION-step_time_ms);  // Delay one fade duration.
        led_pwm_state = LED_PWM_CUSTOM_BLINK_INCREASE;  // Start to increase duty cicle.
        break;

    default:
        led_pwm_state = LED_PWM_CHECK_CUSTOM_BLINK;
        break;

    }
}
#endif // CONFIG_ADVANCED_LED_SETTINGS