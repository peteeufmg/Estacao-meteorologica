const http = require('http');

const PORT = 3001;
let latestReading = null;

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
        latestReading = JSON.parse(body);
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
