# Flask Web Application

A simple Flask web application project structure.

## Project Structure

```
flask_app/
├── app.py          # Main Flask application
├── config.py       # Configuration settings
├── utils.py        # Utility functions
├── requirements.txt
└── README.md
```

## Installation

1. Create a virtual environment:
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

## Usage

Run the application:
```bash
python app.py
```

The server will start at `http://localhost:5000`

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | Welcome message |
| `/health` | GET | Health check |
| `/api/data` | GET | Sample data |

## Configuration

Set environment variables to customize settings:

- `FLASK_DEBUG` - Enable debug mode (True/False)
- `FLASK_HOST` - Server host (default: 0.0.0.0)
- `FLASK_PORT` - Server port (default: 5000)
- `SECRET_KEY` - Secret key for sessions
- `DATABASE_URI` - Database connection string

## License

MIT
# Update 1
