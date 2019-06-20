import base64
import io
import qrmerge.ui

from flask import Flask, jsonify, render_template, request

app = Flask(__name__)


@app.route("/")
def index():
    return render_template('index.html')


@app.route("/encode", methods=['POST', ])
def encode():
    str_left = request.form.get('str_left')
    str_right = request.form.get('str_right')
    mode = request.form.get('mode')
    version = request.form.get('version')
    level = request.form.get('level')
    image = request.form.get('image')
    coded = qrmerge.ui.encode(str_left, str_right, mode, version, level, level, image)
    if coded['error']:
        return jsonify(coded)

    bytes = io.BytesIO()
    qrmerge.ui.draw_qrcode(bytes, coded['layer_down'])
    layer_down = 'data:image/png;base64,' + base64.b64encode(bytes.getvalue()).decode()
    bytes = io.BytesIO()
    qrmerge.ui.draw_qrcode(bytes, coded['layer_up'], bg_color=(204, 255, 204, 255))
    layer_up = 'data:image/png;base64,' + base64.b64encode(bytes.getvalue()).decode()
    bytes = io.BytesIO()
    qrmerge.ui.draw_qrcode(bytes, coded['qrcode_left'])
    qrcode_left = 'data:image/png;base64,' + base64.b64encode(bytes.getvalue()).decode()
    bytes = io.BytesIO()
    qrmerge.ui.draw_qrcode(bytes, coded['qrcode_right'])
    qrcode_right = 'data:image/png;base64,' + base64.b64encode(bytes.getvalue()).decode()
    coded['layer_down'] = layer_down
    coded['layer_up'] = layer_up
    coded['qrcode_left'] = qrcode_left
    coded['qrcode_right'] = qrcode_right
    return jsonify(coded)
