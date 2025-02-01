# Geração de Animações em Matriz de LEDs 5x5

## Descrição
Com fim ao aprendizado com aplicações de Sistemas Embarcados, este projeto apresenta o controle de uma matriz de LEDs endereçáveis (WS2812), através de botões com efeito _Debouncing_. Adicionam-se também, a implementação de interrupção e um LED RGB que pisque no padrão especifíco. Realizado na placa Raspbery Pi Pico W baseada no microcontrolador RP2040, o sistema possui uma matriz de LEDs (pino 7), que exibe números de 0 a 9, conforme são acionados os botões A e B (pinos 5 e 6) para incrementar e decrementar o número exibido. Enquanto o LED RGB (pinos 11, 12 e 13) pisca 5 vezes por segundo. Para isso, utiliza-se o modelo de interrupção IRQ apresentada no PICO SDK (Software Development Kit), além do efeito _Debouncing_ gerado via software e simulação pelo Wokwi. O programa foi realizado na linguagem C.

## Requisitos de Funcionamento
Para o desenvolvimento, devem ser seguidos os seguintes requisitos:
1. Uso de interrupções: Todas as funcionalidades relacionadas aos botões devem ser implementadas
utilizando rotinas de interrupção (IRQ).
2. Debouncing: É obrigatório implementar o tratamento do bouncing dos botões via software.
3. Controle de LEDs: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o
domínio de diferentes tipos de controle.

## Interrupções e _Debouncing_
Ambos são utilizados

## Como exexcutar

## Vídeo de Demonstração
