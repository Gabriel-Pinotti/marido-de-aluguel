import React, { useState } from "react";
import { brl, dataStr } from "../util.js";

export default function Disponibilidade({ trabalhadores, trabalhos }) {
  const [aberto, setAberto] = useState(null);

  if (trabalhadores.length === 0) {
    return <section className="card vazio">Nenhum trabalhador cadastrado.</section>;
  }

  const agendaDe = (cpf) =>
    trabalhos
      .filter((w) => w.cpfTrabalhador === cpf)
      .sort((a, b) => a.ano - b.ano || a.mes - b.mes || a.dia - b.dia);

  return (
    <section className="card">
      <h2>Checar disponibilidade</h2>
      <div className="lista">
        {trabalhadores.map((t) => {
          const expandido = aberto === t.cpf;
          const agenda = agendaDe(t.cpf);
          return (
            <div key={t.cpf} className="trabalhador">
              <button
                className={expandido ? "item selecionado" : "item"}
                onClick={() => setAberto(expandido ? null : t.cpf)}
              >
                <span>{t.nome}</span>
                <span className="habs">
                  {t.habilidades.map((h) => h.nome).join(", ")}
                </span>
              </button>

              {expandido && (
                <div className="agenda">
                  <p className="habilidades-detalhe">
                    {t.habilidades.map((h) => (
                      <span key={h.nome} className="tag">
                        {h.nome} · {brl(h.valor)}
                      </span>
                    ))}
                  </p>
                  <h4>Datas ocupadas</h4>
                  {agenda.length === 0 ? (
                    <p className="vazio">sem compromissos agendados</p>
                  ) : (
                    <ul className="datas">
                      {agenda.map((w, i) => (
                        <li key={i}>
                          <span className="data-celula">{dataStr(w.dia, w.mes, w.ano)}</span>
                          <span className="hab-celula">{w.habilidade}</span>
                          <span className="cliente-celula">cliente: {w.nomeCliente}</span>
                        </li>
                      ))}
                    </ul>
                  )}
                </div>
              )}
            </div>
          );
        })}
      </div>
    </section>
  );
}
