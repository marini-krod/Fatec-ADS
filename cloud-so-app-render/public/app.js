// Busca /api/sistema e preenche a página. Atualiza a cada 5 segundos.
const INTERVALO_MS = 5000;
let temporizador = null;

const $ = (id) => document.getElementById(id);

const NOMES_PLATAFORMA = {
  linux: 'Linux',
  win32: 'Windows',
  darwin: 'macOS',
  freebsd: 'FreeBSD',
};

function preencher(dados) {
  const { ambiente, sistema, cpu, memoria, processo } = dados;
  const plataforma = NOMES_PLATAFORMA[sistema.plataforma] || sistema.plataforma;

  // topo
  const selo = $('ambiente');
  selo.textContent = ambiente.nome === 'Render'
    ? 'Executando no Render'
    : 'Executando localmente';
  selo.classList.toggle('nuvem', ambiente.nome === 'Render');
  $('hostname').textContent = sistema.hostname;
  $('resumo').textContent =
    `${sistema.tipo} ${sistema.versaoKernel} em ${sistema.arquitetura}, ` +
    `${cpu.quantidade} ${cpu.quantidade === 1 ? 'CPU lógica' : 'CPUs lógicas'} e ` +
    `${memoria.total} de memória. ${ambiente.descricao}.`;

  // barra de memória
  $('barra-usada').style.width = `${memoria.percentualUsado}%`;
  $('barra').setAttribute('aria-label',
    `Memória usada: ${memoria.usada} de ${memoria.total} (${memoria.percentualUsado}%)`);
  $('rotulo-usada').textContent = `${memoria.usada} (${String(memoria.percentualUsado).replace('.', ',')}%)`;
  $('legenda-usada').textContent = `${String(memoria.percentualUsado).replace('.', ',')}%`;
  $('rotulo-livre').textContent = memoria.livre;
  $('rotulo-total').textContent = memoria.total;

  const limite = $('limite');
  if (memoria.limiteContainerBytes) {
    const pos = (memoria.limiteContainerBytes / memoria.totalBytes) * 100;
    limite.hidden = false;
    limite.style.left = `${pos}%`;
    $('rotulo-limite').textContent = `Limite do container: ${memoria.limiteContainer}`;
    $('nota-memoria').textContent =
      `Os números acima são da máquina hospedeira. Este container só pode usar ${memoria.limiteContainer}, ` +
      'limite imposto pelo kernel via cgroups — é a virtualização em ação.';
  } else {
    limite.hidden = true;
    $('nota-memoria').textContent =
      'Nenhum limite de container detectado: o processo enxerga a memória do sistema operacional diretamente.';
  }

  // sistema
  $('f-hostname').textContent = sistema.hostname;
  $('f-plataforma').textContent = `${plataforma} (${sistema.plataforma})`;
  $('f-arquitetura').textContent = sistema.arquitetura;
  $('f-cpus').textContent = cpu.paralelismoDisponivel !== cpu.quantidade
    ? `${cpu.quantidade} (${cpu.paralelismoDisponivel} disponíveis)`
    : cpu.quantidade;
  $('f-memtotal').textContent = memoria.total;
  $('f-memlivre').textContent = memoria.livre;
  $('f-uptime').textContent = sistema.uptimeFormatado;
  $('f-modelo').textContent = cpu.modelo;
  $('f-carga').textContent = sistema.plataforma === 'win32'
    ? 'não disponível no Windows'
    : cpu.cargaMedia.map((v) => v.toFixed(2).replace('.', ',')).join(' / ');

  // processo
  $('p-pid').textContent = processo.pid;
  $('p-ppid').textContent = processo.ppid;
  $('p-node').textContent = processo.versaoNode;
  $('p-uptime').textContent = processo.uptimeFormatado;
  $('p-rss').textContent = processo.rss;
  $('p-heap').textContent = `${processo.heapUsado} / ${processo.heapTotal}`;

  const hora = new Date(dados.coletadoEm).toLocaleTimeString('pt-BR');
  $('status').classList.remove('erro');
  $('status').textContent = `Atualiza a cada 5 segundos. Última leitura às ${hora}.`;
}

async function atualizar() {
  try {
    const resposta = await fetch('/api/sistema', { cache: 'no-store' });
    if (!resposta.ok) throw new Error(`HTTP ${resposta.status}`);
    preencher(await resposta.json());
  } catch (erro) {
    $('status').classList.add('erro');
    $('status').textContent =
      `Não foi possível ler /api/sistema (${erro.message}). Verifique se o servidor está rodando.`;
  }
}

function iniciar() {
  atualizar();
  temporizador = setInterval(atualizar, INTERVALO_MS);
  $('alternar').textContent = 'Pausar atualização';
}

function pausar() {
  clearInterval(temporizador);
  temporizador = null;
  $('alternar').textContent = 'Retomar atualização';
  $('status').textContent = 'Atualização pausada.';
}

$('alternar').addEventListener('click', () => (temporizador ? pausar() : iniciar()));

iniciar();
