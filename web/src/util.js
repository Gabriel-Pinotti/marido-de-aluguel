export const brl = (n) =>
  "R$ " + Number(n).toLocaleString("pt-BR", { minimumFractionDigits: 2, maximumFractionDigits: 2 });

const pad = (n) => String(n).padStart(2, "0");
export const dataStr = (d, m, a) => `${pad(d)}/${pad(m)}/${a}`;

// dias inteiros de hoje até a data dada (data-only, sem hora)
export function diasAte(dia, mes, ano) {
  const agora = new Date();
  const hoje = new Date(agora.getFullYear(), agora.getMonth(), agora.getDate());
  const alvo = new Date(ano, mes - 1, dia);
  return Math.round((alvo - hoje) / 86400000);
}

// regra de cancelamento: só com mais de 7 dias de antecedência (>= 8)
export const podeCancelar = (dia, mes, ano) => diasAte(dia, mes, ano) >= 8;

// união das habilidades de todos os trabalhadores (nomes únicos, ordem de aparição)
export function habilidadesDisponiveis(trabalhadores) {
  const vistas = [];
  for (const t of trabalhadores)
    for (const h of t.habilidades)
      if (!vistas.includes(h.nome)) vistas.push(h.nome);
  return vistas;
}
