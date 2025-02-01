/*
 * Por: Daniel Porto Braz
 * 
 * O código a seguir exibe padrões numéricos na matriz de LEDs (WS2812), com botões de interrupção e um LED
 * RGB separado, baseado nos modelos fornecidos pelo professor Wilton Lacerda no curso de capacitação em 
 * Sistemas Embarcados Embarcatech.
 * 
 * Link do repositório de referência:
 * https://github.com/wiltonlacerda/EmbarcaTechU4C4.git
 */


// Bibliotecas para operações com PIO, interrupção e temporizadores
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

// Biblioteca criada para os padrões numéricos de 0 a 9 no modelo da matriz 5x5
#include "numeros.h"

// Definições básicas para a matriz de LEDs 5x5
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

// Definições de pinos de LEDs e botões
#define LED_RED_PIN 13
#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6


// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 200; // Intensidade do vermelho: 200
uint8_t led_g = 0; // Intensidade do verde: 0
uint8_t led_b = 0; // Intensidade do azul: 0


// Buffer para armazenar quais LEDs estão ligados/desligados na matriz 5x5
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Passagem dos 32 bits de brilho do LED RGB
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


// Liga/desliga os LEDs da matriz na sequência
void set_one_led(bool *matriz, uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (matriz[24 - i])
        {
            put_pixel(color); // Liga o LED com 1 no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com 0 no buffer
        }
    }
}


// Inicializa os LEDs 
void init_leds(){
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
}

// Inicializa os botões com pull-up
void init_buttons(){
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}


// Protótipo do callback de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Armazena o numero atual da contagem de 0 a 9
static volatile uint8_t cont = 0;

// Armazena o último momento que o botão foi pressionado
static volatile uint32_t last_time = 0; 



int main()
{
    //Variáveis par configurar o PIO
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    init_leds();
    init_buttons();

    // Interrupções dos botões A e B
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1)
    {
        // Desenha o número na matriz de LEDs
        set_one_led(nums[cont], led_r, led_g, led_b);
        
        // Pisca o LED RGB 5 vezes por segundo
        gpio_put(LED_RED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_RED_PIN, 0);
        sleep_ms(100);
    }

    return 0;
}


void gpio_irq_handler(uint gpio, uint32_t events){
    
    // Guarda o tempo em us desde o boot do sistema
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if(current_time - last_time > 200) { // Evita que ocorram leituras em intervalos menores de 200 ms
        last_time = current_time;

        // Se o botão A foi pressionado e não foi atingido o número 9, incrementa-se o número exibido
        if(!gpio_get(BUTTON_A_PIN) && cont < 9)
            cont++;

        // Se o botão B foi pressionado e não foi atingido o número 0, decrementa-se o número exibido
        else if(!gpio_get(BUTTON_B_PIN) && cont > 0)
            cont--;
    }
}   