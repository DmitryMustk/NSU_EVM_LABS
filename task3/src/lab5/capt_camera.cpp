#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(void) {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Camera Error" << endl;
        return -1;
    }
    CascadeClassifier noseCascade;
    if (!noseCascade.load("../haarcascade_mcs_nose.xml")) {
        cerr << "Nose Classificator Error" << endl;
        return -1;
    }

    int fps = 0;
    double start_time = static_cast<double>(getTickCount());

    while (true) {
        double frame_start_time = static_cast<double>(getTickCount());

        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            cerr << "Empty Camera" << endl;
            break;
        }
        double proc_start_time = static_cast<double>(getTickCount());
        vector<Rect> noses;
        noseCascade.detectMultiScale(frame, noses, 1.1, 100);  // scale_factor=1.3 min_neighbours=4
        for (const Rect& noseRect : noses) {
            int noseX = noseRect.x + noseRect.width / 2;
            int noseY = noseRect.y + noseRect.height / 2;
            int radius = noseRect.width / 2;
            Scalar red(0, 0, 255);
            circle(frame, Point(noseX, noseY), radius, red, -1);  //thickness = -1 to fill the circle
        }
        double proc_end_time = static_cast<double>(getTickCount());

        double frame_end_time = static_cast<double>(getTickCount());
        double frame_elapsed_time = (frame_end_time - frame_start_time) / getTickFrequency();
        double proc_elapsed_time = (proc_end_time - proc_start_time) / getTickFrequency();
        //        putText(frame, "Frame Time: " + to_string(frame_elapsed_time) + "s", Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);

        double end_time = static_cast<double>(getTickCount());
        double elapsed_time = (end_time - start_time) / getTickFrequency();
        double proc_percent = (proc_elapsed_time / elapsed_time * 100);
        fps = static_cast<int>(1.0 / elapsed_time);
        start_time = end_time;

        putText(frame, "FPS: " + to_string(fps), Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
        putText(frame, "Time to processing: " + to_string(proc_percent) + "%", Point(10, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
        imshow("ClownNose", frame);

        char c = waitKey(1);
        if (c == 'z' || c == 27)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
