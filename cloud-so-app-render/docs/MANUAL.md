# Manual — cloud-so-app

**Aplicação Express.js para exibir informações do sistema operacional, executada localmente e publicada na nuvem (Render)**

**Disciplina:** Sistemas Operacionais — Análise e Desenvolvimento de Sistemas, Fatec Itapetininga
**Autor:** Marciel Silva

---

## Sumário

1. Objetivo
2. Instalação das ferramentas
3. Criação do projeto
4. Desenvolvimento da aplicação
5. Execução e testes locais
6. Versionamento no repositório da disciplina
7. Publicação no Render
8. Testes no Render
9. Comparação entre ambientes
10. Problemas comuns e soluções
11. Conclusões finais

Documento complementar: [CONCEITOS_SO.md](CONCEITOS_SO.md) — relação técnica entre a aplicação e os conceitos de Sistemas Operacionais.

---

## 1. Objetivo

Construir uma aplicação web com Node.js e Express chamada `cloud-so-app` que exiba, em uma página, informações do sistema operacional em que está rodando: nome do host, plataforma, arquitetura, quantidade de CPUs, memória total, memória livre e tempo de atividade do sistema. A mesma aplicação é executada em ambientes diferentes (computador local, máquina virtual e nuvem) para comparar como cada um se apresenta ao programa.

---

## 2. Instalação das ferramentas

| Ferramenta | Para quê | Versão usada |
|---|---|---|
| Node.js (LTS) | Executar JavaScript no servidor; inclui o `npm` | 22 ou superior |
| Git | Controle de versão e envio ao GitHub | Mais recente |
| Visual Studio Code | Edição do código | Mais recente |
| Conta no GitHub | Hospedar o repositório da disciplina | — |
| Conta no Render | Publicar a aplicação na nuvem | Plano gratuito |

### 2.1 Windows

Pelo terminal (PowerShell), usando o gerenciador de pacotes `winget`:

```powershell
winget install OpenJS.NodeJS.LTS
winget install Git.Git
winget install Microsoft.VisualStudioCode
```

Também é possível baixar os instaladores em nodejs.org, git-scm.com e code.visualstudio.com. Após instalar, **feche e abra o terminal novamente** para que o `PATH` seja atualizado.

### 2.2 Linux (Ubuntu/Debian)

```bash
sudo apt update && sudo apt install -y git curl
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash
source ~/.bashrc
nvm install --lts
```

### 2.3 Verificação

```bash
node -v      # ex.: v22.x ou v24.x
npm -v
git --version
```

Configuração inicial do Git (uma única vez):

```bash
git config --global user.name "Marciel Silva"
git config --global user.email "seu-email@exemplo.com"
```

> 📸 *Inserir print do terminal com as versões instaladas.*

---

## 3. Criação do projeto

```bash
mkdir cloud-so-app
cd cloud-so-app
npm init -y
npm install express
```

- `npm init -y` cria o `package.json` com valores padrão.
- `npm install express` baixa o Express para `node_modules/` e registra a dependência no `package.json` e no `package-lock.json`.

Em seguida, ajuste o `package.json` para incluir os scripts de execução e a versão mínima do Node:

```json
"scripts": {
  "start": "node server.js",
  "dev": "node --watch server.js"
},
"engines": {
  "node": ">=20"
}
```

- `npm start` é o comando que o Render vai executar.
- `npm run dev` reinicia o servidor automaticamente quando um arquivo é salvo (útil durante o desenvolvimento).

Crie também o arquivo `.gitignore` para não enviar a pasta de dependências ao GitHub:

```
node_modules/
.env
```

### Estrutura final

```
cloud-so-app/
├── server.js            ← servidor Express e coleta das informações do SO
├── package.json
├── package-lock.json
├── .gitignore
├── README.md
├── public/              ← arquivos enviados ao navegador
│   ├── index.html
│   ├── style.css
│   └── app.js
└── docs/
    ├── MANUAL.md        ← este documento
    ├── CONCEITOS_SO.md
    └── img/             ← capturas de tela dos testes
```

