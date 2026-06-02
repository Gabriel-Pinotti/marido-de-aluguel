import React, { useMemo, useState } from "react";
import { contratar } from "../api.js";
import { brl, dataStr, habilidadesDisponiveis } from "../util.js";

const dataValida = (d, m, a) =>
  d >= 1 && d <= 31 && m >= 1 && m <= 12 && a >= 2026;

export default function Contratar({ usuario, trabalhadores, trabalhos, onContratado }) {
  const habilidades = useMemo(
    () => habilidadesDisponiveis(trabalhadores),
    [trabalhadores]
  );

  const [habilidade, setHabilidade] = useState("");
  const [dia, setDia] = useState("");
  const [mes, setMes] = useState("");
  const [ano, setAno] = useState("");
  const [selecionado, setSelecionado] = useState(null);
  const [erro, setErro] = useState("");
  const [sucesso, setSucesso] = useState(null);

  const d = parseInt(dia, 10);
  const m = parseInt(mes, 10);
  const a = parseInt(ano, 10);
  const dataOk = dataValida(d, m, a);

  // trabalhadores com a habilidade escolhida E livres na data escolhida
  const disponiveis = useMemo(() => {
    if (!habilidade || !dataOk) return [];
    return trabalhadores
      .map((t) => {
        const hab = t.habilidades.find((h) => h.nome === habilidade);
        if (!hab) return null;
        const ocupado = trabalhos.some(
          (w) =>
            w.cpfTrabalhador === t.cpf &&
            w.dia === d && w.mes === m && w.ano === a
        );
        return ocupado ? null : { ...t, valor: hab.valor };
      })
      .filter(Boolean);
  }, [trabalhadores, trabalhos, habilidade, dataOk, d, m, a]);

  async function enviar(e) {
    e.preventDefault();
    setErro("");
    try {
      const r = await contratar({
        cpfTrabalhador: selecionado.cpf,
        habilidade,
        dia: d, mes: m, ano: a,
        nomeCliente: usuario.nome,
        cpfCliente: usuario.cpf,
      });
      setSucesso({
        trabalhador: selecionado.nome,
        habilidade,
        data: dataStr(d, m, a),
        valor: r.trabalho.valor,
      });
      setSelecionado(null);
      onContratado();
    } catch (err) {
      setErro(err.message);
    }
  }

  if (sucesso) {
    return (
      <section className="card">
        <h2>Contratação realizada!</h2>
        <dl className="resumo">
          <dt>Trabalhador</dt><dd>{sucesso.trabalhador}</dd>
          <dt>Serviço</dt><dd>{sucesso.habilidade}</dd>
          <dt>Data</dt><dd>{sucesso.data}</dd>
          <dt>Valor</dt><dd>{brl(sucesso.valor)}</dd>
        </dl>
        <button className="primario" onClick={() => setSucesso(null)}>
          Nova contratação
        </button>
      </section>
    );
  }

  if (habilidades.length === 0) {
    return <section className="card vazio">Nenhuma habilidade cadastrada nos trabalhadores.</section>;
  }

  return (
    <section className="card">
      <h2>Contratar serviços</h2>

      <label className="campo">
        <span>Habilidade</span>
        <select
          value={habilidade}
          onChange={(e) => { setHabilidade(e.target.value); setSelecionado(null); }}
        >
          <option value="">— selecione —</option>
          {habilidades.map((h) => <option key={h} value={h}>{h}</option>)}
        </select>
      </label>

      <fieldset className="data">
        <legend>Data desejada</legend>
        <input type="number" placeholder="dia" min="1" max="31" value={dia}
          onChange={(e) => { setDia(e.target.value); setSelecionado(null); }} />
        <input type="number" placeholder="mês" min="1" max="12" value={mes}
          onChange={(e) => { setMes(e.target.value); setSelecionado(null); }} />
        <input type="number" placeholder="ano" min="2026" value={ano}
          onChange={(e) => { setAno(e.target.value); setSelecionado(null); }} />
      </fieldset>
      {dia && mes && ano && !dataOk && (
        <p className="dica erro">Data fora do intervalo (dia 1-31, mês 1-12, ano ≥ 2026).</p>
      )}

      {habilidade && dataOk && (
        <div className="lista">
          <h3>Disponíveis em {dataStr(d, m, a)}</h3>
          {disponiveis.length === 0 ? (
            <p className="vazio">Nenhum trabalhador disponível para {habilidade} nessa data.</p>
          ) : (
            disponiveis.map((t) => (
              <button
                key={t.cpf}
                className={selecionado?.cpf === t.cpf ? "item selecionado" : "item"}
                onClick={() => setSelecionado(t)}
              >
                <span>{t.nome}</span>
                <strong>{brl(t.valor)}</strong>
              </button>
            ))
          )}
        </div>
      )}

      {selecionado && (
        <form className="form-cliente" onSubmit={enviar}>
          <h3>Confirmar contratação</h3>
          <dl className="resumo">
            <dt>Profissional</dt><dd>{selecionado.nome}</dd>
            <dt>Serviço</dt><dd>{habilidade}</dd>
            <dt>Data</dt><dd>{dataStr(d, m, a)}</dd>
            <dt>Valor</dt><dd>{brl(selecionado.valor)}</dd>
            <dt>Cliente</dt><dd>{usuario.nome}</dd>
          </dl>
          {erro && <p className="dica erro">{erro}</p>}
          <button className="primario" type="submit">Confirmar</button>
        </form>
      )}
    </section>
  );
}
