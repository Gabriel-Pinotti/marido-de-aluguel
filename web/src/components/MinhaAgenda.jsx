import React, { useMemo } from "react";
import { brl, dataStr } from "../util.js";

export default function MinhaAgenda({ usuario, trabalhos }) {
  const meus = useMemo(
    () =>
      trabalhos
        .filter((w) => w.cpfTrabalhador === usuario.cpf)
        .sort((a, b) => a.ano - b.ano || a.mes - b.mes || a.dia - b.dia),
    [trabalhos, usuario.cpf]
  );

  const total = meus.reduce((s, w) => s + w.valor, 0);

  return (
    <section className="card">
      <h2>Minha agenda</h2>

      <div className="resumo-faixa">
        <span>
          <strong>{meus.length}</strong> {meus.length === 1 ? "serviço agendado" : "serviços agendados"}
        </span>
        <span>
          Total <strong>{brl(total)}</strong>
        </span>
      </div>

      {meus.length === 0 ? (
        <p className="vazio">Você ainda não tem serviços agendados.</p>
      ) : (
        <ul className="datas">
          {meus.map((w, i) => (
            <li key={i}>
              <span className="data-celula">{dataStr(w.dia, w.mes, w.ano)}</span>
              <span className="hab-celula">{w.habilidade}</span>
              <span className="cliente-celula">Cliente: {w.nomeCliente}</span>
              <span className="valor-celula">{brl(w.valor)}</span>
            </li>
          ))}
        </ul>
      )}
    </section>
  );
}
