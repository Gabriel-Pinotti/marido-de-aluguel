import React, { useState } from "react";
import { cancelarTrabalho } from "../api.js";
import { podeCancelar } from "../util.js";

// Botão de cancelar um agendamento. Some/desabilita se faltar menos de 7 dias;
// o backend C++ é a autoridade final da regra.
export default function BotaoCancelar({ trabalho, onCancelado }) {
  const [erro, setErro] = useState("");
  const [enviando, setEnviando] = useState(false);

  const liberado = podeCancelar(trabalho.dia, trabalho.mes, trabalho.ano);

  async function cancelar() {
    setErro("");
    setEnviando(true);
    try {
      await cancelarTrabalho({
        cpfTrabalhador: trabalho.cpfTrabalhador,
        dia: trabalho.dia,
        mes: trabalho.mes,
        ano: trabalho.ano,
      });
      onCancelado();
    } catch (e) {
      setErro(e.message);
      setEnviando(false);
    }
  }

  return (
    <span className="cancel-wrap">
      <button
        className="cancelar-btn"
        onClick={cancelar}
        disabled={!liberado || enviando}
        title={liberado ? "Cancelar agendamento" : "Prazo encerrado"}
      >
        cancelar
      </button>
      {erro && <span className="dica erro">{erro}</span>}
    </span>
  );
}