---

## 4. Desenvolvimento da aplicação

A aplicação foi dividida em duas partes: o **servidor** (Node.js + Express), que consulta o sistema operacional, e a **página** (HTML, CSS e JavaScript), que exibe os dados e os atualiza a cada 5 segundos.

### 4.1 Servidor (`server.js`)

O núcleo é o módulo nativo `os` do Node.js, que não precisa ser instalado.

```js
const express = require('express');
const os = require('os');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;
```

A linha do `PORT` é essencial para a nuvem: localmente a aplicação usa a porta 3000, mas no Render a porta é definida pela plataforma na variável de ambiente `PORT`.

**Informações exigidas e as funções usadas:**

| Informação | Função do Node.js | Exemplo de retorno |
|---|---|---|
| Nome do host | `os.hostname()` | `DESKTOP-ABC123` |
| Plataforma | `os.platform()` | `win32`, `linux` |
| Arquitetura | `os.arch()` | `x64` |
| Quantidade de CPUs | `os.cpus().length` | `8` |
| Memória total | `os.totalmem()` | bytes, convertidos para GB |
| Memória livre | `os.freemem()` | bytes, convertidos para GB |
| Tempo de atividade | `os.uptime()` | segundos, convertidos para dias/horas/minutos |

**Informações extras** (para relacionar com os conteúdos da disciplina): versão do kernel (`os.release()`), modelo da CPU, carga média (`os.loadavg()`), PID e PPID do processo, memória usada pelo próprio processo (`process.memoryUsage()`) e limite de memória do container (`process.constrainedMemory()`).

Duas funções auxiliares deixam os valores legíveis:

- `formatarBytes()` converte bytes para KB, MB ou GB, com vírgula decimal;
- `formatarTempo()` converte segundos para o formato `2d 5h 13min 40s`.

**Rotas:**

| Rota | Resposta |
|---|---|
| `GET /` | Página `public/index.html` (servida por `express.static`) |
| `GET /api/sistema` | JSON com todas as informações coletadas |
| `GET /health` | `{"status":"ok"}` — verificação de saúde usada pelo Render |

```js
app.use(express.static(path.join(__dirname, 'public')));

app.get('/api/sistema', (req, res) => {
  res.json(coletarInformacoes());
});

app.get('/health', (req, res) => {
  res.json({ status: 'ok', uptime: process.uptime() });
});

app.listen(PORT, () => {
  console.log(`cloud-so-app rodando na porta ${PORT} (PID ${process.pid})`);
});
```

A função `coletarInformacoes()` também identifica o ambiente: se a variável `RENDER` existir (o Render a define automaticamente), a página mostra "Executando no Render"; caso contrário, "Executando localmente".

### 4.2 Página (`public/`)

- `index.html` organiza o conteúdo em quatro áreas: cabeçalho com o nome do host em destaque, barra de memória, lista "Sistema operacional hospedeiro" e lista "Este processo Node.js".
- `app.js` chama `fetch('/api/sistema')`, preenche a página e repete a leitura a cada 5 segundos com `setInterval`. Um botão permite pausar e retomar a atualização.
- `style.css` define o visual, adapta o layout para celular e acompanha o tema claro/escuro do sistema.

Separar a API (JSON) da página permite testar o servidor sem navegador, apenas com `curl` ou `Invoke-RestMethod`.

---

## 5. Execução e testes locais

### 5.1 Executar

```bash
npm start
```

Saída esperada no terminal:

```
cloud-so-app rodando na porta 3000 (PID 12345)
Acesse: http://localhost:3000
```

Abra **http://localhost:3000** no navegador. Para encerrar o servidor, pressione `Ctrl + C` no terminal.

### 5.2 Testar a API pelo terminal

