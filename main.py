from typing import List
from typing_extensions import Final
from imutils.video import VideoStream
from numpy import integer
from data import KeyPoint
from ml import Movenet
import imutils
import cv2
import utils
import arduino

WIDTH: Final = 320

def is_facing_camera(keypoints: List[KeyPoint]) -> bool:
    left_eye, right_eye, left_ear, right_ear, left_shoulder, right_shoulder, *_ = keypoints;
    return left_eye.coordinate.x >= right_eye.coordinate.x or left_ear.coordinate.x >= right_ear.coordinate.x or left_shoulder.coordinate.x >= right_shoulder.coordinate.x

def main():
    # serial = arduino.auto_connect()
    vs = VideoStream(src=0).start()
    pose_detector = Movenet('movenet_singlepose_thunder_float16')
    angle: integer = 127
    absence_count: integer = 0

    while True:
        # line = serial.readline()
        # if line:
        #     print(line.decode('ascii'), end=None)

        frame = cv2.flip(imutils.resize(vs.read(), width=WIDTH), 1)
        person = pose_detector.detect(frame, True)
        if person and person.score >= 0.5 and len(person.keypoints) >= 3:
            absence_count = 0
            (nose, *rest) = person.keypoints;

            new_angle: integer = angle
            if is_facing_camera(rest):
                new_angle = 0 if nose.coordinate.x <= WIDTH * 0.5 else 255
            else:
                new_angle = round(nose.coordinate.x * 255 / WIDTH)

            if new_angle != angle:
                angle = new_angle
                # print("Angle: {}", angle)
                # serial.write((angle).to_bytes(1, 'little'))
                # serial.flush()

            cv2.imshow('movenet_singlepose_thunder_float16', utils.visualize(frame, [person]))

        else:
            absence_count += 1

        if absence_count >= 5:
            if 127 != angle:
                angle = 127
                # serial.write((angle).to_bytes(1, 'little'))
                # serial.flush()

            cv2.imshow('movenet_singlepose_thunder_float16', utils.visualize(frame, []))

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()
    vs.stop()

if __name__ == "__main__":
    main()
