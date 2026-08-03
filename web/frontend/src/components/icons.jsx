export function ThermometerIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <path
        d="M14 14.76V4a2 2 0 0 0-4 0v10.76a4 4 0 1 0 4 0Z"
        stroke="currentColor"
        strokeWidth="1.8"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
      <circle cx="12" cy="17" r="1.6" fill="currentColor" />
    </svg>
  )
}

export function DropletIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <path
        d="M12 3s6 6.5 6 11a6 6 0 1 1-12 0c0-4.5 6-11 6-11Z"
        stroke="currentColor"
        strokeWidth="1.8"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
    </svg>
  )
}

export function WindIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <path
        d="M3 8h11.5a2.5 2.5 0 1 0-2.4-3.2M3 12h15.5a2.5 2.5 0 1 1-2.4 3.2M3 16h8.5a2 2 0 1 1-1.9 2.6"
        stroke="currentColor"
        strokeWidth="1.8"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
    </svg>
  )
}

export function SunIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <circle cx="12" cy="12" r="4.5" stroke="currentColor" strokeWidth="1.8" />
      <g stroke="currentColor" strokeWidth="1.8" strokeLinecap="round">
        <path d="M12 2.5v2.2M12 19.3v2.2M21.5 12h-2.2M4.7 12H2.5M18.4 5.6l-1.6 1.6M7.2 16.8l-1.6 1.6M18.4 18.4l-1.6-1.6M7.2 7.2 5.6 5.6" />
      </g>
    </svg>
  )
}

export function CloudSunIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <g stroke="currentColor" strokeWidth="1.8" strokeLinecap="round">
        <circle cx="8" cy="7" r="3" />
        <path d="M8 1.5v1.4M3.5 7H2M4.3 3.3l1 1M11.7 3.3l-1 1" />
      </g>
      <path
        d="M9 19a4 4 0 0 1-.3-8c.2 0 .5 0 .7 0a5.5 5.5 0 0 1 10.2 2A4 4 0 0 1 19 19H9Z"
        stroke="currentColor"
        strokeWidth="1.8"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
    </svg>
  )
}

export function CloudIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <path
        d="M7 18a4 4 0 0 1-.3-8 5.5 5.5 0 0 1 10.7-1.6A4 4 0 0 1 17 18H7Z"
        stroke="currentColor"
        strokeWidth="1.8"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
    </svg>
  )
}

export function CloudRainIcon(props) {
  return (
    <svg viewBox="0 0 24 24" fill="none" aria-hidden="true" {...props}>
      <path
        d="M7 15a4 4 0 0 1-.3-8 5.5 5.5 0 0 1 10.7-1.6A4 4 0 0 1 17 15H7Z"
        stroke="currentColor"
        strokeWidth="1.8"
        strokeLinecap="round"
        strokeLinejoin="round"
      />
      <g stroke="currentColor" strokeWidth="1.8" strokeLinecap="round">
        <path d="M9 18.5v2M13 18.5v2M17 18.5v2" />
      </g>
    </svg>
  )
}

export function SunCloudIcon(props) {
  return (
    <svg viewBox="0 0 32 32" aria-hidden="true" {...props}>
      <circle cx="9" cy="9" r="6" fill="var(--series-temperature)" />
      <g fill="var(--series-humidity)">
        <circle cx="12" cy="18" r="5" />
        <circle cx="19" cy="15" r="6.5" />
        <circle cx="25" cy="18" r="5" />
        <rect x="8" y="18" width="17" height="8" rx="4" />
      </g>
    </svg>
  )
}
