cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(BitDogLab_Joystick_LEDs C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável (substitua o arquivo .c se necessário)
add_executable(BitDogLab_Joystick_LEDs BitDogLab_Joystick_LEDs.c Inc/ssd1306.c)

pico_set_program_name(BitDogLab_Joystick_LEDs "BitDogLab_Joystick_LEDs")
pico_set_program_version(BitDogLab_Joystick_LEDs "0.1")

# Habilita saída via UART e USB para debug
pico_enable_stdio_uart(BitDogLab_Joystick_LEDs 1)
pico_enable_stdio_usb(BitDogLab_Joystick_LEDs 1)

# Linka as bibliotecas necessárias do SDK
target_link_libraries(BitDogLab_Joystick_LEDs
    pico_stdlib      # Biblioteca padrão do Pico
    hardware_i2c     # Comunicação I2C (para o display SSD1306)
    hardware_adc     # Conversor analógico-digital (para o joystick)
    hardware_pwm     # Controle PWM (para os LEDs)
    hardware_gpio    # Controle de GPIO (para botões, etc.)
)

# Adiciona os diretórios de include, se necessário
target_include_directories(BitDogLab_Joystick_LEDs PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/Inc  
)

# Gera arquivos de saída extras (UF2, BIN, etc.)
pico_add_extra_outputs(BitDogLab_Joystick_LEDs)
