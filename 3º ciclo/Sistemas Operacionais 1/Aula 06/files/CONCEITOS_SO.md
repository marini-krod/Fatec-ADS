# Conceitos de Sistemas Operacionais na aplicação cloud-so-app

**Disciplina:** Sistemas Operacionais — Análise e Desenvolvimento de Sistemas, Fatec Itapetininga
**Autor:** Marciel Silva

Este documento registra onde cada conceito estudado em aula aparece na aplicação `cloud-so-app`: o que o código faz, qual recurso do sistema operacional está por trás e o que pode ser observado na prática.

---

## 1. Visão geral: quem conversa com quem

```
Navegador ──HTTP──▶ Express (JavaScript) ──▶ Node.js (módulos os e process)
                                                   │
                                                   ▼
                                     libuv (biblioteca em C do Node.js)
                                                   │  chamadas de sistema
                                                   ▼
                              Kernel do SO (Linux, Windows ou macOS)
                                                   │
                                                   ▼
                     Hardware real  ou  hardware virtual (VM / container na nuvem)
```

A aplicação não lê o hardware diretamente. Ela pede ao Node.js, que pede à libuv, que faz **chamadas de sistema** ao kernel. É o kernel quem conhece CPUs, memória e tempo de atividade. Por isso o mesmo `server.js` funciona sem alteração no Windows e no Linux: a libuv traduz a pergunta para a chamada certa de cada sistema.

| Função usada | No Linux, o kernel responde por meio de… | No Windows, por meio de… |
|---|---|---|
| `os.hostname()` | `gethostname()` | `GetHostNameW()` |
| `os.platform()`, `os.arch()` | definidos na compilação do Node | definidos na compilação do Node |
| `os.release()`, `os.type()` | `uname()` | `RtlGetVersion()` |
| `os.cpus()` | `/proc/cpuinfo` e `/proc/stat` | `GetSystemInfo()` e registro |
| `os.totalmem()`, `os.freemem()` | `/proc/meminfo` (MemTotal, MemAvailable) | `GlobalMemoryStatusEx()` |
| `os.uptime()` | `/proc/uptime` | `GetTickCount64()` |
| `os.loadavg()` | `/proc/loadavg` | não existe (retorna 0, 0, 0) |

---

## 2. Processos

**Onde aparece:** seção "Este processo Node.js" da página e campo `processo` da rota `/api/sistema`.

Quando executamos `npm start`, o sistema operacional **cria um processo** para o Node.js. Esse processo recebe:

- um **PID** (identificador único do processo), exibido via `process.pid`;
- um **PPID** (PID do processo pai), via `process.ppid`. Localmente, o pai costuma ser o `npm` ou o terminal que iniciou o servidor, o que mostra a **hierarquia de processos** (árvore de processos);
- um **espaço de endereçamento próprio**, isolado dos outros processos;
- um tempo de vida próprio, via `process.uptime()`, diferente do tempo de atividade do sistema (`os.uptime()`).

**Estados do processo.** Na maior parte do tempo o servidor fica **bloqueado/em espera**, aguardando uma conexão de rede. Quando chega uma requisição, o kernel o coloca como **pronto**, o escalonador lhe entrega a CPU e ele passa a **executando** por alguns milissegundos para montar o JSON. Depois volta a esperar.

**Threads.** O JavaScript do Node.js roda em uma única thread principal (o *event loop*), mas o processo possui outras threads criadas pela libuv e pelo V8 (coleta de lixo, pool de threads para E/S). Isso pode ser visto no Gerenciador de Tarefas do Windows (coluna "Threads") ou no Linux com `ps -o nlwp -p <PID>`.

**Comunicação entre processos.** O navegador e o servidor são processos diferentes e se comunicam por **sockets TCP** fornecidos pelo kernel. O servidor "escuta" uma porta (3000 localmente, a definida em `PORT` no Render).

**Como verificar:** compare o PID mostrado na página com o do Gerenciador de Tarefas (aba Detalhes) ou com `tasklist | findstr node` (Windows) / `ps aux | grep node` (Linux). Reinicie o servidor e observe que o PID muda: é um processo novo.

---

## 3. Gerenciamento de memória

**Onde aparece:** barra "Memória do sistema", campos de memória total e livre, e RSS/heap do processo.

A aplicação mostra a memória em dois níveis:

