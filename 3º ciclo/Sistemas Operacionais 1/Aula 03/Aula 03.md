# 🖥️ AULA 3 — SISTEMAS OPERACIONAIS E GIT

> **Resumo de estudo — Sistemas Operacionais**
>
> **Instituição:** Fatec — Faculdade de Tecnologia  
> **Disciplina:** Sistemas Operacionais  
> **Professor:** Prof. Me. Deivison S. Takatu

---

## 📚 1. SISTEMAS OPERACIONAIS

O **Sistema Operacional (SO)** é responsável por gerenciar os recursos do computador e permitir a comunicação entre o usuário, os programas e o hardware.

### 🏢 Mainframes
São sistemas preparados para trabalhar com **grandes volumes de dados e transações**, atendendo muitos usuários.

**Principais características:**
- Alta confiabilidade e disponibilidade;
- Processamento de lotes (*batch*);
- Suporte a vários usuários;
- Segurança e integridade dos dados.

**Exemplos de uso:** bancos, grandes empresas e sistemas críticos.

---

### 🖥️ Sistemas de Servidor
São projetados para oferecer serviços para vários usuários por meio de uma rede.

Podem fornecer:
- Sites e aplicações web;
- Arquivos;
- Bancos de dados;
- Autenticação de usuários.

**Exemplos:** Linux e Windows Server.

---

### ⚡ Sistemas Multiprocessadores
Utilizam **várias CPUs ou vários núcleos** para executar tarefas em paralelo.

Um dos principais desafios é distribuir o trabalho corretamente entre os núcleos.

**Pontos importantes:**
- Balanceamento de carga;
- Sincronização;
- Controle de acesso aos recursos;
- Coerência de cache.

São comuns em servidores de alto desempenho e computação científica.

---

### 💻 Computadores Pessoais
São voltados principalmente para um usuário e priorizam **facilidade de uso e interface gráfica**.

**Exemplos:**
- **Windows:** grande compatibilidade com programas e jogos.
- **macOS:** integração entre hardware e software.
- **Linux:** personalização e uso por desenvolvedores e usuários avançados.

---

### 📱 Sistemas Mobile
São desenvolvidos para celulares e tablets.

Possuem preocupação especial com:
- 🔋 Economia de energia;
- 📍 Sensores como GPS e acelerômetro;
- 📷 Câmera;
- 🔐 Segurança e permissões;
- 📦 Distribuição de aplicativos.

**Exemplos:** Android e iOS.

---

### 🔌 Sistemas Embarcados
São utilizados em equipamentos específicos e geralmente possuem **recursos limitados**.

**Exemplos de aplicação:**
- Micro-ondas;
- Smart TVs;
- Automóveis;
- Sistemas de controle de motores.

**Exemplos de sistemas:** Embedded Linux, QNX e VxWorks.

---

### 🌐 Sistemas de Nós Sensores
São dispositivos pequenos, normalmente alimentados por bateria, que se comunicam por redes sem fio.

O principal objetivo é consumir **o mínimo possível de energia**.

**Aplicações:**
- Monitoramento ambiental;
- Agricultura de precisão;
- Sistemas de vigilância.

**Exemplos:** TinyOS e Contiki.

---

### ⏱️ Sistemas de Tempo Real — RTOS
São sistemas em que o tempo de resposta é muito importante.

#### 🔴 Hard Real-Time
O cumprimento do prazo é crítico. Perder um *deadline* pode causar consequências graves.

**Exemplos:** controle de voo e sistemas de segurança.

#### 🟡 Soft Real-Time
Pequenos atrasos são aceitáveis, embora possam diminuir a qualidade do serviço.

**Exemplos:** streaming e jogos interativos.

---

### 💳 Smart Cards
São sistemas com **recursos muito limitados**, principalmente em memória e processamento.

Por isso, precisam de:
- Gerenciamento eficiente de memória;
- Criptografia;
- Autenticação;
- Proteção contra ataques.

---

# 🔧 2. GIT — CONTROLE DE VERSÃO

O **Git** é um sistema distribuído de controle de versão criado por **Linus Torvalds**.

Ele permite acompanhar as alterações realizadas em um projeto e ajuda a recuperar versões anteriores.

### 🎯 Para que serve o Git?

- Guardar o histórico do projeto;
- Registrar alterações;
- Recuperar versões anteriores;
- Trabalhar em equipe;
- Sincronizar projetos com repositórios remotos, como o GitHub.

---

## ⚙️ 3. CONFIGURAÇÃO INICIAL DO GIT

### 👤 Configurar o nome

```bash
git config --global user.name "Seu Nome"
```

### 📧 Configurar o e-mail

```bash
git config --global user.email "seu@email.com"
```

### 🔎 Verificar a versão instalada

```bash
git --version
```

Essas configurações ajudam o Git a identificar o autor dos commits realizados.

---