PowerShell (Windows):

```powershell
Invoke-RestMethod http://localhost:3000/api/sistema | ConvertTo-Json -Depth 5
Invoke-RestMethod http://localhost:3000/health
```

Linux / Git Bash:

```bash
curl http://localhost:3000/api/sistema
curl http://localhost:3000/health
```

### 5.3 Roteiro de testes

| Nº | Teste | Como executar | Resultado esperado |
|---|---|---|---|
| T01 | Servidor inicia | `npm start` | Mensagem com porta e PID, sem erros |
| T02 | Página carrega | Abrir `http://localhost:3000` | As 7 informações exigidas aparecem preenchidas |
| T03 | API responde | `curl .../api/sistema` | JSON com `sistema`, `cpu`, `memoria`, `processo` |
| T04 | Verificação de saúde | `curl .../health` | `{"status":"ok", ...}` |
| T05 | Rota inexistente | Abrir `.../nao-existe` | Código HTTP 404 |
| T06 | Atualização automática | Aguardar 10 s na página | Tempo de atividade e horário da última leitura mudam |
| T07 | Pausar atualização | Clicar em "Pausar atualização" | Valores congelam; botão muda para "Retomar atualização" |
| T08 | PID confere com o SO | Comparar PID da página com o Gerenciador de Tarefas (`tasklist \| findstr node`) | Mesmo número |
| T09 | Novo processo a cada execução | Parar (`Ctrl+C`) e iniciar de novo | PID diferente; tempo do processo volta a zero |
| T10 | Porta configurável | `$env:PORT=4000; npm start` (PowerShell) ou `PORT=4000 npm start` (Linux) | Aplicação responde na porta 4000 |
| T11 | Servidor parado | Parar o servidor com a página aberta | Mensagem de erro clara no rodapé da página |
| T12 | Layout no celular | DevTools do navegador (F12) → modo dispositivo | Conteúdo em uma coluna, sem rolagem horizontal |

### 5.4 Resultado da validação

Antes da execução no computador pessoal, a aplicação foi validada em uma máquina virtual Linux. Todos os testes de T01 a T12 passaram (no T08, o PID foi conferido com `ps`, equivalente Linux do Gerenciador de Tarefas). Respostas das rotas:

| Rota | Código HTTP |
|---|---|
| `GET /` | 200 |
| `GET /style.css`, `GET /app.js` | 200 |
| `GET /api/sistema` | 200 |
| `GET /health` | 200 |
| `GET /nao-existe` | 404 |

![Página na VM Linux de validação](img/teste-vm-linux-desktop.png)

> 📸 *Inserir aqui o print da página rodando no seu computador (`img/teste-local.png`) e do terminal com `npm start`.*

---

## 6. Versionamento no repositório da disciplina

O projeto deve ser salvo como uma pasta dentro do repositório da disciplina no GitHub.

**Se o repositório da disciplina já existe:**

```bash
git clone https://github.com/<seu-usuario>/<repositorio-da-disciplina>.git
cd <repositorio-da-disciplina>
# copie a pasta cloud-so-app (sem node_modules) para cá
git add cloud-so-app
git commit -m "Adiciona cloud-so-app: app Express com informações do SO, manual e relatório de conceitos"
git push
```

**Se for criar um repositório só para o projeto:**

```bash
cd cloud-so-app
git init
git add .
git commit -m "Primeira versão do cloud-so-app"
git branch -M main
git remote add origin https://github.com/<seu-usuario>/cloud-so-app.git
git push -u origin main
```

Confira no GitHub se a pasta `node_modules/` **não** foi enviada. Quem clonar o projeto recupera as dependências com `npm install`.

---

## 7. Publicação no Render

O Render publica a aplicação diretamente a partir do repositório do GitHub.

1. Acesse render.com e entre com a conta do GitHub.
2. No painel, clique em **New → Web Service**.
3. Escolha **Git Provider**, autorize o acesso e selecione o repositório.
4. Preencha os campos:

