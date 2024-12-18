# ESP32 - INTERFACES HETEROGÊNEAS

Este projeto utiliza um ESP32 para criar uma rede mesh e comunicação BLE (Bluetooth Low Energy). O ESP32 pode operar simultaneamente como um ponto de acesso e como um cliente, permitindo a comunicação entre múltiplos dispositivos na rede mesh e a troca de dados via BLE.

## Funcionalidades

- **Rede Mesh**: Utiliza a biblioteca `painlessMesh` para criar uma rede mesh entre múltiplos dispositivos ESP32.
- **Comunicação BLE**: Utiliza a biblioteca `BLEDevice` para comunicação BLE.
- **Controle de LED RGB**: Utiliza a biblioteca `Adafruit_NeoPixel` para controlar um LED RGB embutido no ESP32-C3.
- **Alternância entre Mesh e BLE**: Alterna entre a rede mesh e a comunicação BLE com base em intervalos de tempo e eventos.

## Requisitos

- ESP32
- Biblioteca `painlessMesh`
- Biblioteca `BLEDevice`
- Biblioteca `Adafruit_NeoPixel`
- Biblioteca `AsyncTCP`

## Instalação

1. Clone este repositório:
    ```sh
    git clone https://github.com/GJSegatto/Interfaces_Heterogeneas.git
    ```

2. Abra o projeto no Arduino IDE ou no PlatformIO.

3. Instale as bibliotecas necessárias:
    - `painlessMesh`
    - `BLEDevice`
    - `Adafruit_NeoPixel`
    - `AsyncTCP`

4. Compile e faça o upload do código para o seu ESP32.

## Uso

### Configuração da Rede Mesh

A rede mesh é configurada com os seguintes parâmetros:
- Prefixo da rede: `REDE_PERIGOSA`
- Senha da rede: `naoentrapfv`
- Porta: `5555`

### Controle de LED RGB

O LED RGB embutido no ESP32-C3 é controlado para indicar diferentes estados:
- Vermelho: Inicialização
- Azul: Modo Bluetooh (BLE)
- Verde: Modo Mesh
- Amarelo: Conteúdo Disponível
