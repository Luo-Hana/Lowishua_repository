import os


class Config:
    """Application configuration class."""
    
    # Basic settings
    DEBUG = os.environ.get('FLASK_DEBUG', False)
    HOST = os.environ.get('FLASK_HOST', '0.0.0.0')
    PORT = int(os.environ.get('FLASK_PORT', 5000))
    
    # Security
    SECRET_KEY = os.environ.get('SECRET_KEY', 'dev-secret-key-change-in-production')
    
    # Database (example)
    DATABASE_URI = os.environ.get('DATABASE_URI', 'sqlite:///app.db')
    
    # Logging
    LOG_LEVEL = os.environ.get('LOG_LEVEL', 'INFO')
