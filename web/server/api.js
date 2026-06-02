// Plugin do Vite: adiciona os endpoints /api/* ao próprio dev-server.
// Nada de servidor extra nem deploy — continua sendo só `npm run dev`.
import {
  lerTrabalhadores,
  salvarTrabalhadores,
  lerTrabalhos,
  adicionarTrabalho,
  lerClientes,
  salvarClientes,
} from "./csv.js";

// --- validações espelhando as regras do C++ ---
const cpfValido = (cpf) => /^[0-9]{11}$/.test(cpf);
const dataValida = (dia, mes, ano) =>
  Number.isInteger(dia) && dia >= 1 && dia <= 31 &&
  Number.isInteger(mes) && mes >= 1 && mes <= 12 &&
  Number.isInteger(ano) && ano >= 2026;

function lerCorpo(req) {
  return new Promise((resolve, reject) => {
    let dados = "";
    req.on("data", (c) => (dados += c));
    req.on("end", () => {
      try {
        resolve(dados ? JSON.parse(dados) : {});
      } catch (e) {
        reject(e);
      }
    });
    req.on("error", reject);
  });
}

function responder(res, status, corpo) {
  res.statusCode = status;
  res.setHeader("Content-Type", "application/json; charset=utf-8");
  res.end(JSON.stringify(corpo));
}

// POST /api/trabalhadores  → cadastrar trabalhador
function cadastrarTrabalhador(body) {
  const nome = (body.nome || "").trim();
  const cpf = (body.cpf || "").trim();
  const habilidades = Array.isArray(body.habilidades) ? body.habilidades : [];

  if (!nome) return { erro: "Nome não pode ser vazio." };
  if (!cpfValido(cpf)) return { erro: "CPF inválido. Digite exatamente 11 números." };

  const trabalhadores = lerTrabalhadores();
  if (trabalhadores.some((t) => t.cpf === cpf)) return { erro: "CPF já cadastrado." };
  if (lerClientes().some((c) => c.cpf === cpf)) return { erro: "CPF já cadastrado." };

  if (habilidades.length === 0) return { erro: "Adicione pelo menos uma habilidade." };
  const habsLimpas = [];
  for (const h of habilidades) {
    const nh = (h.nome || "").trim();
    const valor = Number(h.valor);
    if (!nh) return { erro: "Nome da habilidade não pode ser vazio." };
    if (!(valor > 0)) return { erro: "Valor inválido. Digite um número positivo." };
    habsLimpas.push({ nome: nh, valor });
  }

  const novo = { nome, cpf, habilidades: habsLimpas };
  trabalhadores.push(novo);
  salvarTrabalhadores(trabalhadores);
  return { ok: true, trabalhador: novo };
}

// POST /api/trabalhos  → contratar serviço
function contratar(body) {
  const { cpfTrabalhador, habilidade } = body;
  const dia = Number(body.dia);
  const mes = Number(body.mes);
  const ano = Number(body.ano);
  const nomeCliente = (body.nomeCliente || "").trim();
  const cpfCliente = (body.cpfCliente || "").trim();

  if (!dataValida(dia, mes, ano))
    return { erro: "Data fora do intervalo válido (dia 1-31, mês 1-12, ano >= 2026)." };
  if (!nomeCliente) return { erro: "Nome não pode ser vazio." };
  if (!cpfValido(cpfCliente)) return { erro: "CPF inválido. Digite exatamente 11 números." };

  const trabalhador = lerTrabalhadores().find((t) => t.cpf === cpfTrabalhador);
  if (!trabalhador) return { erro: "Trabalhador não encontrado." };

  const hab = trabalhador.habilidades.find((h) => h.nome === habilidade);
  if (!hab) return { erro: "Trabalhador não possui essa habilidade." };

  // estaLivre: ocupado se já tem qualquer trabalho na mesma data
  const ocupado = lerTrabalhos().some(
    (w) =>
      w.cpfTrabalhador === cpfTrabalhador &&
      w.dia === dia && w.mes === mes && w.ano === ano
  );
  if (ocupado) return { erro: "Trabalhador não está livre nessa data." };

  const trabalho = {
    dia, mes, ano,
    cpfTrabalhador,
    nomeCliente,
    cpfCliente,
    habilidade: hab.nome,
    valor: hab.valor,
  };
  adicionarTrabalho(trabalho);
  return { ok: true, trabalho };
}

// POST /api/clientes  → registrar cliente
function cadastrarCliente(body) {
  const nome = (body.nome || "").trim();
  const cpf = (body.cpf || "").trim();

  if (!nome) return { erro: "Nome não pode ser vazio." };
  if (!cpfValido(cpf)) return { erro: "CPF inválido. Digite exatamente 11 números." };

  const clientes = lerClientes();
  if (clientes.some((c) => c.cpf === cpf)) return { erro: "CPF já cadastrado." };
  if (lerTrabalhadores().some((t) => t.cpf === cpf)) return { erro: "CPF já cadastrado." };

  const novo = { nome, cpf };
  clientes.push(novo);
  salvarClientes(clientes);
  return { ok: true, cliente: novo };
}

export function maridoApiPlugin() {
  return {
    name: "marido-api",
    configureServer(server) {
      server.middlewares.use(async (req, res, next) => {
        const url = req.url.split("?")[0];
        if (!url.startsWith("/api/")) return next();

        try {
          if (req.method === "GET" && url === "/api/trabalhadores")
            return responder(res, 200, lerTrabalhadores());
          if (req.method === "GET" && url === "/api/trabalhos")
            return responder(res, 200, lerTrabalhos());
          if (req.method === "GET" && url === "/api/clientes")
            return responder(res, 200, lerClientes());

          if (req.method === "POST" && url === "/api/trabalhadores") {
            const r = cadastrarTrabalhador(await lerCorpo(req));
            return responder(res, r.erro ? 400 : 201, r);
          }
          if (req.method === "POST" && url === "/api/trabalhos") {
            const r = contratar(await lerCorpo(req));
            return responder(res, r.erro ? 400 : 201, r);
          }
          if (req.method === "POST" && url === "/api/clientes") {
            const r = cadastrarCliente(await lerCorpo(req));
            return responder(res, r.erro ? 400 : 201, r);
          }

          return responder(res, 404, { erro: "Rota não encontrada." });
        } catch (e) {
          return responder(res, 500, { erro: String(e && e.message ? e.message : e) });
        }
      });
    },
  };
}
