// cloud-so-app — exibe informações do sistema operacional em que o Node.js está rodando.
// Disciplina: Sistemas Operacionais — Fatec Itapetininga (ADS)

const express = require('express');
const os = require('os');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000; // O Render define a porta pela variável PORT

// ---------- utilitários de formatação ----------

function formatarBytes(bytes) {
  if (bytes === null || bytes === undefined) return null;
  const unidades = ['B', 'KB', 'MB', 'GB', 'TB'];
  let valor = bytes;
  let i = 0;
  while (valor >= 1024 && i < unidades.length - 1) {
    valor /= 1024;
    i++;
  }
  return `${valor.toFixed(2).replace('.', ',')} ${unidades[i]}`;
}

function formatarTempo(segundosTotais) {
  const s = Math.floor(segundosTotais);
  const dias = Math.floor(s / 86400);
  const horas = Math.floor((s % 86400) / 3600);
  const minutos = Math.floor((s % 3600) / 60);
  const segundos = s % 60;
  const partes = [];
  if (dias) partes.push(`${dias}d`);
  if (dias || horas) partes.push(`${horas}h`);
  partes.push(`${minutos}min`, `${segundos}s`);
  return partes.join(' ');
}

// Em containers (Render, Docker), o os.totalmem() mostra a memória da máquina física/VM,
// mas o processo só pode usar o limite definido pelo cgroup. process.constrainedMemory()
// (Node 19.6+) lê esse limite. Sem limite, ele retorna 0 ou um número gigantesco.
function limiteDeMemoriaDoContainer(memoriaTotal) {
  if (typeof process.constrainedMemory !== 'function') return null;
  const limite = process.constrainedMemory();
  if (!limite || limite >= memoriaTotal) return null;
  return limite;
}

function detectarAmbiente() {
  if (process.env.RENDER) {
    return {
      nome: 'Render',
      descricao: 'Nuvem (PaaS) — container Linux em infraestrutura virtualizada',
      servico: process.env.RENDER_SERVICE_NAME || null,
      url: process.env.RENDER_EXTERNAL_URL || null,
    };
  }
  return {
    nome: 'Local',
    descricao: 'Máquina do desenvolvedor — sistema operacional hospedeiro direto',
    servico: null,
    url: null,
  };
}

// ---------- coleta das informações ----------

function coletarInformacoes() {
  const memoriaTotal = os.totalmem();
  const memoriaLivre = os.freemem();
  const memoriaUsada = memoriaTotal - memoriaLivre;
  const cpus = os.cpus();
  const limiteContainer = limiteDeMemoriaDoContainer(memoriaTotal);
  const memProcesso = process.memoryUsage();

  return {
    ambiente: detectarAmbiente(),
    sistema: {
      hostname: os.hostname(),
      plataforma: os.platform(),          // linux, win32, darwin...
      tipo: os.type(),                    // Linux, Windows_NT, Darwin
      versaoKernel: os.release(),
      arquitetura: os.arch(),             // x64, arm64...
      uptimeSegundos: os.uptime(),
      uptimeFormatado: formatarTempo(os.uptime()),
    },
    cpu: {
      quantidade: cpus.length,
      paralelismoDisponivel: typeof os.availableParallelism === 'function'
        ? os.availableParallelism()
        : cpus.length,
      modelo: cpus.length ? cpus[0].model.trim() : 'indisponível',
      velocidadeMHz: cpus.length ? cpus[0].speed : null,
      cargaMedia: os.loadavg().map((v) => Number(v.toFixed(2))), // 1, 5 e 15 min (sempre 0 no Windows)
    },
    memoria: {
      totalBytes: memoriaTotal,
      livreBytes: memoriaLivre,
      usadaBytes: memoriaUsada,
      total: formatarBytes(memoriaTotal),
      livre: formatarBytes(memoriaLivre),
      usada: formatarBytes(memoriaUsada),
      percentualUsado: Number(((memoriaUsada / memoriaTotal) * 100).toFixed(1)),
      limiteContainerBytes: limiteContainer,
      limiteContainer: formatarBytes(limiteContainer),
    },
    processo: {
      pid: process.pid,
      ppid: process.ppid,
      versaoNode: process.version,
      uptimeSegundos: process.uptime(),
      uptimeFormatado: formatarTempo(process.uptime()),
      rss: formatarBytes(memProcesso.rss),
      rssBytes: memProcesso.rss,
      heapUsado: formatarBytes(memProcesso.heapUsed),
      heapTotal: formatarBytes(memProcesso.heapTotal),
    },
    coletadoEm: new Date().toISOString(),
  };
}

// ---------- rotas ----------

app.use(express.static(path.join(__dirname, 'public')));

// API em JSON: usada pela página e útil para testes com curl/navegador
app.get('/api/sistema', (req, res) => {
  res.json(coletarInformacoes());
});

// Verificação de saúde (o Render pode usar como Health Check Path)
app.get('/health', (req, res) => {
  res.json({ status: 'ok', uptime: process.uptime() });
});

app.listen(PORT, () => {
  console.log(`cloud-so-app rodando na porta ${PORT} (PID ${process.pid})`);
  console.log(`Acesse: http://localhost:${PORT}`);
});
