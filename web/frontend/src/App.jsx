import { useEffect, useState } from 'react'
import StatTile from './components/StatTile'
import { ThermometerIcon, DropletIcon, WindIcon, CloudIcon, SunCloudIcon } from './components/icons'
import { fetchWeatherReading } from './data/weatherSource'
import { SKY_CONDITIONS } from './data/skyConditions'
import './App.css'

function formatUpdatedAt(isoString) {
  if (!isoString) return '—'
  return new Date(isoString).toLocaleTimeString('pt-BR', {
    hour: '2-digit',
    minute: '2-digit',
  })
}

function App() {
  const [reading, setReading] = useState(null)
  const [isRefreshing, setIsRefreshing] = useState(false)

  useEffect(() => {
    let isMounted = true

    const loadReading = async () => {
      setIsRefreshing(true)
      const nextReading = await fetchWeatherReading()
      if (isMounted) {
        setReading(nextReading)
      }
      if (isMounted) {
        setIsRefreshing(false)
      }
    }

    loadReading()

    const intervalId = setInterval(loadReading, 5000)

    return () => {
      isMounted = false
      clearInterval(intervalId)
    }
  }, [])

  const condition = reading?.condition ? SKY_CONDITIONS[reading.condition] : null

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
          <span className={`page__refresh ${isRefreshing ? 'is-refreshing' : ''}`}>
            {isRefreshing ? 'Atualizando...' : 'Sincronizado'}
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
