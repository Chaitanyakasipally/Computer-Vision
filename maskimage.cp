
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;


cv::Mat img; //image holder
Mat img_org; //The original image
Mat img_s;


Mat imgs[13];

Mat bg = imread("imgs/bg_scene.jpg");

//our own function to do the HSV color space convertion
void bgr2hsv(Mat& img, Mat& hsv)
{
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            double b = img.ptr<uchar>(y)[3 * x]; //get the blue channel value
            double g = img.ptr<uchar>(y)[3 * x + 1]; //get the green channel value
            double r = img.ptr<uchar>(y)[3 * x + 2]; //red

            double min, max;

            if (b >= r && b >= g) //b is the largest
            {
                max = b;
                if (r >= g)
                    min = g;
                else
                    min = r;
            }
            else if (g >= r && g >= b)
            {
                max = g;
                if (r >= b)
                    min = b;
                else
                    min = r;
            }
            else
            {
                max = r;
                if (b >= g)
                    min = g;
                else
                    min = b;
            }

            double h, s, v;
            if (max == min)
                h = 0.0;
            else if (max == r && g >= b)
                h = 60.0 * (g - b) / (max - min);
            else if (max == r && g < b)
                h = 60.0 * (g - b) / (max - min) + 360.0;
            else if (max == g)
                h = 60.0 * (b - r) / (max - min) + 120.0;
            else if (max == b)
                h = 60.0 * (r - g) / (max - min) + 240.0;

            if (max == 0.0)
                s = 0.0;
            else
                s = (max - min) / max;

            v = max;


            hsv.ptr<uchar>(y)[3 * x] = (uchar)h; //hue
            hsv.ptr<uchar>(y)[3 * x + 1] = (uchar)s; //saturation
            hsv.ptr<uchar>(y)[3 * x + 2] = (uchar)v; //value



        }
    }
}

//Step 1: define your callback function for mouse event
void onMouse(int event, int x, int y, int flat, void* user_data)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        img_org.copyTo(img);
        if (x + img_s.cols < img.cols && y + img_s.rows < img.rows)
            img_s.copyTo(img(Range(y, y + img_s.rows), Range(x, x + img_s.cols)));
    }
}

int main(int argc, char** argv)
{
    Mat img_hsv,srcImage; //to store the HSV image
    int idx = 0;

    char file_names[200];

    for (int i = 0; i < 13; i++)
    {
        sprintf_s(file_names, "imgs/dinosaur%02d.jpg", i);
        imgs[i] = imread(file_names);
        printf(file_names);
        printf("\n img[%d]: (%d, %d)\n", i, imgs[i].rows, imgs[i].cols);

    }
    for (int i = 0; i < 13; i++) {
        sprintf_s(file_names, "imgs/dinosaur%02d.jpg", i);
        imgs[i] = imread(file_names);
        srcImage = cv::imread("imgs/dinosaur%02d.jpg", CV_LOAD_IMAGE_GRAYSCALE);
        cv::Mat mask(img_src.size(), img_src.type());
    }



    //Assign values for this img

    //Option 1: load an image from file
    img = imread("lion.jpg");
    resize(img, img, Size(img.cols / 2, img.rows / 2));
    resize(img, img_s, Size(img.cols / 6, img.rows / 6));
    img.copyTo(img_org);


    //create the img_hsv mannually
    img_hsv.create(img.rows, img.cols, CV_8UC3);

    //cvtColor(img, img, CV_BGR2HSV);
    bgr2hsv(img, img_hsv);

    //Option 2: get the image from Webcam
    //VideoCapture cam(0); //0 - the first camera
    Mat frame;
    Mat channels[3];

    split(img, channels); //copy each channel of the HSV image into three separate images


    //Option 3: mannually create an image
    Mat img2;
    img2.create(800, 1000, CV_8UC3);
    img2 = Scalar(0, 255, 255);//yellow image
                                //Use Scalar as the input for the constructor
    circle(img2, Point(img2.cols / 2, img2.rows / 2), 120, Scalar(255, 0, 0), 3);


    //cam >> frame; // grab a frame from the camera video
    Mat frame_hsv;
    frame.copyTo(frame_hsv); //can make sure frame_hsv is created with the same size

    //Step 2: register the callback to the system
    namedWindow("Window"); //Must define it first before calling the register function
    setMouseCallback("Window", onMouse);

    while (1)
    {
        if (idx > 12)
        {
            idx = 0;
        }

        imgs[idx].copyTo(bg(Range(bg.rows / 2, bg.rows / 2 + imgs[idx].rows), Range(bg.cols / 2, bg.cols / 2 + imgs[idx].cols)));

        idx++;


        //cam >> frame;   //Get the current frame from the camera
        //bgr2hsv(frame, frame_hsv);
        //cvtColor(frame, frame_hsv, CV_BGR2HSV);

        //imshow("Camera", frame); //show the frame on the screen
        //imshow("Camera HSV", frame_hsv);

        //imshow("hsv", img_hsv);
        imshow("Window", img); //shows the Hue value of the image
        imshow("Small image", img_s);

        imshow("scene", bg);

        uchar c = waitKey(1);
        if (c == 27) //ASCII code for ESc key
            break;
    }
    return 1;
}