import requests
import numpy as np
import time

# Replace with your server IP
SERVER_IP = "192.168.1.177"
UPLOAD_URL = f"http://{SERVER_IP}:5000/upload"
BOX_URL = f"http://{SERVER_IP}:5000/box"

WIDTH, HEIGHT = 320, 240


def send_image():
    """
    Sends a dummy black RGB888 image to the /upload endpoint.
    """
    dummy_image = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)

    # Draw a white rectangle in the center (for visibility)
    cv2.rectangle(dummy_image, (120, 80), (200, 160), (255, 255, 255), -1)

    image_data = dummy_image.tobytes()

    try:
        response = requests.post(
            UPLOAD_URL,
            data=image_data,
            headers={'Content-Type': 'application/octet-stream'},
            timeout=2
        )
        print("[UPLOAD] Server response:", response.text)
    except Exception as e:
        print("[UPLOAD] Error:", e)


def send_box(x, y):
    """
    Sends a dummy bounding box to the /box endpoint.
    """
    payload = {"x": x, "y": y}

    try:
        response = requests.post(
            BOX_URL,
            json=payload,
            headers={'Content-Type': 'application/json'},
            timeout=2
        )
        print(f"[BOX] Sent ({x}, {y}) → Server response:", response.text)
    except Exception as e:
        print("[BOX] Error:", e)


if __name__ == "__main__":
    import cv2  # Added here so OpenCV is optional if you just test without rectangles

    print("Starting dummy client...")
    x, y = 50, 50

    while True:
        send_image()       # Send a dummy image
        send_box(x, y)     # Send bounding box

        # Move the box slightly each time for testing animation
        x = (x + 20) % WIDTH
        y = (y + 15) % HEIGHT

        time.sleep(1)  # 1 FPS for testing
