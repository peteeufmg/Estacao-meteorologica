const WEATHER_API_URL = 'http://localhost:3001/api/weather'

function normalizeReading(data) {
  if (data.updatedAt == null && data.temperature == null && data.humidity == null && data.windSpeed == null) {
    return null
  }

  return {
    temperature: data.temperature ?? null,
    humidity: data.humidity ?? null,
    windSpeed: data.windSpeed ?? null,
    condition: data.condition ?? null,
    updatedAt: data.updatedAt ?? null,
  }
}

export async function fetchWeatherReading() {
  try {
    const response = await fetch(WEATHER_API_URL)
    if (!response.ok) throw new Error('Falha ao buscar leitura')
    const data = await response.json()
    return normalizeReading(data)
  } catch (error) {
    console.warn('Não foi possível buscar a leitura do bridge:', error)
    throw error
  }
}

export function subscribeWeatherReadings(onReading, onError) {
  const eventSource = new EventSource(`${WEATHER_API_URL}/events`)

  eventSource.onmessage = (event) => {
    try {
      onReading(normalizeReading(JSON.parse(event.data)))
    } catch (error) {
      console.warn('Não foi possível interpretar a leitura recebida:', error)
    }
  }

  eventSource.onerror = onError

  return () => eventSource.close()
}
