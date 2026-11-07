import cv2
import numpy as np
import urllib.request

url = 'http://192.168.100.254/capture'

def capture_frame():
    img_resp = urllib.request.urlopen(url, timeout=5)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    return cv2.imdecode(imgnp, -1)

def get_dust_percentage(reference, current):
    # Dono grayscale karo
    ref_gray = cv2.cvtColor(reference, cv2.COLOR_BGR2GRAY)
    cur_gray = cv2.cvtColor(current, cv2.COLOR_BGR2GRAY)
    
    # Absolute difference
    diff = cv2.absdiff(ref_gray, cur_gray)
    
    # Threshold — small noise ignore karo
    _, thresh = cv2.threshold(diff, 25, 255, cv2.THRESH_BINARY)
    
    # Dust percentage — kitne pixels changed
    total_pixels = thresh.shape[0] * thresh.shape[1]
    dust_pixels = cv2.countNonZero(thresh)
    dust_pct = (dust_pixels / total_pixels) * 100
    
    return dust_pct, diff, thresh

def get_status(dust_pct):
    if dust_pct < 5:
        return "CLEAN", (0, 255, 0)
    elif dust_pct < 20:
        return "LIGHT DUST", (0, 255, 255)
    elif dust_pct < 40:
        return "MODERATE DUST", (0, 165, 255)
    else:
        return "HEAVY DUST", (0, 0, 255)

print("Panel bilkul clean rakhen aur Enter dabayein — reference image lega...")
input()
reference = capture_frame()
print("Reference image li — ab detection shuru!")

while True:
    try:
        current = capture_frame()

        if current is not None and reference is not None:
            dust_pct, diff, thresh = get_dust_percentage(reference, current)
            status, color = get_status(dust_pct)

            # Display
            cv2.rectangle(current, (0, 0), (420, 160), (0, 0, 0), -1)
            cv2.putText(current, f'Dust Level: {dust_pct:.1f}%',
                       (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, color, 2)
            cv2.putText(current, f'Status: {status}',
                       (10, 85), cv2.FONT_HERSHEY_SIMPLEX, 0.9, color, 2)

            # Progress bar
            bar_width = int((dust_pct / 100) * 300)
            cv2.rectangle(current, (10, 110), (310, 135), (50, 50, 50), -1)
            cv2.rectangle(current, (10, 110), (10 + bar_width, 135), color, -1)

            cv2.imshow("SolarBot PK - Dust Detection", current)
            cv2.imshow("Difference", diff)
            cv2.imshow("Dust Mask", thresh)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break

    except Exception as e:
        print("Retrying...")
        continue