from flask import Flask
from service import light_controller as light_service

app = Flask('Smart Home')
RESPONSE_OK = ("", 200)


@app.route('/api/light/<room>/<type>/<command>', methods=['GET'])
def control_light(room, type, command):
    light_service.control_light(room, type, command)
    return RESPONSE_OK


@app.route('/', methods=['GET'])
def status():
    return app.send_static_file('index.html')


if __name__ == '__main__':
    app.run(host="0.0.0.0", port=int("1337"), debug=True, use_reloader=True)
