from flask import Flask, jsonify, render_template
import serial
import threading

app = Flask(__name__)

# Set the correct COM port (Windows: COMx, Linux/Mac: /dev/ttyUSBx)
arduino = serial.Serial('COM8', 9600, timeout=1)

button_press_count = 0


def read_serial():
    global button_press_count
    while True:
        if arduino.in_waiting > 0:
            try:
                # Read the incoming serial line and strip any extra whitespace/newlines
                line = arduino.readline().decode().strip()
                # Expecting output in the format: "Total button presses: <number>"
                if "Total button presses:" in line:
                    # Split the line on the colon and take the right-hand part
                    count_str = line.split("Total button presses:")[-1].strip()
                    button_press_count = int(count_str)
            except ValueError:
                # If conversion fails, ignore this line
                pass


# Start a separate thread to read serial data
threading.Thread(target=read_serial, daemon=True).start()


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/data')
def get_data():
    return jsonify({'count': button_press_count})


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True, use_reloader=False)