| Nível | Origem | Significado |
|---|---|---|
| Sistema | `os.totalmem()` / `os.freemem()` | Memória física que o kernel administra e quanto ainda está disponível |
| Processo | `process.memoryUsage().rss` | *Resident Set Size*: páginas do processo que estão de fato na RAM |
| Processo | `heapUsed` / `heapTotal` | Memória que o V8 reservou para objetos JavaScript e quanto está em uso |

**Memória virtual e paginação.** Cada processo enxerga um espaço de endereçamento virtual, que o kernel mapeia para quadros de memória física por meio de **tabelas de páginas**. O RSS mostra apenas a parte residente; o processo pode ter mais memória virtual reservada do que o RSS indica.

**Memória livre não é memória ociosa.** O kernel usa a RAM sobrando como *cache* de disco. No Linux, a libuv calcula a memória livre pelo campo `MemAvailable`, que considera o cache que pode ser liberado. É por isso que o valor de "memória livre" varia mesmo quando a aplicação não faz nada: outros processos e o próprio kernel estão alocando e liberando páginas.

**Coleta de lixo.** O JavaScript não libera memória manualmente. O *garbage collector* do V8 recolhe objetos sem referência, e é possível ver o `heapUsed` subir e cair a cada atualização da página.

**Limite em containers (cgroups).** Em ambientes de nuvem a aplicação roda dentro de um container cujo consumo é limitado pelo kernel por **cgroups**. A função `process.constrainedMemory()` lê esse limite. Quando ele existe, a página desenha uma linha tracejada sobre a barra mostrando quanto o container pode usar de fato. Se o processo ultrapassar o limite, o kernel o encerra (*OOM killer*), mesmo que a máquina hospedeira tenha memória sobrando.

---

## 4. Uso de CPU

**Onde aparece:** quantidade de CPUs, modelo da CPU e carga média.

- `os.cpus().length` retorna o número de **CPUs lógicas** (núcleos × threads por núcleo, quando há *Hyper-Threading*), não necessariamente núcleos físicos.
- `os.availableParallelism()` informa quantas CPUs o processo pode usar. Se for diferente do total, a página mostra os dois valores.
- `os.loadavg()` mostra a **carga média** de 1, 5 e 15 minutos: o número médio de processos executando ou esperando pela CPU. Com 1 CPU, carga 1,0 significa CPU totalmente ocupada; acima disso há processos na fila do escalonador. No Windows esse recurso não existe e a função retorna zeros.

**Escalonamento.** A CPU é compartilhada por todos os processos do sistema. O escalonador do kernel (no Linux, o CFS/EEVDF) divide o tempo de CPU em fatias e alterna entre os processos prontos (**troca de contexto**). Nossa aplicação consome pouquíssima CPU porque passa quase todo o tempo esperando requisições, o que a classifica como uma carga **limitada por E/S** (*I/O-bound*), e não por CPU.

**CPU fracionada na nuvem.** O plano gratuito do Render oferece 0,1 CPU. Isso não é um processador "pequeno": é uma **cota de tempo** aplicada por cgroups. Na prática, o container pode usar cerca de 10 ms de CPU a cada 100 ms. Mesmo assim, `os.cpus()` pode listar todas as CPUs da máquina hospedeira, porque essa função lê as informações do hardware e não a cota do container.

---

## 5. Sistema operacional hospedeiro

**Onde aparece:** nome do host, plataforma, tipo, versão do kernel, arquitetura e tempo de atividade.

O **SO hospedeiro** é o sistema que efetivamente executa o processo Node.js e fornece a ele os serviços de gerência de processos, memória, arquivos e rede.

- **Localmente**, o hospedeiro é o próprio sistema do computador (por exemplo, Windows 11 em x64). A página mostra o nome da máquina e o tempo desde o último boot.
- **No Render**, o código roda em um container Linux. O nome do host passa a ser um identificador gerado pela plataforma, e o tempo de atividade mostrado tende a ser o do kernel da máquina hospedeira, que pode estar ligada há dias, enquanto o processo acabou de nascer. Essa diferença entre `os.uptime()` e `process.uptime()` evidencia que **containers compartilham o kernel do hospedeiro**.
- **Arquitetura** (`x64`, `arm64`) indica o conjunto de instruções da CPU. O Node.js instalado precisa ser compilado para essa arquitetura.

