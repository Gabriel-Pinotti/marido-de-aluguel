import React, { useMemo } from "react";
import { brl, dataStr } from "../util.js";

export default function MeusDados({ usuario, trabalhadores, trabalhos }) {
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
                </li>
              ))}
            </ul>
          )}
        </>
      ) : (
        <>
          <h3>Minhas habilidades</h3>
          {minhasHabilidades.length === 0 ? (
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
