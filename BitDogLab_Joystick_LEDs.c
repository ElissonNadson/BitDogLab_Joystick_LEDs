#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

// Definições dos pinos conforme a atividade
#define LED_RED_PIN     11
#define LED_GREEN_PIN   12
#define LED_BLUE_PIN    13

#define BUTTON_A_PIN    5

#define JOYSTICK_X_PIN  26
#define JOYSTICK_Y_PIN  27
#define JOYSTICK_SEL_PIN 22

#define I2C_SDA_PIN     14
#define I2C_SCL_PIN     15

// Variável global para o display
ssd1306_t display;

int main() {
    // Inicializa a saída padrão (USB/UART)
    stdio_init_all();

    // Inicializa o ADC para o joystick
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Configura os pinos dos LEDs para PWM
    gpio_set_function(LED_RED_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_GREEN_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_BLUE_PIN, GPIO_FUNC_PWM);
    uint slice_red = pwm_gpio_to_slice_num(LED_RED_PIN);
    uint slice_green = pwm_gpio_to_slice_num(LED_GREEN_PIN);
    uint slice_blue = pwm_gpio_to_slice_num(LED_BLUE_PIN);
    pwm_set_wrap(slice_red, 1023);
    pwm_set_wrap(slice_green, 1023);
    pwm_set_wrap(slice_blue, 1023);
    pwm_set_enabled(slice_red, true);
    pwm_set_enabled(slice_green, true);
    pwm_set_enabled(slice_blue, true);

    // Configura o Botão A como entrada com resistor pull-up
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    // Configura o pino do botão do joystick (SEL)
    gpio_init(JOYSTICK_SEL_PIN);
    gpio_set_dir(JOYSTICK_SEL_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_SEL_PIN);

    // Inicializa o I2C para o display SSD1306
    i2c_init(i2c_default, 400 * 1000); // 400kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicializa o display SSD1306
    ssd1306_init(&display, 128, 64, false, 0x3C, i2c_default);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_draw_string(&display, "Inicializando...", 0, 0);
    ssd1306_send_data(&display);

    while (true) {
        // Leitura dos valores do joystick via ADC
        adc_select_input(0);  // Supondo que o eixo X esteja no canal 0 (GP26)
        uint16_t x_val = adc_read();
        adc_select_input(1);  // Supondo que o eixo Y esteja no canal 1 (GP27)
        uint16_t y_val = adc_read();

        // Mapeia os valores lidos (0-4095) para o intervalo do PWM (0-1023)
        uint16_t pwm_red = (x_val * 1023) / 4095;
        uint16_t pwm_blue = (y_val * 1023) / 4095;

        // Atualiza os LEDs:
        // LED vermelho controlado pelo eixo X
        pwm_set_gpio_level(LED_RED_PIN, pwm_red);
        // LED azul controlado pelo eixo Y
        pwm_set_gpio_level(LED_BLUE_PIN, pwm_blue);
        // LED verde acionado pelo Botão A: se pressionado, acende
        if (gpio_get(BUTTON_A_PIN) == 0) {
            pwm_set_gpio_level(LED_GREEN_PIN, 1023);
        } else {
            pwm_set_gpio_level(LED_GREEN_PIN, 0);
        }

        // Atualiza o display com os valores do joystick
        char buffer[32];
        sprintf(buffer, "X: %d  Y: %d", x_val, y_val);
        ssd1306_fill(&display, false);
        ssd1306_draw_string(&display, buffer, 0, 0);
        ssd1306_send_data(&display);

        sleep_ms(100);
    }
    
    return 0;
}
