async function json(res) {
  const corpo = await res.json().catch(() => ({}));
  if (!res.ok) throw new Error(corpo.erro || "Erro inesperado.");
  return corpo;
}

const enviar = (method, url, body) =>
  fetch(url, {
    method,
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
  }).then(json);

const post = (url, body) => enviar("POST", url, body);

export const getTrabalhadores = () => fetch("/api/trabalhadores").then(json);
export const getTrabalhos = () => fetch("/api/trabalhos").then(json);
export const getClientes = () => fetch("/api/clientes").then(json);
export const cadastrarTrabalhador = (body) => post("/api/trabalhadores", body);
export const cadastrarCliente = (body) => post("/api/clientes", body);
export const contratar = (body) => post("/api/trabalhos", body);

export const buscarTrabalhadores = (q) =>
  fetch("/api/trabalhadores/busca?q=" + encodeURIComponent(q)).then(json);
export const editarTrabalhador = (cpf, habilidades) =>
  enviar("PUT", "/api/trabalhadores", { cpf, habilidades });
export const cancelarTrabalho = (body) => enviar("DELETE", "/api/trabalhos", body);
