# 🌐 Dashboard de Monitoramento de SO & Deploy em Nuvem (Render)
### *Evolução de API REST em Node.js/Express para Dashboard Executivo de Infraestrutura*

---

## 📑 Sumário
1. 🛠️ [Visão Geral e Conceitos Fundamentais](#1-️-visão-geral-e-conceitos-fundamentais)
2. ⏱️ [Passo a Passo de Desenvolvimento e Deploy](#2-️-passo-a-passo-de-desenvolvimento-e-deploy)
3. 🎛️ [Métricas do Dashboard e Casos de Uso](#3-️-métricas-do-dashboard-e-casos-de-uso)
4. 🧠 [Componentes Tecnológicos do Sistema](#4-🧠-componentes-tecnológicos-do-sistema)
5. ⚖️ [Avaliação Equilibrada (PaaS Render vs. Alternativas)](#5-⚖️-avaliação-equilibrada-paas-render-vs-alternativas)
6. 🖥️ [Demonstração Prática e Comparativo de Provedores Cloud](#6-🖥️-demonstração-prática-e-comparativo-de-provedores-cloud)
7. 🏁 [Síntese e Conclusão](#7-🏁-síntese-e-conclusão)

---

## 1. 🛠️ Visão Geral e Conceitos Fundamentais

O projeto evolui uma API básica em **Node.js + Express** para transformar a aplicação em um **Dashboard Executivo de Monitoramento de Sistemas Operacionais** hospedado em nuvem.

* **Dashboard de SO:** Coleta e exibe indicadores essenciais da máquina (host, processador, memória, uptime, arquivos e rede).
* **PaaS (Platform as a Service):** Utiliza a plataforma **Render** para automação de *deploy*, integração contínua (CI/CD) via GitHub e gerenciamento de infraestrutura remota.
* **CORS (Cross-Origin Resource Sharing):** Mecanismo de segurança configurado via middleware para controlar requisições entre domínios no navegador.

---

## 2. ⏱️ Passo a Passo de Desenvolvimento e Deploy

O fluxo de trabalho abrange desde a inicialização do projeto em ambiente local até o *deploy* automatizado na nuvem.

1. **Passo 1 (Estrutura Local):** Inicialização da pasta do projeto e do repositório Node.js.
2. **Passo 2 e 3 (Dependências):** Instalação do *Express* e do suporte ao *CORS* via `npm install express cors`.
3. **Passo 4 (Desenvolvimento do Servidor):** Construção do script (`server.js` / `api.js`) para capturar métricas do sistema operacional.
4. **Passo 5 (Versionamento):** Commit e publicação do código-fonte em um repositório no **GitHub**.
5. **Passo 6 (Hospedagem no Render):** Criação do novo *Web Service* no Dashboard do Render conectado ao repositório.
6. **Passo 7 (Build e Execução):** Configuração do *Build Command* (`node`) e *Start Command* (`node index.js` ou `node server.js`), gerando a URL pública `.onrender.com`.

---

## 3. 🎛️ Métricas do Dashboard e Casos de Uso

O painel organiza as informações extraídas do sistema operacional em módulos funcionais:

### 📊 Tabela de Módulos e Indicadores do Dashboard

| Categoria | Indicadores Exibidos | Descrição e Aplicação | Interface / Exibição |
| :-: | :--- | :--- | :--- |
| **Painel Executivo** | **RAM %, CPU Média %, Uptime, Status** | Visão rápida do uso global de recursos da máquina. | Cards de Destaque / Progress Bars |
| **Sistema & SO** | **Host, Sistema, Release, Arquitetura, Node** | Detalhes da plataforma, versão do kernel e runtime. | Tabela de Propriedades do SO |
| **Hardware & CPU** | **Núcleos, Modelo CPU, Load Avg, Cores** | Processamento por núcleo e carga de trabalho. | Lista / Gráfico por Core |
| **Memória RAM** | **Total, Usada, Livre, Por CPU** | Distribuição e volumetria do consumo de RAM. | Indicadores Numéricos e Percentuais |
| **Rede & Arquivos** | **IP Principal, Interfaces, Tabela de Arquivos** | Mapeamento de IPs, rotas e arquivos do projeto. | Tabelas de Arquivos e Interfaces |
| **Ambiente Cloud** | **PORT, NODE_ENV, Kernel AWS, PID** | Identificação do tipo de hospedagem (Local vs. Render). | Status de Execução e Variáveis |

---

## 4. 🧠 Componentes Tecnológicos do Sistema

### 🔹 Camadas da Arquitetura da Aplicação

> ⚡ **Runtime Node.js & Módulo `os`**  
> O Node.js executa o motor do dashboard. O módulo nativo `os` faz a leitura direta das chamadas de sistema do Kernel para obter consumo de RAM, load average e uptime.

> 📦 **API REST & Middleware (Express + CORS)**  
> Servidor HTTP responsável por disponibilizar os endpoints JSON e servir a interface web do dashboard, liberando o acesso seguro via CORS.

> 🖥️ **Interface Web (HTML/CSS/JS Dashboard)**  
> Painel visual dinâmico em frontend que consome a API REST interna e renderiza indicadores, barras de progresso e estatísticas do servidor.

> 🛡️ **Hospedagem & Nuvem (Render PaaS + AWS Underlying)**  
> Plataforma PaaS que executa o contêiner da aplicação sobre infraestrutura AWS, fornecendo isolamento, SSL automático e variáveis de ambiente.

---

## 5. ⚖️ Avaliação Equilibrada (Vantagens e Limitações)

### 🟢 Vantagens
* **Deploy Automatizado:** Integração nativa com GitHub aciona atualizações a cada novo commit.
* **Praticidade PaaS:** Gerenciamento de infraestrutura, SSL e porta sem necessidade de configurar scripts complexos de servidor.
* **Monitoramento Transparente:** Facilita a visualização imediata da diferença entre o ambiente local e o ambiente remoto.
* **Custo Zero para Validação:** Disponibilidade de plano gratuito ideal para projetos acadêmicos e protótipos.

### 🔴 Limitações
* **Comportamento de Hibernate:** Instâncias gratuitas no Render entram em estado de repouso após inatividade, gerando atraso (*cold start*) na primeira requisição.
* **Recursos Computacionais Compartilhados:** Menor previsibilidade de desempenho do uso de CPU e RAM em instâncias gratuitas.
* **Abstração do Sistema Operacional:** O acesso direto ao SO subjacente é restrito pelas camadas de virtualização do PaaS.

---

## 6. 🖥️ Demonstração Prática e Comparativo de Provedores Cloud

Validação do dashboard implantado e relatório comparativo entre plataformas PaaS alternativas:

### 📋 Ações Executadas na Demonstração:
1. Configuração do repositório no GitHub com `server.js` e `package.json`.
2. Conexão do repositório ao dashboard do **Render** e definição dos comandos de build/start.
3. Execução do deploy contínuo gerando o link ativo `https://seu-projeto.onrender.com`.
4. Diagnóstico das métricas capturadas em nuvem (Kernel AWS, IP dinâmico, uso de CPU e RAM).

---

### 📊 Relatório Comparativo: Render vs. Provedores PaaS Alternativos

| Critério / Recursos | **Render** | **Vercel** | **Fly.io** |
| :--- | :--- | :--- | :--- |
| **Foco Principal** | Web Services, APIs, Node.js e Workers | Frontend, Next.js e Serverless Functions | Contêineres globais e Micro-VMs |
| **Execução de Processos** | Servidor Node.js contínuo | Execução orientada a eventos (Serverless) | Instâncias Docker dedicadas |
| **Plano Gratuito** | Sim (com standby/hibernate) | Sim (com limites de Serverless execution) | Trial / Pago por uso flexível |
| **Facilidade de Deploy** | Conectar repositório Git e deploy direto | Deploy instantâneo via CLI / Git | Configuração via `flyctl` e `fly.toml` |
| **Adequação para APIs Node** | Excelente para servidores com Express e WebSocket | Limitado ao tempo de execução de funções | Excelente para serviços de longa duração |

---

## 7. 🏁 Síntese e Conclusão

A evolução do projeto demonstra como uma aplicação **Node.js/Express** pode ser estruturada para coletar dados em tempo real do Kernel do sistema operacional e ser implantada em nuvem corporativa via **PaaS**.

> 🌟 **Conclusão:** A utilização do Render simplifica a distribuição de serviços Web, tornando evidente a relação entre hardware virtualizado, variáveis de ambiente e o sistema operacional subjacente. O relatório comparativo reforça a importância de escolher a plataforma de hospedagem adequada para o tipo de arquitetura desenvolvida.