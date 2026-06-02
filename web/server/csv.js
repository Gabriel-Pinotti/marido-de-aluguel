// Camada de leitura/escrita dos CSVs em ../data — o MESMO formato que o
// programa C++ (original-source/) lê e grava, byte a byte, pra que os dois
// mundos continuem compartilhando os arquivos.
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";

const serverDir = path.dirname(fileURLToPath(import.meta.url));
export const DATA_DIR = path.resolve(serverDir, "..", "..", "data");

const TRABALHADORES = path.join(DATA_DIR, "trabalhadores.csv");
const TRABALHOS = path.join(DATA_DIR, "trabalhos.csv");

const HEADER_TRABALHADORES = "nome,cpf,habilidades";
const HEADER_TRABALHOS =
  "dia,mes,ano,cpf_trabalhador,nome_cliente,cpf_cliente,habilidade,valorOperacao";

// Quebra em linhas não-vazias, descartando o cabeçalho.
function linhasDeDados(caminho) {
  if (!fs.existsSync(caminho)) return [];
  const conteudo = fs.readFileSync(caminho, "utf8");
  const linhas = conteudo.split(/\r?\n/);
  return linhas.slice(1).filter((l) => l.trim() !== "");
}

// Espelha `cout << float` do C++ (precisão padrão, sem zeros à toa): 150, 150.5
function fmtValorTrabalho(n) {
  return Number.isInteger(n) ? String(n) : String(n);
}

// ---------- trabalhadores ----------

export function lerTrabalhadores() {
  return linhasDeDados(TRABALHADORES)
    .map((linha) => {
      const col = linha.split(",");
      if (col.length < 3) return null;
      const nome = col[0];
      const cpf = col[1];
      const tokens = col[2].split(":");
      const habilidades = [];
      for (let i = 0; i + 1 < tokens.length; i += 2) {
        habilidades.push({ nome: tokens[i], valor: parseFloat(tokens[i + 1]) });
      }
      return { nome, cpf, habilidades };
    })
    .filter(Boolean);
}

export function salvarTrabalhadores(trabalhadores) {
  let saida = HEADER_TRABALHADORES + "\n";
  for (const t of trabalhadores) {
    const habs = t.habilidades
      .map((h) => `${h.nome}:${h.valor.toFixed(2)}`)
      .join(":");
    saida += `${t.nome},${t.cpf},${habs}\n`;
  }
  fs.writeFileSync(TRABALHADORES, saida, "utf8");
}

// ---------- trabalhos ----------

export function lerTrabalhos() {
  return linhasDeDados(TRABALHOS)
    .map((linha) => {
      const col = linha.split(",");
      if (col.length < 8) return null;
      const dia = parseInt(col[0], 10);
      const mes = parseInt(col[1], 10);
      const ano = parseInt(col[2], 10);
      const valor = parseFloat(col[7]);
      if ([dia, mes, ano, valor].some((n) => Number.isNaN(n))) return null;
      return {
        dia,
        mes,
        ano,
        cpfTrabalhador: col[3],
        nomeCliente: col[4],
        cpfCliente: col[5],
        habilidade: col[6],
        valor,
      };
    })
    .filter(Boolean);
}

export function adicionarTrabalho(t) {
  let prefixo = "";
  if (fs.existsSync(TRABALHOS)) {
    const atual = fs.readFileSync(TRABALHOS, "utf8");
    if (atual.length > 0 && !atual.endsWith("\n")) prefixo = "\n";
  } else {
    prefixo = HEADER_TRABALHOS + "\n";
  }
  const linha =
    `${t.dia},${t.mes},${t.ano},${t.cpfTrabalhador},` +
    `${t.nomeCliente},${t.cpfCliente},${t.habilidade},${fmtValorTrabalho(t.valor)}\n`;
  fs.appendFileSync(TRABALHOS, prefixo + linha, "utf8");
}
