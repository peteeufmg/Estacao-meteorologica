const http = require('http');

const PORT = 3001;
let latestReading = null;
const weatherClients = new Set();

function broadcastReading(reading) {
  const event = `data: ${JSON.stringify(reading)}\n\n`;
  for (const client of weatherClients) {
    if (client.destroyed) {
      weatherClients.delete(client);
      continue;
    }
    client.write(event);
  }
}

function setCorsHeaders(res) {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');
}

const server = http.createServer((req, res) => {
  setCorsHeaders(res);

  if (req.method === 'OPTIONS') {
    res.writeHead(204);
    res.end();
    return;
  }

  const urlPath = req.url?.split('?')[0] || '/';

  if (req.method === 'POST' && (urlPath === '/api/weather' || urlPath === '/api/weather/')) {
    let body = '';
    req.on('data', chunk => {
      body += chunk.toString();
    });
    req.on('end', () => {
      try {
        latestReading = {
          ...JSON.parse(body),
          updatedAt: new Date().toISOString(),
        };
        broadcastReading(latestReading);
        console.log('Leitura recebida do ESP:', latestReading);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ ok: true }));
      } catch (err) {
        console.error('JSON inválido:', err);
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ ok: false, error: 'JSON inválido' }));
      }
    });
    return;
  }

  if (req.method === 'GET' && urlPath === '/api/weather/events') {
    res.writeHead(200, {
      'Content-Type': 'text/event-stream',
      'Cache-Control': 'no-cache',
      Connection: 'keep-alive',
    });
    res.write('retry: 3000\n\n');
    weatherClients.add(res);
    if (latestReading) {
      res.write(`data: ${JSON.stringify(latestReading)}\n\n`);
    }
    res.on('close', () => weatherClients.delete(res));
    return;
  }

  if (req.method === 'GET' && (urlPath === '/api/weather' || urlPath === '/api/weather/')) {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(latestReading || {}));
    return;
  }

  if (req.method === 'GET' && req.url === '/') {
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({
      ok: true,
      message: 'Bridge ativo. Use POST /api/weather para receber dados do ESP e GET /api/weather para consultar a última leitura.',
    }));
    return;
  }

  res.writeHead(404, { 'Content-Type': 'application/json' });
  res.end(JSON.stringify({ ok: false, error: 'Rota não encontrada. Use /api/weather para enviar ou consultar os dados.' }));
});

server.listen(PORT, () => {
  console.log(`Bridge do dashboard rodando em http://localhost:${PORT}`);
});