# 🚀 4. FLUXO BÁSICO NO VS CODE

Um fluxo simples para começar um projeto com Git é:

**Criar projeto → Inicializar Git → Alterar arquivos → Commit → Publicar no GitHub**

### Passo a passo:

1. Criar ou abrir a pasta do projeto no **VS Code**.
2. Abrir **Controle de Código-Fonte**.
3. Selecionar **Inicializar Repositório**.
4. Fazer as alterações necessárias.
5. Escrever uma mensagem explicando a alteração.
6. Realizar o **Commit**.
7. Publicar a branch no GitHub.

---

# 🌿 5. BOAS PRÁTICAS COM GIT

### ✅ Commits pequenos
Faça commits frequentes e relacionados a alterações específicas. Isso facilita encontrar problemas e voltar para versões anteriores.

### 📝 Mensagens claras
A mensagem do commit deve explicar de forma objetiva **o que foi alterado**.

### 🌱 Branches
Branches permitem desenvolver novas funcionalidades ou correções sem alterar diretamente a branch principal.

### 🧪 Testes
Antes de juntar alterações à branch principal, verifique se o projeto continua funcionando corretamente.

---

# 🧪 6. ATIVIDADES PRÁTICAS

## 📌 Atividade 1 — Configuração e autenticação

Integrar uma IDE, como o VS Code, à conta do GitHub utilizando o Git.

O objetivo é conseguir trabalhar com operações como:

`commit` • `push` • `pull`

---

## 📌 Atividade 2 — Clonagem de repositório

Criar um repositório local, fazer alterações e enviá-las para o GitHub.

Depois, o projeto pode ser recuperado usando:

```bash
git clone <url-do-repositorio>
```

O comando **clone** cria uma cópia local de um repositório remoto.

---

## 📌 Atividade 3 — Pesquisa no GitHub

Pesquisar projetos públicos no GitHub, cloná-los para o computador e observar:

- Organização das pastas;
- Arquivos principais;
- Linguagens utilizadas;
- Estrutura geral do projeto.

---

# 🔎 7. REPOSITÓRIOS ANALISADOS

### 📷 Photography
Projeto de portfólio fotográfico construído com **Jekyll**, utilizando ferramentas web para organização e publicação.

**Estrutura observada:**
- `images/` — imagens;
- `_layouts/` — modelos de páginas;
- `_includes/` — componentes reutilizáveis;
- `assets/` — estilos, scripts e fontes.

---

### 🪐 Awesome Astronomy
Coleção de recursos relacionados à astronomia, reunindo softwares, bases de dados, códigos e comunidades.

**Arquivo principal:** `README.md`

Também possui documentação para contribuição e informações de licença.

---

### 📸 OpenSourcePhotography
Catálogo de ferramentas e projetos de código aberto relacionados à fotografia e vídeo.

O conteúdo é organizado principalmente por documentação e listas de recursos.

---

### 🎵 ProjectM
Projeto de **visualização de música** desenvolvido em C/C++.

Possui componentes relacionados à renderização gráfica, áudio e efeitos visuais.

**Algumas pastas importantes:**
- `src/` — código principal;
- `presets/` — efeitos e configurações visuais;
- `cmake/` — configuração de compilação;
- `.github/` — automações do projeto.

---

### 🎮 SMAPI
API e carregador de mods para **Stardew Valley**, desenvolvido em .NET/C#.

**Principais partes:**
- `src/` — código principal;
- `docs/` — documentação;
- `build/` — scripts de automação.

---

# 🧠 8. PARA LEMBRAR NA PROVA

### Sistemas Operacionais
**SO = gerenciamento de recursos + execução de programas + interação com hardware e usuário.**

### Git
**Git = controle de versões.**

### GitHub
**GitHub = plataforma para armazenar e compartilhar repositórios Git.**

### Commit
**Commit = registro de uma alteração no histórico do projeto.**

### Clone
**Clone = cópia de um repositório para o computador.**

### Branch
**Branch = ramificação utilizada para desenvolver alterações separadamente.**

---

## ⭐ RESUMO EM UMA FRASE

> **A aula apresenta diferentes tipos de sistemas operacionais e mostra como o Git pode ser utilizado para controlar, organizar e compartilhar versões de projetos de software.**

---

### 📚 Referências do material

- TANENBAUM, A. S.; BOS, H. *Sistemas Operacionais Modernos*.
- SILBERSCHATZ, A.; GALVIN, P. B.; GAGNE, G. *Fundamentos de Sistemas Operacionais*.
- STALLINGS, W. *Sistemas Operacionais: Conceitos e Projetos*.
- DENARDIN, G. W.; BARRIQUELLO, C. H. *Sistemas Operacionais de Tempo Real e sua Aplicação em Sistemas Embarcados*.

*Material reorganizado e resumido com base no conteúdo da Aula 3 fornecido pelo aluno.*
