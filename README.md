# 🌱 Sistema de Irrigação Automatizado com RP2040

## 📌 Descrição do Projeto
💧 Este projeto tem como objetivo monitorar e controlar automaticamente a umidade do solo em uma pequena horta, utilizando um sensor de umidade, um joystick para ajuste manual e LEDs para indicar o status do sistema.

🌿 Desenvolvido para a placa **Raspberry Pi Pico (RP2040)**, este sistema proporciona um gerenciamento eficiente da irrigação, otimizando o uso de água e melhorando o cuidado com as plantas.

## 🎯 Funcionalidades Implementadas

### 📡 Monitoramento e Controle da Umidade
- 📊 Leitura contínua da umidade do solo por meio de um **sensor capacitivo**.
- 📍 Ajuste da umidade desejada pelo **joystick analógico**.

### 💡 Indicação Visual com LEDs
- 🔵 **LED Azul**: Indica que a umidade está **abaixo** do nível desejado.
- 🟢 **LED Verde**: Indica que a umidade está **dentro do nível ideal**.
- 🔴 **LED Vermelho**: Indica que a umidade está **acima do nível desejado**.

### 🖥️ Exibição Gráfica no Display OLED SSD1306
- 📊 Mostra **umidade atual** e **umidade desejada**.
- 🔄 Indica se o sistema está operando no modo **manual** ou **automático**.

### 🎮 Interação via Botões e Joystick
- 🔘 **Botão do Joystick (GPIO 22)**: Alterna entre **modo automático** e **manual**.
- ⭕ **Botão A (GPIO 5)**: Liga/desliga o sistema de irrigação.
- 🕹️ **Joystick (GPIO 27)**: Permite **ajustar a umidade desejada**.

## 🛠️ Componentes Utilizados
- 🔌 **Microcontrolador**: Raspberry Pi Pico (RP2040)
- 🎮 **Joystick**: **GPIO 27 (Y)**
- 💡 **LEDs**: **GPIO 11 (Verde), GPIO 12 (Azul), GPIO 13 (Vermelho)**
- 🔘 **Botão do Joystick**: **GPIO 22**
- ⭕ **Botão A**: **GPIO 5**
- 🌊 **Sensor de Umidade**: **GPIO 26 (ADC)**
- 📟 **Display OLED SSD1306**: I2C (**GPIO 14 - SDA, GPIO 15 - SCL**)
- 🚰 **Válvula de Irrigação**: Relé acionado pelo **GPIO 10**

## 📜 Fluxograma do Sistema
![image](https://github.com/user-attachments/assets/c4ccea1f-0df6-4b17-99ce-5f915b90428e)

📊 O sistema segue a seguinte lógica de funcionamento:
1. **Leitura da umidade atual** através do sensor.
2. **Comparação com o valor desejado**.
3. **Acionamento da irrigação** caso a umidade esteja abaixo do limite configurado.
4. **Indicação visual** através dos LEDs.
5. **Exibição das informações** no display OLED.
6. **Alternância entre modos automático e manual** conforme entrada do usuário.

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

### 4️⃣ 🌱 Teste e Validação
- 📡 **Altere a umidade manualmente** com o joystick.
- 💦 **Observe a ativação da irrigação** conforme a umidade.
- 🔘 **Mude entre os modos manual e automático** pelo botão do joystick.

## 🎥 Demonstração
📌 Assista ao vídeo de demonstração completo do projeto:


🎬 [🔗 Clique aqui para assistir o vídeo completo](https://youtu.be/CJd9zCGA4sE)

## 👨‍💻 Autor
**🧑‍💻 Elisson Nadson** - [🌐 GitHub](https://github.com/ElissonNadson)


