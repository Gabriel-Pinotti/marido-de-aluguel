import React, { useCallback, useEffect, useState } from "react";
import { getTrabalhadores, getTrabalhos } from "./api.js";
import Contratar from "./components/Contratar.jsx";
import Disponibilidade from "./components/Disponibilidade.jsx";
import Cadastrar from "./components/Cadastrar.jsx";
import Login from "./components/Login.jsx";

const CHAVE_USUARIO = "marido-usuario";

const ABAS = [
  { id: "contratar", titulo: "Contratar serviços" },
  { id: "disponibilidade", titulo: "Checar disponibilidade" },
  { id: "cadastrar", titulo: "Cadastrar trabalhador" },
];

export default function App() {
  const [usuario, setUsuario] = useState(() => {
    const salvo = localStorage.getItem(CHAVE_USUARIO);
    return salvo ? JSON.parse(salvo) : null;
  });
  const [aba, setAba] = useState("contratar");
  const [trabalhadores, setTrabalhadores] = useState([]);
  const [trabalhos, setTrabalhos] = useState([]);
  const [erroCarga, setErroCarga] = useState("");

  function entrar(u) {
    localStorage.setItem(CHAVE_USUARIO, JSON.stringify(u));
    setUsuario(u);
  }

  function sair() {
    localStorage.removeItem(CHAVE_USUARIO);
    setUsuario(null);
  }

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
    if (usuario) recarregar();
  }, [recarregar, usuario]);

  if (!usuario) return <Login onLogin={entrar} />;

  return (
    <div className="app">
      <header className="topo">
        <div>
          <h1>Marido de Aluguel</h1>
          <p className="sub">interface web · lê e grava direto em <code>data/</code></p>
        </div>
        <div className="usuario">
          <span className="usuario-nome">{usuario.nome}</span>
          <span className="usuario-papel">{usuario.papel}</span>
          <button className="logout" onClick={sair}>sair</button>
        </div>
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
