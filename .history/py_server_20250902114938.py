#!/usr/bin/env python


import numpy as np
import cv2
from flask import Flask, request, Response, jsonify

app = Flask(__name__)

WIDTH, HEIGHT = 320, 240  # QVGA
# WIDTH, HEIGHT = 160, 120  # QQVGA
latest_frame = None


# @app.route('/upload', methods=['POST'])
# def upload():
#     global latest_frame
#     raw_data = request.data
#     img_array = np.frombuffer(raw_data, dtype=np.uint8)
#     img_array = img_array.reshape((HEIGHT, WIDTH, 3))
#     # img_array = cv2.imdecode(img_array, cv2.IMREAD_COLOR)
#     latest_frame = img_array
#     return "OK", 200


@app.route('/upload', methods=['POST'])
def upload():
    global latest_frame
    raw_data = request.data
    img_array = np.frombuffer(raw_data, dtype=np.uint8)

    if img_array.size == HEIGHT * WIDTH * 3:  # If RGB888 data is sent
        try:
            img_array = img_array.reshape((HEIGHT, WIDTH, 3))
            print("Got RGB888 data.")
        except ValueError:
            return "Failed to reshape RGB888 data", 400
    elif img_array.size == HEIGHT * WIDTH * 2:  # If RGB565 data is sent
        try:
            # Reshape to approximately handle RGB565
            img_array = img_array.reshape((HEIGHT, WIDTH, 2))
            img_array = cv2.cvtColor(img_array, cv2.COLOR_BGR5652RGB)  # Convert if needed
        except ValueError:
            return "Failed to reshape RGB565 data", 400
    else:
        return "Invalid data size", 400  # Handle unexpected sizes

    latest_frame = img_array
    return "OK", 200


@app.route('/box', methods=['POST'])
def box():
    """
    Receives bounding box coordinates from ESP32
    and stores them to overlay on the latest frame.
    """
    global latest_boxes
    try:
        data = request.get_json(force=True)
        x = int(data.get("x"))
        y = int(data.get("y"))

        # Clip coordinates to stay inside frame bounds
        x = max(0, min(WIDTH - 1, x))
        y = max(0, min(HEIGHT - 1, y))

        latest_boxes.append((x, y))
        print(f"Received bounding box: ({x}, {y})")
        return jsonify({"status": "OK"}), 200
    except Exception as e:
        print("Error parsing bounding box:", e)
        return jsonify({"error": str(e)}), 400




@app.route('/')
def video_feed():
    def generate():
        global latest_frame
        while True:
            if latest_frame is not None:
                _, jpeg = cv2.imencode('.jpg', latest_frame)
                frame = jpeg.tobytes()
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
    return Response(generate(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)