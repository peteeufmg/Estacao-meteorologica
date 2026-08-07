# Protótipo de Análise Climática 🌤️

## Sobre o Projeto
Este repositório contém os arquivos de configuração, firmwares e documentação técnica de um protótipo de monitoramento climático desenvolvido pelo **Programa de Educação Tutorial da Engenharia Elétrica (PETEE) da UFMG**. 

O sistema foca na coleta local de dados meteorológicos, processa essas informações e as transmite via conexão HTTP utilizando a camada de internet (Wi-Fi), enviando requisições do ESP32 para uma página/servidor web desenvolvido especificamente para receber e armazenar os dados, operando de forma remota e autônoma dentro do campus da universidade.

## Grandezas Monitoradas
O protótipo base foi projetado para medir inicialmente [Sugestões]:
*   **Temperatura** (DHT11 - já temos esse no PETEE) 
*   **Umidade** (DHT11 - já temos esse no PETEE)
*   **Velocidade do Vento (Anemometria)** (https://www.usinainfo.com.br/estacao-meteorologica-arduino/anemometro-para-estacao-meteorologica-e-arduino-sv10-3686.html - Anemômetro que já temos no PETEE)
*   -> Vídeo referência de uso do anemômetro: https://www.youtube.com/watch?v=AgZYoUZm8_4
*   *(Expansível para novos sensores)*

## Especificações Técnicas (V1)
*   **Comunicação:** Conexão HTTP via Wi-Fi (camada de internet). O ESP32 realiza requisições HTTP para uma página/servidor web próprio, responsável por receber e armazenar os dados coletados.
*   **Hardware Base:** Microcontrolador ESP32, com conectividade Wi-Fi nativa.
*   **Processamento:** Tratamento de dados no microcontrolador da borda e definição de janelas de tempo específicas para otimização da transmissão e economia de energia.

## Posicionamento Estratégico e Parcerias
Para garantir a validade científica e a utilidade dos dados coletados, a instalação física dos módulos de sensoriamento no **campus da UFMG** será pautada por estudos de relevo e microclima. 

Espera-se a colaboração e validação interdisciplinar com estudantes e pesquisadores de áreas como:
*   Engenharia Ambiental
*   Geologia
*   Geografia

## Estrutura do Repositório
*   `/firmware`: Códigos-fonte para o microcontrolador ESP32.
*   `/hardware`: Esquemáticos e possíveis arquivos de PCB.
*   `/web`: Página/servidor web que recebe as requisições HTTP do ESP32 com os dados coletados.
*   `/docs`: Documentação adicional, datasheets e especificações de pinagem.

---
**Orientador do Projeto:** Isabela Vogt e Gabriel Lyan
**Petianos responsáveis:** Samara Aquino e Gustavo Henrique
**Instituição:** PETEE - Universidade Federal de Minas Gerais (UFMG)
