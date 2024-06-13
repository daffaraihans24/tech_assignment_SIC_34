from flask import Flask, jsonify, request

app = Flask(__name__)

dataList = []

@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

@app.route("/sensors", methods=['GET', 'POST'])
def routeSensors():
    if request.method == 'GET':
        return sendResponse("success", dataList)
    else:
        data = request.json
        dataList.append(data)
        return sendResponse("success", data)

def sendResponse(status, data):
    return jsonify({
        "status": status,
        "data": data
    })

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
