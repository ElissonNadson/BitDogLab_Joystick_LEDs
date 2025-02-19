# 🎮 Controle de Joystick e Display com RP2040

## 📌 Descrição do Projeto
🕹️ Este projeto tem como objetivo controlar um joystick analógico para ajustar a intensidade de LEDs RGB utilizando PWM, além de exibir a posição do cursor em um display SSD1306 via comunicação I2C.

💡 Desenvolvido para a placa **BitDogLab** com o microcontrolador **RP2040**, este sistema proporciona uma experiência interativa, combinando controle analógico e visualização gráfica.

## 🎯 Funcionalidades Implementadas

### 🎛 Leitura e Processamento do Joystick
- 📡 Captura precisa dos valores analógicos dos eixos **X e Y** utilizando o conversor A/D do RP2040.

### 💡 Controle Suave dos LEDs RGB via PWM
- 🔵 **LED Azul**: Intensidade ajustada com base no eixo **Y** do joystick.
- 🔴 **LED Vermelho**: Intensidade ajustada com base no eixo **X** do joystick.
- 🎨 Controle PWM garante transições suaves na intensidade luminosa.

### 🖥️ Exibição Gráfica no Display OLED SSD1306
- ◼️ Um **quadrado de 8x8 pixels** representa a posição do joystick.
- 🎭 O estilo da borda do display muda ao pressionar o botão do joystick.

### 🎮 Interação via Botões
- 🔘 **Botão do Joystick (GPIO 22)**: Alterna o **LED Verde** e altera a borda do display.
- 🔴 **Botão A (GPIO 5)**: Ativa/desativa os LEDs RGB controlados por PWM.
- ⏳ Implementação de **Interrupções (IRQ)** e **debounce** para evitar leituras errôneas.

## 🛠️ Componentes Utilizados
- 🔌 **Microcontrolador**: RP2040
- 🔲 **Placa**: BitDogLab
- 🎮 **Joystick**: **GPIO 26 (X), GPIO 27 (Y)**
- 💡 **LED RGB**: **GPIO 11 (Verde), GPIO 12 (Azul), GPIO 13 (Vermelho)**
- 🔘 **Botão do Joystick**: **GPIO 22**
- ⭕ **Botão A**: **GPIO 5**
- 📟 **Display OLED SSD1306**: I2C (**GPIO 14 - SDA, GPIO 15 - SCL**)

## 📜 Fluxograma do Sistema e Simulação no Wokwi
📊 Abaixo está o fluxograma do funcionamento do sistema, representando a sequência de inicialização, captura de dados do joystick e atualização dos LEDs e do display SSD1306. Também disponibilizamos uma simulação no Wokwi para testes virtuais:

![Untitled Diagram (12)](https://github.com/user-attachments/assets/79232fca-3903-4dfe-bb4d-c585a5f77480)

[🖥️ Simulação  desse projeto no Wokwi]
![image](https://github.com/user-attachments/assets/d35583b2-8834-4e47-801e-9fcc1ada11ed)

## 🚀 Como Configurar e Executar

### 1️⃣ 🔧 Preparação do Ambiente
- 🛠️ Instale o **SDK do Raspberry Pi Pico**.
- 🖥️ Configure o **CMake** e a **toolchain** do Pico.
- 🔄 Clone o repositório e acesse a pasta do projeto.

### 2️⃣ 🏗️ Compilação e Upload do Código
- 🔄 Compile o código utilizando **CMake**.
- ⬆️ Transfira o binário para o **Raspberry Pi Pico**.

### 3️⃣ 🖥️ Monitoramento da Comunicação
- 📝 Utilize o **Serial Monitor** no **VS Code** para acompanhar a comunicação.

### 4️⃣ 🎮 Teste e Validação
- 🕹️ Interaja com o joystick para controlar os LEDs e a exibição no display.

## 🎥 Demonstração
📌 Assista ao vídeo de demonstração completo do projeto em velocidade normal:

[![▶️ Miniatura do Vídeo](https://img.youtube.com/vi/aka617XHf8E/maxresdefault.jpg)](https://youtu.be/aka617XHf8E)

🎬 [🔗 Clique aqui para assistir o vídeo completo](https://youtu.be/aka617XHf8E)

📌 Assista ao vídeo acelerado (2x) para se enquadrar no prazo do requisito:

[![▶️ Miniatura do Vídeo](https://img.youtube.com/vi/IV6TPQ4hsSQ/maxresdefault.jpg)](https://youtu.be/IV6TPQ4hsSQ)

🎬 [🔗 Clique aqui para assistir o vídeo acelerado (2x)](https://youtu.be/IV6TPQ4hsSQ)


## 👨‍💻 Autor
**🧑‍💻 Elisson Nadson** - [🌐 GitHub](https://github.com/ElissonNadson)

💡 Este projeto foi desenvolvido como parte de um estudo prático sobre **ADC, PWM e comunicação I2C com RP2040**.
