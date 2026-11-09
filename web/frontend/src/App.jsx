import { useEffect, useState } from 'react'
import StatTile from './components/StatTile'
import { ThermometerIcon, DropletIcon, WindIcon, CloudIcon, SunCloudIcon } from './components/icons'
import { fetchWeatherReading } from './data/weatherSource'
import { SKY_CONDITIONS } from './data/skyConditions'
import './App.css'

const REFRESH_INTERVAL_MS = 5 * 60 * 1000

function formatUpdatedAt(isoString) {
  if (!isoString) return '—'
  return new Date(isoString).toLocaleString('pt-BR', {
    hour: '2-digit',
    minute: '2-digit',
    day: '2-digit',
    month: '2-digit',
  })
}

function App() {
  const [reading, setReading] = useState(null)
  const [syncStatus, setSyncStatus] = useState('loading')

  useEffect(() => {
    let isMounted = true

    const loadReading = async () => {
      setSyncStatus('refreshing')
      try {
        const nextReading = await fetchWeatherReading()
        if (isMounted) {
          if (nextReading) {
            setReading(nextReading)
            setSyncStatus('synced')
          } else {
            setSyncStatus('waiting')
          }
        }
      } catch {
        if (isMounted) {
          setSyncStatus('error')
        }
      }
    }

    loadReading()

    const intervalId = setInterval(loadReading, REFRESH_INTERVAL_MS)

    return () => {
      isMounted = false
      clearInterval(intervalId)
    }
  }, [])

  const condition = reading?.condition ? SKY_CONDITIONS[reading.condition] : null
  const statusLabels = {
    loading: 'Buscando leitura...',
    refreshing: 'Atualizando...',
    synced: 'Dados recebidos',
    waiting: 'Aguardando dados do ESP',
    error: 'Falha ao consultar servidor',
  }

  return (
    <div className="page">
      <header className="page__header">
        <SunCloudIcon className="page__logo" />
        <div>
          <h1>Análise Climática</h1>
          <p className="page__subtitle">Estação Meteorológica · PETEE — UFMG</p>
        </div>
      </header>

      <main className="page__main">
        <div className="page__status">
          <span className="page__status-badge">Dados da estação</span>
          <span className={`page__refresh is-${syncStatus}`} role="status" aria-live="polite">
            {statusLabels[syncStatus]}
          </span>
          <span>Última leitura: {formatUpdatedAt(reading?.updatedAt)}</span>
        </div>

        <section className="stat-grid" aria-label="Indicadores climáticos">
          <StatTile
            icon={<DropletIcon />}
            label="Umidade do ar"
            value={reading?.humidity ?? '--'}
            unit="%"
            color="var(--series-humidity)"
          />
          <StatTile
            icon={<ThermometerIcon />}
            label="Temperatura"
            value={reading?.temperature ?? '--'}
            unit="°C"
            color="var(--series-temperature)"
          />
          <StatTile
            icon={<WindIcon />}
            label="Velocidade do vento"
            value={reading?.windSpeed ?? '--'}
            unit="km/h"
            color="var(--series-wind)"
          />
          <StatTile
            icon={condition ? <condition.Icon /> : <CloudIcon />}
            label="Condição do céu"
            value={condition ? condition.label : '--'}
            variant="text"
            color="var(--series-sky)"
          />
        </section>
      </main>
    </div>
  )
}

export default App
