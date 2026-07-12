# Protótipo de Análise Climática 🌤️

## Sobre o Projeto
Este repositório contém os arquivos de configuração, firmwares e documentação técnica de um protótipo de monitoramento climático desenvolvido pelo **Programa de Educação Tutorial da Engenharia Elétrica (PETEE) da UFMG**. 

O sistema foca na coleta local de dados meteorológicos, processa essas informações e as transmite via rádio frequência utilizando a tecnologia LoRa, operando de forma remota e autônoma dentro do campus da universidade.

## Grandezas Monitoradas
O protótipo base foi projetado para medir inicialmente [Sugestões]:
*   **Temperatura** (DHT11 - já temos esse no PETEE) 
*   **Umidade** (DHT11 - já temos esse no PETEE)
*   **Velocidade do Vento (Anemometria)** (https://www.usinainfo.com.br/estacao-meteorologica-arduino/anemometro-para-estacao-meteorologica-e-arduino-sv10-3686.html - Anemômetro que já temos no PETEE)
*   -> Vídeo referência de uso do anemômetro: https://www.youtube.com/watch?v=AgZYoUZm8_4
*   *(Expansível para novos sensores)*

## Especificações Técnicas (V1)
*   **Comunicação:** Tecnologia LoRa (Long Range) para transmissão de pacotes de dados.
*   **Hardware Base (Opções):** Microcontrolador padrão associado a um transceiver LoRa dedicado OU placa integrada (ex: Heltec Meshtastic LoRa32 V3).
*   **Processamento:** Tratamento de dados no microcontrolador da borda e definição de janelas de tempo específicas para otimização da transmissão e economia de energia.

## Posicionamento Estratégico e Parcerias
Para garantir a validade científica e a utilidade dos dados coletados, a instalação física dos módulos de sensoriamento no **campus da UFMG** será pautada por estudos de relevo e microclima. 

Espera-se a colaboração e validação interdisciplinar com estudantes e pesquisadores de áreas como:
*   Engenharia Ambiental
*   Geologia
*   Geografia

## Estrutura do Repositório
*   `/firmware`: Códigos-fonte para o microcontrolador/placa Heltec.
*   `/hardware`: Esquemáticos e possíveis arquivos de PCB.
*   `/docs`: Documentação adicional, datasheets e especificações de pinagem.

---
**Orientador do Projeto:** Isabela Vogt e Gabriel Lyan
**Petianos responsáveis:** Samara Aquino e Gustavo Henrique
**Instituição:** PETEE - Universidade Federal de Minas Gerais (UFMG)
