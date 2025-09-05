#!/usr/bin/env python

import numpy as np
import cv2
from flask import Flask, request, Response, jsonify

app = Flask(__name__)

WIDTH, HEIGHT = 320, 240  # QVGA
latest_frame = None
bb_x = None
bb_y = None


@app.route('/upload', methods=['POST'])
def upload():
    """
    Receives raw image data from ESP32 and updates the latest_frame.
    Handles both RGB888 and RGB565 formats.
    """
    global latest_frame
    raw_data = request.data
    img_array = np.frombuffer(raw_data, dtype=np.uint8)

    if img_array.size == HEIGHT * WIDTH * 3:  # RGB888 data
        try:
            img_array = img_array.reshape((HEIGHT, WIDTH, 3))
            print("Got RGB888 data.")
        except ValueError:
            return "Failed to reshape RGB888 data", 400

    elif img_array.size == HEIGHT * WIDTH * 2:  # RGB565 data
        try:
            img_array = img_array.reshape((HEIGHT, WIDTH, 2))
            img_array = cv2.cvtColor(img_array, cv2.COLOR_BGR5652RGB)
            print("Got RGB565 data.")
        except ValueError:
            return "Failed to reshape RGB565 data", 400

    else:
        return "Invalid data size", 400

    latest_frame = img_array
    return "OK", 200


@app.route('/box', methods=['POST'])
def box():
    """
    Receives bounding box coordinates from ESP32
    and stores them for overlay on the latest frame.
    """
    global bb_x, bb_y
    try:
        data = request.get_json(force=True)
        x = int(data.get("x"))
        y = int(data.get("y"))

        # Clip coordinates to stay within frame bounds
        x = max(0, min(WIDTH - 1, x))
        y = max(0, min(HEIGHT - 1, y))

        bb_x = x
        bb_y = y

        print(f"Received bounding box: ({bb_x}, {bb_y})")
        return jsonify({"status": "OK"}), 200
    except Exception as e:
        print("Error parsing bounding box:", e)
        return jsonify({"error": str(e)}), 400


@app.route('/')
def video_feed():
    """
    MJPEG streaming endpoint.
    Draws a single circle at the latest bounding box position.
    """
    def generate():
        global latest_frame, bb_x, bb_y
        while True:
            if latest_frame is not None:
                # Copy the frame so we don't modify the original
                frame_with_box = latest_frame.copy()

                # Draw circle only if we have a bounding box
                # print(f"Bounding box global vars are: ({bb_x}, {bb_y})")
                if bb_x is not None and bb_y is not None:
                    cv2.circle(frame_with_box, (bb_x, bb_y), 10, (0, 0, 255), 2)
                    print("Drawing box.")

                # Encode frame as JPEG
                _, jpeg = cv2.imencode('.jpg', frame_with_box)
                frame = jpeg.tobytes()

                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

    return Response(generate(), mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
