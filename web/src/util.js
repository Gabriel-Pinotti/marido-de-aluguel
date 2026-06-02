export const brl = (n) =>
  "R$ " + Number(n).toLocaleString("pt-BR", { minimumFractionDigits: 2, maximumFractionDigits: 2 });

const pad = (n) => String(n).padStart(2, "0");
export const dataStr = (d, m, a) => `${pad(d)}/${pad(m)}/${a}`;

// união das habilidades de todos os trabalhadores (nomes únicos, ordem de aparição)
export function habilidadesDisponiveis(trabalhadores) {
  const vistas = [];
  for (const t of trabalhadores)
    for (const h of t.habilidades)
      if (!vistas.includes(h.nome)) vistas.push(h.nome);
  return vistas;
}
