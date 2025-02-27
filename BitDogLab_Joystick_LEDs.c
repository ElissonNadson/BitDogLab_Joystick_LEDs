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
#define LED_VERDE       11  // Umidade ideal
#define LED_AZUL        12  // Umidade baixa
#define LED_VERMELHO    13  // Umidade alta

#define BOTAO_A         5   // Liga/desliga o sistema
#define BOTAO_JOYSTICK  22  // Alterna entre modos de operação

#define SENSOR_UMIDADE  26  // Pino ADC para simular leitura de umidade
#define JOYSTICK_Y      27  // Ajusta a umidade desejada

#define I2C_SDA_PIN     14
#define I2C_SCL_PIN     15
#define PORTA_I2C       i2c1

#define ENDERECO        0x3C
#define LARGURA         128
#define ALTURA          64
#define TAM_QUADRADO    8

// Constantes para o centro do joystick
#define CENTRO_Y        2101

// =============================================================================
// VARIÁVEIS GLOBAIS DE CONTROLE
// =============================================================================
volatile bool sistema_ligado = true;       // true = sistema ligado, false = desligado
volatile bool modo_manual = false;         // true = modo manual, false = automático
volatile uint16_t umidade_desejada = 50;   // Umidade desejada em porcentagem
volatile uint16_t umidade_atual = 0;       // Umidade atual simulada

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
        sistema_ligado = !sistema_ligado;
        printf("Botão A: Sistema %s\n", sistema_ligado ? "ligado" : "desligado");
    }
    
    if (gpio == BOTAO_JOYSTICK && verificar_debounce(&tempo_ultimo_botao_joystick)) {
        modo_manual = !modo_manual;
        printf("Botão Joystick: Modo %s\n", modo_manual ? "manual" : "automático");
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
// FUNÇÃO PARA LER A UMIDADE ATUAL (SIMULADA)
// =============================================================================
uint16_t ler_umidade_atual() {
    adc_select_input(0);   // Lê o pino do sensor de umidade
    return (adc_read() * 100) / 4095;  // Converte para porcentagem
}

// =============================================================================
// FUNÇÃO PARA ATUALIZAR OS LEDs COM BASE NA UMIDADE
// =============================================================================
void atualizar_leds(uint16_t umidade_atual, uint16_t umidade_desejada) {
    if (umidade_atual < umidade_desejada - 10) {
        gpio_put(LED_AZUL, 1);    // Umidade baixa
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_VERMELHO, 0);
    } else if (umidade_atual > umidade_desejada + 10) {
        gpio_put(LED_VERMELHO, 1); // Umidade alta
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_AZUL, 0);
    } else {
        gpio_put(LED_VERDE, 1);   // Umidade ideal
        gpio_put(LED_AZUL, 0);
        gpio_put(LED_VERMELHO, 0);
    }
}

// =============================================================================
// FUNÇÃO PRINCIPAL (main)
// =============================================================================
int main() {
    // Inicialização do sistema e UART
    stdio_init_all();
    printf("Iniciando sistema de irrigação...\n");

    // -------------------------------------------------------------------------
    // Inicialização do ADC para o sensor de umidade e joystick
    // -------------------------------------------------------------------------
    printf("Ligando ADC para o sensor de umidade e joystick...\n");
    adc_init();
    adc_gpio_init(SENSOR_UMIDADE);
    adc_gpio_init(JOYSTICK_Y);

    // -------------------------------------------------------------------------
    // Configuração dos LEDs
    // -------------------------------------------------------------------------
    printf("Configurando LEDs...\n");
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

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
    // Loop Principal do Sistema
    // -------------------------------------------------------------------------
    while (1) {
        if (sistema_ligado) {
            // Leitura da umidade atual (simulada)
            umidade_atual = ler_umidade_atual();

            // Ajuste da umidade desejada com o joystick
            adc_select_input(1);   // Eixo Y do joystick
            uint16_t valor_y = adc_read();
            umidade_desejada = (valor_y * 100) / 4095;

            // Simulação de aumento da umidade no modo automático
            if (!modo_manual && umidade_atual < umidade_desejada) {
                umidade_atual++;
                sleep_ms(100); // Espera 100 ms antes de aumentar a umidade
            }

            // Atualização dos LEDs
            atualizar_leds(umidade_atual, umidade_desejada);

            // Exibição das informações no display
            char buffer[50];
            ssd1306_fill(&ssd, false);
            snprintf(buffer, sizeof(buffer), "Umidade: %d%%", umidade_atual);
            ssd1306_text(&ssd, buffer, 0, 0, true);
            snprintf(buffer, sizeof(buffer), "Desejada: %d%%", umidade_desejada);
            ssd1306_text(&ssd, buffer, 0, 20, true);
            snprintf(buffer, sizeof(buffer), "Modo: %s", modo_manual ? "Manual" : "Auto");
            ssd1306_text(&ssd, buffer, 0, 40, true);
            ssd1306_send_data(&ssd);
        } else {
            // Sistema desligado: apaga os LEDs e exibe mensagem no display
            gpio_put(LED_VERDE, 0);
            gpio_put(LED_AZUL, 0);
            gpio_put(LED_VERMELHO, 0);
            ssd1306_fill(&ssd, false);
            ssd1306_text(&ssd, "Sistema Desligado", 0, 20, true);
            ssd1306_send_data(&ssd);
        }

        sleep_ms(500);  // Atualiza a cada 500ms
    }
    
    return 0;
}