import React, { useMemo, useState } from "react";
import { brl, dataStr } from "../util.js";
import { editarTrabalhador } from "../api.js";
import BotaoCancelar from "./BotaoCancelar.jsx";

const habVazia = () => ({ nome: "", valor: "" });

export default function MeusDados({ usuario, trabalhadores, trabalhos, onAtualizar }) {
  const ehCliente = usuario.papel === "cliente";

  // histórico de contratações do cliente (com nome do profissional)
  const historico = useMemo(() => {
    if (!ehCliente) return [];
    const nomePorCpf = new Map(trabalhadores.map((t) => [t.cpf, t.nome]));
    return trabalhos
      .filter((w) => w.cpfCliente === usuario.cpf)
      .map((w) => ({ ...w, profissional: nomePorCpf.get(w.cpfTrabalhador) || "—" }))
      .sort((a, b) => a.ano - b.ano || a.mes - b.mes || a.dia - b.dia);
  }, [ehCliente, trabalhadores, trabalhos, usuario.cpf]);

  // habilidades do próprio trabalhador
  const minhasHabilidades = useMemo(() => {
    if (ehCliente) return [];
    const eu = trabalhadores.find((t) => t.cpf === usuario.cpf);
    return eu ? eu.habilidades : [];
  }, [ehCliente, trabalhadores, usuario.cpf]);

  const totalGasto = historico.reduce((s, w) => s + w.valor, 0);

  // --- edição de habilidades (trabalhador) ---
  const [editando, setEditando] = useState(false);
  const [habs, setHabs] = useState([]);
  const [erro, setErro] = useState("");

  function abrirEdicao() {
    setHabs(minhasHabilidades.map((h) => ({ nome: h.nome, valor: String(h.valor) })));
    setErro("");
    setEditando(true);
  }
  const setHab = (i, campo, valor) =>
    setHabs((hs) => hs.map((h, j) => (j === i ? { ...h, [campo]: valor } : h)));
  const addHab = () => setHabs((hs) => [...hs, habVazia()]);
  const removeHab = (i) =>
    setHabs((hs) => (hs.length > 1 ? hs.filter((_, j) => j !== i) : hs));

  async function salvar(e) {
    e.preventDefault();
    setErro("");
    const limpas = habs
      .filter((h) => h.nome.trim() !== "" || String(h.valor).trim() !== "")
      .map((h) => ({ nome: h.nome.trim(), valor: Number(h.valor) }));
    try {
      await editarTrabalhador(usuario.cpf, limpas);
      setEditando(false);
      onAtualizar();
    } catch (err) {
      setErro(err.message);
    }
  }

  return (
    <section className="card">
      <h2>Meus dados</h2>

      <dl className="resumo">
        <dt>Nome</dt><dd>{usuario.nome}</dd>
        <dt>CPF</dt><dd>{usuario.cpf}</dd>
        <dt>Conta</dt><dd className="capitalize">{usuario.papel}</dd>
      </dl>

      {ehCliente ? (
        <>
          <h3>Serviços contratados</h3>
          <div className="resumo-faixa">
            <span>
              <strong>{historico.length}</strong>{" "}
              {historico.length === 1 ? "contratação" : "contratações"}
            </span>
            <span>
              Total <strong>{brl(totalGasto)}</strong>
            </span>
          </div>
          {historico.length === 0 ? (
            <p className="vazio">Você ainda não contratou nenhum serviço.</p>
          ) : (
            <ul className="datas">
              {historico.map((w, i) => (
                <li key={i}>
                  <span className="data-celula">{dataStr(w.dia, w.mes, w.ano)}</span>
                  <span className="hab-celula">{w.habilidade}</span>
                  <span className="cliente-celula">com {w.profissional}</span>
                  <span className="valor-celula">{brl(w.valor)}</span>
                  <BotaoCancelar trabalho={w} onCancelado={onAtualizar} />
                </li>
              ))}
            </ul>
          )}
        </>
      ) : (
        <>
          <div className="secao-titulo">
            <h3>Minhas habilidades</h3>
            {!editando && (
              <button className="secundario" onClick={abrirEdicao}>Editar</button>
            )}
          </div>

          {editando ? (
            <form onSubmit={salvar}>
              <fieldset className="habilidades">
                <legend>Habilidades</legend>
                {habs.map((h, i) => (
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
                    <button
                      type="button"
                      className="remover"
                      onClick={() => removeHab(i)}
                      disabled={habs.length === 1}
                      title="remover"
                    >
                      ×
                    </button>
                  </div>
                ))}
                <button type="button" className="secundario" onClick={addHab}>
                  + adicionar habilidade
                </button>
              </fieldset>
              {erro && <p className="dica erro">{erro}</p>}
              <div className="acoes">
                <button className="primario" type="submit">Salvar</button>
                <button className="secundario" type="button" onClick={() => setEditando(false)}>
                  Cancelar
                </button>
              </div>
              <p className="dica">
                Editar não altera serviços já marcados — eles guardam o que foi contratado.
              </p>
            </form>
          ) : minhasHabilidades.length === 0 ? (
            <p className="vazio">Nenhuma habilidade cadastrada.</p>
          ) : (
            <p className="habilidades-detalhe">
              {minhasHabilidades.map((h) => (
                <span key={h.nome} className="tag">
                  {h.nome} · {brl(h.valor)}
                </span>
              ))}
            </p>
          )}
        </>
      )}
    </section>
  );
}
