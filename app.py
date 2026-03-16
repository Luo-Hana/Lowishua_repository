from flask import Flask, jsonify
from config import Config
from utils import format_response

app = Flask(__name__)
app.config.from_object(Config)


@app.route('/')
def index():
    return format_response({'message': 'Welcome to Flask App!'})


@app.route('/health')
def health():
    return format_response({'status': 'healthy'})


@app.route('/api/data')
def get_data():
    return format_response({'data': [1, 2, 3, 4, 5]})


if __name__ == '__main__':
    app.run(
        host=app.config.get('HOST', '0.0.0.0'),
        port=app.config.get('PORT', 5000),
        debug=app.config.get('DEBUG', False)
    )
