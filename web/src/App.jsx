import React, { useCallback, useEffect, useState } from "react";
import { getTrabalhadores, getTrabalhos } from "./api.js";
import Contratar from "./components/Contratar.jsx";
import Disponibilidade from "./components/Disponibilidade.jsx";
import Cadastrar from "./components/Cadastrar.jsx";

const ABAS = [
  { id: "contratar", titulo: "Contratar serviços" },
  { id: "disponibilidade", titulo: "Checar disponibilidade" },
  { id: "cadastrar", titulo: "Cadastrar trabalhador" },
];

export default function App() {
  const [aba, setAba] = useState("contratar");
  const [trabalhadores, setTrabalhadores] = useState([]);
  const [trabalhos, setTrabalhos] = useState([]);
  const [erroCarga, setErroCarga] = useState("");

  const recarregar = useCallback(async () => {
    try {
      const [ts, ws] = await Promise.all([getTrabalhadores(), getTrabalhos()]);
      setTrabalhadores(ts);
      setTrabalhos(ws);
      setErroCarga("");
    } catch (e) {
      setErroCarga(e.message);
    }
  }, []);

  useEffect(() => {
    recarregar();
  }, [recarregar]);

  return (
    <div className="app">
      <header className="topo">
        <h1>Marido de Aluguel</h1>
        <p className="sub">interface web · lê e grava direto em <code>data/</code></p>
      </header>

      <nav className="abas">
        {ABAS.map((a) => (
          <button
            key={a.id}
            className={aba === a.id ? "aba ativa" : "aba"}
            onClick={() => setAba(a.id)}
          >
            {a.titulo}
          </button>
        ))}
      </nav>

      {erroCarga && <div className="alerta erro">{erroCarga}</div>}

      <main className="conteudo">
        {aba === "contratar" && (
          <Contratar
            trabalhadores={trabalhadores}
            trabalhos={trabalhos}
            onContratado={recarregar}
          />
        )}
        {aba === "disponibilidade" && (
          <Disponibilidade trabalhadores={trabalhadores} trabalhos={trabalhos} />
        )}
        {aba === "cadastrar" && <Cadastrar onCadastrado={recarregar} />}
      </main>
    </div>
  );
}
