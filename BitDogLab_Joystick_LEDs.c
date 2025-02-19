// =============================================================================
// INCLUDES E CONFIGURAÇÕES INICIAIS
// =============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "pico/time.h"      
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "Inc/ssd1306.h"    

// =============================================================================
// DEFINIÇÕES DOS PINOS
// =============================================================================
#define LED_VERDE       11
#define LED_AZUL        12
#define LED_VERMELHO    13

#define BOTAO_A         5
#define BOTAO_JOYSTICK  22   

#define JOYSTICK_X      26
#define JOYSTICK_Y      27

#define I2C_SDA_PIN     14
#define I2C_SCL_PIN     15
#define PORTA_I2C       i2c1

#define ENDERECO        0x3C
#define LARGURA         128
#define ALTURA          64
#define TAM_QUADRADO    8

// Constantes para o centro do joystick
#define CENTRO_X        2006
#define CENTRO_Y        2101

// =============================================================================
// VARIÁVEIS GLOBAIS DE CONTROLE
// =============================================================================
volatile bool pwm_ativado = true;       // true = ativado, false = desativado
volatile bool estado_led_verde = false;   // LED verde ligado/desligado
volatile bool estado_borda = true;        // true = borda desenhada

volatile uint32_t tempo_ultimo_botao_A = 0;
volatile uint32_t tempo_ultimo_botao_joystick = 0;

ssd1306_t ssd;   // Instância do display SSD1306

// =============================================================================
// FUNÇÃO DE ANTIRREBOTE (DEBOUNCE)
// =============================================================================
bool verificar_debounce(volatile uint32_t *ultimo_tempo) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - *ultimo_tempo > 100) {
        *ultimo_tempo = tempo_atual;
        return true;
    }
    return false;
}

// =============================================================================
// ROTINAS DE INTERRUPÇÃO PARA OS BOTÕES
// =============================================================================
void interrupcao_botao(uint gpio, uint32_t eventos) {
    if (gpio == BOTAO_A && verificar_debounce(&tempo_ultimo_botao_A)) {
        pwm_ativado = !pwm_ativado;
        printf("Botão A: PWM %s\n", pwm_ativado ? "ativado" : "desativado");
    }
    
    if (gpio == BOTAO_JOYSTICK && verificar_debounce(&tempo_ultimo_botao_joystick)) {
        estado_led_verde = !estado_led_verde;
        gpio_put(LED_VERDE, estado_led_verde);
        estado_borda = !estado_borda;
        printf("Botão Joystick: LED Verde %s, Borda %s\n",
               estado_led_verde ? "ligado" : "desligado",
               estado_borda ? "desenhada" : "não desenhada");
    }
}

// =============================================================================
// FUNÇÃO PARA CONFIGURAR O PWM
// =============================================================================
void configurar_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice_num, 4095);
    pwm_set_enabled(slice_num, true);
}

// =============================================================================
// FUNÇÃO PARA MAPEAR O VALOR DO ADC EM INTENSIDADE
// =============================================================================
uint16_t intensidade_led(uint16_t valor, uint16_t centro) {
    if (valor == centro) {
        return 0;  // Posição central: nenhum brilho
    } else if (valor < centro) {
        return centro - valor;
    } else {
        return valor - centro;
    }
}

// =============================================================================
// FUNÇÃO PRINCIPAL (main)
// =============================================================================
int main() {
    // Inicialização do sistema e UART
    stdio_init_all();
    printf("Iniciando sistema...\n");

    // -------------------------------------------------------------------------
    // Inicialização do ADC para o Joystick
    // -------------------------------------------------------------------------
    printf("Ligando ADC para o Joystick...\n");
    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);

    // -------------------------------------------------------------------------
    // Configuração dos LEDs para PWM (LED Vermelho e LED Azul)
    // -------------------------------------------------------------------------
    printf("Ligando LEDs com PWM (Vermelho e Azul)...\n");
    configurar_pwm(LED_VERMELHO);
    configurar_pwm(LED_AZUL);

    // LED Verde (controlado digitalmente)
    printf("Ligando LED Verde (digital)...\n");
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // -------------------------------------------------------------------------
    // Configuração dos Botões com Interrupções
    // -------------------------------------------------------------------------
    printf("Ligando botões com interrupções...\n");
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_JOYSTICK);
    gpio_set_dir(BOTAO_JOYSTICK, GPIO_IN);
    gpio_pull_up(BOTAO_JOYSTICK);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao_botao);
    gpio_set_irq_enabled_with_callback(BOTAO_JOYSTICK, GPIO_IRQ_EDGE_FALL, true, &interrupcao_botao);

    // -------------------------------------------------------------------------
    // Inicialização do I2C e Configuração do Display SSD1306
    // -------------------------------------------------------------------------
    printf("Ligando Display SSD1306 via I2C...\n");
    i2c_init(PORTA_I2C, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    ssd1306_init(&ssd, LARGURA, ALTURA, false, ENDERECO, PORTA_I2C);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // -------------------------------------------------------------------------
    // Posição Inicial do Quadrado no Display (Centralizado)
    // -------------------------------------------------------------------------
    uint8_t posicao_x = (LARGURA - TAM_QUADRADO) / 2;
    uint8_t posicao_y = (ALTURA - TAM_QUADRADO) / 2;

    // -------------------------------------------------------------------------
    // Loop Principal do Sistema
    // -------------------------------------------------------------------------
    while (1) {
        // Leitura dos valores do Joystick (inverte o eixo Y para movimento natural)
        adc_select_input(1);   // Eixo X
        uint16_t valor_x = adc_read();
        adc_select_input(0);   // Eixo Y
        uint16_t valor_y = adc_read();

        // ---------------------------------------------------------------------
        // Controle dos LEDs PWM: mapeia intensidade conforme o ADC e o centro
        // ---------------------------------------------------------------------
        if (pwm_ativado) {
            uint16_t intensidade_vermelho = intensidade_led(valor_x, CENTRO_X);
            uint16_t intensidade_azul = intensidade_led(valor_y, CENTRO_Y);
            pwm_set_gpio_level(LED_VERMELHO, intensidade_vermelho);
            pwm_set_gpio_level(LED_AZUL, intensidade_azul);
        } else {
            pwm_set_gpio_level(LED_VERMELHO, 0);
            pwm_set_gpio_level(LED_AZUL, 0);
        }

        // ---------------------------------------------------------------------
        // Atualização da Posição do Quadrado no Display
        // ---------------------------------------------------------------------
        posicao_x = (valor_x * (LARGURA - TAM_QUADRADO)) / 4095;
        posicao_y = ((4095 - valor_y) * (ALTURA - TAM_QUADRADO)) / 4095;

        // ---------------------------------------------------------------------
        // Desenho no Display: limpa, desenha o quadrado e a borda (se aplicável)
        // ---------------------------------------------------------------------
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, posicao_y, posicao_x, TAM_QUADRADO, TAM_QUADRADO, true, true);
        ssd1306_rect(&ssd, 0, 0, LARGURA, ALTURA, estado_borda, false);
        ssd1306_send_data(&ssd);

        sleep_ms(10);
    }
    
    return 0;
}
