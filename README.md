# two-layer-qrcode

Live demo: [https://yuantailing.github.io/two-layer-qrcode/](https://yuantailing.github.io/two-layer-qrcode/)

Note that the demo is deployed on [AWS Lambda](https://aws.amazon.com/lambda/), thus it's slower than the reported result in the paper.

## Usage

```sh
$ pip3 install -r requirements.txt
$ make -C qrmerge  # please be patient to wait
$ # put a logo to qrmerge/logo.png as the default logo
$ FLASK_APP=app.py python3 -m flask run --with-threads
```

## System requirements

OS:

- Windows / Linux / macOS

Software:

- Python 3
- MinGW / g++

Memory:

- Compile: (don't know)
- Run: 256 MB
