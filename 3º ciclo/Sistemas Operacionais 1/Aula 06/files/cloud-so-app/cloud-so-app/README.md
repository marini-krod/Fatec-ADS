# cloud-so-app

Aplicação Express.js que exibe informações do sistema operacional em que está rodando: nome do host, plataforma, arquitetura, quantidade de CPUs, memória total e livre, tempo de atividade, além de dados do próprio processo Node.js. Desenvolvida para a disciplina de Sistemas Operacionais (ADS — Fatec Itapetininga).

## Como executar

```bash
npm install
npm start
```

Acesse http://localhost:3000

## Rotas

| Rota | Descrição |
|---|---|
| `/` | Página com as informações do SO (atualiza a cada 5 s) |
| `/api/sistema` | Mesmas informações em JSON |
| `/health` | Verificação de saúde |

## Publicação

Render → New → Web Service. Build: `npm install`. Start: `npm start`. Health check: `/health`.

Aplicação publicada: _[colar a URL do Render aqui]_

## Documentação

- [Manual completo](docs/MANUAL.md) — instalação, criação, desenvolvimento, publicação, testes, comparações e conclusões
- [Conceitos de Sistemas Operacionais](docs/CONCEITOS_SO.md) — processos, memória, CPU, SO hospedeiro, virtualização e nuvem
