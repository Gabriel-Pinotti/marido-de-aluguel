import React, { useEffect, useState } from "react";
import { brl, dataStr } from "../util.js";
import { buscarTrabalhadores } from "../api.js";

export default function Disponibilidade({ trabalhadores, trabalhos }) {
  const [aberto, setAberto] = useState(null);
  const [termo, setTermo] = useState("");
  const [resultados, setResultados] = useState(null); // null = mostra todos

  // busca no backend C++ (debounce p/ não disparar a cada tecla)
  useEffect(() => {
    const t = termo.trim();
    if (!t) {
      setResultados(null);
      return;
    }
    const id = setTimeout(() => {
      buscarTrabalhadores(t)
        .then(setResultados)
        .catch(() => setResultados([]));
    }, 350);
    return () => clearTimeout(id);
  }, [termo]);

  const agendaDe = (cpf) =>
    trabalhos
      .filter((w) => w.cpfTrabalhador === cpf)
      .sort((a, b) => a.ano - b.ano || a.mes - b.mes || a.dia - b.dia);

  const lista = resultados ?? trabalhadores;

  return (
    <section className="card">
      <h2>Profissionais</h2>

      <input
        className="busca"
        placeholder="Buscar por nome ou habilidade…"
        value={termo}
        onChange={(e) => setTermo(e.target.value)}
      />

      {lista.length === 0 ? (
        <p className="vazio">
          {termo.trim() ? "Nenhum profissional encontrado." : "Nenhum trabalhador cadastrado."}
        </p>
      ) : (
      <div className="lista">
        {lista.map((t) => {
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
                          <span className="cliente-celula">Cliente: {w.nomeCliente}</span>
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
      )}
    </section>
  );
}
