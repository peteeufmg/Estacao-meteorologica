import './StatTile.css'

export default function StatTile({ icon, label, value, unit, color, variant = 'number' }) {
  return (
    <article className="stat-tile" style={{ '--tile-color': color }}>
      <div className="stat-tile__icon">{icon}</div>
      <div className="stat-tile__body">
        <p className="stat-tile__label">{label}</p>
        <p className={`stat-tile__value stat-tile__value--${variant}`}>
          {value}
          {unit && <span className="stat-tile__unit">{unit}</span>}
        </p>
      </div>
    </article>
  )
}
