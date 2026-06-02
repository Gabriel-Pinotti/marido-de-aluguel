import React, { useCallback, useEffect, useState } from "react";
import { getTrabalhadores, getTrabalhos } from "./api.js";
import Contratar from "./components/Contratar.jsx";
import Disponibilidade from "./components/Disponibilidade.jsx";
import MinhaAgenda from "./components/MinhaAgenda.jsx";
import MeusDados from "./components/MeusDados.jsx";
import Login from "./components/Login.jsx";

const CHAVE_USUARIO = "marido-usuario";

const ABAS_CLIENTE = [
  { id: "contratar", titulo: "Contratar serviços" },
  { id: "profissionais", titulo: "Profissionais" },
  { id: "meus-dados", titulo: "Meus dados" },
];
const ABAS_TRABALHADOR = [
  { id: "agenda", titulo: "Minha agenda" },
  { id: "meus-dados", titulo: "Meus dados" },
];

export default function App() {
  const [usuario, setUsuario] = useState(() => {
    const salvo = localStorage.getItem(CHAVE_USUARIO);
    return salvo ? JSON.parse(salvo) : null;
  });
  const [aba, setAba] = useState("");
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

  const abas = usuario.papel === "cliente" ? ABAS_CLIENTE : ABAS_TRABALHADOR;
  // mantém a aba válida para o papel atual (cai na primeira se não existir)
  const abaAtual = abas.some((a) => a.id === aba) ? aba : abas[0].id;

  return (
    <div className="app">
      <header className="topo">
        <div>
          <h1>Empreguetes</h1>
        </div>
        <div className="usuario">
          <span className="usuario-nome">{usuario.nome}</span>
          <span className="usuario-papel">{usuario.papel}</span>
          <button className="logout" onClick={sair}>Sair</button>
        </div>
      </header>

      <nav className="abas">
        {abas.map((a) => (
          <button
            key={a.id}
            className={abaAtual === a.id ? "aba ativa" : "aba"}
            onClick={() => setAba(a.id)}
          >
            {a.titulo}
          </button>
        ))}
      </nav>

      {erroCarga && <div className="alerta erro">{erroCarga}</div>}

      <main className="conteudo">
        {abaAtual === "contratar" && (
          <Contratar
            usuario={usuario}
            trabalhadores={trabalhadores}
            trabalhos={trabalhos}
            onContratado={recarregar}
          />
        )}
        {abaAtual === "profissionais" && (
          <Disponibilidade trabalhadores={trabalhadores} trabalhos={trabalhos} />
        )}
        {abaAtual === "agenda" && (
          <MinhaAgenda usuario={usuario} trabalhos={trabalhos} onAtualizar={recarregar} />
        )}
        {abaAtual === "meus-dados" && (
          <MeusDados
            usuario={usuario}
            trabalhadores={trabalhadores}
            trabalhos={trabalhos}
            onAtualizar={recarregar}
          />
        )}
      </main>
    </div>
  );
}