| Campo | Valor |
|---|---|
| Name | `cloud-so-app` |
| Language | `Node` |
| Branch | `main` |
| Root Directory | `cloud-so-app` **somente se** o projeto estiver em uma subpasta do repositório da disciplina; em repositório próprio, deixe vazio |
| Build Command | `npm install` |
| Start Command | `npm start` |
| Instance Type | `Free` |

5. Em **Advanced**, defina **Health Check Path** como `/health`.
6. **Não** crie a variável `PORT`: o Render a define sozinho.
7. Clique em **Deploy Web Service** e acompanhe a aba **Logs**. O deploy terminou quando aparecer a mensagem `cloud-so-app rodando na porta ...` e o status ficar **Live**.
8. A URL pública aparece no topo do painel, no formato `https://cloud-so-app-xxxx.onrender.com`.

Cada `git push` na branch `main` dispara um novo deploy automaticamente (*Auto-Deploy*).

**Características do plano gratuito:** 512 MB de RAM e 0,1 de CPU; o serviço é desligado após 15 minutos sem acessos e leva cerca de um minuto para religar no acesso seguinte.

> 📸 *Inserir prints: tela de configuração do serviço, logs do deploy e status "Live".*

---

## 8. Testes no Render

Repita o roteiro da seção 5.3 trocando `http://localhost:3000` pela URL do Render. Testes específicos da nuvem:

| Nº | Teste | Resultado esperado |
|---|---|---|
| R01 | Abrir a URL pública | Página carrega com selo "Executando no Render" |
| R02 | `GET /api/sistema` | `ambiente.nome` igual a `"Render"` |
| R03 | `GET /health` | `{"status":"ok"}` |
| R04 | HTTPS | Cadeado no navegador (certificado fornecido pelo Render) |
| R05 | Acesso de outro dispositivo | Página abre no celular, pela rede móvel |
| R06 | Limite do container | Barra de memória mostra a linha "Limite do container" (se a plataforma expuser o limite) |
| R07 | Cold start | Após mais de 15 min sem acesso, a primeira requisição demora; PID e tempo do processo reiniciam |
| R08 | Auto-deploy | Alterar um texto, dar `git push` e ver a mudança publicada |

> 📸 *Inserir print da página publicada (`img/teste-render.png`) e da resposta de `/api/sistema`.*

---

## 9. Comparação entre ambientes

Preencha as colunas "Local" e "Render" com os valores lidos na página. A coluna "VM Linux" traz os valores obtidos na validação.

| Informação | VM Linux (validação) | Local | Render |
|---|---|---|---|
| Nome do host | `vm` | | |
| Plataforma / tipo | `linux` / Linux | | |
| Versão do kernel | `6.18.44-fc-v64` | | |
| Arquitetura | `x64` | | |
| Quantidade de CPUs | 1 | | |
| Modelo da CPU | Intel Xeon @ 2.10 GHz | | |
| Memória total | 3,91 GB | | |
| Memória livre | ≈ 3,4 a 3,6 GB | | |
| Limite do container | não detectado | | |
| Tempo de atividade do sistema | minutos | | |
| PID / PPID | 181 / 1 | | |
| Memória do processo (RSS) | ≈ 61 MB | | |
| Carga média | 0,01 / 0,01 / 0,00 | | |
| Versão do Node.js | v22.22.2 | | |

### 9.1 O que observar

**Local (Windows).** A plataforma aparece como `win32` e a carga média fica zerada, porque o Windows não possui esse indicador. O nome do host é o nome do computador, e a quantidade de CPUs reflete todos os núcleos lógicos do processador. O tempo de atividade corresponde ao tempo desde o último boot (no Windows, a "Inicialização rápida" pode fazer esse número ser maior do que parece, pois desligar não reinicia completamente o kernel).