A rota `/api/sistema` também mostra a abstração que o SO oferece: o programador pede "memória total" e não precisa saber se o valor veio de `/proc/meminfo` ou de uma API do Windows.

---

## 6. Virtualização

**Onde aparece:** selo de ambiente, nota sobre limite de container e comparação entre ambientes.

| Tipo | Como isola | Kernel | Exemplo no projeto |
|---|---|---|---|
| Máquina virtual (VM) | Hipervisor simula hardware | Cada VM tem o seu | VM Linux usada na validação; servidores físicos da nuvem rodando VMs |
| Container | Namespaces + cgroups no mesmo kernel | Compartilhado com o hospedeiro | Instância do Render que executa o `cloud-so-app` |

**Namespaces** dão ao container uma visão própria de PIDs, rede, nome do host e sistema de arquivos. Por isso, dentro do container, o processo pode ter um PID baixo e um hostname diferente do da máquina física.

**Cgroups** limitam os recursos: memória (512 MB no plano gratuito) e CPU (0,1). É o motivo de a aplicação precisar ler `process.constrainedMemory()` para saber o limite real.

**Evidência observada.** Durante a validação, a aplicação foi executada em uma VM Linux cujo kernel informou a versão `6.18.44-fc-v64`, com 1 CPU lógica e 3,91 GB de memória. Uma máquina física raramente tem esse perfil "enxuto" de exatamente 1 CPU: ele é típico de uma VM criada sob medida por um hipervisor. O sufixo `fc` na versão do kernel sugere uma microVM do tipo Firecracker, tecnologia usada por provedores de nuvem.

---

## 7. Computação em nuvem

**Onde aparece:** publicação no Render, uso da variável `PORT`, rota `/health` e detecção da variável `RENDER`.

O Render é uma plataforma do tipo **PaaS** (*Platform as a Service*): o desenvolvedor entrega o código e a plataforma cuida de servidor, SO, rede, certificados HTTPS e execução.

| Modelo | Quem gerencia o SO | Exemplo |
|---|---|---|
| IaaS | O cliente | AWS EC2, Azure VM |
| **PaaS** | **O provedor** | **Render**, Heroku |
| SaaS | O provedor (tudo) | Gmail, Google Drive |

Características da nuvem observadas no projeto:

- **Configuração por ambiente:** o servidor usa `process.env.PORT`, porque quem decide a porta é a plataforma.
- **Detecção do ambiente:** o Render define `RENDER=true`, o que permite à página exibir "Executando no Render".
- **Monitoramento automatizado:** a rota `/health` pode ser configurada como *Health Check Path*, e a plataforma reinicia o serviço se ele parar de responder.
- **Elasticidade e economia:** no plano gratuito, o serviço é desligado após 15 minutos sem acessos e religado sob demanda (*cold start*, cerca de um minuto). O processo é destruído e recriado, o que é visível pelo PID e pelo tempo de execução do processo zerados após o religamento.
- **Recursos compartilhados e medidos:** a mesma máquina física atende vários clientes, cada um com sua cota de CPU e memória.

---

## 8. Quadro-resumo

| Elemento da aplicação | Conceito de SO | O que se observa |
|---|---|---|
| `process.pid`, `process.ppid` | Processos e hierarquia | PID muda a cada reinício; PPID aponta para o processo pai |
| `process.uptime()` × `os.uptime()` | Ciclo de vida do processo × do sistema | Processo recém-criado em um sistema ligado há mais tempo |
| `os.totalmem()`, `os.freemem()` | Gerenciamento de memória física | Memória livre oscila mesmo sem uso da aplicação |
| `process.memoryUsage()` | Memória virtual, RSS e heap | Heap sobe e desce com a coleta de lixo |
| `process.constrainedMemory()` | Cgroups / virtualização | Limite de 512 MB no Render, diferente da memória do hospedeiro |
| `os.cpus()`, `os.loadavg()` | CPU, escalonamento e carga | CPUs lógicas; carga perto de zero (aplicação I/O-bound) |
| `os.platform()`, `os.release()`, `os.arch()` | SO hospedeiro e abstração de hardware | Mesmo código, respostas diferentes em cada sistema |
| `app.listen(PORT)` | Sockets e comunicação entre processos | Navegador e servidor conversam via TCP |
| `process.env.RENDER`, `PORT` | Computação em nuvem (PaaS) | A plataforma injeta a configuração no processo |
