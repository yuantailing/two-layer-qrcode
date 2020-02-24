import base64
import io
import json
import os
import qrmerge.ui
import shutil

def lambda_handler(event, context):
    str_left = event['body-json']['str_left']
    str_right = event['body-json']['str_right']
    mode = event['body-json']['mode']
    version = event['body-json']['version']
    level = event['body-json']['level']
    image = event['body-json']['image']
    if image:
        image = base64.b64decode(image)

    shutil.copy('qrmerge/main.exe', '/tmp/main.exe')
    os.chmod('/tmp/main.exe', 0o755)
    coded = qrmerge.ui.encode(str_left, str_right, mode, version, level, level, image)
    if coded['error']:
        return coded

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
    return coded
