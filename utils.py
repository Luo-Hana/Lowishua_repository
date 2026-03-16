from datetime import datetime
from functools import wraps


def format_response(data, status='success'):
    """Format API response with standard structure."""
    return {
        'status': status,
        'data': data,
        'timestamp': datetime.utcnow().isoformat()
    }, 200


def log_request(func):
    """Decorator to log incoming requests."""
    @wraps(func)
    def wrapper(*args, **kwargs):
        from flask import request
        print(f"[{datetime.utcnow()}] {request.method} {request.path}")
        return func(*args, **kwargs)
    return wrapper


def validate_json(required_fields):
    """Decorator to validate JSON payload."""
    def decorator(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            from flask import request, jsonify
            if not request.is_json:
                return jsonify({'error': 'Content-Type must be application/json'}), 400
            
            data = request.get_json()
            missing = [field for field in required_fields if field not in data]
            if missing:
                return jsonify({'error': f'Missing fields: {missing}'}), 400
            
            return func(*args, **kwargs)
        return wrapper
    return decorator
