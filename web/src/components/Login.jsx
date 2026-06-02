import React, { useEffect, useState } from "react";
import {
  getTrabalhadores,
  getClientes,
  cadastrarTrabalhador,
  cadastrarCliente,
} from "../api.js";

const habVazia = () => ({ nome: "", valor: "" });
const norm = (s) => s.trim().toLowerCase();

export default function Login({ onLogin }) {
  const [modo, setModo] = useState("login"); // 'login' | 'registro'
  const [papel, setPapel] = useState("cliente"); // 'cliente' | 'trabalhador'
  const [nome, setNome] = useState("");
  const [cpf, setCpf] = useState("");
  const [habilidades, setHabilidades] = useState([habVazia()]);
  const [erro, setErro] = useState("");
  const [conflito, setConflito] = useState(false); // CPF já usado pelo papel oposto

  // Indicador ao vivo: ao registrar, checa se o CPF já pertence ao OUTRO papel.
  useEffect(() => {
    if (modo !== "registro" || !/^[0-9]{11}$/.test(cpf)) {
      setConflito(false);
      return;
    }
    let ativo = true;
    (async () => {
      try {
        const lista = papel === "cliente" ? await getTrabalhadores() : await getClientes();
        if (ativo) setConflito(lista.some((u) => u.cpf === cpf));
      } catch {
        if (ativo) setConflito(false);
      }
    })();
    return () => {
      ativo = false;
    };
  }, [cpf, papel, modo]);

  const setHab = (i, campo, valor) =>
    setHabilidades((hs) => hs.map((h, j) => (j === i ? { ...h, [campo]: valor } : h)));
  const addHab = () => setHabilidades((hs) => [...hs, habVazia()]);
  const removeHab = (i) =>
    setHabilidades((hs) => (hs.length > 1 ? hs.filter((_, j) => j !== i) : hs));

  function trocarModo(novo) {
    setModo(novo);
    setErro("");
  }

  async function entrar(e) {
    e.preventDefault();
    setErro("");
    const alvo = norm(nome);
    const cpfAlvo = cpf.trim();
    if (!alvo) return setErro("Digite seu nome.");
    if (!cpfAlvo) return setErro("Digite seu CPF.");
    try {
      const lista = papel === "trabalhador" ? await getTrabalhadores() : await getClientes();
      const achado = lista.find((u) => norm(u.nome) === alvo && u.cpf === cpfAlvo);
      if (!achado) {
        setErro("Nome ou CPF incorreto, verifique os dados e tente novamente.");
        return;
      }
      onLogin({ papel, nome: achado.nome, cpf: achado.cpf });
    } catch (err) {
      setErro(err.message);
    }
  }

  async function registrar(e) {
    e.preventDefault();
    setErro("");
    if (conflito) {
      setErro("Este CPF já está cadastrado.");
      return;
    }
    try {
      if (papel === "cliente") {
        const r = await cadastrarCliente({ nome, cpf });
        onLogin({ papel, nome: r.cliente.nome, cpf: r.cliente.cpf });
      } else {
        const habs = habilidades
          .filter((h) => h.nome.trim() !== "" || String(h.valor).trim() !== "")
          .map((h) => ({ nome: h.nome.trim(), valor: Number(h.valor) }));
        const r = await cadastrarTrabalhador({ nome, cpf, habilidades: habs });
        onLogin({ papel, nome: r.trabalhador.nome, cpf: r.trabalhador.cpf });
      }
    } catch (err) {
      setErro(err.message);
    }
  }

  return (
    <div className="login-tela">
      <div className="login-card">
        <header className="login-topo">
          <h1>Marido de Aluguel</h1>
          <p className="sub">entre ou registre-se para continuar</p>
        </header>

        <div className="papel">
          <label className={papel === "cliente" ? "papel-op ativo" : "papel-op"}>
            <input
              type="radio"
              name="papel"
              checked={papel === "cliente"}
              onChange={() => setPapel("cliente")}
            />
            Cliente
          </label>
          <label className={papel === "trabalhador" ? "papel-op ativo" : "papel-op"}>
            <input
              type="radio"
              name="papel"
              checked={papel === "trabalhador"}
              onChange={() => setPapel("trabalhador")}
            />
            Trabalhador
          </label>
        </div>

        {modo === "login" ? (
          <form onSubmit={entrar}>
            <label className="campo">
              <span>Nome</span>
              <input value={nome} onChange={(e) => setNome(e.target.value)} autoFocus />
            </label>
            <label className="campo">
              <span>CPF (11 dígitos, apenas números)</span>
              <input value={cpf} onChange={(e) => setCpf(e.target.value)} />
            </label>
            {erro && <p className="dica erro">{erro}</p>}
            <button className="primario bloco" type="submit">Entrar</button>
            <button className="link-btn" type="button" onClick={() => trocarModo("registro")}>
              Criar uma conta
            </button>
          </form>
        ) : (
          <form onSubmit={registrar}>
            <label className="campo">
              <span>Nome completo</span>
              <input value={nome} onChange={(e) => setNome(e.target.value)} />
            </label>
            <label className="campo">
              <span>CPF (11 dígitos, apenas números)</span>
              <input
                value={cpf}
                onChange={(e) => setCpf(e.target.value)}
                className={conflito ? "campo-conflito" : ""}
              />
            </label>
            {conflito && (
              <p className="dica erro indicador-conflito">
                ⚠ Este CPF já está cadastrado.
              </p>
            )}

            {papel === "trabalhador" && (
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
                    <button
                      type="button"
                      className="remover"
                      onClick={() => removeHab(i)}
                      disabled={habilidades.length === 1}
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
            )}

            {erro && <p className="dica erro">{erro}</p>}
            <button className="primario bloco" type="submit" disabled={!!conflito}>
              Registrar e entrar
            </button>
            <button className="link-btn" type="button" onClick={() => trocarModo("login")}>
              Já tenho uma conta
            </button>
          </form>
        )}
      </div>
    </div>
  );
}
