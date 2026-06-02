// Plugin do Vite: a web NÃO faz mais lógica de negócio em JS. Cada rota /api/*
// executa o binário C++ (cpp/), que carrega os CSVs nas classes, valida, opera
// e devolve JSON. Este arquivo é só a ponte HTTP <-> processo.
import { execFile } from "node:child_process";
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";

const serverDir = path.dirname(fileURLToPath(import.meta.url));
const ROOT = path.resolve(serverDir, "..", ".."); // raiz do projeto (onde fica data/)
const SRC_DIR = path.join(ROOT, "cpp", "src");
const BINARIO = path.join(ROOT, "cpp", "marido"); // artefato de build (gitignored)

// --- compilação do binário (uma vez ao subir o dev server) ---
let compilacao = null;
function garantirBinario() {
  if (compilacao) return compilacao;
  compilacao = new Promise((resolve, reject) => {
    const fontes = fs
      .readdirSync(SRC_DIR)
      .filter((f) => f.endsWith(".cpp"))
      .map((f) => path.join("cpp", "src", f));
    const args = ["-std=c++17", "-O2", "-o", "cpp/marido", "cpp/main.cpp", ...fontes];
    console.log("[marido] compilando binário C++…");
    execFile("g++", args, { cwd: ROOT }, (err, _out, stderr) => {
      if (err) {
        console.error("[marido] falha ao compilar o C++:\n" + (stderr || err.message));
        reject(new Error(stderr || "Falha ao compilar o backend C++."));
      } else {
        console.log("[marido] binário pronto: cpp/marido");
        resolve();
      }
    });
  }).catch((e) => {
    compilacao = null; // permite nova tentativa (após o usuário corrigir o C++)
    throw e;
  });
  return compilacao;
}

// --- execução de um comando do binário ---
function runCli(args) {
  return new Promise((resolve) => {
    execFile(
      BINARIO,
      args,
      { cwd: ROOT, maxBuffer: 16 * 1024 * 1024 },
      (error, stdout, stderr) => resolve({ error, stdout, stderr })
    );
  });
}

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
  res.end(typeof corpo === "string" ? corpo : JSON.stringify(corpo));
}

// formata [{nome,valor}] -> "nome:valor:nome:valor" (formato que o C++ espera)
function habilidadesParaArg(habilidades) {
  if (!Array.isArray(habilidades)) return "";
  return habilidades.map((h) => `${h.nome}:${h.valor}`).join(":");
}

// roda o comando e traduz a saída do C++ para uma resposta HTTP
async function executar(res, args, okStatus) {
  await garantirBinario();
  const { error, stdout, stderr } = await runCli(args);

  // falha de spawn (binário ausente etc.) — error.code vira string tipo "ENOENT"
  if (error && typeof error.code !== "number") {
    return responder(res, 500, { erro: "Falha ao executar o backend C++: " + error.message });
  }

  let parsed;
  try {
    parsed = JSON.parse(stdout);
  } catch {
    return responder(res, 500, {
      erro: "Saída inesperada do backend C++.",
      detalhe: (stdout || "") + (stderr || ""),
    });
  }

  const status = parsed && parsed.erro ? 400 : okStatus;
  return responder(res, status, parsed);
}

export function maridoApiPlugin() {
  return {
    name: "marido-api",
    configureServer(server) {
      garantirBinario().catch(() => {}); // compila já no boot (erros logados acima)

      server.middlewares.use(async (req, res, next) => {
        const url = req.url.split("?")[0];
        if (!url.startsWith("/api/")) return next();

        try {
          if (req.method === "GET" && url === "/api/trabalhadores")
            return await executar(res, ["list", "trabalhadores"], 200);
          if (req.method === "GET" && url === "/api/trabalhos")
            return await executar(res, ["list", "trabalhos"], 200);
          if (req.method === "GET" && url === "/api/clientes")
            return await executar(res, ["list", "clientes"], 200);

          if (req.method === "POST" && url === "/api/trabalhadores") {
            const b = await lerCorpo(req);
            return await executar(
              res,
              ["add-trabalhador", String(b.nome ?? ""), String(b.cpf ?? ""), habilidadesParaArg(b.habilidades)],
              201
            );
          }
          if (req.method === "POST" && url === "/api/clientes") {
            const b = await lerCorpo(req);
            return await executar(res, ["add-cliente", String(b.nome ?? ""), String(b.cpf ?? "")], 201);
          }
          if (req.method === "POST" && url === "/api/trabalhos") {
            const b = await lerCorpo(req);
            return await executar(
              res,
              [
                "contratar",
                String(b.cpfTrabalhador ?? ""),
                String(b.habilidade ?? ""),
                String(b.dia ?? ""),
                String(b.mes ?? ""),
                String(b.ano ?? ""),
                String(b.nomeCliente ?? ""),
                String(b.cpfCliente ?? ""),
              ],
              201
            );
          }

          return responder(res, 404, { erro: "Rota não encontrada." });
        } catch (e) {
          return responder(res, 500, { erro: String(e && e.message ? e.message : e) });
        }
      });
    },
  };
}
