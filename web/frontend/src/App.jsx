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

  useEffect(() => {
    fetchWeatherReading().then(setReading)
  }, [])

  const condition = reading ? SKY_CONDITIONS[reading.condition] : null

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
          <span className="page__status-badge">Dados de exemplo</span>
          <span>Última leitura: {formatUpdatedAt(reading?.updatedAt)}</span>
        </div>

        <section className="stat-grid" aria-label="Indicadores climáticos">
          <StatTile
            icon={<DropletIcon />}
            label="Umidade do ar"
            value={reading ? reading.humidity : '--'}
            unit="%"
            color="var(--series-humidity)"
          />
          <StatTile
            icon={<ThermometerIcon />}
            label="Temperatura"
            value={reading ? reading.temperature : '--'}
            unit="°C"
            color="var(--series-temperature)"
          />
          <StatTile
            icon={<WindIcon />}
            label="Velocidade do vento"
            value={reading ? reading.windSpeed : '--'}
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
