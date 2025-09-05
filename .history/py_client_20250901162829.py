import requests
import numpy as np

# Replace with the correct server URL
SERVER_URL = "http://192.168.1.177:5000/upload"

def send_image():
    # Create a dummy image, e.g., a black image with shape (240, 320, 3)
    width, height = 320, 240
    dummy_image = np.zeros((height, width, 3), dtype=np.uint8)
    
    # Convert the image to bytes
    image_data = dummy_image.tobytes()
    
    # Send a POST request to the server with the image data
    response = requests.post(SERVER_URL, data=image_data, headers={'Content-Type': 'application/octet-stream'})
    
    # Print the server's response
    print("Server response:", response.text)

if __name__ == "__main__":
    send_image()