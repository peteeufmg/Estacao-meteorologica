// Placeholder reading — swap the body for a fetch() to the ESP32's HTTP endpoint once it exists.
export async function fetchWeatherReading() {
  return {
    temperature: 24.6,
    humidity: 58,
    windSpeed: 12.4,
    condition: 'parcialmente-nublado',
    updatedAt: new Date().toISOString(),
  }
}
