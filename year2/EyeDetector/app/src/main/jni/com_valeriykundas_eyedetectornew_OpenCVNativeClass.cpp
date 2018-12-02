#include "com_valeriykundas_eyedetectornew_OpenCVNativeClass.h"
#include <jni.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

String face_cascade_name = "/storage/emulated/0/download/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "/storage/emulated/0/download/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
bool first_time_load = true;

bool detect_face(Mat &frame) {
    if (first_time_load) {
        if (!face_cascade.load(face_cascade_name)) {
            return false;
        }
        if (!eyes_cascade.load(eyes_cascade_name)) {
            return false;
        }
        first_time_load = false;
    }

    vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.3, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));

    for (size_t i = 0; i < faces.size(); i++) {
        rectangle(frame, Point(faces[i].x, faces[i].y),
                  Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
                  Scalar(255, 255, 255), 4);

        Mat faceROI = frame_gray(faces[i]);
        vector<Rect> eyes;

        eyes_cascade.detectMultiScale(faceROI, eyes, 1.3, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));

        for (size_t j = 0; j < eyes.size(); j++) {
            rectangle(frame, Point(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y),
                      Point(faces[i].x + eyes[j].x + eyes[j].width,
                            faces[i].y + eyes[j].y + eyes[j].height), Scalar(255, 255, 255), 4);
        }
    }
    return true;
}

JNIEXPORT jboolean JNICALL Java_com_valeriykundas_eyedetectornew_OpenCVNativeClass_faceDetection
  (JNIEnv *, jclass, jlong matAddrRgba) {
    Mat& rgba = *(Mat*)matAddrRgba;
    bool res = detect_face(rgba);
    return true;
}

