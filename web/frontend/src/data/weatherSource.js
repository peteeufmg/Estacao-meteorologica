export async function fetchWeatherReading() {
  try {
    const response = await fetch('http://localhost:3001/api/weather')
    if (!response.ok) throw new Error('Falha ao buscar leitura')
    const data = await response.json()

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
  } catch (error) {
    console.warn('Não foi possível buscar a leitura do bridge:', error)
    throw error
  }
}
