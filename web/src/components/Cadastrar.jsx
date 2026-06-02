import React, { useState } from "react";
import { cadastrarTrabalhador } from "../api.js";
import { brl } from "../util.js";

const habVazia = () => ({ nome: "", valor: "" });

export default function Cadastrar({ onCadastrado }) {
  const [nome, setNome] = useState("");
  const [cpf, setCpf] = useState("");
  const [habilidades, setHabilidades] = useState([habVazia()]);
  const [erro, setErro] = useState("");
  const [sucesso, setSucesso] = useState(null);

  const setHab = (i, campo, valor) =>
    setHabilidades((hs) => hs.map((h, j) => (j === i ? { ...h, [campo]: valor } : h)));
  const addHab = () => setHabilidades((hs) => [...hs, habVazia()]);
  const removeHab = (i) =>
    setHabilidades((hs) => (hs.length > 1 ? hs.filter((_, j) => j !== i) : hs));

  async function enviar(e) {
    e.preventDefault();
    setErro("");
    const habs = habilidades
      .filter((h) => h.nome.trim() !== "" || String(h.valor).trim() !== "")
      .map((h) => ({ nome: h.nome.trim(), valor: Number(h.valor) }));
    try {
      const r = await cadastrarTrabalhador({ nome, cpf, habilidades: habs });
      setSucesso(r.trabalhador);
      setNome(""); setCpf(""); setHabilidades([habVazia()]);
      onCadastrado();
    } catch (err) {
      setErro(err.message);
    }
  }

  if (sucesso) {
    return (
      <section className="card">
        <h2>Trabalhador cadastrado!</h2>
        <dl className="resumo">
          <dt>Nome</dt><dd>{sucesso.nome}</dd>
          <dt>CPF</dt><dd>{sucesso.cpf}</dd>
          <dt>Habilidades</dt>
          <dd>{sucesso.habilidades.map((h) => `${h.nome} (${brl(h.valor)})`).join(", ")}</dd>
        </dl>
        <button className="primario" onClick={() => setSucesso(null)}>
          Cadastrar outro
        </button>
      </section>
    );
  }

  return (
    <section className="card">
      <h2>Cadastrar trabalhador</h2>
      <form onSubmit={enviar}>
        <label className="campo">
          <span>Nome completo</span>
          <input value={nome} onChange={(e) => setNome(e.target.value)} />
        </label>
        <label className="campo">
          <span>CPF (11 dígitos, apenas números)</span>
          <input value={cpf} onChange={(e) => setCpf(e.target.value)} />
        </label>

        <fieldset className="habilidades">
          <legend>Habilidades</legend>
          {habilidades.map((h, i) => (
            <div key={i} className="hab-linha">
              <input
                placeholder="habilidade"
                value={h.nome}
                onChange={(e) => setHab(i, "nome", e.target.value)}
              />
              <input
                type="number"
                placeholder="valor (R$)"
                min="0"
                step="0.01"
                value={h.valor}
                onChange={(e) => setHab(i, "valor", e.target.value)}
              />
              <button type="button" className="remover" onClick={() => removeHab(i)}
                disabled={habilidades.length === 1} title="remover">
                ×
              </button>
            </div>
          ))}
          <button type="button" className="secundario" onClick={addHab}>
            + adicionar habilidade
          </button>
        </fieldset>

        {erro && <p className="dica erro">{erro}</p>}
        <button className="primario" type="submit">Cadastrar</button>
      </form>
    </section>
  );
}