**VM Linux.** O perfil enxuto (1 CPU, cerca de 4 GB) é definido por quem criou a máquina virtual, não pelo hardware físico. O PPID igual a 1 indica que o processo foi adotado pelo processo inicial do sistema após o terminal que o iniciou ser encerrado.

**Render.** O sistema será sempre Linux, independentemente do sistema usado no desenvolvimento. Os valores de CPU e memória total tendem a ser os da máquina hospedeira e podem ser muito maiores do que o plano contratado (0,1 CPU e 512 MB), porque o container compartilha o kernel e enxerga parte das informações do hospedeiro. O limite real é aplicado por cgroups. O tempo de atividade do sistema pode estar em dias, enquanto o tempo do processo está em minutos.

### 9.2 Síntese

| Aspecto | Local | VM | Nuvem (Render) |
|---|---|---|---|
| Quem administra o SO | O usuário | Quem criou a VM | O provedor |
| Isolamento | Nenhum (processo comum) | Hipervisor, kernel próprio | Namespaces e cgroups, kernel compartilhado |
| Recursos | Todo o hardware | Fatia definida na criação | Cota do plano |
| Acesso | Só na própria máquina | Rede da VM | Público, via HTTPS |
| Disponibilidade | Enquanto o computador estiver ligado | Enquanto a VM estiver ligada | Contínua (com pausa por inatividade no plano gratuito) |

---

## 10. Problemas comuns e soluções

| Sintoma | Causa provável | Solução |
|---|---|---|
| `'node' não é reconhecido como comando` | Terminal aberto antes da instalação | Fechar e abrir o terminal; reinstalar o Node marcando "Add to PATH" |
| `Error: Cannot find module 'express'` | Dependências não instaladas | Rodar `npm install` na pasta do projeto |
| `EADDRINUSE: address already in use :::3000` | Outro processo usando a porta | Encerrar o processo antigo ou usar outra porta (`PORT=4000`) |
| Página abre, mas os dados não aparecem | Servidor parado ou erro na API | Ver a mensagem no rodapé e o terminal; testar `/api/sistema` |
| Render: build falha com `package.json not found` | Projeto em subpasta | Preencher **Root Directory** com `cloud-so-app` |
| Render: `No open ports detected` | Porta fixa no código | Usar `process.env.PORT` em `app.listen` |
| Render: primeira visita demora cerca de 1 minuto | Serviço gratuito desligado por inatividade | Comportamento esperado (*cold start*) |

---

## 11. Conclusões finais

O projeto mostrou, de forma prática, que um programa nunca acessa o hardware diretamente: ele depende do sistema operacional, que oferece uma interface padronizada por meio das chamadas de sistema. O mesmo arquivo `server.js` funcionou no Windows, em uma máquina virtual Linux e em um container na nuvem, e em cada ambiente o SO respondeu com dados diferentes para as mesmas perguntas.

A comparação entre ambientes deixou visíveis conceitos que normalmente são abstratos: o PID muda a cada execução porque cada inicialização cria um processo novo; a memória livre varia porque o kernel está constantemente gerenciando páginas e cache; a carga média próxima de zero confirma que a aplicação passa quase todo o tempo bloqueada, aguardando E/S de rede.

Na nuvem, a virtualização apareceu de duas formas: a máquina hospedeira enxergada pelo módulo `os` não corresponde aos recursos contratados, e o limite real é imposto pelo kernel por cgroups. Isso reforça a diferença entre máquina virtual, que tem kernel próprio, e container, que compartilha o kernel do hospedeiro.

Por fim, a publicação no Render ilustrou o modelo PaaS: não foi preciso instalar ou configurar sistema operacional, servidor web ou certificado, apenas entregar o código e respeitar convenções da plataforma, como ler a porta da variável `PORT`. Em troca dessa conveniência, aceita-se menos controle sobre o ambiente e limitações como o desligamento por inatividade no plano gratuito.
