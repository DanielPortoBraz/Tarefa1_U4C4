# Exibição de números na matriz de LEDs 5x5 com botões de interrupção

## Descrição
Com fim ao aprendizado com aplicações de Sistemas Embarcados, este projeto apresenta o controle de uma matriz de LEDs endereçáveis (WS2812), através de botões com efeito _Debouncing_. Adicionam-se também, a implementação de interrupção e um LED RGB que pisque no padrão especifíco. Realizado na placa Raspbery Pi Pico W baseada no microcontrolador RP2040, o sistema possui uma matriz de LEDs 5x5 (pino 7), que exibe números de 0 a 9, conforme são acionados os botões A e B (pinos 5 e 6) para incrementar e decrementar o número exibido. Enquanto o LED RGB (pinos 11, 12 e 13) pisca 5 vezes por segundo. Para isso, utiliza-se o modelo de interrupção IRQ apresentada no PICO SDK (Software Development Kit), além do efeito _Debouncing_ gerado via software e simulação pelo Wokwi. O programa foi realizado na linguagem C.

## Requisitos de Funcionamento
Para o desenvolvimento, devem ser seguidos os seguintes requisitos:
1. Uso de interrupções: Todas as funcionalidades relacionadas aos botões devem ser implementadas
utilizando rotinas de interrupção (IRQ).
2. Debouncing: É obrigatório implementar o tratamento do bouncing dos botões via software.
3. Controle de LEDs: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o
domínio de diferentes tipos de controle.

## Interrupções e _Debouncing_
Nesta parte do programa, utilizou-se os padrões apresentados no PICO SDK e as aulas do curso do Embarcatech. Assim, as interrupções são configuradas para acionarem somente quando os GPIOs de entrada, neste caso os botões A e B estiverem em nível baixo (devido ao resistor pull-up associado a cada um). No código, sua configuração é feita através de uma função para ativar a interrupção e outra intitulada como callback, esta que guarda a lógica ao usar a interrupção. Seguem os trechos no código:

```c
// Função para ativar a interrupção
// Interrupções dos botões A e B
gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

// Protótipo do callback de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);
```
Observa-se que ambas as funções seguem o padrão de parâmetros, com a alteração somente na passagem de pinos e o evento que o ativa (ex: BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL) na função de ativação da interrupção.

Dentro da função callback, é construída a lógica para a operação com os botões e o tratamento do efeito _bouncing_. Tal efeito consiste nas condições mecânicas do botão gerarem ruídos ao pressioná-lo ou soltá-lo, com isso há o risco de serem feitas múltiplas leituras indesejadas. Veja a imagem que ilustra este efeito: 

![image](https://github.com/user-attachments/assets/eaffd5a9-07a8-4fc1-bb8d-4380f28ec48e)

Para tratar o efeito _bouncing_ no sistema apresentado, utiliza-se das funções de temporização que permitem a comparação entre dois eventos em momentos distintos. Neste caso, os momentos comparados são a última vez que o botão foi pressionado e a vez atual, com isso, caso haja um intervalo de pelo menos 200 ms, é permitida a leitura do sinal do botão, e a garantia que múltiplas leituras consecutivas não sejam realizadas. Segue o código com a solução apresentada:

```c
void gpio_irq_handler(uint gpio, uint32_t events){
    
    // Guarda o tempo em us desde o boot do sistema
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if(current_time - last_time > 200) { // Evita que ocorram leituras em intervalos menores de 200 ms
        last_time = current_time; // Atualiza o último momento em que houve a leitura do botão
        ...
    }
}   
```

## Como exexcutar

Para rodar o código na **Raspberry Pi Pico W**, siga os passos abaixo:

1. **Compile o código** utilizando o SDK do Raspberry Pi Pico.
2. **Gere o arquivo `.uf2`** para carregamento na placa.
3. **Conecte a Raspberry Pi Pico W** ao computador segurando o botão `BOOTSEL`.
4. **Arraste e solte o arquivo `.uf2`** na unidade que aparecer no seu computador.
5. **Aguarde a reinicialização** automática da placa.
6. **Teste a funcionalidade** interagindo com os botões e observando a matriz de LEDs.

Caso utilize o **Wokwi** para simulação, basta carregar o código e rodar a simulação.

📌 *Certifique-se de ter todas as dependências instaladas, como o PICO SDK e as bibliotecas necessárias para os LEDs WS2812.*

## Vídeo de Demonstração
Link do vídeo de demonstração na placa BitDogLab: https://youtube.com/shorts/ekta5z6iFGA?feature=share
