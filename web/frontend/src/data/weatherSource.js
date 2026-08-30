export async function fetchWeatherReading() {
  try {
    const response = await fetch('http://localhost:3001/api/weather')
    if (!response.ok) throw new Error('Falha ao buscar leitura')
    const data = await response.json()

    return {
      temperature: data.temperature ?? null,
      humidity: data.humidity ?? null,
      windSpeed: data.windSpeed ?? null,
      condition: data.condition ?? null,
      updatedAt: new Date().toISOString(),
    }
  } catch (error) {
    console.warn('Não foi possível buscar a leitura do bridge:', error)
    return {
      temperature: null,
      humidity: null,
      windSpeed: null,
      condition: null,
      updatedAt: new Date().toISOString(),
    }
  }
}
